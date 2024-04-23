#include "ft_irc.hpp"

# define RPL_W "001"

int	Server::execWelcome(ssize_t it)
{
	if (_password != "")
	{
		if (verif(it) > 0)
			return (1);
		_client[it].sendRPL001();
		_client[it].sendRPL002();
		_client[it].sendRPL003();
		return (0);
	}
	return (0);
}

int Server::execMode(ssize_t it)
{
	std::string mode = _client[it].getMode();
	int	i = 1;
	std::string key;
	std::vector<std::string> rpl;

	if (mode[0] != '#')
		_client[it].sendRPL004();
	else
	{
		while (mode[i] && mode[i] != ' ')
			i++;
		if (!mode[i])
			return (1);
		key = std::string(mode.begin(), mode.begin() + i);
		for (int idx = 0; idx < _totalChannels; idx++ )
		{
			if (key == _channel[idx].getName())
			{
				rpl =_channel[idx].execMode(_client[it].getNick(), mode.substr(i + 1, mode.length()));
				for (std::vector<std::string>::iterator ite = rpl.begin(); ite != rpl.end(); ite++)
					_client[it].sendPrivMsg(_channel[idx].getName(), " :" + *ite, "Server");
			}
		}
	}
	return (0);
}

int	Server::execPong(ssize_t it)
{
	_client[it].sendPong();
	_client[it].setPing("");
	return (0);
}

int		Server::verifChannels(std::string nameChannel)
{
	for (int it = 0; it < _totalChannels; it++ )
	{
		if (_channel[it].getName() == nameChannel)
			return 1;
	}
	_totalChannels++;
	return 0;
}

int		Server::checkingAccess(int itCh, int itCl)
{
	if (_channel[itCh].getAccess() == CH_INVITE + CH_PASSWORD && (_client[itCl].getPassword() != _channel[itCh].getPassword() || _channel[itCh].getInvite(itCl) == 0))
		return (std::cout << "Channel invitation and password required." << std::endl, _client[itCl].sendErrInviteOnlyChan(_channel[itCh].getName()), _client[itCl].sendErrBadKey(_channel[itCh].getName()), 3);
	else if (_channel[itCh].getAccess() == CH_PASSWORD && _client[itCl].getPassword() != _channel[itCh].getPassword())
		return (std::cout << "Channel password required." << std::endl, _client[itCl].sendErrBadKey(_channel[itCh].getName()), 1);
	else if (_channel[itCh].getAccess() == CH_INVITE && _channel[itCh].getInvite(itCl) == 0)
		return (std::cout << "Channel invitation required." << std::endl, _client[itCl].sendErrInviteOnlyChan(_channel[itCh].getName()), 2);
	else if (_channel[itCh].getTotalMember() == _channel[itCh].getMaxMembers())
		return (std::cout << "Channel already full." << std::endl, _client[itCl].sendErrChannelIsFull(_channel[itCh].getName()), 4);
	return(0);
}

int Server::execJoin(ssize_t it)
{
	if (_client[it].getChannel().find_first_of(" ") != std::string::npos)
	{
		_client[it].setPassword(_client[it].getChannel());
		_client[it].setChannel(_client[it].getPassword().substr(0, _client[it].getChannel().find_first_of(" ")));
		_client[it].setPassword(_client[it].getPassword().substr(_client[it].getChannel().length() + 1, _client[it].getPassword().length()));
	}
	if (this->verifChannels(_client[it].getChannel()) == 0)
	{
		_channel.push_back(Channels(_client[it].getChannel()));
		std::cout << "Channel " << _client[it].getChannel() << " created by " << _client[it].getNick() << "." << std::endl;
	}
	for (int ite = 0; ite < _totalChannels; ite++ )
	{
		if (_channel[ite].getName() == _client[it].getChannel())
		{
			if (this->checkingAccess(ite, it) > 0)
				return (1);
			if (_channel[ite].getAccess() != CH_INVITE + CH_PASSWORD && _channel[ite].getAccess() != CH_INVITE)
				_channel[ite].addMember(_client[it].getNick(), it, 0);
			std::cout << "Client " << _client[it].getNick() << " " << it << " joined channel " << _channel[ite].getName() << "." << std::endl;
			_client[it].sendRPL331(_channel[ite].getName());
			_client[it].sendRPL353(_channel[ite].getName(), _channel[ite].getListNames());
			_client[it].sendRPL366(_channel[ite].getName());
			_client[it].sendPrivMsg(_channel[ite].getName(), " :Bienvenue sur ce Channel ! N'hésitez pas à m'appeler. Pour savoir tout ce que je fais, utiliser la fonction <<!bot help>>", _channel[ite].getBotName());

		}
	}
	return (0);
}

