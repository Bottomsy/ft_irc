# ft_irc 💬
<p align="center">
  <em>An IRC server implementation in C++98. A 1337/42 Network project to recreate your own Internet Relay Chat server.</em><br>
  Made by <a href="https://github.com/Alikhairi42">Abdelali Khairi</a> and <a href="https://github.com/SalahEddineElhimeur">Salah Eddine Elhimeur</a> and <a href="https://github.com/kirkl9tall">Abdelilah Ismaili Alaoui</a>.
</p>

---

## 📖 Table of Contents
- [About](#-about)
- [Features](#-features)
- [Installation](#-installation)
- [Usage](#-usage)
- [IRC Commands](#-irc-commands)
- [Architecture](#-architecture)
- [Testing](#-testing)
- [Resources](#-resources)
- [Authors](#-authors)

---

## 🎯 About

**ft_irc** is a project from the 42 Network curriculum that challenges you to implement your own IRC (Internet Relay Chat) server in C++98. The server must handle multiple clients simultaneously, support channel operations, and be compliant with the IRC protocol (RFC 1459).

This project teaches:
- **Network Programming**: Socket programming and TCP/IP communication
- **I/O Multiplexing**: Using `poll()`, `select()`, or `epoll()`
- **Protocol Implementation**: Understanding and implementing IRC protocol
- **Concurrent Programming**: Handling multiple clients without threading
- **C++98 Standards**: Writing robust and clean C++ code

---

## ✨ Features

### Core Functionality
- ✅ **Multi-client support** using I/O multiplexing (`poll()`)
- ✅ **Non-blocking I/O** operations
- ✅ **Password authentication** for server access
- ✅ **Nickname and username** management
- ✅ **Private messaging** between users
- ✅ **Channel system** with full IRC channel support

### Channel Operations
- 🔹 **Join/Part channels**
- 🔹 **Channel operators** with special privileges
- 🔹 **Channel modes**:
  - `i` - Invite-only channel
  - `t` - Topic restrictions (only operators can change)
  - `k` - Channel password/key
  - `o` - Operator privileges
  - `l` - User limit
- 🔹 **Kick/Invite** users
- 🔹 **Topic management**

### IRC Commands Implemented
```
PASS, NICK, USER, JOIN, PART, PRIVMSG, NOTICE
KICK, INVITE, TOPIC, MODE, QUIT, PING, PONG
```

---

## 🛠️ Installation

### Prerequisites
```bash
# C++98 compiler (g++ or clang++)
# GNU Make
# Git
```

### Clone and Build
```bash
# Clone the repository
git clone git@github.com:Alikhairi42/ft_irc.git
cd ft_irc

# Compile the project
make

# Clean object files
make clean

# Full clean (remove executable)
make fclean

# Rebuild everything
make re
```

---

## 🚀 Usage

### Starting the Server
```bash
./ircserv <port> <password>
```

**Example:**
```bash
./ircserv 6667 SecurePass123
```

**Parameters:**
- `<port>`: Port number for the IRC server (1024-65535 recommended)
- `<password>`: Server password for client authentication

### Connecting with IRC Clients

#### Using **irssi** (Recommended)
```bash
irssi
/connect -nocap localhost 6667 SecurePass123
/nick YourNickname
/join #general
```

#### Using **WeeChat**
```bash
weechat
/server add myirc localhost/6667 -password=SecurePass123
/connect myirc
/nick YourNickname
/join #channel
```

#### Using **HexChat/XChat**
1. Add new network: `localhost/6667`
2. Set server password: `SecurePass123`
3. Connect and join channels

#### Using **netcat** (for testing)
```bash
nc localhost 6667
```
Then send commands manually:
```
PASS SecurePass123
NICK alice
USER alice 0 * :Alice Smith
JOIN #test
PRIVMSG #test :Hello everyone!
```

---

## 📝 IRC Commands

### Authentication (Required First)
```irc
PASS <password>              # Authenticate with server password
NICK <nickname>              # Set your nickname
USER <user> 0 * :<realname> # Set username and real name
```

### Channel Management
```irc
JOIN #channel [key]          # Join a channel (with optional password)
PART #channel [message]      # Leave a channel
TOPIC #channel [new_topic]   # View or set channel topic
MODE #channel [modes]        # View or change channel modes
```

### Messaging
```irc
PRIVMSG <target> :<message>  # Send message to user or channel
NOTICE <target> :<message>   # Send notice (like PRIVMSG but no auto-reply)
```

### Operator Commands
```irc
KICK #channel <user> [reason]     # Kick user from channel
INVITE <nickname> #channel         # Invite user to invite-only channel
MODE #channel +o <nickname>        # Give operator status
MODE #channel -o <nickname>        # Remove operator status
```

### Channel Modes
```irc
MODE #channel +i              # Set invite-only
MODE #channel +t              # Topic settable by ops only
MODE #channel +k password     # Set channel password
MODE #channel +l 50           # Set user limit to 50
MODE #channel +o alice        # Give operator to alice
MODE #channel -i              # Remove invite-only
```

### Utility
```irc
QUIT [message]                # Disconnect from server
PING <token>                  # Ping server (automatic)
```

---

## 🏗️ Architecture

### Project Structure
```
ft_irc
├── Makefile           
├── README.md            
│── includes/
│       ├── Channel.hpp
│       ├── Client.hpp
│       ├── Server.hpp
├── incs/
│   ├── IrcServ.hpp       
│   └── asayadBot.hpp  
├── srcs/
│   ├── main.cpp  
│   ├── server/
│   │   ├── Server.cpp         
│   │   ├── Client.cpp
│   │   ├── Channel.cpp       
│   │   ├── Server_tools.cpp     
│   │   └── Request_handling.cpp
│   ├── commands/
│   │   ├── Auth.cpp            
│   │   ├── Auth_toolbox.cpp   
│   │   ├── Invite.cpp
│   │   ├── Join.cpp
│   │   ├── Kick.cpp 
│   │   ├── Mode.cpp
│   │   ├── PrivMsg.cpp
│   │   └── Topic.cpp
└──

### Class Overview

#### Server Class
- Manages server socket and client connections
- Handles I/O multiplexing with `poll()`
- Processes incoming commands
- Maintains list of clients and channels

#### Client Class
- Represents a connected user
- Stores nickname, username, authentication status
- Manages client socket and buffers
- Tracks joined channels

#### Channel Class
- Manages channel members and operators
- Handles channel modes and topic
- Controls access (invite-only, password, limit)

#### Command Class
- Parses IRC protocol messages
- Executes appropriate command handlers
- Validates permissions and parameters

---

## 🧪 Testing

### Basic Workflow Test
```bash
# Terminal 1: Start server
./ircserv 6667 test123

# Terminal 2: Client 1 (Alice)
nc localhost 6667
PASS test123
NICK alice
USER alice 0 * :Alice Wonder
JOIN #general
PRIVMSG #general :Hi everyone!

# Terminal 3: Client 2 (Bob)
nc localhost 6667
PASS test123
NICK bob
USER bob 0 * :Bob Builder
JOIN #general
PRIVMSG #general :Hey Alice!
PRIVMSG alice :Private message to Alice
```

### Channel Operator Test
```bash
# Alice creates and becomes operator of #private
JOIN #private
MODE #private +i              # Make invite-only
MODE #private +k secret123    # Set password
MODE #private +o bob          # Give Bob operator
KICK #private charlie :No spam! # Kick charlie
INVITE dave #private          # Invite dave
```

### Stress Test
```bash
# Use a script to connect multiple clients
for i in {1..10}; do
  (echo -e "PASS test123\nNICK user$i\nUSER user$i 0 * :User $i\nJOIN #test\nPRIVMSG #test :Message from user$i\nQUIT" | nc localhost 6667) &
done
```

### Testing with Real IRC Client (Recommended)
```bash
# Install irssi
sudo apt-get install irssi  # Ubuntu/Debian
brew install irssi          # macOS

# Connect and test
irssi
/connect localhost 6667 test123
/nick testuser
/join #test
/msg #test Hello world!
/quit
```

---

## 📚 Resources

### IRC Protocol Documentation
- [RFC 1459 - Internet Relay Chat Protocol](https://tools.ietf.org/html/rfc1459)
- [RFC 2812 - Internet Relay Chat: Client Protocol](https://tools.ietf.org/html/rfc2812)
- [Modern IRC Documentation](https://modern.ircdocs.horse/)

### Network Programming
- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/)
- [TCP/IP Sockets in C](https://cs.baylor.edu/~donahoo/practical/CSockets/)

### C++ Resources
- [C++ Reference](https://en.cppreference.com/)
- [C++98 Standard](https://www.lirmm.fr/~ducour/Doc-objets/ISO+IEC+14882-1998.pdf)

### Testing Tools
- [irssi IRC Client](https://irssi.org/)
- [WeeChat](https://weechat.org/)
- [HexChat](https://hexchat.github.io/)

---

## 👥 Authors

<table>
  <tr>
    <td align="center">
      <a href="https://github.com/Alikhairi42">
        <img src="https://github.com/Alikhairi42.png" width="100px;" alt="Abdelali Khairi"/><br>
        <sub><b>Abdelali Khairi</b></sub>
      </a><br>
      <sub>Server Architecture & Commands</sub>
    </td>
    <td align="center">
      <a href="https://github.com/SalahEddineElhimeur">
        <img src="https://github.com/SalahEddineElhimeur.png" width="100px;" alt="Salah Eddine Elhimeur"/><br>
        <sub><b>Salah Eddine Elhimeur</b></sub>
      </a><br>
      <sub>Channel Management & Protocol</sub>
    </td>
      <td align="center">
      <a href="https://github.com/kirkl9tall">
        <img src="https://github.com/kirkl9tall.png" width="100px;" alt="Abdelilah Ismaili Alaoui"/><br>
        <sub><b>Abdelilah Ismaili Alaoui</b></sub>
      </a><br>
      <sub>Channel Management & Protocol</sub>
    </td>
  </tr>
</table>

---

## ⚠️ Common Issues & Solutions

### Port Already in Use
```bash
# Find process using the port
lsof -i :6667
# Kill the process
kill -9 <PID>
```

### Connection Refused
- Check if server is running
- Verify firewall settings
- Ensure correct port number

### Authentication Failed
- Password must be sent BEFORE NICK and USER commands
- Check password matches server password
- Ensure client sends `PASS` command first

### Messages Not Appearing
- Verify you've joined the channel (`JOIN #channel`)
- Check channel modes (might be moderated)
- Ensure proper message format: `PRIVMSG #channel :message`

---

## 📄 License

This project is part of the **42 Network / 1337 School** curriculum.  
Educational purposes only.

---

## 🎓 Learning Outcomes

By completing this project, you will master:
- ✅ Socket programming in C++
- ✅ Network protocol implementation
- ✅ I/O multiplexing techniques
- ✅ Event-driven programming
- ✅ Real-time multi-client server architecture
- ✅ RFC standard compliance
- ✅ Memory and resource management

---

<p align="center">
  <em>Made with 💻 and ☕ at 1337 School</em>
</p>
