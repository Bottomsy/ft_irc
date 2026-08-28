#include "server.hpp"

int send_error(int fd, int code, Client *client, std::string params)
{
// error format: :IRC <code> <nick> <params> :<error message>\r\n
    switch (code)
    {
        case 400:
            send(fd, ":IRC 400 ", 8, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " ", 1, 0);
            send(fd, params.c_str(), params.size(), 0);
            send(fd, " :Invalid command\r\n", 17, 0);
            break;
        case 401:
            send(fd, ":IRC 401 ", 8, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " ", 1, 0);
            send(fd, params.c_str(), params.size(), 0);
            send(fd, " :No such nick/channel\r\n", 23, 0);
            break;
        case 403:
            send(fd, ":IRC 403 ", 8, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " ", 1, 0);
            send(fd, params.c_str(), params.size(), 0);
            send(fd, " :Channel not found\r\n", 20, 0);
            break;
        case 404:
            send(fd, ":IRC 404 ", 8, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " ", 1, 0);
            send(fd, params.c_str(), params.size(), 0);
            send(fd, " :Cannot send to channel\r\n", 26, 0);
            break;
        case 405:
            send(fd, ":IRC 405 ", 8, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " ", 1, 0);
            send(fd, params.c_str(), params.size(), 0);
            send(fd, " :Too many channels\r\n", 21, 0);
            break;
        case 442:
            send(fd, ":IRC 442 ", 8, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " ", 1, 0);
            send(fd, params.c_str(), params.size(), 0);
            send(fd, " :User is not on that channel\r\n", 31, 0);
            break;
        case 443:
            send(fd, ":IRC 443 ", 8, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " ", 1, 0);
            send(fd, params.c_str(), params.size(), 0);
            send(fd, " :User already on channel\r\n", 27, 0);
            break;
        case 471:
            send(fd, ":IRC 471 ", 8, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " ", 1, 0);
            send(fd, params.c_str(), params.size(), 0);
            send(fd, " :Channel is full\r\n", 20, 0);
            break;
        case 473:
            send(fd, ":IRC 473 ", 8, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " ", 1, 0);
            send(fd, params.c_str(), params.size(), 0);
            send(fd, " :Channel is invite only\r\n", 25, 0);
            break;
        case 475:
            send(fd, ":IRC 475 ", 8, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " ", 1, 0);
            send(fd, params.c_str(), params.size(), 0);
            send(fd, " :Incorrect password\r\n", 23, 0);
            break;
        case 482:
            send(fd, ":IRC 482 ", 8, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " ", 1, 0);
            send(fd, params.c_str(), params.size(), 0);
            send(fd, " :You're not channel operator\r\n", 32, 0);
            break;
        default:
            send(fd, ":IRC 500 ", 8, 0);
            send(fd, client->get_nick().c_str(), client->get_nick().size(), 0);
            send(fd, " :Unknown error\r\n", 16, 0);
            break;
    }
    return (-1);
}