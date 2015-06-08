#include "player.h"

Player::Player(std::string characterNickname, std::string login) :
    TStartAttack(&Player::attack, this)
{
    qDebug() << "Load player: " << QString::fromStdString(characterNickname);
    qDebug() << "Character nickname size: " << characterNickname.size();
    nickname = QString::fromStdString(characterNickname);
    qDebug() << "Character nickname: " << QString::fromStdString(characterNickname);
    qDebug() << "Current path: " << QDir::currentPath();
    qDebug() << "[Player::player] User login: " << QString::fromStdString(login);
    QString path = QDir::currentPath() + "/Characters/" + QString::fromStdString(login) + "_" + QString::fromStdString(characterNickname) + ".txt";
    QFile character(path);
    QString line;
    unsigned short uLine;
    QList<unsigned short> statsList;

    if ( character.exists() ) {
        if ( !character.open(QIODevice::ReadOnly) ) {
            qDebug() << "File not open [Player]";
        }

        while ( !character.atEnd() ) {
            line = character.readLine();
            line = line.remove(line.size() - 1, 1);

            uLine = line.toUShort();
            qDebug() << "ULine: " << uLine;
            statsList.append(uLine);
        }
        characterClass = statsList.at(0);
        stats.hitPoints = statsList.at(1);
        stats.manaPoints = statsList.at(2);
        stats.attack = statsList.at(3);
        stats.defence = statsList.at(4);
        position.x = statsList.at(5);
        position.y = statsList.at(6);

    } else {
        qDebug() << "[Player::Player] Character file not found";
    }
    character.close();
}

Player::~Player()
{

}

void Player::attack()
{
    inFight = true;
    timer->start(10000);
    inFight = false;
}

