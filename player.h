#ifndef PLAYER_H
#define PLAYER_H
#include <QFile>
#include <QDebug>
#include <QDir>
#include <QTimer>
#include <SFML/Graphics.hpp>
#include <QtSql>

class Player
{
public:
    Player(QSqlDatabase *_charactersDB, std::string characterNickname);
    ~Player();

public:
    QString nickname;
    bool inFight;
    unsigned short characterClass;

    struct Stats {
        unsigned short hitPoints;
        unsigned short manaPoints;
        unsigned short attack;
        unsigned short defence;
    } stats;

    sf::Vector2f position;

public:
    QTimer *timer = new QTimer;
    sf::Thread TStartAttack;
public:
    void attack();

private:
    QSqlDatabase *charactersDB;
    enum Character {
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

#endif // PLAYER_H
