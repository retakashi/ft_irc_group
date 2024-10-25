# ft_IRC

<img width="1020" alt="ft_IRC_image" src="https://github.com/user-attachments/assets/458ed8cc-b41f-4ef6-b9cb-9ce3fb18cc45">

## Overview
A simple reimplementation of IRC.

## Implementation Environment
- **Programming Language**: C++98
- **Operating Systems**: macOS, Ubuntu 22.04.4(Jammy)
- **Compiler**:
  - **macOS**: Apple Clang 15.0.0 (clang-1500.3.9.4), Target: arm64-apple-darwin23.6.0
  - **Ubuntu**: GCC 11.4.0 (Ubuntu 11.4.0-1ubuntu1~22.04)

## Command Usage

### PASS
`PASS <password>`
- Sets the password used to connect to the server. Used if the client needs to authenticate before connecting to the server.

### NICK
`NICK <nickname>`
- Sets or changes the client's nickname. The nickname is the user's identifier in IRC.

### USER
`USER <username> <mode> <unused> <realname>`
- Sets the client's user information. Usually sent with the `NICK` command when connecting to a server.

### JOIN
`JOIN <channel> [<key>]`
- Joins the specified channel. Use the channel name in the format `#channel`.
- You can set a key when joining the channel.

### PRIVMSG
`PRIVMSG <recipient> <message>`
- Sends a message to the specified user or channel. To specify multiple recipients, separate them with a period ('.').
### TOPIC
`TOPIC <channel> [<topic>]`
- Displays or sets the topic of the channel. To set the topic, specify `<topic>`.

### INVITE
`INVITE <nickname> <channel>`
- Invite the user with the specified nickname to join the channel.

### KICK
`KICK <channel> <user> [<comment>]`
- Kick the specified user from the specified channel. Optionally, provide a reason with `<comment>`.

### MODE
`MODE <channel> <flags> [<flag's param>]`
- Set the mode for the channel or user. Used to set administrative permissions and restrictions for the channel.
 - o (Operator): Grants or removes operator status for a user, allowing them to manage the channel. This requires specifying a user as a parameter.

 - t (Topic Protection): Allows only channel operators to change the channel topic. No additional parameters are required.

 - k (Key): Sets a password for the channel, restricting access to users who know the key. The key (password) is required as a parameter.

 - l (Limit): Sets a limit on the number of users who can join the channel. The maximum number of users is required as a parameter.

 - i (Invite Only): Restricts the channel to an invite-only mode, allowing only invited users to join. No additional parameters are required.

These flags are used to control access and moderation within an IRC channel.

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

## ⚙️Connect IRC Client 
To start chatting using the nc command, you can do the following:
macOS:
`nc -c <ip address> <port>`
Ubuntu:
`nc -C <ip address> <port>`
- IP ADDRESS: Host IP address.
- PORT: The PORT that the server listening on.

To connect to the Irssi client, you can do the following:
`irssi`
`/connect ft_irc <port> -n <nickname> -w <password>`


## Reference
[Internet Relay Chat Protocol](https://datatracker.ietf.org/doc/html/rfc1459)

[Internet Relay Chat: Channel Management](https://datatracker.ietf.org/doc/html/rfc2811)

[Internet Relay Chat: Client Protocol](https://datatracker.ietf.org/doc/html/rfc2812#section-3.2)


[irssi](https://irssi.org/search/?q=user_name&check_keywords=yes&area=default#)

[https://qiita.com/gu-chi/items/4ba1da7628603c2099e1#322-part-message](https://medium.com/@afatir.ahmedfatir/small-irc-server-ft-irc-42-network-7cee848de6f9)


