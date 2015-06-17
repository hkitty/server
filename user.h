#ifndef USERS_H
#define USERS_H
#include <iostream>
#include <player.h>
#include <QDebug>
#include <QtSql>

class User
{
public:
    User(QSqlDatabase *_charactersDB, int ID, std::string ip, unsigned short port);
    ~User();

public:
    int userID;

    bool userStatus;

    struct Character {
        QString Nickname;
        unsigned short ClassId;
    };

    std::string userIP;
    unsigned short userPort;

    Player *player;
    QSqlDatabase *charactersDB;
    QList<Character*> characters;


public:
    int getStatus();

    void deletePlayer();
    void reloadCharacters();
    void setStatus(bool _status);
    void chooseCharacter(std::string nickname);

    bool removeCharacter(std::string nickname);
    bool newCharacter(int ID, std::string nickname, int classID);

    QList<User::Character *> getCharacters();

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
