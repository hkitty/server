#ifndef PARSER_H
#define PARSER_H
#include <SFML/Network.hpp>
#include <QDebug>
#include <task.h>
#include <unistd.h>
#include <accounts.h>

class Parser
{
public:
    Parser();
    ~Parser();

public:
    int i = 0;
    unsigned short const mark = 1337;

    Accounts *accounts = new Accounts;

public:
    void receive(Task task);
    void kickUser(unsigned short userID);

private:
    enum Commands {
        Login,
        GetUserCharacters,
        Registration,
        NewCharacter,
        ChooseCharacter
    };

    sf::Packet outPacket;
    sf::UdpSocket socket;

private:
    void loginAccept(int command, std::string ip, unsigned short port);
    void loginReject(int command, std::string ip, unsigned short port);
    void registration(int command, std::string ip, unsigned short port, std::string log, std::string pass);
    void getUserCharacters(int command, std::string ip, unsigned short port);
    void chooseUserCharacter(int command, std::string ip, unsigned short port, std::string characterNickname);
};

#endif // PARSER_H
