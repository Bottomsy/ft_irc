#include "channel/Channel.hpp"

bool C_Channel::isInviteOnly() const
{
    return _inviteOnly;
}

void C_Channel::setInviteOnly(bool val)
{
    _inviteOnly = val;
}

bool C_Channel::isTopicRestricted() const
{
    return _topicRestricted;
}

void C_Channel::setTopicRestricted(bool val)
{
    _topicRestricted = val;
}

void C_Channel::setKey(const std::string& key)
{
    _key = key;
}

void C_Channel::removeKey()
{
    _key.clear();
}

void C_Channel::setLimit(size_t limit)
{
    _userLimit = limit;
}
