Channel ya forma parte del binario, porque el Makefile compila src/Channel.cpp, pero ninguna otra clase lo incluye ni lo llama. En ejecución un canal no llega a existir: Server solo guarda clientes, y el hueco de receiveData donde iría el comando sigue vacío. La clase es la sala; el servidor es quien debe poseer las salas y los comandos quienes deben entrar, hablar y salir.

Quién guarda a quién
La relación que encaja con el resto del proyecto es esta:

Server posee los Client (ya lo hace, en _clients) y también debe poseer los Channel.
Channel guarda punteros a esos mismos Client. No los crea y su destructor no los borra.
Client no necesita una lista de canales. Para saber si alguien está dentro se pregunta al canal con hasMember.
El comando (JOIN, PRIVMSG, PART…) decide la regla. Channel solo actualiza miembros, operadores, tema, clave, límite e invitaciones.
Replies convierte el resultado en una línea IRC. El canal no escribe en el socket.
Eso se traduce en un mapa nuevo dentro de Server:


std::map<std::string, Channel *> _channels;
La clave es el nombre del canal, el mismo texto que devuelve getName().
El valor es el canal creado con new.
Buscar #42 es buscar esa clave. Crearlo es new Channel("#42") y guardarlo en el mapa.
Al cerrar el servidor se hace delete de cada canal y después el delete de los clientes que ya tienes. El canal solo suelta punteros; no toca los Client.
El mismo objeto Client que está en _clients es el que se pasa a addMember. La clase identifica a las personas por la dirección del puntero. Una copia del cliente sería otra dirección y hasMember diría que no está.

Lo que ya hace la clase
El mapa _members es la lista de presencia. El bool dice si ese miembro es operador. _invited es otra lista: personas citadas que todavía no han entrado. El tema, la clave, el límite, el modo invitación y el modo tema restringido ya están guardados, aunque todavía no hay métodos para leerlos ni cambiarlos.

addMember hace tres cosas:


Channel.cpp
Ln 18–25

void Channel::addMember(Client *client, bool isOperator) {
    if (client == NULL)
        return;
    std::map<Client *, bool>::iterator it = _members.find(client);
    if (it == _members.end())
        _members.insert(std::make_pair(client, isOperator));
    _invited.erase(client);
}
Un puntero nulo no entra en el mapa.
Si esa dirección ya está, el bool se queda como estaba. Para ascender a alguien que ya es miembro se usa addOperator, no una segunda llamada a addMember.
Si no está, se inserta el par (cliente, es operador).
La invitación se consume al entrar. Si después sale de un canal +i, hace falta un INVITE nuevo.
addOperator solo pone true si la persona ya es miembro. removeMember borra presencia e invitación; al desaparecer del mapa desaparece también el flag de operador. hasMember e isOperator buscan esa misma dirección. isEmpty y memberCount le sirven al servidor para borrar la sala cuando se va el último, y al JOIN para decidir el límite y quién es el primer operador.

El primer miembro se marca antes de insertar:


bool founder = channel.memberCount() == 0;
channel.addMember(&client, founder);
memberCount() == 0 significa que la sala todavía está vacía, así que este cliente es quien la abre.
addMember con true lo guarda como operador.
Si la pregunta se hiciera después de insertar, el conteo ya sería 1 y la condición habría cambiado de significado.
El enlace que falta al desconectar
deleteFd hoy borra el Client y deja de conocerlo. Cuando ese puntero también viva dentro de un canal, la sala se quedaría con una dirección ya liberada. El borrado del cliente tiene que pasar antes por todos los canales:


void Server::removeClientFromChannels(Client *client)
{
    std::map<std::string, Channel *>::iterator it = _channels.begin();
    while (it != _channels.end())
    {
        it->second->removeMember(client);
        if (it->second->isEmpty())
        {
            delete it->second;
            _channels.erase(it++);
        }
        else
            ++it;
    }
}
it recorre el mapa de salas del servidor.
removeMember quita a ese cliente de la sala y de sus invitaciones. Si no estaba en esa sala, el erase del mapa no cambia nada.
isEmpty usa el método que ya tienes: la sala sin miembros no tiene por qué seguir reservada.
delete destruye el Channel. Sus punteros desaparecen con él y los Client de los demás siguen siendo propiedad del servidor.
erase(it++) borra la clave y deja el iterador en la sala siguiente. En la otra rama solo se avanza.
Esa función se llama desde deleteFd antes de delete it->second. El mismo camino vale para QUIT: primero se avisa a los compañeros con Replies::quit, después se saca al cliente de las salas y al final se cierra el descriptor.

Cómo entra un comando hasta el canal
El camino completo, cuando escribas el registro de comandos, queda así:

