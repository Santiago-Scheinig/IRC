#ifndef MESSAGE_HPP
# define MESSAGE_HPP

class Message
{
	private:
		Message(const Message &other);
		Message &operator=(const Message &other);

	public:
		Message();
		~Message();
};

#endif
