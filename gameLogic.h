#ifndef GAMELOGIC_H
#define GAMELOGIC_H
#include <SFML/Network.hpp>
#include <accounts.h>

class GameLogic
{
public:
    GameLogic(Accounts *_accounts);
    ~GameLogic();

public:
    void sendNewPlayer(int command, std::string ip, unsigned short port);
    void playerAttack(int command, std::string ip, unsigned short port, std::string nickname);
    void playerMove(int command, std::string ip, unsigned short port, sf::Vector2f position);
    void getEnemys(int command, std::string ip, unsigned short port);
    void detach(int command, std::string ip, unsigned short port);

private:
    unsigned short const mark = 1337;

    Accounts *accounts;

    sf::UdpSocket socket;
    sf::Packet outPacket;
};

#endif // GAMELOGIC_H
