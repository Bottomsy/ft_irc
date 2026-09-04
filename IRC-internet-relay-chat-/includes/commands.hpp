#pragma once

#include "Server.hpp"
#include "Client.hpp"
#include <vector>
#include <iostream>

class C_Server;
class C_commands{
    public:
    virtual ~C_commands() {}
    virtual void execute (C_Server& server,C_Client& client, const std::vector<std::string>& args) = 0;

};
