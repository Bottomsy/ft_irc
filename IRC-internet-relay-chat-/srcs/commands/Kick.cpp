#include "Kick.hpp"
#include "Channel.hpp"


C_kick::C_kick(){

}
C_kick::~C_kick(){

}
void C_kick::execute(C_Server& server,C_Client& client, const std::vector<std::string>& args){

    if (args.size() < 2){
        client.sendMessage(":server 461 " + client.getNickname() + " KICK :Not enough parameters\r\n");
        return;
    }
    std::string channel_name = args[0];
    std::string target_nick = args[1];
    std::string reason = (args.size() > 2) ? args[2] : "";

    C_Channel* channel = server.findChannel(channel_name);

    if (!channel){
        client.sendMessage(":server 403 " +client.getNickname() + " " + channel_name + " :No such channel\r\n");
        return;
    }
    if (!channel->hasClient(client)){
        client.sendMessage(":server 442 " + client.getNickname() + " " + channel_name + " :You're not on that channel\r\n");
        return;
    }
    if (!channel->isOperator(client)){
        client.sendMessage(":server 482 " + client.getNickname() + " " + channel_name + " :You're not channel operator\r\n");
        return;
    }
    C_Client* target = server.findClientByNickname(target_nick);

    if (!target){
        client.sendMessage(":server 401 " + client.getNickname() + " " + target_nick + " :No such nick/channel\r\n");

        return;
    }
    if (!channel->hasClient(*target)){
        client.sendMessage(":server 441 " + client.getNickname() + " " + target_nick + " " + channel_name + " :They aren't on that channel\r\n");
        return;
    }
    std::string msg = ":" + client.getNickname() + " KICK " + channel_name + " " + target_nick ;
    if (!reason.empty())
        msg += " :" + reason + "\r\n";
    else 
    msg += "\r\n";
    channel->broadcast(msg,NULL);
    channel->removeClient(*target);
    target->sendMessage("You were kicked from " + channel_name + ":" + reason + "\r\n");
}