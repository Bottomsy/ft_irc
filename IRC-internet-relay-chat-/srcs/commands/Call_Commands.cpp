#include "Server.hpp"
#include "Client.hpp"
#include "commands.hpp"

void C_Server::dispatchCommand(t_command &cmd, int fd)
{
    C_Client *client = connectedClients[fd];
    if (!client || cmd.command.empty())
        return;

    if (cmd.command == "PASS")
        processPassword(client, cmd);
    else if (cmd.command == "NICK")
        processNickname(client, cmd);
    else if (cmd.command == "USER")
        processUserRegistration(client, cmd);
    else if (cmd.command == "PING")
        respondToPing(client, cmd);
    else if (cmd.command == "QUIT")
        closeClientSession(client, cmd);
    else if (!registrationComplete(client))
        sendError(client, 451, "", "", "You have not registered");
    else if (cmd.command == "TOPIC")
        processTopic(client, cmd);
    else if (cmd.command == "MODE")
        processMode(client, cmd);
    else if (commandHandlers.find(cmd.command) != commandHandlers.end())
        commandHandlers[cmd.command]->execute(*this, *client, cmd.args);
    else
        sendError(client, 421, "", cmd.command, "Unknown command");
}
