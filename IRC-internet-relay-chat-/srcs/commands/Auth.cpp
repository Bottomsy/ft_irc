#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"

bool C_Server::nicknameIsTaken(std::string nickname)
{
    std::set<std::string>::iterator pos = registeredNicknames.find(nickname);
    return (pos != registeredNicknames.end());
}
void C_Server::processNickname(C_Client *client, t_command &cmd)
{
    std::string special = "-_[]\\`^{}";
    if (cmd.args.empty())
    {
        sendError(client, 431, "NICK", "", "No nickname given");
        return;
    }
    std::string nick = cmd.args[0];
    if (nick.size() == 0)
    {
        sendError(client, 431, "NICK", "", "No nickname given");
        return;
    }
    if (nick.size() > 9 || isdigit(nick[0]) || nick[0] == '-')
    {
        sendError(client, 432, "NICK", nick, "Erroneous nickname");
        return;
    }
    for (size_t i = 0; i < nick.size(); i++)
    {
        if (!isalnum(nick[i]) && special.find(nick[i]) == std::string::npos)
        {
        sendError(client, 432, "NICK", nick, "Erroneous nickname");
        return;
        }
    }
    if (nicknameIsTaken(nick))
    {
        sendError(client, 433, "NICK", nick, "Nickname is already in use");
        return;
    }
    if (!client->getNickname().empty())
    {
        std::set<std::string>::iterator it = registeredNicknames.find(client->getNickname());
        if (it != registeredNicknames.end())
            registeredNicknames.erase(it);
    }
    client->setNickname(nick);
    registeredNicknames.insert(nick);
    client->updateAuthentication(AUTH_NICK);
    if (registrationComplete(client) && (client->authenticationState() & AUTH_WELC) != AUTH_WELC)
    {
        client->updateAuthentication(AUTH_WELC);
        client->sendMessage(":server 001 " + client->getNickname() + " :Welcome to the Internet Relay Network\r\n");
    }
}

void C_Server::processPassword(C_Client *client, t_command &cmd)
{
    if (client->authenticationState() & AUTH_PASS)
    {
        sendError(client, 462, "PASS", "", "You may not reregister");
        return;
    }
    if (cmd.args.empty() || cmd.args.size() != 1)
    {
        sendError(client, 461, "PASS", "", "Not enough parameters");
        return;
    }
    std::string password = cmd.args[0];
    if (password != serverPassword)
    {
        client->updateAuthentication(QUIT);
        sendError(client, 464, "PASS", "", "Password incorrect");
        return;
    }
    client->updateAuthentication(AUTH_PASS);
    if (registrationComplete(client) && (client->authenticationState() & AUTH_WELC) != AUTH_WELC)
    {
        client->updateAuthentication(AUTH_WELC);
        client->sendMessage(":server 001 " + client->getNickname() + " :Welcome to the Internet Relay Network\r\n");
    }
}
void C_Server::processUserRegistration(C_Client *client, t_command &cmd)
{
    if (cmd.args.empty() || cmd.args.size() < 4)
    {
        sendError(client, 461, "USER", "", "Not enough parameters");
        return;
    }
    if (client->getUsername() != "")
    {
        sendError(client, 462, "USER", "", "You may not reregister");
        return;
    }
    client->setUsername(cmd.args[0]);
    client->setRealName(cmd.args[3]);
    client->updateAuthentication(AUTH_USER);
    if (registrationComplete(client) && (client->authenticationState() & AUTH_WELC) != AUTH_WELC)
    {
        client->updateAuthentication(AUTH_WELC);
        client->sendMessage(":server 001 " + client->getNickname() + " :Welcome to the Internet Relay Network\r\n");
    }
}
void C_Server::respondToPing(C_Client *client, t_command &cmd)
{
    if (cmd.args.empty())
        client->sendMessage("PONG\r\n");
    else
        client->sendMessage("PONG :" + cmd.args[0] + "\r\n");
}
void C_Server::closeClientSession(C_Client *client, t_command &cmd)
{
    client->updateAuthentication(QUIT);
    if (cmd.args.empty())
        client->sendMessage("Closing connection\r\n");
    else
        client->sendMessage("Closing connection (" + cmd.args[0] + ")\r\n");
}
bool C_Server::registrationComplete(C_Client *client)
{
    return ((client->authenticationState() & AUTH_DONE) == AUTH_DONE);
}
