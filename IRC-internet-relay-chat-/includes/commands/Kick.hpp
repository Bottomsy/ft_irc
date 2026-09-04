#pragma once

#include  "commands/commands.hpp"


class C_kick : public C_commands{
   
    public:
    C_kick();
    ~C_kick();
    void execute(C_Server& server,C_Client& client, const std::vector<std::string>& args);
};