int	Server::channelMsg(ssize_t it)
{
	std::string		channel;
	std::string		msg = _client[it].getMsg();

	for (int idx = 0; 1; idx++)
	{
		if (msg[idx] == '\0')
			return (std::cout << "\033[1;32mChannel Msg\033[0m : Not enough parameters." << std::endl, _client[it].sendErrNeedMoreParams("CHAN MSG"), 0);
		if	(msg[idx] == ' ')
		{
			channel = std::string(msg.begin(), msg.begin() + idx);
			std::cout << "\033[1;32mChannel Msg\033[0m : channel :" << channel << "." << std::endl;
			msg = std::string(msg.begin() + idx, msg.end());
			std::cout << "\033[1;32mChannel Msg\033[0m : msg" << msg << "." << std::endl;
			break;
		}
	}
	for (int idx = 0; 1; idx++)
	{
		if (idx == _totalChannels)
			return (std::cout << "\033[1;32mChannel Msg\033[0m : No such channel." << std::endl, _client[it].sendErrNoSuchChannel(channel), 0);
		if (_channel[idx].getName() == channel )
		{
			for (int index = 0; index <= _channel[idx].getTotalMember(); index++)
			{
				if (index == _channel[idx].getTotalMember())
					return (std::cout << "\033[1;32mChannel Msg\033[0m : " << _client[it].getNick() << ": You're not on that channel." << std::endl, _client[it].sendErrNotOnChannel(channel), 1);
				else if (it == _channel[idx].getIt(index)) // segfault when multiple clients
					break;
			}
			if (msg.substr(2, 5) == "!bot ")
				return (this->execBot(it, idx, channel, msg), 1);
			for (int i = 0; i < _channel[idx].getTotalMember(); i++)
			{
				if(_channel[idx].getIt(i) != it)
					_client[_channel[idx].getIt(i)].sendPrivMsg(_channel[idx].getName(), msg, _client[it].getNick());
			}
			return (std::cout << "\033[1;32mChannel Msg\033[0m : Client " <<  _client[it].getNick() << " has send a message to channel " << _channel[idx].getName() << "." << std::endl, 0);
		}
	}
	return (0);
}

int Server::privMsg(ssize_t it)
{
	std::string		receiver;
	std::string		msg = _client[it].getMsg();

	for (int idx = 0; 1; idx++)
	{
		if (msg[idx] == '\0')
			return (std::cout << "\033[1;32mPriv Msg\033[0m : Not enough parameters." << std::endl, _client[it].sendErrNeedMoreParams("PRIV MSG"), 0);
		if (msg[idx] == ' ')
		{
			receiver = std::string(msg.begin(), msg.begin() + idx);
			std::cout << "\033[1;32mPriv Msg\033[0m : receiver :" << receiver << "." << std::endl;
			msg = std::string(msg.begin() + idx, msg.end());
			std::cout << "\033[1;32mPriv Msg\033[0m : msg" << msg << "." << std::endl;
			break;
		}
	}
	for (int idx = 0; 1; idx++)
	{
		if (idx == _totalClients)
			return (std::cout << "\033[1;32mPriv Msg\033[0m : No such Client." << std::endl, _client[it].sendPrivMsg("@error", " :No such Client.", "Server") ,_client[it].sendErrNoSuchNick(receiver), 0);
		if (_client[idx].getNick() == receiver)
		{
			_client[idx].sendPrivMsg(_client[idx].getNick(), msg, _client[it].getNick());
			return (std::cout << "\033[1;32mPriv Msg\033[0m : Client " <<  _client[it].getNick() << " has send a message to " << receiver << "." << std::endl, 0);
		}
	}
	return 0;
}

