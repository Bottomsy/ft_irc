#include "commands/Kick.hpp"
#include "channel/Channel.hpp"


C_kick::C_kick(){

}
C_kick::~C_kick(){

}
void C_kick::execute(C_Server& server,C_Client& client, const std::vector<std::string>& args){

    if (args.size() < 2){
        server.sendError(&client, 461, "KICK", "", "Not enough parameters");
        return;
    }
    std::string channel_name = args[0];
    std::string target_nick = args[1];
    std::string reason = (args.size() > 2) ? args[2] : "";

    C_Channel* channel = server.findChannel(channel_name);

    if (!channel){
        server.sendError(&client, 403, "", channel_name, "No such channel");
        return;
    }
    if (!channel->hasClient(client)){
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
    if (!channel->hasClient(*target)){
        server.sendError(&client, 441, "", target_nick + " " + channel_name, "They aren't on that channel");
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