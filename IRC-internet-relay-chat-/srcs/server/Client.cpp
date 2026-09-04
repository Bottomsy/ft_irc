#include "Client.hpp"
#include "Server.hpp"

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


void C_Client::leaveChannel(C_Channel *ch)
{
    joinedChannels.erase(ch);
}

void C_Client::joinChannel(C_Channel *ch)
{
    joinedChannels.insert(ch);
}

void C_Client::appendReceivedData(char *buffer, size_t len)
{
    this->receiveBuffer.append(buffer, len);
    if (this->receiveBuffer.size() > 512)
        this->receiveBuffer.clear();
}

bool C_Client::containsCompleteLine()
{
    return (receiveBuffer.find("\r\n") != std::string::npos || receiveBuffer.find("\n") != std::string::npos);
}

void C_Client::sendMessage(std::string msg) const
{
    send(socketDescriptor, msg.C_str(), msg.size(), 0);
}

std::string C_Client::takeNextLine()
{
    std::string line;
    size_t pos = receiveBuffer.find("\r\n");
    if (pos != std::string::npos)
    {
        line = receiveBuffer.substr(0, pos);
        receiveBuffer.erase(0, pos + 2);
    }
    else
    {
        pos = receiveBuffer.find("\n");
        if (pos != std::string::npos)
        {
            line = receiveBuffer.substr(0, pos);
            receiveBuffer.erase(0, pos + 1);
        }
    }
    return line;
}

void C_Client::parseCommandLine(std::string line, t_command &cmd)
{
    size_t pos = 0;
    size_t end;
    if (line.empty())
        return;
    if (line[0] == ':')
    {
        pos = line.find(" ");
        if (pos == std::string::npos)
            return;
        pos++;
    }
    end = line.find(" ", pos);
    if (end == std::string::npos)
    {
        cmd.command = line.substr(pos);
        for (size_t i = 0; i < cmd.command.length(); i++)
            cmd.command[i] = toupper(cmd.command[i]);
        return;
    }
    cmd.command = line.substr(pos, end - pos);
    for (size_t i = 0; i < cmd.command.length(); i++)
        cmd.command[i] = toupper(cmd.command[i]);
    pos = end + 1;
    while (pos < line.length())
    {
        while (pos < line.length() && line[pos] == ' ')
            pos++;
        if (pos == line.length())
            break;
        if (line[pos] == ':')
        {
            cmd.args.push_back(line.substr(pos + 1));
            break;
        }
        end = line.find(" ", pos);
        if (end == std::string::npos)
        {
            cmd.args.push_back(line.substr(pos));
            break;
        }
        cmd.args.push_back(line.substr(pos, end - pos));
        pos = end + 1;
    }
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
