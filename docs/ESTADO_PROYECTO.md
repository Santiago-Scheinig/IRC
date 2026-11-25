# Estado actual del proyecto frente al subject

**Fecha de revisión:** 25 de noviembre de 2026
**Rama revisada:** `santy-dev`

Este documento contrasta los requisitos de `en.subject.pdf`, la documentación
del repositorio y el código presente en la rama. Es una evaluación del estado
observado, no una declaración de aprobación del subject.

## Criterio de estado

- **Cumplido:** implementación comprobada con el alcance indicado.
- **Parcial:** hay implementación, pero faltan comportamiento requerido o
  pruebas suficientes.
- **Pendiente:** falta una parte importante o la implementación actual falla.

## Objetivos del subject y estado

| Objetivo | Estado | Comprobación y brecha |
|---|---|---|
| Entregar `ircserv` compilable en C++98 con `Makefile` y argumentos `<port> <password>`. | **Cumplido** | El `Makefile` configura `-std=c++98 -Wall -Wextra -Werror`; una compilación directa de todos los fuentes con esas opciones terminó correctamente. `Config` valida cantidad y formato de argumentos. |
| Aceptar varios clientes sin bloquear el servicio: I/O no bloqueante y un único `poll()` (o equivalente) para las operaciones de red. | **Parcial** | La prueba avanzada mantuvo ocho clientes conectados, unidos al mismo canal y verificó la difusión de mensajes. Siguen fallando la limpieza de clientes desconectados y el mantenimiento de canales tras `QUIT`; no se forzaron escrituras parciales de kernel ni agotamiento de descriptores. |
| Autenticar y registrar clientes con contraseña, nickname y username antes de permitirles participar. | **Pendiente — prioridad crítica** | Se confirmó que `NICK`/`USER` permiten obtener `001`–`004` sin `PASS`. `PASS ` vacío devuelve `461` pero activa el estado de autenticación; el comando desnudo `PASS` termina en `464` y cierra la conexión. |
| Permitir registro de nickname y username e informar del registro completado. | **Parcial** | Existen handlers `NICK` y `USER` y se emiten numerics de bienvenida, pero el orden y las condiciones no aseguran el flujo de registro exigido. La prueba básica solo confirma el caso feliz. |
| Permitir unirse a canales y conversar por canal y mediante mensajes privados. | **Parcial** | Pasan los recorridos básicos, `JOIN` por lista, mensajes privados y difusión a ocho clientes. `JOIN` no devuelve `353`/`366`; `JOIN 0` no genera `PART`; comandos con canales inexistentes pueden cerrar al cliente en vez de enviar `403`. |
| Proporcionar comandos de canal y operador: `KICK`, `INVITE`, `TOPIC` y `MODE`, incluidos los modos `i`, `t`, `k`, `o` y `l`. | **Pendiente** | `MODE +o` funciona para un miembro existente; `i`, `t`, `k`, `l`, retiradas de modo y consulta de modos no funcionan. `INVITE` envía el evento, pero omite `341` y no exige operador. Destinos inexistentes en `MODE +o`, `KICK` e `INVITE` causaron `SIGSEGV`. |
| Mantener la estabilidad y no terminar inesperadamente ante errores de clientes o condiciones límite. | **Pendiente — prioridad crítica** | Se reprodujeron tres cierres del proceso con `SIGSEGV` (`-11`) y la desconexión abrupta deja pertenencias obsoletas. Un `QUIT` puede hacer que otros miembros pierdan acceso al canal. La prueba de ocho clientes pasó, pero no se probaron escrituras parciales de kernel ni agotamiento real de descriptores. |
| Documentar el proyecto y cómo compilarlo y ejecutarlo. | **Parcial** | El `README.md` explica la arquitectura, pero no ofrece un ejemplo completo de ejecución ni resume el comportamiento soportado. Varias páginas describen un diseño objetivo que no coincide con la implementación actual. |
| Verificar compatibilidad con un cliente IRC real. | **Pendiente** | No se ha documentado una prueba con HexChat, irssi o WeeChat. La prueba disponible usa sockets TCP propios y cubre únicamente un recorrido básico. |

## Objetivos cumplidos y comprobados

- **Compilación estricta:** todos los fuentes compilaron con `-Wall -Wextra
  -Werror -std=c++98`.
- **Recorrido TCP básico:** `test_irc.py` pasó **5/5** comprobaciones: registro
  de dos clientes, creación/unión a canal y entrega de un mensaje a otro
  miembro.
- **Pruebas avanzadas:** se ejecutaron **121 casos dirigidos**: **64 correctos**,
  **56 fallidos** y **1 informativo**. El inventario con resultado y
  observaciones de cada caso está en
  [RESULTADOS_PRUEBAS.md](./RESULTADOS_PRUEBAS.md).
- **Netcat:** `netcat-openbsd` se instaló en el espacio del usuario, sin
  modificar paquetes del sistema. `nc` verificó el registro y un `JOIN` real;
  la respuesta observada todavía omite la lista de nombres `353`/`366`.
- **Base de transporte presente:** hay un bucle `poll()`, sockets no
  bloqueantes, buffers de entrada/salida y conservación del resto de una
  escritura parcial.
- **Configuración de inicio:** `Config` requiere puerto y contraseña, y
  comprueba su formato.

Estos resultados no demuestran el cumplimiento integral del subject. En
particular, el smoke test no prueba contraseña incorrecta, permisos, modos,
limpieza tras desconexión ni resistencia a errores.

