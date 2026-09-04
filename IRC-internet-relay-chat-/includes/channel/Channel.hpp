#pragma once 

#include <iostream>
#include <set>
#include <map>
class C_Client;

class C_Channel {
private:
    std::string _name;
    std::string _topic;
    std::string _key;

    std::set<const C_Client*> _members;
    std::set<const C_Client*> _operators;
    std::set<const C_Client*> _invited;

    bool _inviteOnly;
    bool _topicRestricted;
    size_t _userLimit;
    C_Channel(const C_Channel& other);
    C_Channel& operator=(const C_Channel& other);
    public:
    C_Channel(const std::string& name,C_Client& creator);
    ~C_Channel();
    
    const std::string& getName() const{return _name;}
    std::string getTopic() {return _topic;}

    void setTopic(std::string topic){_topic = topic;}
    void addClient(C_Client& client,const std::string& given_key);
    void inviteClient(C_Client& client);
    void removeClient(C_Client& client);

    bool hasClient(const C_Client& client) const;
    bool isOperator(const C_Client& client) const;
    bool isInvited(const C_Client& client) const;
    void add_key(const std::string& key);
    void broadcast(const std::string& msg, const C_Client* sender);
    void setInviteOnly(bool val);
    void setTopicRestricted(bool val);
    void setKey(const std::string& key);
    void removeKey();
    void setLimit(size_t limit);
    void addOperator(C_Client& client);
    void removeOperator(C_Client& client);
    bool isTopicRestricted()const;
    bool isInviteOnly()const;
    void kick(C_Client& client);
    bool is_empty()const;

};