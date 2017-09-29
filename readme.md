# Chatbot

[![Build Status](https://travis-ci.org/vinitkumar/chatbot.svg?branch=master)](https://travis-ci.org/vinitkumar/chatbot)

Chatbot is a simple bot written in C. The base algorithm is that our conversation is based on keywords. The bot seaches for this keyword and present a suitable response on basis of that keyword.

## Installation

Installation is very easy via `MakeFile` :

Just run:

```
chatbot ➤ make
gcc src/chatbot.c -o chat
```

## Usage

After running `make all` you get a executable named as `chat`. Now run `./chat` and start chatting with the bot.

## Demo

```
chatbot ➤ ./chat
$ Chatbot v1.0.0!

$ (user) hi

$ (chatbot) hello

$ (user) python

$ (chatbot) Yo, I love Python

$ (user) What

$ (chatbot) It is clear, ain't it?

$ (user) exit
```

## Issues and Pull request.

Feel free to create an issue if you notice a bug. Pull request are really
welcome with good fixes.
