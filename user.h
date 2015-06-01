#ifndef USERS_H
#define USERS_H
#include <iostream>
#include <player.h>
#include <QDebug>

class User
{
public:
    User(std::string log, std::string pass, std::string ip, unsigned short port);
    ~User();

public:
    int userStatus; //online/offline/afk
    int userID;

    QList<QString> nickList;

    std::string userLog;
    std::string userPass;
    std::string userIP;
    std::string userPort;
    std::string pathToPlayer;
    std::string nickname;
    Player *player;

public:
    int getStatus();
    void chooseCharacter(std::string characterNickname);
};

#endif // USERS_H