int Server::execMsg(ssize_t it)
{
	std::string		msg = _client[it].getMsg();

	if (msg[0] == '#')
		this->channelMsg(it);
	else
		this->privMsg(it);
	return 0;
}

int	Server::execInvite(ssize_t it)
{
	std::string	to_invite;
	std::string channel = _client[it].getInvite();

	if (channel.find_first_of(" ") != std::string::npos)
	{
		to_invite = channel.substr(0, channel.find_first_of(" "));
		channel = channel.substr(channel.find_first_of(" ") + 1, channel.length());
		if (channel[0] != '#')
			channel = "#" + channel;
	}
	std::cout << "Channel: " << channel << "." << channel.length() << std::endl;
	for(std::vector<Channels>::iterator i = _channel.begin(); i != _channel.end(); i++)
	{
		if (i->getName() == channel)
		{
			for(std::vector<Clients>::iterator ite = _client.begin(); ite != _client.end(); ite++)
			{
				std::cout << "Client: " << ite->getNick() << std::endl;
				if (ite->getNick() == to_invite)
				{
					i->addMember(ite->getNick(), std::distance(_client.begin(), ite), 1);
					ite->sendRPL341(channel, to_invite);
					_client[it].sendRPL341(channel, to_invite);
					std::cout << "Client " << ite->getNick() << " " << std::distance(_client.begin(), ite) << " got invited to join channel " << i->getName() << " by " << _client[it].getNick() << std::endl;
					break;
				}
			}
		}
	}
	return (0);
}

int	Server::execKick(ssize_t it)
{
	std::string	channel;
	std::string client;
	std::string	arg = _client[it].getKick();

	for (int idx = 0; 1; idx++)
	{
		if (arg[idx] == '\0')
			return (std::cout << "\033[1;32mKick\033[0m : Not enough parameters." << std::endl, _client[it].sendErrNeedMoreParams("KICK"), 0);
		if	(arg[idx] == ' ')
		{
			channel = std::string(arg.begin(), arg.begin() + idx);
			std::cout << "\033[1;32mKick\033[0m : channel :" << channel << "." << std::endl;
			arg = std::string(arg.begin() + idx + 1, arg.end());
			if (arg.find_first_of(" ") == std::string::npos)
				return (std::cout << "\033[1;32mKick\033[0m : Client argument missing." << std::endl, _client[it].sendPrivMsg(channel, " :Not found people you want to kick on that channel.", "Server"), 1);
			client = std::string(arg.begin(), arg.begin() + arg.find_first_of(" "));
			std::cout << "\033[1;32mKick\033[0m : client :" << client << "." << std::endl;
			if (arg.find_first_of(":") != std::string::npos)
				arg = std::string(arg.begin() + arg.find_first_of(":") + 1, arg.end());
			else
				arg = "";
			std::cout << "\033[1;32mKick\033[0m : arg :" << arg << "." << std::endl;
			break;
		}
	}
	for (int idx = 0; 1; idx++)
	{
		if (idx == _totalChannels)
			return (std::cout << "\033[1;32mKick\033[0m : No such channel." << std::endl, _client[it].sendErrNoSuchChannel(channel), 0);
		if (_channel[idx].getName() == channel )
		{
			for (int index = 0; index <= _channel[idx].getTotalMember(); index++)
			{
				if (index == _channel[idx].getTotalMember())
					return (std::cout << "\033[1;32mChannel Msg\033[0m :" << _client[it].getNick() << " You're not on that channel." << std::endl, _client[idx].sendErrNotOnChannel(channel), 0);
				else if (it == _channel[idx].getIt(index))
					break;
			}
			if (_channel[idx].isOperator(_client[it].getNick()) == 0)
				return (serverErr("\033[1;32mKick\033[0m", " :Permission Denied- You're not an IRC operator", it, idx), _client[it].sendErrNoPrivileges(), 1);
			for (int i = 0; i < _channel[idx].getTotalMember(); i++)
			{
				for (int index = 0; index <= _channel[idx].getTotalMember(); index++)
				{
					if (index == _channel[idx].getTotalMember())
						return (std::cout << "\033[1;32mChannel Msg\033[0m :" << _client[it].getNick() << " Not found the people you want to kick on that channel." << std::endl, _client[it].sendPrivMsg(channel, " :Not found people you want to kick on that channel.", "Server"), 1); //dhjkl
					else if (client == _channel[idx].getMembers(index))
						break;
				}
				_client[_channel[idx].getIt(i)].sendKickMsg(_channel[idx].getName(), client, _client[it].getNick(), arg);
				if (client == _client[_channel[idx].getIt(i)].getNick())
					_client[_channel[idx].getIt(i)].sendPrivMsg("@" + channel.substr(1, channel.length() - 1), " :You have been kicked from channel " + channel + " by " +  _client[it].getNick() + " for " + arg, "Server");
			}
			_channel[idx].deleteClient(client);
			return (std::cout << "\033[1;32mKick\033[0m : Client " <<  client << " kicked from channel " << _channel[idx].getName() << " by " << _client[it].getNick() << " for " + arg << "." << std::endl, 0);
		}
	}
	return (0);
}

