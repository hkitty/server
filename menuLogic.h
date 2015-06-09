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
    void checkLogin(int command, std::string ip, unsigned short port, std::string login, std::string password);
    void getUserCharacters(int command, std::string ip, unsigned short port);
    void registration(int command, std::string ip, unsigned short port, std::string log, std::string pass);

    void chooseUserCharacter(int command, std::string ip, unsigned short port, std::string characterNickname);
    void deleteCharacter(int command, std::string ip, unsigned short port, std::string characterNickname, unsigned short id);
    void newCharacter(int command, std::string ip, unsigned short port, std::string characterNickname, int characterClassID);

private:
    void loginAccept(int command, std::string ip, unsigned short port);
    void loginReject(int command, std::string ip, unsigned short port);
    bool deleteFromUserFiles(std::string ip, unsigned short port, std::string nickname, unsigned short id);
    void deleteFromCharacters(std::string userLogin, std::string characterNickname);

private:
    unsigned short const mark = 1337;
    Accounts *accounts;
    sf::UdpSocket socket;
    sf::Packet outPacket;
};

#endif // MENULOGIC_H
