#ifndef USERS_H
#define USERS_H
#include <iostream>
#include <player.h>
#include <QDebug>
#include <QtSql>

class User
{
public:
    User();
    User(QSqlDatabase *_charactersDB, int ID, std::string ip, unsigned short port);
    ~User();

public:
    bool userStatus; //online/offline/afk
    int userID;

    struct Character {
        QString Nickname;
        unsigned short ClassId;
    };

    QList<Character*> characters;    

    std::string userIP;
    unsigned short userPort;

    Player *player;
    QSqlDatabase *charactersDB;

public:
    int getStatus();
    void deletePlayer();
    bool newCharacter(std::string nickname, int classID);
    void deleteCharacter(std::string nickname, unsigned short id);
    void chooseCharacter(std::string characterNickname);
    QList<Character *> getCharacters();
    void setStatus(bool _status);

private:
    enum Characters {
        ID,
        Nickname,
        ClassID,
        HitPoints,
        ManaPoints,
        Attack,
        Defence,
        PositionX,
        PositionY
    };
};

#endif // USERS_H