## Trabajo prioritario

### P0 — Bloqueantes para una evaluación fiable

1. **Corregir el parser de comandos sin parámetros.** Un comando IRC desnudo se
   está almacenando como si el propio verbo fuese un parámetro; esto provoca
   numerics erróneos, comandos silenciosos y desconexiones.
2. **Corregir el registro y la autenticación.** `PASS ` vacío activa
   autenticación pese al `461`, y `NICK`/`USER` permiten registro sin
   contraseña. Mantener una condición única de registro para todos los
   comandos protegidos.
3. **Hacer segura la gestión de errores y los punteros.** Comprobar destinos
   inexistentes antes de usarlos: `MODE +o`, `KICK` e `INVITE` con nickname
   desconocido tumbaron el proceso con `SIGSEGV`.
4. **Corregir la limpieza de clientes y canales.** Al desconectar, retirar el
   cliente de todos los canales, notificar a los miembros restantes, borrar
   los canales vacíos y eliminar la conexión sin dejar punteros obsoletos.

### P1 — Funcionalidad obligatoria de canales

5. **Completar `JOIN` y `PART`.** Aplicar clave, invite-only y límite; impedir
   miembros duplicados; devolver los numerics de nombres; verificar
   correctamente la salida y el borrado de canales.
6. **Completar privilegios y modos.** Implementar y probar `i`, `t`, `k`, `o`
   y `l`, incluyendo consulta de modos y casos de usuario que pertenece o no
   al canal.
7. **Completar `KICK` e `INVITE`.** Validar existencia, pertenencia y operador;
   emitir las respuestas y eventos de protocolo y actualizar el estado del
   canal de forma consistente.
8. **Revisar el formato de respuestas numéricas.** Separar parámetros
   intermedios del texto trailing y comprobar códigos, argumentos y
   terminadores CRLF con pruebas de protocolo.

### P2 — Cobertura y cierre de calidad

9. **Mantener y ampliar la suite TCP:** ya están registrados los casos
   ejecutados en [RESULTADOS_PRUEBAS.md](./RESULTADOS_PRUEBAS.md). Quedan por
   probar escrituras parciales reales, agotamiento de descriptores y un cliente
   IRC real.
10. **Probar con un cliente IRC real** y registrar qué flujos se validaron.
11. **Actualizar README y documentación de diseño** para que reflejen la
    arquitectura real, las funciones implementadas y el modo de ejecución.
12. **Completar `MILESTONES.md`**: actualmente las secciones de M1–M3 no tienen
    tareas asignadas y M4 conserva casillas sin marcar; por tanto, no es una
    fuente fiable del progreso actual.

## Alcance adicional del proyecto

Además de los mínimos del subject, el [handoff técnico](../AGENT_HANDOFF.md)
define expectativas de compatibilidad IRC y una lista más amplia de comandos.
Conviene seguirlas, pero no se deben confundir con los requisitos mínimos del
subject:

- `PING` no está implementado; tampoco están `WHO` ni `WHOIS`.
- El handoff requiere difundir los cambios de `NICK` a los compañeros de canal;
  la implementación actual solo responde al propio cliente.
- El handoff documenta casos de protocolo adicionales —como respuestas
  numéricas completas, `CAP`, listas de `JOIN`, `JOIN 0` y eventos de
  `INVITE`— que deben probarse antes de considerar completa la compatibilidad
  del proyecto.

## Diferencias entre documentación e implementación

- `README.md` presenta `Client`, `Replies` y comandos en `src/commands/`; en
  esta rama el estado de conexión vive en `Socket`, las respuestas se generan
  desde `Socket`/`ACommand` y los handlers están agrupados en
  `CommandRegistry.cpp`.
- `docs/ARCHITECTURE.md` propone que `Channel` represente el estado del canal y
  que el servidor coordine el transporte; la implementación actual hace que
  `Channel::broadcast()` llame directamente a `Socket::queueOutput()`.
- `docs/MESSAGE.md` incluye una decisión pendiente sobre `IrcMessage` que ya
  no corresponde al código actual, donde `Message` está implementado y lo
  consume `Parser`.
- `docs/MILESTONES.md` es una plantilla inicial, no un registro actualizado
  de objetivos completados.

Estas discrepancias documentales no sustituyen los fallos funcionales P0/P1,
pero conviene resolverlas para que el equipo pueda usar la documentación como
contrato de trabajo.

## Alcance de la comprobación

Se revisaron `en.subject.pdf`, `README.md`, los documentos de `docs/`, los
headers y fuentes actuales, y `test_irc.py`. La compilación de comprobación se
emitió en `/tmp` para no sobrescribir el ejecutable ni los objetos existentes.
`make` indicó que no había objetivos pendientes de reconstrucción; por ello,
la verificación de compilación independiente se hizo compilando todos los
fuentes con las opciones del proyecto. No se modificaron fuentes ni se
consideran validadas las rutas no cubiertas por las pruebas indicadas.

## Referencias del repositorio

- [Subject del proyecto](../en.subject.pdf)
- [README](../README.md)
- [Handoff técnico](../AGENT_HANDOFF.md)
- [Arquitectura](./ARCHITECTURE.md)
- [Hitos](./MILESTONES.md)
- [Parser](./PARSER.md)
- [Replies](./REPLIES.md)
- [Message](./MESSAGE.md)
- [Prueba TCP disponible](../test_irc.py)
- [Resultados de pruebas avanzadas](./RESULTADOS_PRUEBAS.md)
