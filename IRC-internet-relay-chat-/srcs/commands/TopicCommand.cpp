#include "server/Server.hpp"
#include "client/Client.hpp"

void C_Server::processTopic(C_Client *client, t_command &cmd)
{
    if (cmd.args.empty())
    {
        client->sendMessage("Syntax : <Topic> <Channel>\r\n");
        return;
    }

    std::map<std::string, C_Channel*>::iterator channelIt;
    for (channelIt = activeChannels.begin(); channelIt != activeChannels.end(); channelIt++)
    {
        if (channelIt->second->getName() == cmd.args[0])
            break;
    }

    if (channelIt == activeChannels.end())
    {
        client->sendMessage("There is no such a channel name\r\n");
        return;
    }

    C_Channel *channel = channelIt->second;

    if (cmd.args.size() == 1)
    {
        if (!client->belongsToChannel(channel->getName()))
        {
            client->sendMessage("You are not in this channel\r\n");
            return;
        }
        if (channel->getTopic().empty())
        {
            client->sendMessage(":server 331 " + client->getNickname() + " " + cmd.args[0] + " :No topic is set\r\n");
        }
        else
        {
            client->sendMessage(":server 332 " + client->getNickname() + " " + cmd.args[0] + " :" + channel->getTopic() + "\r\n");
        }
    }
    else if (cmd.args.size() == 2)
    {
        if (!client->belongsToChannel(channel->getName()))
        {
            client->sendMessage("You are not in this channel\r\n");
            return;
        }
        if (channel->isTopicRestricted() && !channel->isOperator(*client))
        {
            client->sendMessage("You are not an operator\r\n");
            return;
        }
        channel->setTopic(cmd.args[1]);
        channel->broadcast(":" + client->getNickname() + " TOPIC " + cmd.args[0] + " :" + cmd.args[1], NULL);
    }
    else
    {
        client->sendMessage("Syntax : <Topic> <Channel>\r\n");
    }
}
