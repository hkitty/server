#include "player.h"

Player::Player(std::string characterNickname, std::string login)
{
    qDebug() << "Create player";
    QString path = QString::fromStdString(login) + "_" + QString::fromStdString(characterNickname);
    QFile character(path);
    QString _hp;
    QString _mp;
    QString _atk;
    QString _def;

    if ( character.exists() ) {
        if ( !character.open(QIODevice::ReadOnly) ) {
            qDebug() << "File not open [Player]";
        }

        while ( !character.atEnd() ) {
            _hp = character.readLine();
            _hp = _hp.remove(_hp.size() - 1, 1);
            hitPoints = _hp.toInt();

            _mp = character.readLine();
            _mp = _mp.remove(_mp.size() - 1, 1);
            manaPoints = _mp.toInt();

            _atk = character.readLine();
            _atk = _atk.remove(_atk.size() - 1, 1);
            attack = _atk.toInt();

            _def = character.readLine();
            _def = _def.remove(_def.size() - 1, 1);
            defence = _def.toInt();
        }
    }
    character.close();
}

Player::~Player()
{

}

