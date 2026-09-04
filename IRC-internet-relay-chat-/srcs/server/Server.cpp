#include "server/Server.hpp"
#include "client/Client.hpp"
#include "commands/commands.hpp"
#include <sstream>

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

void C_Server::sendError(C_Client *client, int code,
                         const std::string& command,
                         const std::string& parameter,
                         const std::string& message)
{
    std::ostringstream codeStream;
    codeStream << code;

    std::string target = client->getNickname();
    if (target.empty())
        target = "*";

    std::string response = ":server " + codeStream.str() + " " + target;
    if (!command.empty())
        response += " " + command;
    if (!parameter.empty())
        response += " " + parameter;
    if (!message.empty())
        response += " :" + message;
    response += "\r\n";

    client->sendMessage(response);
}

C_Channel *C_Server::findChannel(const std::string &name)
{
    std::map<std::string, C_Channel *>::iterator channelIt = activeChannels.find(name);
    if (channelIt == activeChannels.end())
        return 0;
    return channelIt->second;
}

void C_Server::registerChannel(const std::string &name, C_Channel *channel)
{
    activeChannels[name] = channel;
}

bool C_Server::hasChannel(const std::string &name) const
{
    return activeChannels.find(name) != activeChannels.end();
}

void C_Server::releaseResources()
{
    for (std::map<std::string, C_Channel *>::iterator channelIt = activeChannels.begin(); channelIt != activeChannels.end(); channelIt++)
        delete channelIt->second;

    activeChannels.clear();
    for (std::map<std::string, C_commands*>::iterator commandIt = commandHandlers.begin(); commandIt != commandHandlers.end(); ++commandIt)
        delete commandIt->second;

    for (std::map<int, C_Client*>::iterator clientIt = connectedClients.begin(); clientIt != connectedClients.end(); clientIt++)
    {
        std::cout << "Closing FD: " << clientIt->first << std::endl;
        close(clientIt->first);
        delete clientIt->second;
    }
    monitoredSockets.clear();
    close(listeningSocket);
    std::cout << "Server Shutdown Cleanly." << std::endl;
}

C_Client *C_Server::findClientByNickname(const std::string &nickname)
{
    for (std::map<int, C_Client *>::iterator clientIt = connectedClients.begin(); clientIt != connectedClients.end(); clientIt++)
    {
        if (clientIt->second->getNickname() == nickname)
            return clientIt->second;
    }
    return (NULL);
}

C_Server::~C_Server()
{
}
