#ifndef SERVER_HPP
# define SERVER_HPP

# include "ft_irc.hpp"
# include "Clients.hpp"

class Clients;

class Channels;

class Server
{
    private:

		std::vector<Clients>		_client;
		std::vector<Channels>		_channel;
	    std::vector<struct pollfd>	_fds;
	    std::string					_password;
	    std::string					_port;
	    int                         _sockfd;
		int                         _totalClients;
		int							_totalChannels;
		static int					_ext;
	    struct  sockaddr_in         serv_addr;

		/////////////////////Init Serv/////////////////////
	    int							initSocket(void);
	    int							initServ(void);

    public:

		Server(std::string password, std::string port);
		~Server();

		////////////////////GET AND SET////////////////////
	    std::vector<struct pollfd>	&getFds(void);
		std::string					&getPort(void);
		std::string					&getPassword(void);
		int							getTotalClients(void);
		int							getExt(void);

		////////////////////// Exec //////////////////////
		int							execSwitch( ssize_t it);
		int							execWelcome( ssize_t it);
		int							execMode(ssize_t it);
		int							execJoin(ssize_t it);
		int							execPong(ssize_t it);
		int 						execMsg(ssize_t it);
		int							execInvite(ssize_t it);
		int							execKick(ssize_t it);
		int							execTopic(ssize_t it);

	    void						waitClients(void);
	    void						receiveMsgs(void);
	    int							sendMsgs(void);
		void						parseMsgs(void);
		int							privMsg(ssize_t it);
		int							channelMsg(ssize_t it);

		////////////////////// Verif //////////////////////
		int							verif(ssize_t it);
		int							deleteClient(ssize_t it);

		////////////////////// Channel //////////////////////
		void						manageChannel(int it);
		int							verifChannels(std::string nameChannel);
		int							checkingAccess(int itCh, int itCl);

		//////////////////////// Bot ////////////////////////
		int							execBot(ssize_t itCl, int itCh, std::string channel, std::string msg);
		int							execBotMembers(ssize_t itCl, int itCh, std::string channel, std::string arg);
		int							execBotName(ssize_t itCl, int itCh, std::string channel, std::string arg);
		int							execBotHelp(ssize_t it, int itCh, std::string channel, std::string arg);
		int							execBotCommands(ssize_t itCl, int itCh, std::string channel,  std::string arg);


		static void					signalHandle(int signal);
		void						serverErr(std::string key, std::string msg, ssize_t it, int idx);


};

#endif
