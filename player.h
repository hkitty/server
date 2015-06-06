#ifndef PLAYER_H
#define PLAYER_H
#include <QFile>
#include <QDebug>
#include <QDir>
#include <SFML/Graphics.hpp>

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
    sf::Vector2f position;
};

#endif // PLAYER_H
