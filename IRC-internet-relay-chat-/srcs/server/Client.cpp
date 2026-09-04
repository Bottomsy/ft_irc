#include "Client.hpp"

C_Client::~C_Client()
{
}

C_Client::C_Client(int fd, std::string ip) : socketDescriptor(fd), ipAddress(ip), authenticationFlags(0)
{
}

const std::string &C_Client::getNickname() const
{
    return nickname;
}

int C_Client::getSocketDescriptor() const
{
    return socketDescriptor;
}

const std::string &C_Client::getUsername() const
{
    return username;
}

std::string C_Client::getRealName() const
{
    return this->realname;
}

int C_Client::authenticationState() const
{
    return this->authenticationFlags;
}

void C_Client::setNickname(const std::string &nick)
{
    nickname = nick;
}

void C_Client::setUsername(const std::string &user)
{
    username = user;
}

void C_Client::setRealName(std::string name)
{
    this->realname = name;
}

void C_Client::updateAuthentication(int value)
{
    this->authenticationFlags |= value;
}
