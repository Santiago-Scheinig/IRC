# Resultados de pruebas del servidor IRC

**Fecha:** 25 de noviembre de 2026
**Rama:** `santy-dev`
**Binario probado:** compilación temporal de los fuentes actuales
**Cambios de código:** ninguno

## Resumen

| Grupo | Resultado |
|---|---:|
| Compilación estricta C++98 | **Correcta** |
| Smoke test del repositorio (`test_irc.py`) | **5/5 correctos** |
| Netcat: `CAP LS` y registro + `JOIN` | **Correctos** |
| Casos avanzados dirigidos | **64 correctos / 56 fallidos / 1 informativo** |

Los 121 casos avanzados se ejecutaron con un servidor aislado por caso o por
escenario para que un fallo no contaminara los siguientes. Los casos
informativos se separan de los fallos porque el comportamiento no está fijado
como requisito obligatorio.

## Entorno y reproducibilidad

- Se compilaron todos los `.cpp` con las opciones del `Makefile`:
  `-Wall -Wextra -Werror -std=c++98`. El binario de comprobación se dejó en
  `/tmp/ircserv-audit`; no se ejecutó `make re`, ya que limpiaría `ircserv` y
  `obj/`, que ya estaban presentes y sin seguimiento en el worktree.
- `test_irc.py` se ejecutó contra una copia temporal de ese binario y terminó
  con **5/5**: registro de dos clientes, `JOIN` y envío de mensajes al canal.
- Netcat no estaba instalado. La instalación del paquete del sistema requirió
  una contraseña de `sudo`, así que se instaló `netcat-openbsd` 1.229-1 en el
  espacio del usuario, sin cambiar paquetes del sistema:
  `~/.local/bin/nc` → `~/.local/opt/netcat-openbsd-1.229/usr/bin/nc.openbsd`.
  `nc` quedó disponible en `PATH`.
- El intercambio por Netcat verificó tanto `CAP LS` como
  `PASS`/`NICK`/`USER`/`JOIN`. El servidor devolvió `001`–`004`, `JOIN` y
  `331`; no emitió `353`/`366`.

## Inventario de casos

**Leyenda:** ✅ resultado esperado; ❌ resultado incorrecto o comportamiento
requerido no implementado; ℹ️ informativo, no computado como correcto ni
incorrecto.

### Correctos (64)

