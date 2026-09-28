#ifndef PARSER_HPP
# define PARSER_HPP

//IrcMessage
//Representa una línea IRC ya separada por el parser.

class Parser
{
	private:
		Parser(const Parser &other);
		Parser &operator=(const Parser &other);

	public:
		Parser();
		~Parser();
};


/*PRIVMSG #general :Hola a todos

command  = "PRIVMSG"
params   = ["#general"]
trailing = "Hola a todos"

IrcParser
Responsable únicamente de convertir texto IRC en IrcMessage.

No debe conocer clientes, canales ni permisos.

Debe:

reconocer \r\n;
conservar fragmentos incompletos;
aceptar varias líneas en un mismo buffer;
conservar los espacios del parámetro trailing;
normalizar el comando a mayúsculas.*/

#endif
