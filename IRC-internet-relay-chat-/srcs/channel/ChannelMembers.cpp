#include "Channel.hpp"
#include "Client.hpp"

void C_Channel::addClient(C_Client& client, const std::string& given_key)
{
    if (!_key.empty() && _key != given_key)
    {
        client.sendMessage("wrong password!\r\n");
        return;
    }
    if (_userLimit != 0 && _members.size() >= _userLimit)
    {
        client.sendMessage("Channel is full\r\n");
        return;
    }
    if (hasClient(client))
    {
        client.sendMessage("client is already in \r\n");
        return;
    }
    if (_members.insert(&client).second)
    {
        client.joinChannel(this);
        std::string msg = ":" + client.getNickname() + " JOIN " + _name;
        broadcast(msg, &client);
        client.sendMessage("You joined " + _name + "\r\n");

        std::string list = "Members : ";
        for (std::set<const C_Client*>::iterator it = _members.begin(); it != _members.end(); it++)
            list += (*it)->getNickname() + " ";
        client.sendMessage(list + "\r\n");

        if (!_topic.empty())
            client.sendMessage("Topic: " + _topic + "\r\n");
    }
}

void C_Channel::inviteClient(C_Client& client)
{
    _invited.insert(&client);
}

void C_Channel::removeClient(C_Client& client)
{
    _members.erase(&client);
    _operators.erase(&client);
    _invited.erase(&client);
    client.leaveChannel(this);
}

bool C_Channel::hasClient(const C_Client& client) const
{
    return _members.find(&client) != _members.end();
}

bool C_Channel::isOperator(const C_Client& client) const
{
    return _operators.find(&client) != _operators.end();
}

void C_Channel::broadcast(const std::string& msg, const C_Client* sender)
{
    for (std::set<const C_Client*>::iterator it = _members.begin(); it != _members.end(); it++)
    {
        if (*it != sender)
            (*it)->sendMessage(msg + "\r\n");
    }
}

void C_Channel::addOperator(C_Client& client)
{
    _operators.insert(&client);
}

void C_Channel::removeOperator(C_Client& client)
{
    _operators.erase(&client);
}

bool C_Channel::isInvited(const C_Client& client) const
{
    return _invited.find(&client) != _invited.end();
}

void C_Channel::kick(C_Client& client)
{
    _members.erase(&client);
}

bool C_Channel::is_empty() const
{
    return _members.empty();
}
