#ifndef SERVER_H
#define SERVER_H

#include "SFML/Network.hpp"
#include <QDebug>
#include <unistd.h>
#include <parser.h>
#include <queue.h>
#include <banlist.h>
#include <accounts.h>

class MessageController
{
public:
    MessageController();
    ~MessageController();

private:
    unsigned short port;
    bool isStarted = false;

public:
    void startReceiver(int _port);
    void stop();
    void startParser();

private:
    void receiver();

private:
    sf::UdpSocket socket;
    sf::IpAddress sender;
    sf::Packet packet;

    sf::Thread TStartReceiver;
    sf::Thread TStartParser;

public:
    Parser *parser = new Parser;
    Queue *queue = new Queue;
    BanList *banlist = new BanList;
};

#endif // SERVER_H
