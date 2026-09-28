#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <csignal>
# include <string>

class Config
{
	private:
		int			_port;
		std::string _passwd;

		Config();
		Config(const Config &other);
		Config &operator=(const Config &other);

		static bool _parsePort(const std::string &s, int &port);
		static bool _parsePasswd(const std::string &passwd);

	public:
		Config(int argc, char **argv);
		~Config();

		static void installSignalHandlers();

		int getPort() const;
		const std::string &getPasswd() const;
};

#endif
