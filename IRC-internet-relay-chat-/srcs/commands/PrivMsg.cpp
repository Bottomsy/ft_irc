#include "PrivMsgCommand.hpp"
#include "Channel.hpp"


void C_privMsgCommand::execute(C_Server& server,C_Client& client, const std::vector<std::string>& args){

    if (args.empty()){
        server.sendError(&client, 411, "PRIVMSG", "", "No recipient given (PRIVMSG)");
        return;
    }
    if (args.size() < 2) {
        server.sendError(&client, 412, "PRIVMSG", "", "No text to send");
        return;
    }
    std::string target = args[0];
    std::string msg = args[1];
    
    if (target[0] == '#'){
        C_Channel *channel = server.findChannel(target);
        if (!channel){
            server.sendError(&client, 403, "", target, "No such channel");
            return;
        }
        if  ( !channel->hasClient(client)){
            server.sendError(&client, 404, "", target, "Cannot send to channel");
            return;
        }
        std::string full_msg = ":" + client.getNickname() +  " PRIVMSG " + target + " :" + msg + "\r\n";
        channel->broadcast(full_msg,&client);
    }
    else{
        C_Client *client_targeted = server.findClientByNickname(target);

        if (!client_targeted){
            server.sendError(&client, 401, "", target, "No such nick/channel");
            return;
        }
        std::string full_message = ":" + client.getNickname() + " PRIVMSG " + target + " :" + msg + "\r\n";
        client_targeted->sendMessage(full_message);
    }
}
   C_privMsgCommand::~C_privMsgCommand(){}
   C_privMsgCommand::C_privMsgCommand(){}