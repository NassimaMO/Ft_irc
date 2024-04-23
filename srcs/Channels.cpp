#include "Channels.hpp"

Channels :: Channels (std::string	nameChannel) : _name(nameChannel)
{
	_topic = "";
	_password = "";
	_botName = "bot";
	_maxMembers = -1;
	_totalMember = 0;
	_topicAccess = -1;
	_access = 0;
	_invite = -1;
}

Channels :: ~Channels ()
{

}

std::string	Channels::getListNames()
{
	std::string	to_return = "";
	int			idx = 0;

	for(std::vector<std::string>::iterator i = _member.begin(); i != _member.end(); i++)
	{
		for(std::vector<std::string>::iterator ite = _operator.begin(); ite != _operator.end(); ite++)
		{
			if (*i == *ite)
			{
				to_return += " @" + *i + " ";
				break;
			}
			else if (ite == _operator.end() - 1)
			{
				to_return += " " + *i + " ";
				break;
			}
		}
		idx++;
	}
	return (to_return);
}

std::string		Channels :: getName()
{
	return _name;
}

std::string		Channels::getPassword()
{
	return _password;
}

int	Channels::getTotalMember()
{
	return _totalMember;
}

int	Channels::getIt(int i)
{
	std::vector<int>::iterator it = _memberIt.begin();
	std::advance(it, i);
	return *it;
}

std::string	Channels::getMembers(int i)
{
	return _member[i];
}

void	Channels::setInvite(int i)
{
	_inviteMembers.push_back(i);
}

void	Channels::deleteInvite(int i)
{
	for (size_t idx = 0; idx != _inviteMembers.size(); idx++)
	{
		if (i == _inviteMembers[idx])
		{
			_inviteMembers.erase(_inviteMembers.begin() + idx);
			break;
		}
	}
}

int	Channels::getInvite(int i)
{
	for (std::vector<int>::iterator ite = _inviteMembers.begin(); ite != _inviteMembers.end(); ite++)
	{
		if (i == *ite)
			return (1);
	}
	return (0);
}

std::vector<std::string> Channels::getMember(void)
{
	return (_member);
}

int	Channels::getAccess(void)
{
	return (_access);
}

void	Channels::addMember(std::string member, int it)
{
	_member.push_back(member);
	_memberIt.push_back(it);
	_totalMember++;
	if (_totalMember == 1)
		_operator.push_back(member);
	return;
}

void	Channels::deleteClient(std::string member)
{
	int idx = 0;

	std::cout << _member[idx] << "." << std::endl;
	std::cout << member << "." << std::endl;
	while (idx < _totalMember && _member[idx] != member)
		idx++;
	if (idx < _totalMember)
	{
		std::cout << _name << ": Client " << _member[idx] << " disconnected from channel." << std::endl;
		if (this->isOperator(member) == 1)
		{
			for(size_t i = 0; i < _operator.size(); i++)
			{
				if (_operator[i] == member)
					_operator.erase(_operator.begin() + i);
			}
		}
		_member.erase(_member.begin() + idx);
		_memberIt.erase(_memberIt.begin() + idx);
		_totalMember--;
		std::cout << "Names: " << getListNames() << std::endl;
	}
}

std::string	Channels::getTopic()
{
	return _topic;
}

void		Channels::setTopic(std::string topic)
{
	_topic = topic;
	return;
}

std::string	Channels::getBotName()
{
	return _botName;
}

void		Channels::setBotName(std::string botName)
{
	_botName = botName;
	return;
}

int			Channels::getMaxMembers(void)
{
	return (_maxMembers);
}

int	Channels::getTopicAccess(void)
{
	return (_topicAccess);
}

int	Channels::isOperator(std::string member)
{
	for (std::vector<std::string>::iterator i = _operator.begin(); i != _operator.end(); i++)
	{
		if (member == *i)
			return (1);
	}
	return (0);
}
