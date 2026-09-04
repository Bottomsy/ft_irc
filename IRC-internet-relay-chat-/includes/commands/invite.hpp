#pragma once 

#include "commands/commands.hpp"

class C_invite : public C_commands{
    public:
    void execute (C_Server& server,C_Client& client, const std::vector<std::string>& args);
    C_invite();
    ~C_invite();
};