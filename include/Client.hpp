#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <string>

// Owner: track A (networking), state machine driven by track B (registration).
// One instance per connected fd. Buffers are strings on purpose: recv() may
// deliver partial or multiple lines per call, so Server appends raw bytes
// to _inputBuffer and pulls out complete "\r\n"-terminated lines to parse.
// Same idea in reverse for _outputBuffer on non-blocking writes.
class Client
{
	public:
		enum RegistrationState
		{
			UNREGISTERED,
			NICK_SET,
			USER_SET,
			REGISTERED
		};

		explicit Client(int fd);
		~Client();
		Client(const Client& other);
		Client& operator=(const Client& other);

		int					getFd() const;

		const std::string&	getNickname() const;
		void				setNickname(const std::string& nickname);

		const std::string&	getUsername() const;
		void				setUsername(const std::string& username);

		const std::string&	getRealname() const;
		void				setRealname(const std::string& realname);

		RegistrationState	getState() const;
		void				setState(RegistrationState state);

		bool				hasProvidedPassword() const;
		void				setProvidedPassword(bool value);

		std::string&		inputBuffer();
		std::string&		outputBuffer();

	private:
		Client();

		int					_fd;
		std::string			_nickname;
		std::string			_username;
		std::string			_realname;
		RegistrationState	_state;
		bool				_providedPassword;
		std::string			_inputBuffer;
		std::string			_outputBuffer;
};

#endif
