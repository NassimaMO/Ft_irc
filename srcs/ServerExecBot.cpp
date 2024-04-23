#include "ft_irc.hpp"

int	Server::execBotMembers(ssize_t itCl, int itCh, std::string channel, std::string arg)
{
	(void)arg;
	std::string listMember = _channel[itCh].getListNames();
	std::string msg = " :Voici la liste des membres :" + listMember;
	std::cout << "\033[1;31mBot\033[0m : List of member display." << std::endl;
	_client[itCl].sendPrivMsg(channel, msg, _channel[itCh].getBotName());

	return 0;
}

int	Server::execBotName(ssize_t itCl, int itCh, std::string channel, std::string arg)
{
	_channel[itCh].setBotName(arg);
	std::string msg = " :Mon nom a bien été changé, je suis maintenant " + arg;
	std::cout << "\033[1;31mBot\033[0m : bot name have been changed."<< std::endl;
	_client[itCl].sendPrivMsg(channel, msg, _channel[itCh].getBotName());
	return 0;
}

int	Server::execBotHelp(ssize_t itCl, int itCh, std::string channel,  std::string arg)
{
	(void)arg;
	_client[itCl].sendPrivMsg(channel, " :Voici la liste des commandes que je peux executer :", _channel[itCh].getBotName());
	_client[itCl].sendPrivMsg(channel, " :◦ members: affiche les membres du channel;", _channel[itCh].getBotName());
	_client[itCl].sendPrivMsg(channel, " :◦ name: permet de changer mon nom;", _channel[itCh].getBotName());
	_client[itCl].sendPrivMsg(channel, " :◦ commands: affiche toutes les commandes du channel.", _channel[itCh].getBotName());
	std::cout << "\033[1;31mBot\033[0m : bot displaying the bot commands available."<< std::endl;
	return 0;
}

int	Server::execBotCommands(ssize_t itCl, int itCh, std::string channel,  std::string arg)
{
	(void)arg;
	_client[itCl].sendPrivMsg(channel, " :Voici la liste des commandes que tu peux executer : ", _channel[itCh].getBotName());
	_client[itCl].sendPrivMsg(channel, " :◦ MODE: ", _channel[itCh].getBotName());
	_client[itCl].sendPrivMsg(channel, " :   i: Définir/supprimer le canal sur invitation uniquement;", _channel[itCh].getBotName());
	_client[itCl].sendPrivMsg(channel, " :   t : Définir/supprimer les restrictions de la commande TOPIC pour les opérateurs de canaux", _channel[itCh].getBotName());
	_client[itCl].sendPrivMsg(channel, " :   k : Définir/supprimer la clé du canal (mot de passe)", _channel[itCh].getBotName());
	_client[itCl].sendPrivMsg(channel, " :   o : Donner/retirer le privilège de l’opérateur de canal", _channel[itCh].getBotName());
	_client[itCl].sendPrivMsg(channel, " :   l : Définir/supprimer la limite d’utilisateurs pour le canal", _channel[itCh].getBotName());
	_client[itCl].sendPrivMsg(channel, " :◦ KICK - Ejecter un client du channel;", _channel[itCh].getBotName());
	_client[itCl].sendPrivMsg(channel, " :◦ INVITE - Inviter un client au channel;", _channel[itCh].getBotName());
	_client[itCl].sendPrivMsg(channel, " :◦ TOPIC - Modifier ou afficher le thème du channel;", _channel[itCh].getBotName());
	std::cout << "\033[1;31mBot\033[0m : bot displaying the channel commands available."<< std::endl;
	return (0);
}


int	Server::execBot(ssize_t itCl, int itCh, std::string channel, std::string msg)
{
	std::string		command;
	std::string		arg = "";
	std::string		botCommand[] = {"members", "name", "help", "commands"};
	int				idx = 7;
	int				(Server::*botExec[])(ssize_t, int, std::string, std::string) = {&Server::execBotMembers, &Server::execBotName, &Server::execBotHelp, &Server::execBotCommands};

	while (msg[idx] != '\0' && msg[idx] != ' ')
		idx++;
	if (msg[idx] == '\0')
		command = std::string(msg.begin() + 7, msg.begin() + idx);
	else
	{
		command = std::string(msg.begin() + 7, msg.begin() + idx);
		arg = std::string(msg.begin() + idx + 1, msg.end());
	}
	std::cout << "\033[1;31mBot\033[0m : command :>" << command << "." <<std::endl;
	std::cout << "\033[1;31mBot\033[0m : arg :" << arg << "." << std::endl;
	for (int idex = 0; idex < 4; idex++)
	{
		if (command == botCommand[idex])
			(this->*botExec[idex])(itCl, itCh , channel, arg);
	}
	return (std::cout << std::endl, 0);
}

