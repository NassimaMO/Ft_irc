#include "ft_irc.hpp"

int	strtoint(std::string src)
{
	std::stringstream	ss;
	int				dst;

	ss << src;
	ss >> dst;
	return (dst);
}

std::string	inttostr(int src)
{
	std::stringstream	ss;
	std::string			dst;

	ss << src;
	ss >> dst;
	return (dst);
}

int main(int argc, char **argv)
{
	std::string port;
	std::string password;

	if (argc != 3)
		return (std::cerr << "Error: Must have one port and one password" << std::endl, 1);
	port = argv[1];
	password = argv[2];
	if (validArgs(port, password))
		return (1);
	Server server(password, port);
	while (server.getExt() == 0)
	{
		int err;
		err = poll(server.getFds().data(), server.getTotalClients() + 1, 1000);
		if (err < 0)
			return (-1);
		else
		{
			if (server.getFds()[0].revents > 0)
				server.waitClients();
			server.receiveMsgs();
			server.parseMsgs();
			server.sendMsgs();
		}
	}
	//sleep(1);
	//close(socket_client);
	return (0);
}


// socket
// sockaddr_in --> inet_addr, htons
// bind
// listen
// accept
// connect
// recv
// send
