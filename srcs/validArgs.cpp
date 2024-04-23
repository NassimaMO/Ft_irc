#include "ft_irc.hpp"


static int	validPort(std::string port)
{
	if (strtoint(port) < 0 || strtoint(port) > 65535)
		return (1);
	return (0);
}
static int validPassword(std::string password)
{
	for (std::string::iterator it = password.begin(); it != password.end(); ++it)
	{
		if (!std::isprint(*it))
			return (std::cout << "Error: Password can't contain non printable character" << std::endl, 1);
	}
	return (0);
}

int validArgs( std::string port, std::string password )
{
	if (validPort(port) || validPassword(password))
		return (1);
	return (0);
}
