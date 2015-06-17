#include "player.h"

Player::Player(QSqlDatabase *_charactersDB, std::string _nickname) :
    TStartAttack(&Player::attack, this)
{

    charactersDB = _charactersDB;

//    if ( !charactersDB->open() ) {
//        qDebug() << "CharactersDB open error: " << charactersDB->lastError().text();
//    } else {

    QSqlQuery QLoadCharacter(*charactersDB);

    QLoadCharacter.prepare("SELECT * FROM CharacterList WHERE nickname=:nickname");
    QLoadCharacter.bindValue(":nickname", QString::fromStdString(_nickname));

        if ( !QLoadCharacter.exec() ) {
            qDebug() << "Load character error: " << QLoadCharacter.lastError().text();
            charactersDB->close();
        } else {
            while ( QLoadCharacter.next() ) {
                nickname = QLoadCharacter.value(Character::Nickname).toString();
                classID = QLoadCharacter.value(Character::ClassID).toUInt();
                stats.hitPoints = QLoadCharacter.value(Character::HitPoints).toUInt();
                stats.manaPoints = QLoadCharacter.value(Character::ManaPoints).toUInt();
                stats.attack = QLoadCharacter.value(Character::Attack).toUInt();
                stats.defence = QLoadCharacter.value(Character::Defence).toUInt();
                position.x = QLoadCharacter.value(Character::PositionX).toFloat();
                position.y = QLoadCharacter.value(Character::PositionY).toFloat();
            }
        }
//        charactersDB->close();
//    }
}

Player::~Player()
{
//    if ( !charactersDB->open() ) {
//        qDebug() << "CharactersDB open error: " << charactersDB->lastError().text();
//    } else {
        QSqlQuery QSaveCharacter(*charactersDB);

        QSaveCharacter.prepare("UPDATE CharacterList SET hitPoints=:hitPoints, manaPoints=:manaPoints, positionX=:positionX, positionY=:positionY WHERE nickname=:nickname");
        QSaveCharacter.bindValue(":hitPoints", QString::number(stats.hitPoints));
        QSaveCharacter.bindValue(":manaPoints", QString::number(stats.manaPoints));
        QSaveCharacter.bindValue(":positionX", QString::number(position.x).toFloat());
        QSaveCharacter.bindValue(":positionY", QString::number(position.y).toFloat());
        QSaveCharacter.bindValue(":nickname", nickname);

        if ( !QSaveCharacter.exec() ) {
            qDebug() << "Character save error: " << QSaveCharacter.lastError().text();
//            charactersDB->close();
        } else {
            qDebug() << "Character " << nickname << " saved";
        }
//        charactersDB->close();
//    }
}

void Player::attack()
{
    sf::Clock clock;

    sf::Time time = clock.restart();
    inFight = true;
    qDebug() << "bool" << inFight;
    while ( clock.getElapsedTime().asMilliseconds() < 5000) {
        time = clock.getElapsedTime();

        qDebug() << "Timeleft: " << time.asMilliseconds();
    }

    inFight = false;
    qDebug() << "bool" << inFight;
}

