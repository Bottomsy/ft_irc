#include "server.hpp"

int send_error(int fd, int code, Client *client, std::string params)
{
// error format: :IRC <code> <nick> <params> :<error message>\r\n
    switch (code)
    {
        case 400:
            send(fd, ":IRC 400 ", 9, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " ", 1, 0);
            send(fd, params.c_str(), params.size(), 0);
            send(fd, " :Invalid command\r\n", 17, 0);
            return 1;
        case 401:
            send(fd, ":IRC 401 ", 9, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " ", 1, 0);
            send(fd, params.c_str(), params.size(), 0);
            send(fd, " :No such nick/channel\r\n", 23, 0);
            return 1;
        // ... Channel errors ... //
        case 403:
            send(fd, ":IRC 403 ", 9, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " ", 1, 0);
            send(fd, params.c_str(), params.size(), 0);
            send(fd, " :Channel not found\r\n", 20, 0);
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
            send(fd, " :User not in channel\r\n", 24, 0);
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
            send(fd, " :Channel key already set\r\n", 28, 0);
            return 1;
        case 471:
            send(fd, ":IRC 471 ", 9, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " ", 1, 0);
            send(fd, params.c_str(), params.size(), 0);
            send(fd, " :Channel is full\r\n", 20, 0);
            return 1;
        case 472:
            send(fd, ":IRC 472 ", 9, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " ", 1, 0);
            send(fd, params.c_str(), params.size(), 0);
            send(fd, " :Unknown channel mode\r\n", 17, 0);
            return 1;
        case 473:
            send(fd, ":IRC 473 ", 9, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " ", 1, 0);
            send(fd, params.c_str(), params.size(), 0);
            send(fd, " :Channel is invite only\r\n", 25, 0);
            return 1;
        case 475:
            send(fd, ":IRC 475 ", 9, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " ", 1, 0);
            send(fd, params.c_str(), params.size(), 0);
            send(fd, " :Incorrect password\r\n", 23, 0);
            return 1;
        case 477:
            send(fd, ":IRC 477 ", 9, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " ", 1, 0);
            send(fd, params.c_str(), params.size(), 0);
            send(fd, " :Channel mode is invalid\r\n", 22, 0);
            return 1;
        case 479:
            send(fd, ":IRC 479 ", 9, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " ", 1, 0);
            send(fd, params.c_str(), params.size(), 0);
            send(fd, " :Bad channel name\r\n", 25, 0);
            return 1;
        case 482:
            send(fd, ":IRC 482 ", 9, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " ", 1, 0);
            send(fd, params.c_str(), params.size(), 0);
            send(fd, " :You're not channel operator\r\n", 32, 0);
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

