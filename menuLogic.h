#ifndef MENULOGIC_H
#define MENULOGIC_H
#include <SFML/Network.hpp>
#include <accounts.h>

class MenuLogic
{
public:
    MenuLogic(Accounts *_accounts);
    ~MenuLogic();

public:
    void getUserCharacters(int command, std::string ip, unsigned short port);
    void registration(int command, std::string ip, unsigned short port, std::string log, std::string password);
    void checkLogin(int command, std::string ip, unsigned short port, std::string login, std::string password);

    void chooseUserCharacter(int command, std::string ip, unsigned short port, std::string nickname);
    void deleteCharacter(int command, std::string ip, unsigned short port, std::string nickname);
    void newCharacter(int command, std::string ip, unsigned short port, std::string nickname, int classID);

private:
    void loginAccept(int command, std::string ip, unsigned short port);
    void loginReject(int command, std::string ip, unsigned short port);

private:
    unsigned short const mark = 1337;

    Accounts *accounts;

    sf::UdpSocket socket;
    sf::Packet outPacket;
};

#endif // MENULOGIC_H
