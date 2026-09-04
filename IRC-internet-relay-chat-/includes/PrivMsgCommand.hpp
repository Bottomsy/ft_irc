#pragma once

#include "commands.hpp"

class C_privMsgCommand : public C_commands{
    public:
    void execute (C_Server& server,C_Client& client, const std::vector<std::string>& args);
    ~C_privMsgCommand();
    C_privMsgCommand();
};