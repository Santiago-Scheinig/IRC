# Parser

## Objetivo

`Parser` convierte bytes acumulados del protocolo IRC en mensajes estructurados. No conoce clientes, canales, permisos ni sockets.

```text
Client::inputBuffer -> Parser -> Message
```

## Entrada y salida

Entrada conceptual:

```text
"PRIVMSG #general :Hola a todos\r\n"
```

Salida conceptual:

```text
command     = "PRIVMSG"
params      = ["#general"]
trailing    = "Hola a todos"
hasTrailing = true
```

La API concreta debe acordarse con el equipo. Puede ser una función que reciba una línea completa o una operación que consuma un buffer y produzca cero, uno o varios mensajes.

## Orden de construcción

1. Definir el límite máximo de línea.
2. Definir qué ocurre con una línea que supera ese límite.
3. Buscar siempre `\r\n`, no solo `\n`.
4. Conservar fragmentos incompletos.
5. Procesar varias líneas en una sola lectura.
6. Extraer el prefijo opcional.
7. Extraer el comando.
8. Separar parámetros normales.
9. Tratar `:` como inicio del trailing.
10. Normalizar el comando a mayúsculas.
11. Devolver mensajes o errores según la política acordada.

## Gramática práctica

Una línea IRC tiene esta forma:

```text
[:prefix] COMMAND [param ...] [:trailing]\r\n
```

Reglas:

- El prefijo solo existe si la línea empieza por `:`.
- El comando termina en el primer espacio después del inicio.
- Los parámetros normales se separan por espacios.
- El primer `:` que inicia el trailing consume el resto de la línea.
- Los espacios del trailing se conservan.
- El terminador `\r\n` no forma parte del contenido del mensaje.

## Casos TCP obligatorios

### Fragmento incompleto

Primera lectura:

```text
"PRIVMSG #gen"
```

No se produce ningún mensaje. Se conserva el buffer.

Segunda lectura:

```text
"eral :Hola\r\n"
```

Ahora se parsea la línea completa.

### Varias líneas

Entrada recibida en una sola lectura:

```text
"PING :one\r\nPING :two\r\n"
```

El parser debe producir dos mensajes y dejar el buffer vacío.

### Línea excesiva

Una línea sin `\r\n` que supera el límite debe seguir una política explícita: rechazarla, desconectar al cliente o descartar hasta el siguiente terminador. La decisión debe documentarse y probarse.

## Lo que no debe hacer

- No llamar a `recv()` ni `send()`.
- No acceder a miembros privados de `Client`.
- No consultar `Server`.
- No crear respuestas numéricas.
- No interpretar el significado de `JOIN`, `PASS` o `PRIVMSG`.

## Pruebas mínimas

- Línea simple con `PING`.
- Comando recibido en minúsculas y salida en mayúsculas.
- Línea fragmentada entre dos lecturas.
- Dos o más líneas en una lectura.
- Trailing con espacios internos y finales significativos.
- Prefijo completo.
- Línea sin `\r\n`.
- Línea que supera el límite.
