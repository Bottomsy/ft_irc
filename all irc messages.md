# IRC numerics — ft_irc minimal set

This document lists the minimal IRC numeric replies and errors relevant to the 42 `ft_irc` project. It presents a compact, well-formatted reference students can use while implementing the server.

## Recommended minimum set

### Registration

```
001 RPL_WELCOME
002 RPL_YOURHOST
003 RPL_CREATED
004 RPL_MYINFO
```

### Errors / Registration checks

```
431 ERR_NONICKNAMEGIVEN
432 ERR_ERRONEUSNICKNAME
433 ERR_NICKNAMEINUSE
451 ERR_NOTREGISTERED
461 ERR_NEEDMOREPARAMS
462 ERR_ALREADYREGISTERED
464 ERR_PASSWDMISMATCH
```

### General commands

```
400 ERR_UNKNOWNERROR
421 ERR_UNKNOWNCOMMAND
```

### Users

```
401 ERR_NOSUCHNICK
406 ERR_WASNOSUCHNICK
441 ERR_USERNOTINCHANNEL
443 ERR_USERONCHANNEL
```

### Channels

```
403 ERR_NOSUCHCHANNEL
404 ERR_CANNOTSENDTOCHAN
405 ERR_TOOMANYCHANNELS ( no need)
442 ERR_NOTONCHANNEL
```

### Channel restrictions / modes

```
471 ERR_CHANNELISFULL
473 ERR_INVITEONLYCHAN
475 ERR_BADCHANNELKEY
477 ERR_NOCHANMODES
482 ERR_CHANOPRIVSNEEDED
```

### Operators

```
381 RPL_YOUREOPER
481 ERR_NOPRIVILEGES
491 ERR_NOOPERHOST (idk)
```

## Examples

Successful registration (example):

```text
:irc.example.com 001 alice :Welcome to the Internet Relay Network alice
:irc.example.com 002 alice :Your host is irc.example.com, running version 1.0
:irc.example.com 003 alice :This server was created Sat Aug 29 2026
:irc.example.com 004 alice irc.example.com 1.0 oi o
:irc.example.com 005 alice CHANTYPES=# PREFIX=(ov)@+ :are supported by this server
```

Unknown command example:

```text
:irc.example.com 421 alice FOOBAR :Unknown command
```

Missing parameters example:

```text
:irc.example.com 461 alice JOIN :Not enough parameters
```

Invalid channel example:

```text
:irc.example.com 403 alice channel :No such channel
```

---

If you'd like a narrower or different subset (for example only registration + channel errors), tell me which categories to keep and I'll trim further.
