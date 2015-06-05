#ifndef PLAYER_H
#define PLAYER_H
#include <QFile>
#include <QDebug>
#include <QDir>

class Player
{
public:
    Player(std::string characterNickname, std::string login);
    ~Player();

public:
    QString nickname;
    unsigned short characterClass;

    struct Stats {
        unsigned short hitPoints;
        unsigned short manaPoints;
        unsigned short attack;
        unsigned short defence;
    } stats;
};

#endif // PLAYER_H
