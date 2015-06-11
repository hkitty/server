#include "player.h"

Player::Player(QSqlDatabase *_charactersDB, std::string characterNickname) :
    TStartAttack(&Player::attack, this)
{

    charactersDB = _charactersDB;

    if ( !charactersDB->open() ) {
        qDebug() << "CharactersDB open error: " << charactersDB->lastError().text();
    } else {

    QSqlQuery QLoadCharacter(*charactersDB);

    QLoadCharacter.prepare("SELECT * FROM CharacterList WHERE nickname=:nickname");
    QLoadCharacter.bindValue(":nickname", QString::fromStdString(characterNickname));

        if ( !QLoadCharacter.exec() ) {
            qDebug() << "Load character error: " << QLoadCharacter.lastError().text();
        } else {
            while ( QLoadCharacter.next() ) {
                nickname = QLoadCharacter.value(Character::Nickname).toString();
                characterClass = QLoadCharacter.value(Character::ClassID).toUInt();
                stats.hitPoints = QLoadCharacter.value(Character::HitPoints).toUInt();
                stats.manaPoints = QLoadCharacter.value(Character::ManaPoints).toUInt();
                stats.attack = QLoadCharacter.value(Character::Attack).toUInt();
                stats.defence = QLoadCharacter.value(Character::Defence).toUInt();
                position.x = QLoadCharacter.value(Character::PositionX).toFloat();
                position.y = QLoadCharacter.value(Character::PositionY).toFloat();
            }
        }
        charactersDB->close();
    }
}

Player::~Player()
{
    if ( !charactersDB->open() ) {
        qDebug() << "CharactersDB open error: " << charactersDB->lastError().text();
    } else {
        QSqlQuery QSaveCharacter(*charactersDB);

        QSaveCharacter.prepare("INSERT INTO Characters(hitPoints, manaPoints, positionX, positionY)"
                               "VALUES(:hitPoints, :manaPoints, :positionX, :positionY) WHEWRE nickname=:nickname");
        QSaveCharacter.bindValue(":hitPoints", QString::number(stats.hitPoints));
        QSaveCharacter.bindValue(":manaPoints", QString::number(stats.manaPoints));
        QSaveCharacter.bindValue(":positionX", QString::number(position.x).toFloat());
        QSaveCharacter.bindValue(":positionY", QString::number(position.y).toFloat());
        charactersDB->close();
    }
}

void Player::attack()
{
    inFight = true;
    timer->start(10000);
    inFight = false;
}

