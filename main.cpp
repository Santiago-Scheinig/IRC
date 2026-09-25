#include "Config.hpp"
#include "Server.hpp"
#include <iostream>
#include <exception>

int	main(int argc, char** argv)
{
	try
	{
		Config	config(argc, argv);
		Server	server(config);

		server.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << "ircserv: " << e.what() << std::endl;
		return (1);
	}
	return (0);
}
