#include "Channel.hpp"
#include "Client.hpp"

C_Channel::C_Channel(const std::string& name, C_Client& creator)
    : _name(name), _inviteOnly(false), _topicRestricted(false), _userLimit(0)
{
    if (_members.insert(&creator).second)
    {
        _operators.insert(&creator);
        creator.joinChannel(this);
    }
}

void C_Channel::add_key(const std::string& key)
{
    _key = key;
}

C_Channel::C_Channel(const C_Channel& other)
{
    (void)other;
}

C_Channel& C_Channel::operator=(const C_Channel& other)
{
    (void)other;
    return (*this);
}

C_Channel::~C_Channel()
{
}
