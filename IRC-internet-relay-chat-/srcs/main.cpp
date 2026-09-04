#include "Server.hpp"

bool isValidPassword(std::string password){
    if(password.empty())
        return false;
    if(password.length() > 20)
        return false;

    for (size_t i = 0; i < password.length(); i++) {
        if (std::isspace(password[i]))
            return false;
    }
    return true;
}

int main(int ac, char **av){

    if(ac != 3){
        std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
        return 1;
    }

    try
    {
        int port = std::atoi(av[1]);
        if (port < 1024 || port > 65535)
            throw std::runtime_error("Invalid port: must be between 1024 and 65535");
        if (!isValidPassword(av[2]))
            throw std::runtime_error("Invalid password");

        signal(SIGINT, C_Server::SignalHandler);
        signal(SIGQUIT, C_Server::SignalHandler);
        signal(SIGTERM, C_Server::SignalHandler);

        C_Server ser(port, av[2]);
        ser.initializeNetwork();
        ser.runEventLoop();
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    std::cout << "Server Stopped." << std::endl;
    return 0;
}