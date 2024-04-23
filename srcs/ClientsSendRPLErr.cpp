# include "ft_irc.hpp"

//RPL: successful registration
void Clients::sendRPL001(void) //WELCOME
{
	std::string	RPL001 = ":127.0.0.1 001 " + _nick + " Welcome to the Internet Relay Network, " + _nick + "\r\n";
	send(_sockfd, RPL001.c_str(), RPL001.length(), MSG_DONTWAIT);
}

void Clients::sendRPL002(void) //YOURHOST
{
	std::string	RPL002 = ":127.0.0.1 002 " + _nick + " Your host is nahima, running version 1.0\r\n";
	send(_sockfd, RPL002.c_str(), RPL002.length(), MSG_DONTWAIT);
}

void Clients::sendRPL003(void) //CREATED
{
	std::string	RPL003 = ":127.0.0.1 003 " + _nick + " This server was created today\r\n";
	send(_sockfd, RPL003.c_str(), RPL003.length(), MSG_DONTWAIT);
}

void Clients::sendRPL004(void) //MYINFO
{
	std::string	RPL004 = ":127.0.0.1 004 " + _nick + " nahima version 1.0 " + _mode + "\r\n";
	send(_sockfd, RPL004.c_str(), RPL004.length(), MSG_DONTWAIT);
}

void Clients::sendPong(void) //PONG: NEED TO FIND OUT HOW TO WRITE IT
{
	std::string	RPLPong = "PONG :localhost\r\n";
	send(_sockfd, RPLPong.c_str(), RPLPong.length(), MSG_DONTWAIT);
}

void Clients::sendRPL331(std::string channel) //NO TOPIC
{
	std::string	RPL331 = ":127.0.0.1 331 " + channel + " No topic is set\r\n";
	send(_sockfd, RPL331.c_str(), RPL331.length(), MSG_DONTWAIT);
}

void Clients::sendRPL353(std::string channel, std::string listName) //NAMREPLY
{
    std::string	RPL353 = ":127.0.0.1 353 " + _nick + " = " + channel + " :" + listName + "\r\n";
    send(_sockfd, RPL353.c_str(), RPL353.length(), MSG_DONTWAIT);
}

void Clients::sendRPL366(std::string channel) //ENDOFNAMES
{
    std::string	RPL366 = ":127.0.0.1 366 " + _nick + " " + channel + " :End of /NAMES list\r\n";
    send(_sockfd, RPL366.c_str(), RPL366.length(), MSG_DONTWAIT);
}

void Clients::sendPrivMsg(std::string channel, std::string msg, std::string name)
{
	std::string	privMsg = ":" + name + " PRIVMSG " + channel + msg + "\r\n";
	//std::cout << ">>" << privMsg << "<<" << std::endl;
	send(_sockfd, privMsg.c_str(), privMsg.length(), MSG_DONTWAIT);
}

void Clients::sendKickMsg(std::string channel, std::string client, std::string name, std::string reason)
{
	std::string kickMsg = ":" + name + "@localhost" + " KICK " + channel + " " + client + " " + reason + "\r\n";

	//std::cout << ">>" << kickMsg << "<<" << std::endl;
	send(_sockfd, kickMsg.c_str(), kickMsg.length(), MSG_DONTWAIT);
}

void Clients::sendRPL332(std::string channel, std::string topic) //TOPIC
{
	std::string	RPL332 = ":127.0.0.1 332 " + _nick + " " + channel + " :" + topic + "\r\n";
	//std::cout << ">>" << RPL332 << "<<" << std::endl;
	send(_sockfd, RPL332.c_str(), RPL332.length(), MSG_DONTWAIT);
}

void Clients::sendRPL341(std::string channel, std::string invited) // INVITE (The invite has been sent)
{
	std::string	RPL341 = ":127.0.0.1 341 " + _nick + " " + invited + " " + channel + "\r\n";
	send(_sockfd, RPL341.c_str(), RPL341.length(), MSG_DONTWAIT);
}

//ERRORS

void	Clients::sendErrPass(void)
{
	std::string	Err464 = ":127.0.0.1 464 " + _nick + " Password incorrect\r\n";
	send(_sockfd, Err464.c_str(), Err464.length(), MSG_DONTWAIT);
}

void	Clients::sendErrBadKey(std::string channel)
{
	std::string	Err475 = ":127.0.0.1 475 " + _nick + " " + channel + " Cannot join channel (+k) - bad key\r\n";
	send(_sockfd, Err475.c_str(), Err475.length(), MSG_DONTWAIT);
}

void	Clients::sendErrNickCollision(void)
{
	std::string	Err436 = ":127.0.0.1 436 " + _nick + " Nickname collision KILL from " + _name + "@nahima\r\n";
	send(_sockfd, Err436.c_str(), Err436.length(), MSG_DONTWAIT);
}

void	Clients::sendErrInviteOnlyChan(std::string channel)
{
	std::string Err473 = ":127.0.0.1 473 " + _nick + " " + channel + " Cannot join channel (+i) - No invitation\r\n";
	send(_sockfd, Err473.c_str(), Err473.length(), MSG_DONTWAIT);
}

void	Clients::sendErrChannelIsFull(std::string channel)
{
	std::string Err471 = ":127.0.0.1 471 " + _nick + " " + channel + " Cannot join channel (+l) - Limit exceeded\r\n";
	send(_sockfd, Err471.c_str(), Err471.length(), MSG_DONTWAIT);
}

void	Clients::sendErrNoSuchNick(std::string nick)
{
	std::string Err401 = ":127.0.0.1 401 " + _nick + " " + nick + " :No such nick\r\n";
	send(_sockfd, Err401.c_str(), Err401.length(), MSG_DONTWAIT);
}

void	Clients::sendErrNoSuchChannel(std::string channel)
{
	std::string Err403 = ":127.0.0.1 403 " + _nick + " " + channel + " :No such channel\r\n";
	send(_sockfd, Err403.c_str(), Err403.length(), MSG_DONTWAIT);
}

void	Clients::sendErrNotOnChannel(std::string channel)
{
	std::string Err442 = ":127.0.0.1 442 " + _nick + " " + channel + " :You're not on that channel\r\n";
	send(_sockfd, Err442.c_str(), Err442.length(), MSG_DONTWAIT);
}

void	Clients::sendErrNeedMoreParams(std::string command)
{
	std::string Err461 = ":127.0.0.1 461 " + _nick + " " + command + " :Not enough parameters\r\n";
	send(_sockfd, Err461.c_str(), Err461.length(), MSG_DONTWAIT);
}

void	Clients::sendErrNoPrivileges()
{
	std::string Err481 = ":127.0.0.1 481 " + _nick + " :Permission Denied- You're not an IRC operator\r\n";
	send(_sockfd, Err481.c_str(), Err481.length(), MSG_DONTWAIT);
}