int	Server::execTopic(ssize_t it)
{
	std::string	channel;
	std::string	buffer = _client[it].getTopic();

	for (int idx = 0; 1; idx++)
	{
		if (buffer[idx] == '\0')
			return (std::cout << "\033[1;32mTopic\033[0m : Not enough parameters.", _client[it].sendErrNeedMoreParams("TOPIC"), 0);
		if	(buffer[idx] == ':')
		{
			channel = std::string(buffer.begin(), buffer.begin() + (idx - 1));
			std::cout << "\033[1;32mTopic\033[0m : channel :" << channel << "." << std::endl;
			buffer = std::string(buffer.begin() + idx + 1, buffer.end());
			break;
		}
	}
	for (int idx = 0; 1; idx++)
	{
		if (idx == _totalChannels)
			return (std::cout << "\033[1;32mTopic\033[0m : No such channel.", _client[it].sendErrNoSuchChannel(channel), 0);
		if (_channel[idx].getName() == channel )
		{
			for (int index = 0; index <= _channel[idx].getTotalMember(); index++)
			{
				if (index == _channel[idx].getTotalMember())
					return (std::cout << "\033[1;32mChannel Msg\033[0m :" << _client[it].getNick() << " You're not on that channel." << std::endl, _client[idx].sendErrNotOnChannel(channel), 1);
				else if (it == _channel[idx].getIt(index)) // segfault when multiple clients
					break;
			}
			std::cout << "Buffer: " << buffer << "." << std::endl;
			if (_channel[idx].getTopicAccess() != -1 && _channel[idx].isOperator(_client[it].getNick()) == 0)
				return (serverErr("\033[1;32mTopic\033[0m", " :Permission Denied- You're not an IRC operator", it, idx), _client[it].sendErrNoPrivileges(), 1);
			_channel[idx].setTopic(buffer);
			for (int i = 0; i < _channel[idx].getTotalMember(); i++)
				_client[_channel[idx].getIt(i)].sendRPL332(channel, buffer);
			return (std::cout << "\033[1;32mTopic\033[0m :" << _client[it].getNick() << " changed the topic to \"" << buffer << "\" on channel " << _channel[idx].getName() << "." << std::endl, 0);
		}
	}
	return (0);
}

int Server::execSwitch(ssize_t it)
{
	int		(Server::*exec[])(ssize_t) = {&Server::execWelcome, &Server::execMode, &Server::execPong, &Server::execJoin, &Server::execMsg, &Server::execInvite, &Server::execKick, &Server::execTopic, &Server::deleteClient };

	if ((long unsigned int)(_client[it].getAnswerMsgs() - 1) < sizeof(exec) / sizeof(exec[0]) && (this->*exec[_client[it].getAnswerMsgs() - 1])(it))
		return (_client[it].setAnswerMsgs(0), 1);
	_client[it].setAnswerMsgs(0);
	return (std::cout << std::endl, 0);
}

