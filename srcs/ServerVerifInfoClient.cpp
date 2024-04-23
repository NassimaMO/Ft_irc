#include "Server.hpp"

int	Server::deleteClient(ssize_t it)
{
	std::cout << "Client " << _client[it].getNick() << " disconnected." << std::endl;
	close(_client[it].getSock());
	_client.erase(_client.begin() + it);
	_totalClients--;
	return (0);
}

int Server::verif(ssize_t it)
{
	if (_password != _client[it].getPassword())
		return (_client[it].sendErrPass(), this->deleteClient(it), 1);
	for (int i = 0; i < _totalClients; i++)
	{
		if (i != it && _client[i].getNick() == _client[it].getNick())
			return (_client[it].sendErrNickCollision(), this->deleteClient(it), 1);
	}
	return (0);
}
