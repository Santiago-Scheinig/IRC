# Message

## Objetivo

`Message` representa una orden IRC ya interpretada. Es un valor de datos: contiene información, pero no actúa sobre sockets, clientes o canales.

La documentación de arquitectura define el contrato conceptual como:

```text
texto IRC -> Parser -> Message
```

## Responsabilidades

Un mensaje debe poder representar:

- un prefijo opcional;
- un comando normalizado;
- parámetros separados por espacios;
- un parámetro trailing que puede contener espacios;
- si existe o no trailing.

Ejemplo:

```text
PRIVMSG #general :Hola a todos\r\n
```

Resultado:

```text
prefix      = ""
command     = "PRIVMSG"
params      = ["#general"]
trailing    = "Hola a todos"
hasTrailing = true
```

## Orden de construcción

1. Fijar con el equipo los campos definitivos del mensaje.
2. Mantener el tipo como datos, sin dependencias de `Server` o `Channel`.
3. Definir constructor y destructor siguiendo la OFC del repositorio.
4. Añadir constructores útiles solo si el contrato del `Parser` los necesita.
5. Añadir getters constantes si el dispatcher debe leer el mensaje sin modificarlo.
6. Probar copias y mensajes vacíos o incompletos según la política acordada.

## Decisión pendiente del contrato actual

El repositorio contiene `IrcMessage` en `Parser.hpp`, mientras `Message.hpp` todavía no define sus campos. Antes de implementar la lógica definitiva hay que acordar si:

- `IrcMessage` se convierte en `Message` y vive en `Message.hpp`; o
- `IrcMessage` permanece como estructura auxiliar y `Message` se convierte en una clase envolvente.

No deben coexistir dos representaciones distintas sin una razón clara.

## Ejemplos de parsing

### PING

Entrada:

```text
PING :hello\r\n
```

Resultado:

```text
command     = "PING"
params      = []
trailing    = "hello"
hasTrailing = true
```

### JOIN

Entrada:

```text
JOIN #general\r\n
```

Resultado:

```text
command     = "JOIN"
params      = ["#general"]
trailing    = ""
hasTrailing = false
```

### Prefijo

Entrada:

```text
:nick!user@host PRIVMSG #general :Hola\r\n
```

Resultado:

```text
prefix      = "nick!user@host"
command     = "PRIVMSG"
params      = ["#general"]
trailing    = "Hola"
hasTrailing = true
```

## Lo que no debe hacer

- No leer desde sockets.
- No modificar `Client`.
- No validar permisos.
- No construir respuestas.
- No decidir si un comando existe.

## Pruebas mínimas

- Comparar un mensaje producido desde una línea simple.
- Verificar que el comando se normaliza a mayúsculas.
- Verificar que el trailing conserva todos sus espacios internos.
- Verificar que un mensaje con prefijo conserva el prefijo sin `:`.
- Verificar que una línea sin trailing marca `hasTrailing = false`.