| ID | Caso comprobado |
|---|---|
| A01 | `PASS` correcto, `NICK` y `USER` producen `001`–`004` |
| A02 | Contraseña incorrecta devuelve `464` y no completa el registro |
| A04 | `PASS` repetido devuelve `462` |
| A06 | `CAP LS` antes del registro |
| A07 | `CAP LIST` después del registro |
| A08 | `CAP REQ` de capacidad no soportada devuelve `NAK` |
| A09 | `CAP END` se acepta sin respuesta |
| A11 | Subcomando `CAP` inválido devuelve `410` |
| A14 | Sintaxis inválida de nickname devuelve `432` |
| A15 | Nickname duplicado devuelve `433` |
| A17 | `USER` sin parámetros devuelve `461` |
| A18 | `USER` después del registro devuelve `462` |
| A19 | `JOIN` con contraseña aceptada, pero sin registro completo, devuelve `451` |
| A21 | `JOIN` a nombre sin prefijo de canal devuelve `403` |
| A23 | `JOIN #uno,#dos` une a ambos canales |
| A26 | `PART` por cliente que no pertenece devuelve `442` |
| A27 | `PART` elimina pertenencia y difunde el motivo |
| A30 | `PRIVMSG` sin texto devuelve `412` |
| A31 | `PRIVMSG` privado llega al usuario destino |
| A32 | `PRIVMSG` a nickname desconocido devuelve `401` |
| A33 | `PRIVMSG` a canal se difunde a los otros miembros, no al emisor |
| A34 | `PRIVMSG` al canal por un no miembro devuelve `404` |
| A38 | Un operador puede establecer y difundir `TOPIC` |
| A45 | `MODE +o` da privilegio a un miembro existente |
| A47 | `KICK` sin parámetros devuelve `461` |
| A48 | `KICK` por un no miembro devuelve `442` |
| A49 | `KICK` por un no operador devuelve `482` |
| A50 | `KICK` desde fuera del canal devuelve `442` |
| A52 | `KICK` válido se difunde y elimina al destino del canal |
| A53 | `INVITE` sin parámetros devuelve `461` |
| A54 | `INVITE` por un no miembro devuelve `442` |
| A56 | `INVITE` a miembro existente devuelve `443` |
| A59 | Comando desconocido devuelve `421` |
| A63 | Línea TCP fragmentada entre escrituras se reconstruye correctamente |
| A64 | Varias líneas IRC en una sola escritura TCP se procesan |
| A65 | Ocho clientes registrados simultáneamente pueden unirse y recibir difusión |
| A67 | El servidor acepta una conexión nueva después de un `QUIT` |
| A69 | Línea de 513 bytes sin terminador devuelve `417` |
| A70 | Línea malformada no termina el proceso servidor |
| A71 | `SIGINT` termina el servidor limpiamente |
| E01–E06 | `PART`, `PRIVMSG`, `TOPIC`, `MODE`, `KICK` e `INVITE` devuelven `451` antes del registro completo |
| E07 | `MODE` por un no miembro devuelve `442` |
| E08 | `MODE` por un no operador devuelve `482` |
| E09 | `MODE +o` sin nickname destino devuelve `461` |
| E10 | Consulta de `TOPIC` por no miembro devuelve `442` |
| E13 | `PART` del último miembro permite recrear el canal |
| E16 | Los comandos se aceptan en minúsculas |
| E17 | Argumentos de inicio inválidos (sin argumentos, puerto cero, puerto no numérico o contraseña vacía) son rechazados |
| E18 | Netcat completa registro y `JOIN`; recibe `001`–`004`, `JOIN` y `331` |
| C01 | `CAP LS 302` devuelve la lista vacía con formato `CAP LS` |
| N01 | Netcat envía `CAP LS` y recibe `:irc.42.com CAP * LS :` |
| X08 | Repetir `JOIN` al mismo canal no duplica el evento ni la pertenencia |
| E20 | `CAP ` con parámetro vacío devuelve `461` |
| E21 | `NICK ` con nickname vacío devuelve `431` |
| E22 | `JOIN ` con canal vacío devuelve `461` |
| E23 | `PART ` con canal vacío devuelve `461` |
| E24 | `PRIVMSG ` con destinatario vacío devuelve `411` |
| E25 | `TOPIC ` con canal vacío devuelve `461` |
| E26 | `MODE ` con objetivo vacío devuelve `461` |

### Fallidos (56)

