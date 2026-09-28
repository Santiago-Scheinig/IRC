# Replies

## Objetivo

`Replies` construye respuestas IRC formateadas. Es una utilidad sin estado: no guarda clientes ni canales y no decide cuándo enviar.

```text
comando o evento -> Replies -> texto IRC -> Client::outputBuffer
```

## Responsabilidades

- Formatear respuestas numéricas.
- Construir prefijos del servidor cuando proceda.
- Añadir siempre `\r\n` al final.
- Mantener un formato uniforme para todos los comandos.
- Evitar que cada comando concatene respuestas a mano.

## Orden de construcción

1. Definir cómo se representa el nombre del servidor.
2. Definir una función común para el prefijo.
3. Definir una función común para el terminador `\r\n`.
4. Implementar primero respuestas de registro y errores básicos.
5. Añadir respuestas de `PING/PONG`.
6. Añadir respuestas de canales y mensajería.
7. Probar exactamente el texto generado, incluyendo espacios y terminador.

## Ejemplos

### Bienvenida

Contrato conceptual:

```text
001 <nick> :Welcome to the IRC server\r\n
```

### Nickname ocupado

```text
433 <nick> <requestedNick> :Nickname is already in use\r\n
```

### PONG

Entrada:

```text
PING :hello
```

Respuesta conceptual:

```text
PONG :hello\r\n
```

La forma final del prefijo debe acordarse con `Server` y con el cliente de referencia.

## Relación con las otras clases

`Replies` no debe recibir bytes de red directamente.

Flujo correcto:

```text
Parser
  -> Message
CommandRegistry
  -> comando concreto
comando
  -> Replies crea texto
Client
  -> appendOutput(texto)
Server
  -> send() cuando POLLOUT está listo
```

## Errores relevantes

La implementación deberá cubrir, según el comando:

```text
401 403 431 433 442 451 461 462 464
471 472 473 475 482
```

Cada error debe tener:

- código numérico correcto;
- nickname o canal requerido;
- texto final adecuado;
- terminador `\r\n`.

## Lo que no debe hacer

- No llamar a `send()`.
- No modificar `Client` directamente.
- No consultar sockets.
- No decidir permisos.
- No duplicar la gramática del parser.

## Pruebas mínimas

- Cada respuesta termina en `\r\n`.
- Los códigos numéricos aparecen en la posición correcta.
- El nickname, canal y parámetros se insertan sin alterar espacios.
- Una respuesta de error no imprime contraseñas.
- Dos respuestas consecutivas pueden añadirse al `outputBuffer` sin perder orden.
