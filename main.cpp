#include "Config.hpp"
#include "Server.hpp"
#include <iostream>
#include <exception>

int	main(int argc, char** argv)
{
	Config::installSignalHandlers();
	try
	{
		Config	config(argc, argv);
		Server	server(config.getPort(), config.getPasswd());

		server.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << "ircserv: " << e.what() << std::endl;
		return (1);
	}
	return (0);
}