| ID | Caso comprobado | Resultado observado |
|---|---|---|
| A03 | `PASS` sin parámetros | Devuelve `464` y cierra; se esperaba `461`. El parser trata el comando desnudo como si fuese un parámetro. |
| A05 | Registrar con `NICK`/`USER` sin `PASS` | Se acepta y se emiten `001`–`004`. |
| A10 | `CAP` sin subcomando | Devuelve `410`, no `461`; relacionado con el parseo de comandos desnudos. |
| A12 | Forma `CAP * LS` | Devuelve `410`; no reconoce la forma documentada. |
| A13 | `NICK` sin parámetros | No responde y almacena `NICK` como nickname. |
| A16 | Cambio de `NICK` registrado | El propio usuario recibe el evento, pero los miembros del canal no. |
| A20 | `JOIN` sin canal | Devuelve `403` para `JOIN`, no `461`; el parser usa el verbo como parámetro. |
| A22 | `JOIN` válido | Devuelve `JOIN` y `331`, pero faltan `353` y `366`. |
| A24 | `JOIN 0` con canal compartido | No difunde `PART`; emite `QUIT` y elimina el canal aunque quedan miembros. |
| A25 | `PART` sin canal | El cliente se desconecta en vez de recibir `461`. |
| A28 | `PART` de canal inexistente | El cliente se desconecta; no recibe `403`. |
| A29 | `PRIVMSG` sin destinatario | Devuelve `412` en vez de `411`; el parser trata `PRIVMSG` como parámetro. |
| A35 | `PRIVMSG` a canal inexistente | El cliente se desconecta; no recibe `403`. |
| A36 | `TOPIC` sin canal | El cliente se desconecta en vez de recibir `461`. |
| A37 | Consulta de topic vacío | Devuelve `331 alice :`; omite el parámetro del canal. |
| A39 | Miembro no operador cambia topic con `+t` desactivado | Devuelve `482`. El handoff restringe el cambio solo cuando el modo `+t` está activo. |
| A40 | `MODE` sin objetivo | El cliente se desconecta en vez de recibir `461`. |
| A41 | Consulta `MODE #canal` | No devuelve estado ni respuesta. |
| A42 | `MODE +i` y posterior intento de entrada | El modo no se anuncia ni se aplica; el nuevo cliente entra. |
| A43 | `MODE +k` y entrada con clave incorrecta | El modo no se anuncia ni se aplica; la entrada se permite. |
| A44 | `MODE +l` y límite excedido | El modo no se anuncia ni se aplica; el segundo cliente entra. |
| A46 | `MODE +o` con nickname inexistente | El proceso servidor termina con `SIGSEGV` (`-11`), sin `441`. |
| A51 | `KICK` con nickname inexistente | El proceso servidor termina con `SIGSEGV` (`-11`), sin `441`. |
| A55 | `INVITE` por no operador | El evento se envía al destino; falta la comprobación de operador definida por el handoff. |
| A57 | `INVITE` a nickname inexistente | El proceso servidor termina con `SIGSEGV` (`-11`), sin `401`. |
| A58 | `INVITE` válido | No se envía el `341` al invitador; el evento al destino sí se genera. |
| A60 | `PING :token` | Devuelve `421`; no existe respuesta `PONG`. |
| A61 | `WHO` a nickname conocido/desconocido | Devuelve `421`; no se generan `352`/`315`. |
| A62 | `WHOIS` a nickname conocido | Devuelve `421`; no se generan `311`/`312`/`318`. |
| A66 | `QUIT :motivo` en canal | A los compañeros se les anuncia `QUIT :Client Quit`, no el motivo solicitado. |
| A68 | Desconexión TCP abrupta y nuevo cliente en el canal | No se retira la pertenencia antigua; el nuevo cliente puede no recibir su `JOIN` y el miembro restante no puede usar correctamente el canal. |
| E11 | Consulta de topic establecido | Devuelve `332 a :hello`; falta el nombre del canal. |
| E12 | `KICK` a usuario existente fuera del canal | Devuelve `441 a :b`; faltan parámetros de canal y el formato IRC completo. |
| E14 | `QUIT` de miembro con otros usuarios en el canal | El servidor sigue vivo, pero el miembro restante pierde acceso al canal y recibe EOF al intentar usarlo. |
| M01 | `PING :token` | Devuelve `421 alice :PING`, no `PONG`. |
| M02 | `PING token` | Devuelve `421 alice :PING`, no `PONG`. |
| M03 | `WHO` conocido | Devuelve `421 alice :WHO`. |
| M04 | `WHO` desconocido | Devuelve `421 alice :WHO`, sin terminador `315`. |
| M05 | `WHOIS` conocido | Devuelve `421 alice :WHOIS`. |
| M06 | `WHOIS` desconocido | Devuelve `421 alice :WHOIS`, sin `401`/`318`. |
| M07 | `MODE +t` | Sin respuesta ni cambio de estado. |
| M08 | `MODE -i` | Sin respuesta ni cambio de estado. |
| M09 | `MODE -t` | Sin respuesta ni cambio de estado. |
| M10 | `MODE -k` | Sin respuesta ni cambio de estado. |
| M11 | `MODE -o` | Sin respuesta ni cambio de estado. |
| M12 | `MODE -l` | Sin respuesta ni cambio de estado. |
| C02 | Forma `CAP * END` | Devuelve `410`; no acepta la forma documentada. |
| X01 | `TOPIC` a canal inexistente | Cierra la conexión; no devuelve `403`. |
| X02 | `MODE` a canal inexistente | Cierra la conexión; no devuelve `403`. |
| X03 | `KICK` en canal inexistente | Cierra la conexión; no devuelve `403`. |
| X04 | `INVITE` a canal inexistente | Cierra la conexión; no devuelve `403`. |
| X05 | `INVITE` válido y destinatarios | El destino recibe `INVITE`, pero el invitador no recibe `341`. |
| X06 | `MODE +o` a nickname existente fuera del canal | Devuelve `441 alice :bob`; faltan canal y formato numérico completo. |
| X07 | Letra de modo no soportada (`+z`) | No devuelve `472` ni otra respuesta. |
| X09 | `JOIN 0` siendo único miembro | No envía evento `PART` ni respuesta al cliente. |
| E19 | `PASS ` con parámetro vacío | Aunque devuelve `461`, el estado de autenticación se activa; después `NICK`/`USER` bastan para obtener `001`–`004`. |

