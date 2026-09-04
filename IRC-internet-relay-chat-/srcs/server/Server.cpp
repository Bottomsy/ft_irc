#include "Server.hpp"
#include "PrivMsgCommand.hpp"
#include "JoinCommand.hpp"
#include "Kick.hpp"
#include "invite.hpp"


bool C_Server::_signal = false;

void C_Server::SignalHandler(int signum)
{
    (void)signum;
    std::cout << std::endl
              << "Signal Received!" << std::endl;
    C_Server::_signal = true;
    return;
}

C_Server::C_Server(int port, std::string pass) : listenPort(port), serverPassword(pass)
{
}

C_Channel *C_Server::findChannel(const std::string &name)
{
    std::map<std::string, C_Channel *>::iterator it = activeChannels.find(name);
    if (it == activeChannels.end())
        return 0;
    return it->second;
}

void C_Server::registerChannel(const std::string &name, C_Channel *channel)
{
    activeChannels[name] = channel;
}

bool C_Server::hasChannel(const std::string &name) const
{
    return activeChannels.find(name) != activeChannels.end();
}

void C_Server::initializeNetwork()
{
    listeningSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (listeningSocket == -1)
        throw std::runtime_error("failed to create socket");

    int en = 1;
    if (setsockopt(listeningSocket, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1)
        throw std::runtime_error("failed to set option (SO_REUSEADDR) on socket");

    if (fcntl(listeningSocket, F_SETFL, O_NONBLOCK) == -1)
        throw std::runtime_error("failed to set option (O_NONBLOCK) on socket");

    struct sockaddr_in add;
    add.sin_family = AF_INET;
    add.sin_port = htons(listenPort);
    add.sin_addr.s_addr = INADDR_ANY;

    if (bind(listeningSocket, (struct sockaddr *)&add, sizeof(add)) == -1)
        throw std::runtime_error("failed to bind socket");

    if (listen(listeningSocket, SOMAXCONN) == -1)
        throw std::runtime_error("listen() failed");

    struct pollfd newPoll;
    newPoll.fd = listeningSocket;
    newPoll.events = POLLIN;
    newPoll.revents = 0;

    commandHandlers["PRIVMSG"] = new C_privMsgCommand();
    commandHandlers["JOIN"]    = new C_JoinCommand();
    commandHandlers["KICK"]    = new C_kick();
    commandHandlers["INVITE"]  = new C_invite();
    monitoredSockets.push_back(newPoll);
}

void C_Server::runEventLoop()
{
    while (C_Server::_signal == false)
    {
        if (poll(monitoredSockets.data(), monitoredSockets.size(), -1) == -1 && C_Server::_signal == false)
            throw std::runtime_error("poll() failed");

        for (size_t i = 0; i < monitoredSockets.size(); i++)
        {

            if (monitoredSockets[i].revents & POLLIN)
            {

                if (monitoredSockets[i].fd == listeningSocket)
                    this->acceptConnection();
                else
                    this->receiveClientData(monitoredSockets[i].fd);
            }
        }
    }
    this->releaseResources();
}
C_Server::~C_Server()
{
}
void C_Server::acceptConnection()
{
    struct sockaddr_in clientAddr;
    struct pollfd clientPollfd;
    socklen_t len = sizeof(clientAddr);

    int clientFd = accept(listeningSocket, (struct sockaddr *)&clientAddr, &len);
    if (clientFd == -1)
    {
        std::cerr << "accept() failed" << std::endl;
        return;
    }
    if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1)
    {
        std::cerr << "fcntl() failed" << std::endl;
        close(clientFd);
        return;
    }
    clientPollfd.fd = clientFd;
    clientPollfd.events = POLLIN;
    clientPollfd.revents = 0;
    monitoredSockets.push_back(clientPollfd);
    C_Client *p = new C_Client(clientFd, inet_ntoa(clientAddr.sin_addr));
    connectedClients[clientFd] = p;
    std::cout << "Client <" << clientFd << "> Connected" << std::endl;
}
void C_Server::disconnectClient(int fd)
{

    std::map<int, C_Client*>::iterator cl = connectedClients.find(fd);
    if (cl == connectedClients.end())
        return;
    C_Client *client = cl->second;
    for (std::map<std::string, C_Channel*>::iterator it = activeChannels.begin() ;it!=activeChannels.end();){
        C_Channel * channel = it->second;
        if (channel->hasClient(*client))
        {

            channel->kick(*client);
            channel->broadcast( client->getNickname() + " QUIT :Quit\r\n",NULL);
            if (channel->is_empty())
            {
                delete channel;
                std::map<std::string, C_Channel*>::iterator tmp = it;
                it++;
                activeChannels.erase(tmp);
                continue;
            }
        }
        it++;
    }
            registeredNicknames.erase(client->getNickname());
            connectedClients.erase(fd);
            for (size_t i = 0; i < monitoredSockets.size(); i++)
                {
                    if (monitoredSockets[i].fd == fd)
                    {
                        monitoredSockets.erase(monitoredSockets.begin() + i);
                        break;
                    }
                }
            close(fd);
            delete client;
}
void C_Server::receiveClientData(int fd)
{
    char buffer[512];
    memset(buffer, 0, sizeof(buffer));
    ssize_t bytes = recv(fd, buffer, sizeof(buffer) - 1, 0);
    C_Client *client = connectedClients[fd];
    if (bytes > 0)
    {
        buffer[bytes] = '\0';
        client->appendReceivedData(buffer, bytes);
        while (((client->authenticationState() & QUIT) != QUIT) && client->containsCompleteLine())
        {
            t_command cmd;
            std::string line = client->takeNextLine();
            client->parseCommandLine(line, cmd);
            dispatchCommand(cmd, fd);
        }
        if ((client->authenticationState() & QUIT) == QUIT){
            disconnectClient(fd);
        }
    }
    else if (bytes == 0)
    {
        std::cout << "Client <" << fd << "> Disconnected" << std::endl;
        disconnectClient(fd);
    }
    else
    {
        perror("recv() failed"); 
        disconnectClient(fd);
    }
}
void C_Server::releaseResources()
{
    for (std::map<std::string, C_Channel *>::iterator it = activeChannels.begin(); it != activeChannels.end(); it++)
        delete it->second;

    activeChannels.clear();
    for (std::map<std::string, C_commands*>::iterator it = commandHandlers.begin(); it != commandHandlers.end(); ++it)
        delete it->second;

    for (std::map<int, C_Client*>::iterator it = connectedClients.begin(); it != connectedClients.end(); it++)
    {
        std::cout << "Closing FD: " << it->first << std::endl;
        close(it->first);
        delete it->second;
    }
    monitoredSockets.clear();
    close(listeningSocket);
    std::cout << "Server Shutdown Cleanly." << std::endl;
}

C_Client *C_Server::findClientByNickname(const std::string &nickame)
{
    for (std::map<int, C_Client *>::iterator at = connectedClients.begin(); at != connectedClients.end(); at++)
    {
        if (at->second->getNickname() == nickame)
            return at->second;
    }
    return (NULL);
}
