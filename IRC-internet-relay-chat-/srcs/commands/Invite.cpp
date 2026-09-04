#include "invite.hpp"
#include "Channel.hpp"

void C_invite::execute(C_Server& server, C_Client& client ,const std::vector<std::string>& args){

    if (args.size() < 2 ){
        client.sendMessage(":server 461 " + client.getNickname() + " INVITE :Not enough parameters\r\n");
        return;
    }
    std::string target_nick = args[0];
    std::string channel_name = args[1];

    C_Channel* channel  = server.findChannel(channel_name);

    if (!channel){
        client.sendMessage(":server 403 " + client.getNickname() + " " + channel_name + " :No such channel\r\n");
        return;
    }
    if (!channel->hasClient(client))
    {
        client.sendMessage(":server 442 " + client.getNickname() + " " + channel_name +  " :You're not on that channel\r\n");
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
    if (channel->hasClient(*target)){
        client.sendMessage(":server 443 " + client.getNickname() + " " + target_nick + " " + channel_name + " :is already on channel\r\n");
        return;
    }
    channel->inviteClient(*target);
    std::string msg = ":" + client.getNickname() + "!" + client.getUsername() + "@localhost INVITE " + target_nick + " :" + channel_name + "\r\n";
    target->sendMessage(msg);
    client.sendMessage(":server 341 " + client.getNickname() + " " + target_nick + " " + channel_name + "\r\n");
}
    C_invite::C_invite(){}
    C_invite::~C_invite(){}