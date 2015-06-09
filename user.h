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
    bool userStatus; //online/offline/afk
    unsigned short userID;

    struct Character {
        QString Nickname;
        unsigned short ClassId;
    };

    QList<Character*> characters;
    QList<QString> nickList;

    std::string userLog;
    std::string userPass;
    std::string userIP;
    unsigned short userPort;

    std::string nickname;
    Player *player;

public:
    int getStatus();
    void deletePlayer();
    void newCharacter(std::string characterNickname, int characterClass);
    void deleteCharacter(std::string nickname, unsigned short id);
    void chooseCharacter(std::string characterNickname);
    void setStatus(bool _status);
};

#endif // USERS_H
