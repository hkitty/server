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
    void getEnemys(int command, std::string ip, unsigned short port);
    void sendNewPlayer(int command, std::string ip, unsigned short port);
    void playerMove(int command, std::string ip, unsigned short port, sf::Vector2f vect);
    void playerAttack(int command, std::string, unsigned short port);
    void deattach(std::string ip, unsigned short port);

private:
    unsigned short const mark = 1337;
    Accounts *accounts;
    sf::UdpSocket socket;
    sf::Packet outPacket;
};

#endif // GAMELOGIC_H
