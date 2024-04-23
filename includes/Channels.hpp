#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# include "ft_irc.hpp"

# define CH_INVITE 2
# define CH_PASSWORD 1
# define CH_OPEN 0

# define ERR_CHANNEL_NAME -7
# define ERR_NAME -6
# define ERR_OOP -5
# define ERR_ARGS -4
# define ERR_OP -3
# define ERR_PASS -2
# define ERR_LIMIT -1


class	Channels
{
	private :
		std::string					_name;
		std::string					_topic;
		std::string					_password;
		std::string					_buffer;
		std::vector<std::string> 	_member;
		std::map<int, int>			_memberIt;
		std::vector<std::string>	_operator;
		int							_topicAccess;
		std::string					_botName;
		int							_totalMember;
		int							_access;
		int							_maxMembers;
		int							_invite;

	public :
		Channels(std::string name);
		~Channels();
		std::string					getName();
		std::string 				getPassword();
		void						setInvite(int i, int invite);
		int							getInvite(int i);
		void						addMember(std::string name, int it, int invite);
		int							getTotalMember();
		int							getAccess(void);
		std::string					getListNames();
		std::string					getMembers(int i);
		int							getIt(int i);
		void						deleteClient(std::string member);
		std::string					getTopic();
		void						setTopic(std::string topic);
		std::string					getBotName();
		void						setBotName(std::string topic);
		int							getMaxMembers(void);
		int							getTopicAccess(void);
		int							isOperator(std::string member);

		std::vector<std::string>	execMode(std::string client, std::string mode);
		void						Mode(int key, std::vector<std::string> &err);
		std::string					execInvite(int key);
		std::string					execTopic(int key);
		std::string					execKey(int key);
		std::string					execOperator(int key);
		std::string					execLimitUser(int key);

};

#endif
