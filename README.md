# ft_IRC

<img width="1020" alt="ft_IRC_image" src="https://github.com/user-attachments/assets/458ed8cc-b41f-4ef6-b9cb-9ce3fb18cc45">

## Overview

This project is about creating our own IRC server. We used an actual IRC client to connect to our server and test it. IRC (Internet Relay Chat) is a protocol for real-time text messaging between internet-connected computers created in 1988. It is mainly used for group discussion in chat rooms called “channels” although it supports private messages between two users, data transfer, and various client-side commands.

## Requirement
- We have to develop an IRC server in C++ 98.
- The server must be capable of handling multiple clients at the same time and never hang.
- Only 1 poll() (or equivalent) can be used for handling all these operations (read, write, but also listen, and so forth).
- An IRC client must be able to connect to your server without encountering any error.
- Communication between client and server has to be done via TCP/IP (v4).
- Using the IRC client with our server must be similar to using it with any official IRC server. However, we only have to implement the following features:
　　- We must be able to authenticate, set a nickname, a username, join a channel, send and receive private messages using the IRC client.
    - All the messages sent from one client to a channel have to be forwarded to every other client that joined the channel.
    - We must have operators and regular users.
    - Then, we have to implement the commands that are specific to operators.

## Command Usage

### PASS
`PASS <password>`
- Sets the password used to connect to the server. Used if the client needs to authenticate before connecting to the server.

### NICK
`NICK <nickname>`
- Sets or changes the client's nickname. The nickname is the user's identifier in IRC.

### USER
`USER <username> <hostname> <servername> <realname>`
- Sets the client's user information. Usually sent with the `NICK` command when connecting to a server.

### JOIN
`JOIN <channel> [<key>]`
- Joins the specified channel. Use the channel name in the format `#channel`.

### PRIVMSG
`PRIVMSG <recipient> <message>`
- Sends a message to the specified user or channel.

### TOPIC
`TOPIC <channel> [<topic>]`
- Displays or sets the topic of the channel. To set the topic, specify `<topic>`. You must be a channel administrator to change the topic.

### INVITE
`INVITE <nickname> <channel>`
- Invite the user with the specified nickname to join the channel.

### KICK
`KICK <channel> <user> [<comment>]`
- Kick the specified user from the specified channel. Optionally, provide a reason with `<comment>`.

### MODE
`MODE <channel|nickname> <flags>`
- Set the mode for the channel or user. Used to set administrative permissions and restrictions for the channel.

### PART
`PART <channel> [<message>]`
- Leave the specified channel. Optionally, specify a message when leaving.

### LIST
`LIST [<channel>]`
- List all channels on the server, or a specified channel.

## ⚙️Start IRC Server
To compile the program, use:
`make`

To start the Server, use:
`./ircserv <port> <password>`

- port: The port number on which your IRC server will be listening to for incoming IRC connections.
- password: The connection password. It will be needed by any IRC client that tries to connect to your server. 

To connect to the server, you can use:
`nc -c <IP ADDRESS> <PORT>`
- IP ADDRESS: Host IP address.
- PORT: The PORT that the server listening on.

To start irc cliet, you can use:
`irssi -c 127.0.0.1 -n <nickname> -w <password>`
You can also use an IRC Client e.g irssi, LimeChat...

## Reference
[Internet Relay Chat Protocol](https://datatracker.ietf.org/doc/html/rfc1459)

[Internet Relay Chat: Channel Management](https://datatracker.ietf.org/doc/html/rfc2811)

[Internet Relay Chat: Client Protocol](https://datatracker.ietf.org/doc/html/rfc2812#section-3.2)


[irssi](https://irssi.org/search/?q=user_name&check_keywords=yes&area=default#)

[https://qiita.com/gu-chi/items/4ba1da7628603c2099e1#322-part-message](https://medium.com/@afatir.ahmedfatir/small-irc-server-ft-irc-42-network-7cee848de6f9)


