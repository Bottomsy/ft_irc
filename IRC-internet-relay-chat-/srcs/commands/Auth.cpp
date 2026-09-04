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
        std::string target = "*";
        if (!client->getNickname().empty())
            target = client->getNickname();
        client->sendMessage(":server 431 " + target + " NICK :No nickname given\r\n");
        return;
    }
    std::string nick = cmd.args[0];
    std::string target = "*";
    if (!client->getNickname().empty())
        target = client->getNickname();
    if (nick.size() == 0)
    {
        client->sendMessage(":server 431 " + target + " NICK :No nickname given\r\n");
        return;
    }
    if (nick.size() > 9 || isdigit(nick[0]) || nick[0] == '-')
    {
        client->sendMessage(":server 432 " + target + " " + nick + " :Erroneous nickname\r\n");
        return;
    }
    for (size_t i = 0; i < nick.size(); i++)
    {
        if (!isalnum(nick[i]) && special.find(nick[i]) == std::string::npos)
        {
        client->sendMessage(":server 432 " + target + " " + nick + " :Erroneous nickname\r\n");
        return;
        }
    }
    if (nicknameIsTaken(nick))
    {
        client->sendMessage(":server 433 " + target + " " + nick + " :Nickname is already in use\r\n");
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
    std::string target = "*";
    if (!client->getNickname().empty())
        target = client->getNickname();
    if (client->authenticationState() & AUTH_PASS)
    {
        client->sendMessage(":server 462 " + target + " PASS :You may not reregister\r\n");
        return;
    }
    if (cmd.args.empty() || cmd.args.size() != 1)
    {
        client->sendMessage(":server 461 " + target + " PASS :Not enough parameters\r\n");
        return;
    }
    std::string password = cmd.args[0];
    if (password != serverPassword)
    {
        client->updateAuthentication(QUIT);
        client->sendMessage(":server 464 " + target + " PASS :Password incorrect\r\n");
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
    std::string target = "*";
    if (!client->getNickname().empty())
    target = client->getNickname();
    if (cmd.args.empty() || cmd.args.size() < 4)
    {
        client->sendMessage(":server 461 " + target + " USER :Not enough parameters\r\n");
        return;
    }
    if (client->getUsername() != "")
    {
        client->sendMessage(":server 462 " + target +  " USER :You may not reregister\r\n");
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
