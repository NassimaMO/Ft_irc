#include "ft_irc.hpp"

std::vector<struct pollfd>  &Server::getFds(void)
{
	return (_fds);
}

std::string	&Server::getPort(void)
{
	return (_port);
}

std::string	&Server::getPassword(void)
{
	return (_password);
}

int Server::getTotalClients(void)
{
	return (_totalClients);
}

int Server::getExt(void)
{
	return (_ext);
}
