#include "player.h"

Player::Player(std::string characterNickname, std::string login)
{
    qDebug() << "Create player";
    nickname = QString::fromStdString(characterNickname);

    QString path = QString::fromStdString(login) + "_" + QString::fromStdString(characterNickname);
    QFile character(path);
    QString _hp;
    QString _mp;
    QString _atk;
    QString _def;
    QString _characterClass;

    if ( character.exists() ) {
        if ( !character.open(QIODevice::ReadOnly) ) {
            qDebug() << "File not open [Player]";
        }

        while ( !character.atEnd() ) {
            _characterClass = character.readLine();
            _characterClass = _characterClass.remove(_characterClass.size() - 1, 1);
            characterClass = _characterClass.toUShort();

            _hp = character.readLine();
            _hp = _hp.remove(_hp.size() - 1, 1);
            stats.hitPoints = _hp.toUShort();

            _mp = character.readLine();
            _mp = _mp.remove(_mp.size() - 1, 1);
            stats.manaPoints = _mp.toUShort();

            _atk = character.readLine();
            _atk = _atk.remove(_atk.size() - 1, 1);
            stats.attack = _atk.toUShort();

            _def = character.readLine();
            _def = _def.remove(_def.size() - 1, 1);
            stats.defence = _def.toUShort();
        }
    }
    character.close();
}

Player::~Player()
{

}

