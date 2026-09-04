#include "client/Client.hpp"
#include <sys/socket.h>

void C_Client::joinChannel(C_Channel *ch)
{
    joinedChannels.insert(ch);
}

void C_Client::leaveChannel(C_Channel *ch)
{
    joinedChannels.erase(ch);
}

bool C_Client::belongsToChannel(std::string channel)
{
    for (std::set<C_Channel*>::iterator it = joinedChannels.begin(); it != joinedChannels.end(); it++)
    {
        if ((*it)->getName() == channel)
            return true;
    }
    return false;
}

void C_Client::sendMessage(std::string msg) const
{
    send(socketDescriptor, msg.c_str(), msg.size(), 0);
}
