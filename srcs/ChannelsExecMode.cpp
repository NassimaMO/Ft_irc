#include "Channels.hpp"

std::string		Channels::execInvite(int key)
{
	_invite = key;
	if (key == -1 && (_access == 2 || _access == 3))
		return (_access -= CH_INVITE, std::cout << _name << ": Mode: Invitation mode removed." << std::endl, "Invitation mode removed.");
	else if (key == 1 && (_access == 1 || _access == 0))
		return (_access += CH_INVITE, std::cout << _name << ": Mode: Invitation mode set." << std::endl, "Invitation mode set.");
	return ("Invitation already set.");
}

std::string	Channels::execTopic(int key)
{
	_topicAccess = key;
	std::cout << _name << ": Mode: Topic access set to " << _topicAccess << "." << std::endl;
	return ("Topic access set to " + inttostr(_topicAccess) + ".");
}

std::string	Channels::execKey(int key)
{
	if (key == -1)
	{
		_password = "";
		if (_access == 1 || _access == 3)
			_access -= CH_PASSWORD;
		return (std::cout << _name << ": Mode: Channel password removed." << std::endl, "Channel password removed.");
	}
	else
	{
		if (_buffer.find_first_of(" ") != std::string::npos)
		{
			_password = _buffer.substr(_buffer.find_first_of(" ") + 1, _buffer.length());
			if (_password.find_first_of(" ") != std::string::npos)
			{
				_password = _password.substr(0, _password.find_first_of(" "));
				_buffer = _buffer.substr(0, _buffer.find_first_of(" ") + 1) + _buffer.substr(_buffer.find_first_of(" ") + 1 + _password.length() + 1, _buffer.length());
			}
			else
				_buffer = _buffer.substr(0, _buffer.find_first_of(" "));
			if (_access == 2 || _access == 0)
				_access += CH_PASSWORD;
			return (std::cout << _name << ": Mode: Channel password set to \"" << _password << "\"." << std::endl, "Channel password set to \"" + _password + "\".");
		}
	}
	return (std::cout << _name << ": Mode: Missing password.", "Missing password.");
}

std::string	Channels::execOperator(int key)
{
	std::string	member = "";

	if (_buffer.find_first_of(" ") != std::string::npos)
	{
		member = _buffer.substr(_buffer.find_first_of(" ") + 1, _buffer.length());
		if (member.find_first_of(" ") != std::string::npos)
		{
			member = member.substr(0, member.find_first_of(" ") + 1);
			_buffer = _buffer.substr(0, _buffer.find_first_of(" ") + 1) + _buffer.substr(_buffer.find_first_of(" ") + 1 + member.length(), _buffer.length());
		}
		else
			_buffer = _buffer.substr(0, _buffer.find_first_of(" "));
	}
	for(std::vector<std::string>::iterator i = _member.begin(); i != _member.end(); i++)
	{
		if (*i == member)
		{
			for(size_t idx = 0; idx < _operator.size(); idx++)
			{
				if (_operator[idx] == member && key == -1 && idx == 0)
					return (std::cout << _name << ": Mode: Original operator cannot have his channel rights removed." << std::endl, "Original operator cannot have his channel rights removed.");
				else if (_operator[idx] == member && key == -1)
					return (std::cout << "Operator " << member << " " << idx << " got his channel rights revoked on " << _name << "." << std::endl, _operator.erase(_operator.begin() + idx), "Operator " + member + " " + inttostr(idx) + " got his channel rights revoked.");
				else if (_operator[idx] != member && key == 1)
					return (std::cout << "Member " << member << " " << idx << " acquired channel rights on " << _name << "." << std::endl, _operator.push_back(member), "Member " + member + " " + inttostr(idx) + " acquired channel rights.");
				return (std::cout << _name << ": Mode: " << member << " " << idx << " already in his assigned position." << std::endl, "Member already in his assigned position.");
			}
		}
	}
	return (std::cout << _name << ": Mode: No such member/operator found." << std::endl, "No such member/operator found.");
}

std::string	Channels::execLimitUser(int key)
{
	std::string	buf;

	if (key == -1)
	{
		_maxMembers = -1;
		return (std::cout << _name << ": Mode: Channel members limit removed." << std::endl, "Channel members limit removed.");
	}
	else if (_buffer.find_first_of(" ") != std::string::npos)
	{
		buf = _buffer.substr(_buffer.find_first_of(" ") + 1, _buffer.length());
		if (buf.find_first_of(" ") != std::string::npos)
		{
			buf = buf.substr(0, buf.find_first_of(" ") + 1);
			_buffer = _buffer.substr(0, _buffer.find_first_of(" ") + 1) + _buffer.substr(_buffer.find_first_of(" ") + 1 + buf.length(), _buffer.length());
		}
		else
			_buffer = _buffer.substr(0, _buffer.find_first_of(" "));
		for (size_t i = 0; i < buf.length(); i++)
		{
			if (!isdigit(buf[i]))
				return (std::cout << _name << ": Mode: Non numeric argument given." << std::endl, "Non numeric argument given.");
		}
		_maxMembers = strtoint(buf);
		if (_maxMembers < _totalMember)
		{
			std::cout << _name << ": Mode: Members exceeding limit. Limit changed to " + inttostr(_maxMembers) + "." << std::endl;
			_maxMembers = _totalMember;
			return ("Members exceeding limit. Limit changed to " + inttostr(_maxMembers) + ".");
		}
		return (std::cout <<_name << ": Mode: Channel members limit changed to " << _maxMembers << "." << std::endl, "Channel members limit changed to " + inttostr(_maxMembers) + ".");
	}
	return (std::cout << _name << ": Mode: Limit argument missing." << std::endl, "Limit argument missing." );
}

void Channels::Mode(int key, std::vector<std::string> &err)
{
	std::string		(Channels::*execFunct[])(int) = {&Channels::execInvite, &Channels::execTopic, &Channels::execKey, &Channels::execOperator, &Channels::execLimitUser};
	char 		modeChoice[] = {'i', 't', 'k', 'o', 'l'};

	_buffer = _buffer.substr(1, _buffer.length());
	for (int i = 0; i < 5; i++)
	{
		if (_buffer[0] == modeChoice[i])
		{
			err.push_back((this->*execFunct[i])(key));
			i = -1;
			_buffer = _buffer.substr(1, _buffer.length());
		}
	}
}

std::vector<std::string> Channels::execMode(std::string client, std::string	mode)
{
	std::vector<std::string>		rpl;

	for(std::vector<std::string>::iterator it = _operator.begin(); it != _operator.end() || _operator.size() == 0; it++)
	{
		if (client.empty() || client == *it)
		{
			_buffer = mode;
			while (!_buffer.empty())
			{
				if (_buffer[0] == '+')
					Mode(1, rpl);
				else if (_buffer[0] == '-')
					Mode(-1, rpl);
				else if (!_buffer.empty())
					return (std::cout << _name << ": Mode: Too many arguments: " << _buffer << std::endl, rpl.push_back("Too many arguments: " + _buffer), rpl);
			}
			return (rpl);
		}
	}
	return (std::cout << _name << ": Mode: " << client << " do not have channel modification rights." << std::endl, rpl.push_back(client + " do not have channel modification rights."), rpl);
}
