# ft_IRC

<img width="1020" alt="ft_IRC_image" src="https://github.com/user-attachments/assets/458ed8cc-b41f-4ef6-b9cb-9ce3fb18cc45">

# Overview

Internet Relay Chat or IRC is a text-based communication protocol on the Internet.
It offers real-time messaging that can be either public or private. Users can exchange direct messages and join group channels.
IRC clients connect to IRC servers in order to join channels. IRC servers are connecte together to form a network.

## Requirement
You have to develop an IRC server in C++ 98.
Your executable will be run as follows:
`./ircserv <port> <password>`
• port: The port number on which your IRC server will be listening to for incoming IRC connections.
• password: The connection password. It will be needed by any IRC client that tries to connect to your server

The server must be capable of handling multiple clients at the same time and never
hang.
• Forking is not allowed. All I/O operations must be non-blocking.
• Only 1 poll() (or equivalent) can be used for handling all these operations (read, write, but also listen, and so forth).

• Several IRC clients exist. You have to choose one of them as a reference. Your reference client will be used during the evaluation process.
• Your reference client must be able to connect to your server without encountering any error.
• Communication between client and server has to be done via TCP/IP (v4 or v6).
• Using your reference client with your server must be similar to using it with any official IRC server. However, you only have to implement the following features:
◦ You must be able to authenticate, set a nickname, a username, join a channel, send and receive private messages using your reference client.
◦ All the messages sent from one client to a channel have to be forwarded to every other client that joined the channel.
◦ You must have operators and regular users.
◦ Then, you have to implement the commands that are specific to channel
operators:
∗ KICK - Eject a client from the channel
∗ INVITE - Invite a client to a channel
∗ TOPIC - Change or view the channel topic
∗ MODE - Change the channel’s mode:
· i: Set/remove Invite-only channel
· t: Set/remove the restrictions of the TOPIC command to channel operators
· k: Set/remove the channel key (password)
· o: Give/take channel operator privilege

## Usage

## Features

## Reference

## Author

[twitter](https://twitter.com/Kotabrog)

## Licence

[MIT](https://......)
