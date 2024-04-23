#include "Clients.hpp"

std::string	Clients::getBuffer()
{
	return (_buffer);
}

void	Clients::setBuffer(std::string buf)
{
	_buffer += buf;
	return;
}

int	Clients::getSock()
{
	return (_sockfd);
}

bool	Clients::getUpdate()
{
	return(_update);
}

void	Clients::setUpdate(bool bin)
{
	_update = bin;
}

std::string	Clients::getPassword()
{
	return (_password);
}

void	Clients::setPassword(std::string str)
{
	_password = str;
	return ;
}

std::string	Clients::getNick()
{
	return (_nick);
}

std::string	Clients::getPing(void)
{
	return (_ping);
}

void	Clients::setPing(std::string string)
{
	_ping = string;
}

std::string Clients::getChannel()
{
	return (_channel);
}

void		Clients::setChannel(std::string	src)
{
	_channel = src;
	return ;
}

std::string	Clients::getMsg()
{
	return (_msg);
}

void	Clients::setMsg(std::string msg)
{
	_msg = msg;
	return;
}

std::string	Clients::getInvite(void)
{
	return _invite;
}

std::string	Clients::getMode()
{
	return _mode;
}

std::string	Clients::getKick()
{
	return _kick;
}

std::string	Clients::getTopic()
{
	return _topic;
}

void		Clients::setTopic(std::string topic)
{
	_topic = topic;
	return;
}
