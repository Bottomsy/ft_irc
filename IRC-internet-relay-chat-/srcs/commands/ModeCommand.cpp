#include "Server.hpp"
#include "Client.hpp"

void C_Server::processMode(C_Client *client, t_command &cmd)
{
    if (cmd.args.size() < 2)
    {
        client->sendMessage("MODE  needs args\r\n");
        return;
    }

    std::string channelName = cmd.args[0];
    std::string mode = cmd.args[1];
    C_Channel *channel = findChannel(channelName);

    if (!channel)
    {
        client->sendMessage("channel does not exist\r\n");
        return;
    }
    if (!channel->isOperator(*client))
    {
        client->sendMessage("You are not operator\r\n");
        return;
    }

    if (mode == "+i")
    {
        channel->setInviteOnly(true);
    }
    else if (mode == "-i")
    {
        channel->setInviteOnly(false);
    }
    else if (mode == "+o")
    {
        if (cmd.args.size() < 3)
        {
            client->sendMessage("MODE +0 needs args\r\n");
            return;
        }
        std::string targetNickname = cmd.args[2];
        C_Client *target = findClientByNickname(targetNickname);
        if (!target)
        {
            client->sendMessage("User not found\r\n");
            return;
        }
        if (!channel->hasClient(*target))
        {
            client->sendMessage("User not in channel\r\n");
            return;
        }
        channel->addOperator(*target);
        channel->broadcast(":" + client->getNickname() + " MODE " + channelName + " +o " + targetNickname, NULL);
    }
    else if (mode == "-o")
    {
        if (cmd.args.size() < 3)
        {
            client->sendMessage("MODE -o needs a nickname\r\n");
            return;
        }
        std::string targetNickname = cmd.args[2];
        C_Client *target = findClientByNickname(targetNickname);
        if (!target)
        {
            client->sendMessage("User not found\r\n");
            return;
        }
        if (!channel->hasClient(*target))
        {
            client->sendMessage("User not in channel\r\n");
            return;
        }
        channel->removeOperator(*target);
        channel->broadcast(":" + client->getNickname() + " MODE " + channelName + " -o " + targetNickname, NULL);
    }
    else if (mode == "+t")
    {
        channel->setTopicRestricted(true);
        channel->broadcast(":" + client->getNickname() + " MODE " + channelName + " +t", NULL);
    }
    else if (mode == "-t")
    {
        channel->setTopicRestricted(false);
        channel->broadcast(":" + client->getNickname() + " MODE " + channelName + " -t", NULL);
    }
    else if (mode == "+k")
    {
        if (cmd.args.size() < 3)
        {
            client->sendMessage("MODE +k needs a password\r\n");
            return;
        }
        channel->setKey(cmd.args[2]);
        channel->broadcast(":" + client->getNickname() + " MODE " + channelName + " +k", NULL);
    }
    else if (mode == "-k")
    {
        channel->removeKey();
        channel->broadcast(":" + client->getNickname() + " MODE " + channelName + " -k", NULL);
    }
    else if (mode == "+l")
    {
        if (cmd.args.size() < 3)
        {
            client->sendMessage("MODE +l needs a number\r\n");
            return;
        }
        int limit = std::atoi(cmd.args[2].c_str());
        if (limit <= 0)
        {
            client->sendMessage("Invalid limit\r\n");
            return;
        }
        channel->setLimit(limit);
        channel->broadcast(":" + client->getNickname() + " MODE " + channelName + " +l " + cmd.args[2], NULL);
    }
    else if (mode == "-l")
    {
        channel->setLimit(0);
        channel->broadcast(":" + client->getNickname() + " MODE " + channelName + " -l", NULL);
    }
}
