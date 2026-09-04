#pragma once

#include <iostream>
#include "Channel.hpp"
#include <cstdio>
#include <vector>
class C_Channel;

typedef struct s_command
{
    std::string command;
    std::vector<std::string> args;
} t_command;

enum _AUTH
{
    AUTH_PASS = 1 << 0,
    AUTH_USER = 1 << 1,
    AUTH_NICK = 1 << 2,
    AUTH_WELC = 1 << 3,
    QUIT = 1 << 4,
    AUTH_DONE = AUTH_PASS | AUTH_NICK | AUTH_USER
};
class C_Client
{
private:
    C_Client();
    C_Client(const C_Client &other);
    C_Client &operator=(const C_Client &other);
    int socketDescriptor;
    std::string nickname;
    std::string username;
    std::string realname;
    std::string ipAddress;
    std::string receiveBuffer;
    int authenticationFlags;
    std::set<C_Channel *> joinedChannels;

public:
    ~C_Client();
    C_Client(int, std::string);

    
    
    const std::string &getNickname() const ;
    const std::string &getUsername() const;
    std::string getRealName() const;
    int authenticationState() const;
    int getSocketDescriptor() const;
    
    
    void setNickname(const std::string &nick) ;
    void setUsername(const std::string &user);
    void setRealName(std::string name);
    void updateAuthentication(int value);



    void joinChannel(C_Channel *ch);
    void leaveChannel(C_Channel *ch);
    void appendReceivedData(char *, size_t);
    bool containsCompleteLine();
    void sendMessage(std::string) const;
    std::string takeNextLine();
    void parseCommandLine(std::string, t_command &);
    bool belongsToChannel(std::string);

};
