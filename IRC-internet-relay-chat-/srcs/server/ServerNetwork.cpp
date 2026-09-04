#include "Server.hpp"
#include "PrivMsgCommand.hpp"
#include "JoinCommand.hpp"
#include "Kick.hpp"
#include "invite.hpp"

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
