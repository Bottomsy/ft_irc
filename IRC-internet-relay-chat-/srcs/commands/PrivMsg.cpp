#include "PrivMsgCommand.hpp"
#include "Channel.hpp"


void C_privMsgCommand::execute(C_Server& server,C_Client& client, const std::vector<std::string>& args){

    if (args.empty()){
        client.sendMessage(":server 411 " +client.getNickname() +" :No recipient given (PRIVMSG)\r\n");
        return;
    }
    if (args.size() < 2) {
        client.sendMessage(":server 412 " +client.getNickname() + " :No text to send\r\n");
        return;
    }
    std::string target = args[0];
    std::string msg = args[1];
    
    if (target[0] == '#'){
        C_Channel *channel = server.findChannel(target);
        if (!channel){
            client.sendMessage(":server 403 " + client.getNickname() + " " + target + " :No such channel\r\n");
            return;
        }
        if  ( !channel->hasClient(client)){
            client.sendMessage(":server 404 " + client.getNickname() + " " + target + " :Cannot send to channel\r\n");
            return;
        }
        std::string full_msg = ":" + client.getNickname() +  " PRIVMSG " + target + " :" + msg + "\r\n";
        channel->broadcast(full_msg,&client);
    }
    else{
        C_Client *client_targeted = server.findClientByNickname(target);

        if (!client_targeted){
            client.sendMessage(":server 401 " + client.getNickname() + " " + target + " :No such nick/channel\r\n");
            return;
        }
        std::string full_message = ":" + client.getNickname() + " PRIVMSG " + target + " :" + msg + "\r\n";
        client_targeted->sendMessage(full_message);
    }
}
   C_privMsgCommand::~C_privMsgCommand(){}
   C_privMsgCommand::C_privMsgCommand(){}