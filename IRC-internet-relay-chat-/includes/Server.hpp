#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <signal.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <vector>
#include <map>
#include <netinet/in.h>
#include <cstdlib>
#include <csignal>
#include <cstring>
#include <arpa/inet.h>
#include <set>

typedef struct s_command t_command;
class C_Channel;
class C_commands;
class C_Client;
class C_Server
{
    private:
        int listenPort;
        std::string serverPassword;
        int listeningSocket;
        static bool _signal;
        std::vector<struct pollfd> monitoredSockets;
        std::set<std::string> registeredNicknames;
        std::map<int, C_Client*> connectedClients;
        std::map<std::string, C_commands*> commandHandlers;
        std::map<std::string, C_Channel*> activeChannels;
        C_Server(); 
        C_Server(const C_Server& other);
        C_Server& operator=(const C_Server& other);
    public:
        C_Server(int port, std::string pass);
        void initializeNetwork();
        void runEventLoop();
        static void SignalHandler(int signum);
        void acceptConnection();
        void receiveClientData(int fd);
        void disconnectClient(int fd);
        void releaseResources();
        C_Channel* findChannel(const std::string& name);
        void registerChannel(const std::string& name, C_Channel* channel);
        bool hasChannel(const std::string& name) const;
        C_Client* findClientByNickname(const std::string&);

        static bool is_quit(){return _signal;}

            int authenticate(t_command &cmd, C_Client *client);
            void dispatchCommand(t_command &, int);
            void processTopic(C_Client*, t_command&);
            void processMode(C_Client*, t_command&);
            void processPassword(C_Client*, t_command&);
            void processNickname(C_Client*, t_command&);
            void processUserRegistration(C_Client*, t_command&);
            void respondToPing(C_Client*, t_command&);
            void closeClientSession(C_Client*, t_command&);
        bool registrationComplete(C_Client *);
        bool nicknameIsTaken(std::string);
        void sendError(C_Client *, int, const std::string&, const std::string&, const std::string&);
        ~C_Server();
};

#endif