receiveData ya convierte bytes en un Message.
Si el resultado es PARSE_MESSAGE, el registro ejecuta el comando que coincide con msg.getCommand().
JOIN lee el nombre del canal en msg.getParams()[0]. Si falta, responde con un numérico 461 y no crea sala.
El nombre válido para esta clase empieza por #. El comando lo comprueba antes de construir el Channel.
Server busca esa clave en _channels. Si no existe, crea el canal y lo guarda.
El comando consulta el estado de la sala y, si las reglas lo permiten, llama a addMember.
Con el prefijo nick!user@host pide a Replies::join la línea ya terminada en \r\n.
Esa línea se encola en cada miembro y enableWrite marca el descriptor de cada uno. En la vuelta siguiente de poll, sendData la envía.
Las reglas de los campos privados se aplican en el comando, porque el canal no conoce la contraseña que el usuario acaba de escribir:

clave no vacía y distinta del parámetro: numérico 475;
memberCount() ya alcanzó _limit cuando _limit es mayor que cero: numérico 471;
sala de solo invitación y el cliente no está en _invited: numérico 473;
hasMember verdadero: el cliente ya está dentro y no se vuelve a insertar.
Para poder escribir esas condiciones hacen falta lecturas y cambios que la clase todavía no expone: tema, clave, límite, solo invitación, tema restringido, invite(Client *), isInvited y quitar el operador (MODE -o pone el bool en false sin borrar al miembro).

Repartir un mensaje sin acoplar el canal al servidor
El canal sabe quién está dentro. El servidor es el único que puede activar POLLOUT. Por eso el reparto se parte en dos métodos.

En el canal, la visita:


void Channel::broadcast(const std::string &line, const Client *except) const
{
    std::map<Client *, bool>::const_iterator it = _members.begin();
    for (; it != _members.end(); ++it)
    {
        if (it->first == except)
            continue;
        it->first->queueOutput(line);
    }
}
El iterador recorre cada par guardado en _members.
it->first es el Client * de ese miembro.
except es quien no debe recibir la copia. En un PRIVMSG al canal se pasa el emisor. En un JOIN se pasa NULL para que también le llegue a quien entra.
queueOutput añade la línea al buffer de ese cliente. Aquí el .cpp del canal sí necesita #include "Client.hpp", porque por fin llama a un método del cliente. Mientras solo guarda punteros, la declaración adelantada class Client; basta.
En el servidor, la activación de escritura usa el mismo recorrido. Lo natural es que broadcast deje los descriptores anotados, así que el comando puede pedir al canal la lista de miembros o repetir el alta de POLLOUT con un método de Server que recorra esa lista y llame a enableWrite(miembro->getFd()). Sin esa segunda parte el texto queda en _outputBuffer y poll no llama a sendData.

La lista de nombres del 353 sale del mismo mapa. Con el bool se decide el prefijo @:


std::string Channel::namesList() const
{
    std::string names;
    std::map<Client *, bool>::const_iterator it = _members.begin();
    for (; it != _members.end(); ++it)
    {
        if (!names.empty())
            names += " ";
        if (it->second)
            names += "@";
        names += it->first->getNickName();
    }
    return names;
}
names empieza vacío y va acumulando nicks separados por un espacio.
it->second es el flag de operador que guardó addMember o addOperator.
El resultado es el argumento names de Replies::namesReply. Después, solo para quien acaba de entrar, se encola Replies::endOfNames.
Cada comando, sobre esta misma sala
JOIN, después de addMember, encola Replies::join a todos, incluido el que entra. A continuación, solo a ese cliente, le manda el tema si _topic no está vacío (topicReply) y siempre namesReply más endOfNames.

PRIVMSG hacia un nombre que empieza por # busca el canal, exige hasMember del emisor y llama a broadcast con Replies::privmsg, pasando al emisor como except.

PART y KICK construyen su línea con Replies, la reparten a todos los miembros y después llaman a removeMember. En el KICK la persona expulsada tiene que seguir en _members durante el reparto, porque también recibe la línea. Si isEmpty() queda verdadero, el servidor destruye el canal.

TOPIC sin texto nuevo no toca _topic: responde topicReply o noTopic según esté vacío. Con texto nuevo, si _topicRestricted está activo, el comando exige isOperator antes de cambiar el tema y después reparte Replies::topic.

MODE cambia los campos que ya existen (_inviteOnly, _topicRestricted, _key, _limit y el bool de operador) y reparte Replies::mode a los miembros.

INVITE inserta al cliente invitado en _invited. addMember lo retirará de ahí cuando entre.

QUIT y deleteFd recorren _channels, reparten Replies::quit a quien siga dentro y luego ejecutan removeClientFromChannels.