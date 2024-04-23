#include "Clients.hpp"
#include "ft_irc.hpp"

Clients::Clients(int socket_client) : _sockfd(socket_client)
{
	_update = 0;
	_password = "";
	_nick = "";
	_name = "";
	_buffer = "";
	_channel = "";
	_mode = "";
	_ping = "";
	_msg = "";
	_answerMsgs = 0;
}

Clients::~Clients()
{

}

void	Clients::parseError(void)
{
	std::cout << "Command not found: " << _buffer << std::endl;
	_buffer.clear();
}

void	Clients::parse(std::string command, int i, std::string &src)
{
	std::string bufferPart(_buffer.begin(), _buffer.begin() + command.length());

	if (bufferPart == command)
	{
		ssize_t it = command.length();
		while (_buffer[it] != '\0' && (_buffer[it] != '\r' || _buffer[it + 1] != '\n'))
			it++;
		if (_buffer[it] == '\r' && _buffer[it + 1] == '\n')
		{
			std::cout << "\033[1;33mParse\033[0m : Command :" << command << "." << std::endl;
			std::cout << _buffer.substr(command.length() + 1, it - (command.length() + 1)) << std::endl;
			if (command != "CAP LS" && command != "QUIT")
			{
				src = _buffer.substr(command.length() + 1, it - (command.length() + 1));
				std::cout << "\033[1;33mParse\033[0m : Args :" << src << "." << std::endl;
			}
			_buffer.erase(0, it + 2);
			_answerMsgs = i;
		}
	}
}

void	Clients::parseMsgs()
{
	std::string command[] = {"CAP LS", "USER", "MODE", "PING", "JOIN", "PRIVMSG", "INVITE", "KICK", "TOPIC", "QUIT", "PASS", "NICK", "WHOIAM"};
	std::string	*src[] = {NULL, &_name, &_mode, &_ping, &_channel, &_msg, &_invite, &_kick, &_topic, NULL, &_password, &_nick, NULL};

	for (int i = 0; i < 12; i++)
		parse(command[i], i, *src[i]);
	if (!_buffer.empty() && _buffer[_buffer.length() - 2] == '\r' && _buffer[_buffer.length() - 1] == '\n')
		parseError();
	_update = 0;
}

void	Clients::sendMsgs(void)
{
	return;
}

int		Clients::getAnswerMsgs()
{
	return (_answerMsgs);
}

void		Clients::setAnswerMsgs(int answerMsgs)
{
	_answerMsgs = answerMsgs;
	return;
}
