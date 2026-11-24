# 📨 Message Class

## 1. ¿Qué es Message?

La clase `Message` representa un comando IRC que ya ha sido procesado por el Parser.

Su función es **almacenar de forma organizada la información de un mensaje IRC**, para que posteriormente los comandos del servidor puedan acceder fácilmente a sus diferentes partes.

Por ejemplo, cuando un cliente envía:

```irc
:alice!user@localhost PRIVMSG #general :Hello everyone!
```

La clase `Message` almacena:
- El prefix del emisor.
- El comando recibido.
- Los parámetros del comando.
- El trailing, si existe.
- Un booleano que indica si el trailing está presente.

**Importante:** `Message` no interpreta ni valida comandos. Esa responsabilidad corresponde al Parser.

---

## 2. 📦 Atributos de la clase

| Atributo | Tipo | Descripción |
|---|---|---|
| `_prefix` | `std::string` | Identifica al emisor del mensaje. |
| `_cmd` | `std::string` | Contiene el comando IRC. |
| `_params` | `vector<string>` | Almacena los parámetros anteriores al trailing. |
| `_trailing` | `std::string` | Almacena el último parámetro, que puede contener espacios. |
| `_hasTrailing` | `bool` | Indica si el mensaje contiene un trailing. |

### ¿Qué diferencia hay entre params y trailing?

En IRC, los parámetros normales se separan mediante espacios.

Sin embargo, cuando aparece `:`, el resto de la línea se considera un único parámetro, aunque contenga espacios.

Por ejemplo:

```irc
PRIVMSG #general :Hello everyone!
```

Se almacena así:

```cpp
_cmd         = "PRIVMSG";
_params      = ["#general"];
_trailing    = "Hello everyone!";
_hasTrailing = true;
```

El trailing permite conservar el texto completo sin dividirlo en diferentes parámetros.

---

## 3. ⚙️ ¿Cómo se guarda un comando paso a paso?

El proceso comienza cuando un cliente envía un comando al servidor.

### Paso 1. Recepción del mensaje

El cliente envía una línea IRC:

```irc
:alice!user@localhost PRIVMSG #general :Hello everyone!
```

El servidor recibe los datos y los entrega al Parser.

### Paso 2. Procesamiento por el Parser

El Parser analiza la línea y separa sus componentes:

```text
Prefix:   alice!user@localhost
Command:  PRIVMSG
Params:   #general
Trailing: Hello everyone!
```

### Paso 3. Creación del objeto Message

Una vez separados los componentes, el Parser crea un objeto `Message` utilizando su constructor:

```cpp
Message message(
    "alice!user@localhost",
    "PRIVMSG",
    params,
    "Hello everyone!",
    true
);
```

Donde `params` es un vector que contiene:

```cpp
params[0] = "#general";
```

### Paso 4. Almacenamiento de los datos

El constructor copia los datos recibidos en los atributos privados:

```cpp
Message::Message(
    const std::string &prefix,
    const std::string &cmd,
    const std::vector<std::string> &params,
    const std::string &trailing,
    bool hasTrailing
)
    : _prefix(prefix),
      _cmd(cmd),
      _params(params),
      _trailing(trailing),
      _hasTrailing(hasTrailing)
{}
```

A partir de este momento, el objeto `Message` contiene toda la información del comando.

### Paso 5. Acceso a los datos

El servidor o el comando correspondiente puede consultar los datos mediante los getters:

```cpp
message.getPrefix();
message.getCommand();
message.getParams();
message.getTrailing();
message.hasTrailing();
```

Los getters devuelven referencias constantes cuando trabajan con strings o vectores, evitando copias innecesarias y evitando modificar los datos almacenados.

---

## 4. 💬 Ejemplos de comandos

### Ejemplo 1: JOIN

Mensaje recibido:

```irc
JOIN #general
```

Contenido de `Message`:

```cpp
_prefix      = "";
_cmd         = "JOIN";
_params      = ["#general"];
_trailing    = "";
_hasTrailing = false;
```

### Ejemplo 2: PRIVMSG

Mensaje recibido:

```irc
PRIVMSG #general :Hello everyone!
```

Contenido de `Message`:

```cpp
_prefix      = "";
_cmd         = "PRIVMSG";
_params      = ["#general"];
_trailing    = "Hello everyone!";
_hasTrailing = true;
```

### Ejemplo 3: NICK

Mensaje recibido:

```irc
NICK newNickname
```

Contenido de `Message`:

```cpp
_prefix      = "";
_cmd         = "NICK";
_params      = ["newNickname"];
_trailing    = "";
_hasTrailing = false;
```

### Ejemplo 4: USER

Mensaje recibido:

```irc
USER alice 0 * :Alice Smith
```

Contenido de `Message`:

```cpp
_prefix      = "";
_cmd         = "USER";
_params      = ["alice", "0", "*"];
_trailing    = "Alice Smith";
_hasTrailing = true;
```

### Ejemplo 5: Mensaje con prefix

Mensaje recibido:

```irc
:alice!user@localhost JOIN #general
```

Contenido de `Message`:

```cpp
_prefix      = "alice!user@localhost";
_cmd         = "JOIN";
_params      = ["#general"];
_trailing    = "";
_hasTrailing = false;
```

---

## 5. 🔍 Getters disponibles

La clase proporciona cinco getters públicos:

| Getter | Información que devuelve |
|---|---|
| `getPrefix()` | Emisor del mensaje. |
| `getCommand()` | Comando IRC. |
| `getParams()` | Vector de parámetros. |
| `getTrailing()` | Texto del trailing. |
| `hasTrailing()` | Indica si existe trailing. |

Estos métodos permiten que otras partes del servidor consulten el contenido del mensaje sin acceder directamente a sus atributos privados.

---

## 6. ⚠️ Errores y validaciones soportadas

La clase `Message` **no implementa un sistema propio de errores ni realiza validaciones del protocolo IRC**.

Su responsabilidad se limita a almacenar los datos que recibe.

Por tanto, no comprueba:
- Si el comando existe.
- Si el comando tiene todos los parámetros necesarios.
- Si los parámetros tienen un formato correcto.
- Si el prefix es válido.
- Si el trailing está permitido para ese comando.

Estas comprobaciones corresponden al Parser y a la implementación de cada comando.

La clase sí proporciona un constructor por defecto que inicializa todos sus atributos vacíos y establece `_hasTrailing` en `false`.

---

## 7. 🧩 Resumen del funcionamiento

El recorrido completo de un mensaje es:

```text
CLIENT
  |
  | Envía un comando IRC
  v
SERVER
  |
  | Recibe los datos
  v
PARSER
  |
  | Analiza y separa los componentes
  v
MESSAGE
  |
  | Almacena prefix, command, params y trailing
  v
COMMAND HANDLER
  |
  | Consulta los datos mediante getters
  v
EXECUTION
```

**En conclusión:** `Message` funciona como una estructura de almacenamiento que conecta el Parser con los comandos del servidor. Su objetivo es mantener la información de cada mensaje organizada, accesible y protegida mediante atributos privados y getters constantes.