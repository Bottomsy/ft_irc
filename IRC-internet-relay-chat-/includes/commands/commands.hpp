#pragma once

#include "server/Server.hpp"
#include "client/Client.hpp"
#include <vector>
#include <iostream>

class C_Server;
class C_commands{
    public:
    virtual ~C_commands() {}
    virtual void execute (C_Server& server,C_Client& client, const std::vector<std::string>& args) = 0;

};
