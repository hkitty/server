#ifndef USERS_H
#define USERS_H
#include <iostream>
#include <player.h>
#include <QDebug>

class User
{
public:
    User();
    User(std::string log, std::string pass, std::string ip, unsigned short port);
    ~User();

public:
    int userStatus; //online/offline/afk
    int userID;

    struct Character {
        QString Nickname;
        unsigned short ClassId;
    };

    QList<Character> characters;
    QList<QString> nickList; //For check double nick

    std::string userLog;
    std::string userPass;
    std::string userIP;
    unsigned short userPort;
    std::string pathToPlayer;
    std::string nickname;
    Player *player;

public:
    int getStatus();
    void newCharacter(std::string characterNickname, unsigned short characterClass);
    void chooseCharacter(std::string characterNickname);
};

#endif // USERS_H