### Informativo (1)

| ID | Caso | Resultado |
|---|---|---|
| E15 | `QUIT` antes de enviar `PASS` | Devuelve `464` por la puerta global de contraseña. El handoff indica que `QUIT` *puede* permitirse antes del registro, así que se registra como política y no se cuenta como fallo obligatorio. |

## Resultados adicionales de ejecución

- Los casos de destino inexistente en `MODE +o`, `KICK` e `INVITE` fueron
  repetidos por separado para verificar el estado del proceso: los tres
  terminaron con código `-11` (`SIGSEGV`).
- Se comprobaron los canales inexistentes en `TOPIC`, `MODE`, `KICK` e
  `INVITE`: en todos, el cliente recibió EOF en vez del numeric `403`.
- `INVITE` sí envía el evento al usuario invitado, pero no confirma la acción al
  emisor con `341`.

## Hallazgos prioritarios

1. **Autenticación:** se puede completar el registro sin contraseña; `PASS`
   desnudo se interpreta incorrectamente y cierra con `464`.
2. **Caídas del proceso:** `MODE +o`, `KICK` e `INVITE` con nickname destino
   inexistente terminan el servidor con `SIGSEGV` (`-11`).
3. **Parser de comandos sin argumentos:** comandos desnudos como `NICK`, `JOIN`,
   `PART`, `PRIVMSG`, `TOPIC` y `MODE` producen resultados incorrectos, cierre
   de cliente o falta de respuesta.
4. **Ciclo de vida del canal:** una desconexión abrupta deja punteros de miembro
   obsoletos; el `QUIT` normal puede borrar del servidor un canal que aún tiene
   miembros.
5. **Modos:** solo se observó implementación parcial de `+o`; `i`, `t`, `k`,
   `l` y las operaciones de retirada no funcionan.
6. **Protocolo incompleto:** faltan `PING`, `WHO`, `WHOIS`, `341` para
   `INVITE`, respuestas de nombres y parámetros de canal en algunos numerics.

## Limitaciones de esta ejecución

- No se forzó una escritura parcial del kernel ni se simuló agotamiento real de
  descriptores; se verificó la lógica de buffers mediante pruebas del código y
  escenarios TCP, pero esos casos requieren una prueba de estrés dedicada.
- No se probó HexChat, irssi o WeeChat.
- Los tres casos con `SIGSEGV` se aislaron en procesos de servidor separados;
  no se modificó ni reinició el servidor del usuario.
- La suite es una comprobación puntual de esta rama. No sustituye una
  evaluación oficial del subject.
