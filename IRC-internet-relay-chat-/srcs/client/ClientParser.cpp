#include "client/Client.hpp"

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
