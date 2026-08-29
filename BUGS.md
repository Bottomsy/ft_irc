# IRC Server Bug Review

I pulled the subject, cloned the repo, and built and ran it against live IRC traffic rather than only reading the code. The skeleton is real: there is a single `poll()`, no forking, and the required commands are present (`PASS`, `NICK`, `USER`, `JOIN`, `PRIVMSG`, `KICK`, `INVITE`, `TOPIC`, `MODE`).

That said, there are several issues that would likely fail an evaluation immediately. The most severe problems are the crashes and the build configuration issues.

## 🔴 Crashes reproduced

### 1. Bare `PRIVMSG` segfaults the server

This is a real crash, and it happens consistently.

```text
PASS pass
NICK bob
USER bob
PRIVMSG
```

The bug is in `command_hub()`: it does `server->send_message(info["NICK"], args[0], args)` in the `else` branch without checking whether `args` is empty. If there are zero arguments, `args[0]` reads past the end of the vector.

This was reproduced on two fresh server instances.

#### Fix

```cpp
if (cmd == "PRIVMSG")
{
    if (args.empty())
    {
        std::string msg = ":irc_server 411 " + info["NICK"] + " :No recipient given (PRIVMSG)\r\n";
        send(_fd, msg.c_str(), msg.size(), 0);
    }
    else if (args[0][0] == '#')
        server->send_group_msg(args[0].substr(1), args, this);
    else
        server->send_message(info["NICK"], args[0], args);
}
```

### 2. `Server::remove_client()` reads a client after erasing it

This is an intermittent crash caused by undefined behavior.

```cpp
void Server::remove_client(int t)
{
    close(clients[t - 1]->get_fd());
    poll_vec.erase(poll_vec.begin() + t);
    clients.erase(clients.begin() + (t - 1));      // erased first...
    name_list.erase(clients[t - 1]->get_nick());   // ...then read at the same index
}
```

After the erase, `clients[t - 1]` may refer to the next client, or may be out of bounds if it was the last element. This matches the classic signature of a nondeterministic crash.

The correct pattern is to save the nickname before erasing.

#### Fix

```cpp
void Server::remove_client(int t)
{
    std::string nick = clients[t - 1]->get_nick();
    close(clients[t - 1]->get_fd());
    poll_vec.erase(poll_vec.begin() + t);
    clients.erase(clients.begin() + (t - 1));
    name_list.erase(nick);
}
```

---

## 🔴 The project does not build under the actual constraints

### Executable name mismatch

The assignment expects the binary to be run as:

```bash
./ircserv <port> <password>
```

Your Makefile currently builds a binary named `server` instead. That is a simple but very real compliance failure.

### Required flags are never passed

Your Makefile uses:

```make
$(CC) $(Srcs) -o $(Name)
```

but the flags are defined as `$(Flags)` and never actually included. The intended flags are:

```make
-std=c++98 -Wall -Wextra -Werror
```

When I enabled them manually, the project failed to compile for several reasons:

- extended initializer lists are C++11-only
- `std::stoi` does not exist in C++98
- missing `#include <cstdlib>` for `exit` / `atoi`
- numerous signed/unsigned comparison errors across `channel.cpp`, `server.cpp`, and `client.cpp`

### Missing source file in `Srcs`

`helper.cpp` is not included in the build, so a plain `make` fails to link with `send_error` undefined.

### Working Makefile example

```make
CC = c++
Flags = -std=c++98 -Wall -Wextra -Werror
Srcs = main.cpp client.cpp channel.cpp server.cpp helper.cpp
Name = ircserv

all: $(Name)

$(Name): $(Srcs) $(Headers)
	$(CC) $(Flags) $(Srcs) -o $(Name)

clean:
	rm -f $(Name)

fclean: clean

re: fclean all
```

### Header guard typo

`channel.hpp` has a broken include guard:

```cpp
#ifndef CAHNNEL_HPP
#define CHANNEL_HPP
```

The `#ifndef` name does not match the `#define`, so the guard does not actually protect the file. It works only by accident depending on include order.

---

## 🟠 Compliance and robustness gaps

### Non-blocking sockets are missing

There is no usage of `fcntl(fd, F_SETFL, O_NONBLOCK)` anywhere. The server is relying on `poll()` readiness alone, but `send()` calls are not gated by `POLLOUT` and can block. In a single-threaded server, a blocked client can freeze everything.

### Messages are not in proper IRC format

The server sends ad-hoc human-readable responses like:

```text
channel created and joined successfully\r\n
```

instead of actual IRC protocol lines such as:

```text
:nick!user@host JOIN :#chan\r\n
```

This is one of the biggest gaps between “logic works” and “it behaves like IRC” in a real client.

### QUIT / disconnect does not clean up channel membership

`remove_client()` and `read_from_socket()` clean up `poll_vec`, `clients`, and `name_list`, but they never remove the user from the channel membership structures (`Channel::clients`, `operators`, `users`). Later, the OS may reuse the same file descriptor, and a new client can receive messages meant for a channel they never joined.

### `+k` password mode is not checked on `JOIN`

`create_channel` checks invitation-only and user limits, but it does not validate `getPassword()` against the join attempt.

### Channel `PRIVMSG` echoes back to the sender

There is no self-skip check in `Channel::send_msg`, so the sender receives their own message again.

---

## 🟡 Smaller issues

- `run_serv()` can skip a client event when a disconnect shifts `poll_vec` mid-iteration.
- `registration()` caps `PASS/NICK/USER` values at 10 characters, which is too restrictive for real-world input.
- Any command other than `PASS`, `NICK`, `USER`, or `CAP` during registration is treated as fatal and disconnects the client.
- Only one pending invite is stored per client; a second invite overwrites the first.
- `Client` and `Channel` objects are allocated with `new` but never deleted on disconnect.
- There is no `PART` implementation; only a dead commented stub exists.
- The fake identity `!user@host` is hardcoded instead of using the actual `USER` and host information.
- A compiled server binary is committed to git; a `.gitignore` would be appropriate.

---

## Recommendation

The quickest path is:

1. fix the two crash bugs
2. correct the Makefile and build constraints
3. verify the binary name and compile flags
4. then tackle the IRC protocol formatting issues

The protocol-formatting work is the bigger task, but it is also the one most likely to matter in a live defense against a real IRC client.

If you want, I can take the next step and turn this into a clean, actionable fix checklist for the codebase.
