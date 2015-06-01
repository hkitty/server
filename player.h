#ifndef PLAYER_H
#define PLAYER_H
#include <QFile>
#include <QDebug>

class Player
{
public:
    Player(std::string characterNickname, std::string login);
    ~Player();

public:
    QString nick;

    int hitPoints;
    int manaPoints;

    int attack;
    int defence;
};

#endif // PLAYER_H
