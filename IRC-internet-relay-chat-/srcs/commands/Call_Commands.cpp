#include "Server.hpp"
#include "Client.hpp"
#include "commands.hpp"

void C_Server::dispatchCommand(t_command &cmd, int fd)
{
    C_Client *client = connectedClients[fd];
    if (!client)
        return ;
    if (cmd.command.empty())
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
        client->sendMessage(":server 451 * :You have not registered\r\n");
    else if (cmd.command == "TOPIC"){
        if (cmd.args.empty()){
            client->sendMessage("Syntax : <Topic> <Channel>\r\n");
            return;
        }
        std::map<std::string,C_Channel*>::iterator it ;
        for  (it  = activeChannels.begin();it != activeChannels.end();it++){
            if (it->second->getName() == cmd.args[0])
                break;
        }
        if (it == activeChannels.end()){
            client->sendMessage("There is no such a channel name\r\n");
            return ;
        }
        if (cmd.args.size() == 1){
            if (!client->belongsToChannel(it->second->getName())) {
                client->sendMessage("You are not in this channel\r\n");
                return;
            }
            if (it->second->getTopic().empty()) {
                client->sendMessage(":server 331 " + client->getNickname() + " " + cmd.args[0] + " :No topic is set\r\n");
            }
            else{
                client->sendMessage(":server 332 " + client->getNickname() + " " + cmd.args[0] + " :" + it->second->getTopic() + "\r\n");
            }
        }
        else if (cmd.args.size() == 2)
        {
            if (!client->belongsToChannel(it->second->getName())) {
                client->sendMessage("You are not in this channel\r\n");
                return;
            }
            if (it->second->isTopicRestricted() && !it->second->isOperator(*client))
            {
                    client->sendMessage("You are not an operator\r\n");
                    return;
            }
            else
            {
                it->second->setTopic(cmd.args[1]);
                it->second->broadcast(":" + client->getNickname() + " TOPIC " + cmd.args[0] + " :" + cmd.args[1], NULL);
            }
        }
        else {
            client->sendMessage("Syntax : <Topic> <Channel>\r\n");
        }
    }
    else if ( cmd.command == "MODE"){

        if  (cmd.args.size() < 2 ){
            client->sendMessage("MODE  needs args\r\n");
            return;
        }
        std::string channel_name = cmd.args[0];
        std::string mode = cmd.args[1];
        C_Channel* channel = findChannel(channel_name);
        if (!channel){
            client->sendMessage("channel does not exist\r\n");
            return;
        }
        if (!channel->isOperator(*client)){
            client->sendMessage("You are not operator\r\n");
            return;
        }
        if (mode == "+i"){
            channel->setInviteOnly(true);
        }
        else if (mode == "-i"){
            channel->setInviteOnly(false);
        }
        else if ( mode == "+o"){
            if (cmd.args.size() < 3){
                client->sendMessage("MODE +0 needs args\r\n");
                return;
            }
            std::string target_nick = cmd.args[2];
            C_Client* target = findClientByNickname(target_nick);
            if (!target){
                client->sendMessage("User not found\r\n");
                return;
            }
            if (!channel->hasClient(*target)){
                client->sendMessage("User not in channel\r\n");
                return;
            }
            channel->addOperator(*target);
            channel->broadcast(":"+client->getNickname()+ " MODE "+ channel_name + " +o " + target_nick, NULL);
        }
        else if (mode == "-o"){    
            
            if (cmd.args.size() < 3) {
                client->sendMessage("MODE -o needs a nickname\r\n");
                return;
            }

            std::string targetNick = cmd.args[2];
            C_Client* target = findClientByNickname(targetNick);
            if (!target) {
                client->sendMessage("User not found\r\n");
                return;
            }
            if (!channel->hasClient(*target)) {
                client->sendMessage("User not in channel\r\n");
                return;
            }
            channel->removeOperator(*target);
             channel->broadcast(":" + client->getNickname() + " MODE " + channel_name + " -o " + targetNick, NULL);
        }
        else if (mode == "+t"){
            channel->setTopicRestricted(true);
            channel->broadcast(":" + client->getNickname() +" MODE " + channel_name + " +t", NULL);
        }
        else if (mode == "-t") {
            channel->setTopicRestricted(false);
            channel->broadcast(":" + client->getNickname() +" MODE " + channel_name + " -t", NULL);
        }
        else if (mode == "+k") {
            if (cmd.args.size() < 3) {
                client->sendMessage("MODE +k needs a password\r\n");
                return;
            }
            std::string key = cmd.args[2];
            channel->setKey(key);
            channel->broadcast(":" + client->getNickname() +" MODE " + channel_name + " +k", NULL);
        }
        else if (mode == "-k") {
            channel->removeKey();
            channel->broadcast(":" + client->getNickname() +" MODE " + channel_name + " -k", NULL);
        }
        else if (mode == "+l") {
        
            if (cmd.args.size() < 3) {
                client->sendMessage("MODE +l needs a number\r\n");
                return;
            }
            int limit = std::atoi(cmd.args[2].c_str());
        
            if (limit <= 0) {
                client->sendMessage("Invalid limit\r\n");
                return;
            }
            channel->setLimit(limit);
            channel->broadcast(":" + client->getNickname() +" MODE " + channel_name + " +l " + cmd.args[2], NULL);
        }
        else if (mode == "-l") {
            channel->setLimit(0);
            channel->broadcast(":" + client->getNickname() +" MODE " + channel_name + " -l", NULL);
        }
    }
    else
    {
          if (commandHandlers.find(cmd.command) != commandHandlers.end())
              commandHandlers[cmd.command]->execute(*this, *client, cmd.args); 
        else
                client->sendMessage(":server 421 * : " + cmd.command + " Unknown command\r\n");
    }
}



