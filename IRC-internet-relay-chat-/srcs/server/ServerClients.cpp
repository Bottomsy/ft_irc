#include "server/Server.hpp"
#include "client/Client.hpp"

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
    C_Client *client = new C_Client(clientFd, inet_ntoa(clientAddr.sin_addr));
    connectedClients[clientFd] = client;
    std::cout << "Client <" << clientFd << "> Connected" << std::endl;
}

void C_Server::disconnectClient(int fd)
{
    std::map<int, C_Client*>::iterator clientIt = connectedClients.find(fd);
    if (clientIt == connectedClients.end())
        return;

    C_Client *client = clientIt->second;
    for (std::map<std::string, C_Channel*>::iterator channelIt = activeChannels.begin(); channelIt != activeChannels.end();)
    {
        C_Channel *channel = channelIt->second;
        if (channel->hasClient(*client))
        {
            channel->kick(*client);
            channel->broadcast(client->getNickname() + " QUIT :Quit\r\n", NULL);
            if (channel->is_empty())
            {
                delete channel;
                std::map<std::string, C_Channel*>::iterator removedChannel = channelIt;
                channelIt++;
                activeChannels.erase(removedChannel);
                continue;
            }
        }
        channelIt++;
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
        if ((client->authenticationState() & QUIT) == QUIT)
            disconnectClient(fd);
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
