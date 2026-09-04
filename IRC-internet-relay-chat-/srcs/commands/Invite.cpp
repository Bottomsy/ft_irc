#include "commands/invite.hpp"
#include "channel/Channel.hpp"

void C_invite::execute(C_Server& server, C_Client& client ,const std::vector<std::string>& args){

    if (args.size() < 2 ){
           server.sendError(&client, 461, "INVITE", "", "Not enough parameters");
        return;
    }
    std::string target_nick = args[0];
    std::string channel_name = args[1];

    C_Channel* channel  = server.findChannel(channel_name);

    if (!channel){
           server.sendError(&client, 403, "", channel_name, "No such channel");
        return;
    }
    if (!channel->hasClient(client))
    {
           server.sendError(&client, 442, "", channel_name, "You're not on that channel");
        return;
    }

    if (!channel->isOperator(client)){
           server.sendError(&client, 482, "", channel_name, "You're not channel operator");
        return;
    }

    C_Client* target = server.findClientByNickname(target_nick);

    if (!target){
           server.sendError(&client, 401, "", target_nick, "No such nick/channel");
        return;
    }
    if (channel->hasClient(*target)){
           server.sendError(&client, 443, "", target_nick + " " + channel_name, "is already on channel");
        return;
    }
    channel->inviteClient(*target);
    std::string msg = ":" + client.getNickname() + "!" + client.getUsername() + "@localhost INVITE " + target_nick + " :" + channel_name + "\r\n";
    target->sendMessage(msg);
    client.sendMessage(":server 341 " + client.getNickname() + " " + target_nick + " " + channel_name + "\r\n");
}
    C_invite::C_invite(){}
    C_invite::~C_invite(){}