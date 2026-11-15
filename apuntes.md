Si queremos que el servidor no maneje el parseo, cada cleinte deberá de hacerlo. Debido a que cada cliente debe tener su propio parser,
Si hubiese un parser por el que cada cliente debe de pasar, el parser deberia de llamarlo server por lo que no queremos que server tenga esta responsabilidad.

La logica de que cada cliente tenga su parser es predominante, asi cada cliente maneja sus propios mensajes y si lo hace varias veces, siendo esto una utilidad para manejar errores
no fatales, que afectan al cliente y no al servidor.

std::string _inputBuffer;

Client deberia de manjear el inputbuffer, actualemnte lo hace parser. Por lo que esto se deberia de ver si modificarlo.

La gestion del e_num seria responsabilidad del propio parser ya que ni cleinte ni server necesitan conocer esto. solo se devuelve el objeto message a server.