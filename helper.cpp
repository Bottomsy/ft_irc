#include "server.hpp"

int send_error(int fd, int code, Client *client, std::string params)
{
// error format: :IRC <code> <nick> <params> :<error message>\r\n
    switch (code)
    {
        // .. registration messages ... //
        case 001:
            send(fd, ":IRC 001 ", 9, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " :Welcome to the IRC server\r\n", 30, 0);
            return 1;
        // ... registration errors ... //
        case 431:
            send(fd, ":IRC 431 ", 9, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " :No nickname given\r\n", 21, 0);
            return 1;
        case 432:
            send(fd, ":IRC 432 ", 9, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " ", 1, 0);
            send(fd, params.c_str(), params.size(), 0);
            send(fd, " :Erroneous nickname\r\n", 22, 0);
            return 1;
        case 433:
            send(fd, ":IRC 433 ", 9, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " ", 1, 0);
            send(fd, params.c_str(), params.size(), 0);
            send(fd, " :Nickname is already in use\r\n", 30, 0);
            return 1;
        case 461:
            send(fd, ":IRC 461 ", 9, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " ", 1, 0);
            send(fd, params.c_str(), params.size(), 0);
            send(fd, " :Not enough parameters\r\n", 25, 0);
            return 1;
        case 462:
            send(fd, ":IRC 462 ", 9, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " :Already registered\r\n", 22, 0);
            return 1;
        case 464:
            send(fd, ":IRC 464 ", 9, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " :Password incorrect\r\n", 22, 0);
            return 1;

        // ... command errors ... //
        case 400:
            send(fd, ":IRC 400 ", 9, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " ", 1, 0);
            send(fd, params.c_str(), params.size(), 0);
            send(fd, " :Invalid command\r\n", 19, 0);
            return 1;
        case 401:
            send(fd, ":IRC 401 ", 9, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " ", 1, 0);
            send(fd, params.c_str(), params.size(), 0);
            send(fd, " :No such nick/channel\r\n", 24, 0);
            return 1;
        case 411:
            send(fd, ":IRC 411 ", 9, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " :No recipient given (PRIVMSG)\r\n", 32, 0);
            return 1;
        case 412:
            send(fd, ":IRC 412 ", 9, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " :No text to send\r\n", 19, 0);
            return 1;
        // ... Channel errors ... //
        case 403:
            send(fd, ":IRC 403 ", 9, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " ", 1, 0);
            send(fd, params.c_str(), params.size(), 0);
            send(fd, " :Channel not found\r\n", 21, 0);
            return 1;
        case 404:
            send(fd, ":IRC 404 ", 9, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " ", 1, 0);
            send(fd, params.c_str(), params.size(), 0);
            send(fd, " :Cannot send to channel\r\n", 26, 0);
            return 1;
        case 405:
            send(fd, ":IRC 405 ", 9, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " ", 1, 0);
            send(fd, params.c_str(), params.size(), 0);
            send(fd, " :Too many channels\r\n", 21, 0);
            return 1;
        case 441:
            send(fd, ":IRC 441 ", 9, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " ", 1, 0);
            send(fd, params.c_str(), params.size(), 0);
            send(fd, " :User not in channel\r\n", 23, 0);
            return 1;
        case 442:
            send(fd, ":IRC 442 ", 9, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " ", 1, 0);
            send(fd, params.c_str(), params.size(), 0);
            send(fd, " :You are not on that channel\r\n", 31, 0);
            return 1;
        case 443:
            send(fd, ":IRC 443 ", 9, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " ", 1, 0);
            send(fd, params.c_str(), params.size(), 0);
            send(fd, " :User already on channel\r\n", 27, 0);
            return 1;
        case 467:
            send(fd, ":IRC 467 ", 9, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " ", 1, 0);
            send(fd, params.c_str(), params.size(), 0);
            send(fd, " :Channel key already set\r\n", 27, 0);
            return 1;
        case 471:
            send(fd, ":IRC 471 ", 9, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " ", 1, 0);
            send(fd, params.c_str(), params.size(), 0);
            send(fd, " :Channel is full\r\n", 19, 0);
            return 1;
        case 472:
            send(fd, ":IRC 472 ", 9, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " ", 1, 0);
            send(fd, params.c_str(), params.size(), 0);
            send(fd, " :Unknown channel mode\r\n", 24, 0);
            return 1;
        case 473:
            send(fd, ":IRC 473 ", 9, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " ", 1, 0);
            send(fd, params.c_str(), params.size(), 0);
            send(fd, " :Channel is invite only\r\n", 26, 0);
            return 1;
        case 475:
            send(fd, ":IRC 475 ", 9, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " ", 1, 0);
            send(fd, params.c_str(), params.size(), 0);
            send(fd, " :Incorrect password\r\n", 22, 0);
            return 1;
        case 477:
            send(fd, ":IRC 477 ", 9, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " ", 1, 0);
            send(fd, params.c_str(), params.size(), 0);
            send(fd, " :Channel mode is invalid\r\n", 27, 0);
            return 1;
        case 479:
            send(fd, ":IRC 479 ", 9, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " ", 1, 0);
            send(fd, params.c_str(), params.size(), 0);
            send(fd, " :Bad channel name\r\n", 20, 0);
            return 1;
        case 482:
            send(fd, ":IRC 482 ", 9, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " ", 1, 0);
            send(fd, params.c_str(), params.size(), 0);
            send(fd, " :You're not channel operator\r\n", 31, 0);
            return 1;
        // ... custom errors ... //
        case 6969:
            send(fd, ":IRC ", 5, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " Successfully created channel #", 31, 0);
            send(fd, params.c_str(), params.size(), 0);
            send(fd, "\r\n", 2, 0);
            return 1;
        case 42069:
            send(fd, ":IRC ", 5, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, "!", 1, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, "@host JOINED #", 14, 0);
            send(fd, params.c_str(), params.size(), 0);
            send(fd, "\r\n", 2, 0);
            return 1;
        case 6767:
            send(fd, ":IRC ", 5, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, "!", 1, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, "@host Has been kicked from #", 28, 0);
            send(fd, params.c_str(), params.size(), 0);
            send(fd, "\r\n", 2, 0);
            return 1;
        case 6769:
            send(fd, ":IRC ", 5, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, "!", 1, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, "@host Has left #", 16, 0);
            send(fd, params.c_str(), params.size(), 0);
            send(fd, "\r\n", 2, 0);
            return 1;
        case 1337:
            send(fd, ":IRC ", 5, 0);
            send(fd, "PASS must be sent first\r\n", 25, 0);
            send(fd, "\r\n", 2, 0);
            return 1;



        /*
            Not implemented channel errors:
                467 - Channel key already set
                477 - Channel is banned
        */
        // ... User errors ... //
            // case 
        // ... Server errors ... //
        case 451:
            send(fd, ":IRC 451 ", 9, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " ", 1, 0);
            send(fd, params.c_str(), params.size(), 0);
            send(fd, " :You have not registered\r\n", 27, 0);
            return 1;
        
        default:
            send(fd, ":IRC 500 ", 9, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " :Unknown error\r\n", 16, 0);
            return 1;
    }
    send(fd, "UNKNOWN ERROR CODE(GO CHECK CODE)\r\n", 34, 0);
    return (-1);
}

int check_hashtag(std::string name, Client *client)
{
    if (name[0] != '#')
    {
        send_error(client->get_fd(), 479, client, name);
        return 1;
    }
    return 0;
}

void send_invite_message(int fd, Client *client, std::string cname)
{
    send(fd, ":IRC ", 5, 0);
    send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
    send(fd, "!", 1, 0);
    send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
    send(fd, "@host INVITED you to #", 22, 0);
    send(fd, cname.c_str(), cname.size(), 0);
    send(fd, "\r\n", 2, 0);
}

