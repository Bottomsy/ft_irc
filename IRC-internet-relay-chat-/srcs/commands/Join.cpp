#include "commands/JoinCommand.hpp"
#include "channel/Channel.hpp"

void C_JoinCommand::execute (C_Server& server,C_Client& client, const std::vector<std::string>& args){

    if  (args.empty()){
        server.sendError(&client, 461, "JOIN", "", "Not enough parameters");
        return;
    }

    std::string channel_name = args[0];
    if ( channel_name[0] != '#'){
        server.sendError(&client, 403, "", channel_name, "No such channel");
        return;
    }
    std::string channel_password = (args.size() > 1)? args[1]:"";
    C_Channel* channel = server.findChannel(channel_name);
    
    if (channel){
        if (channel->isInviteOnly()){
            if (channel->isInvited(client)){
                channel->addClient(client,channel_password);
            }
            else{
                server.sendError(&client, 473, "", channel_name, "Cannot join channel (+i)");
                return;
            }
        }
        else{
            channel->addClient(client,channel_password);
        }
    }
    else{
        channel = new C_Channel(channel_name,client);
        server.registerChannel(channel_name,channel);
        client.sendMessage("You created and joined " + channel_name + "\r\n");
    }
}

C_JoinCommand::~C_JoinCommand(){}
    
C_JoinCommand::C_JoinCommand(){}