#include "gameLogic.h"

GameLogic::GameLogic(Accounts *_accounts)
{
    accounts = _accounts;
}

GameLogic::~GameLogic()
{

}

void GameLogic::getEnemys(int command, std::string ip, unsigned short port)
{
    QList<User*>::iterator it = accounts->users.begin();
    std::string nickname;
    qDebug() << "[GL::gE] Command: " << command;

    while ( it != accounts->users.end() ) {
        if ( (*it)->userIP == ip && (*it)->userPort == port ) {
            qDebug() << "[GL::gE] u ";
            qDebug() << (*it)->player->nickname;

        } else {
            qDebug() << "[GL::gE] not u";

//            qDebug() << "NICKNAME:!!!: " << (*it)->player->nickname;
            if ( (*it)->player->nickname != nullptr ) {
                nickname = (*it)->player->nickname.toStdString();
                outPacket << mark << command << nickname << (*it)->player->classID << (*it)->player->position.x << (*it)->player->position.y;
            } else {
                outPacket << mark << command;
            }


            socket.send(outPacket, ip, port);

            outPacket.clear();
        }
    it++;
    }
}

void GameLogic::sendNewPlayer(int command, std::string ip, unsigned short port)
{
    std::cout << "IP: " << ip << " Port: " << port << "\n";

    QList<User*>::iterator it = accounts->users.begin();
    std::string nickname;
    User *user;

    while ( it != accounts->users.end() ) {
        if ( (*it)->userIP == ip && (*it)->userPort == port ) {
            user = *it;
            nickname = user->player->nickname.toStdString();

        } else {
            //--TODO n f
        }
        it++;
    }

    it = accounts->users.begin();

    outPacket << mark << command << nickname << user->player->classID << user->player->position.x << user->player->position.y;

    while ( it != accounts->users.end() ) {

        if ( (*it)->userIP == ip && (*it)->userPort == port) {
        } else {
            socket.send(outPacket, (*it)->userIP, (*it)->userPort);
        }
        it++;
    }

    outPacket.clear();
}

void GameLogic::playerMove(int command, std::string ip, unsigned short port, sf::Vector2f position)
{
    QList<User*>::iterator it = accounts->users.begin();
    std::string characterNickname;
    User *user;

    while ( it != accounts->users.end() ) {
        if ( (*it)->userIP == ip && (*it)->userPort == port ) {
            user = *it;
            (*it)->player->position = position;
            characterNickname = user->player->nickname.toStdString();
        } else {
            //--TODO n f
        }
        it++;
    }
    std::cout << "ID: " << user->userID << " Vec.x: " << position.x << " Vec.y: " << position.y << "\n";

    it = accounts->users.begin();
    outPacket << mark << command << characterNickname << position.x << position.y;

    while ( it != accounts->users.end() ) {
        if ( (*it)->userIP == ip && (*it)->userPort == port ) {
        } else {
            socket.send(outPacket, (*it)->userIP, (*it)->userPort);
        }
        it++;
    }

    outPacket.clear();
}

void GameLogic::playerAttack(int command, std::string ip, unsigned short port, std::string nickname)
{

    int position = accounts->getUserListPosition(ip, port);
    int enemyID;
    std::string attackerNickname;
    std::string targetNickname;
    outPacket << mark << command;

    QList<User*>::iterator it = accounts->users.begin();

    while ( it != accounts->users.end() ) {
        if ( (*it)->player->nickname.toStdString() == nickname ) {
            enemyID = (*it)->userID;
        } else {
            qDebug() << "[GL:pA] User n f";
            //--TODO n f
        }
        it++;
    }

    int enemyPosition = accounts->getUserListPosition(enemyID);

    if ( position < 0 || enemyPosition < 0 ) {
        qDebug() << "[GL:pA] Error";
    } else {
        if ( !accounts->users.at(position)->player->inFight ) {
            attackerNickname = accounts->users.at(position)->player->nickname.toStdString();
            targetNickname = accounts->users.at(enemyPosition)->player->nickname.toStdString();

            outPacket << attackerNickname << targetNickname;
            accounts->users.at(position)->player->TStartAttack.launch();
            unsigned short damage = accounts->users.at(position)->player->stats.attack -
                    accounts->users.at(enemyPosition)->player->stats.defence;
            qDebug() << "Damage = " << damage;
            accounts->users.at(enemyPosition)->player->stats.hitPoints -= damage;
            sf::Packet toEnemy;
            toEnemy << mark << command + 1 << accounts->users.at(enemyPosition)->player->stats.hitPoints;
            socket.send(toEnemy, accounts->users.at(enemyPosition)->userIP, accounts->users.at(enemyPosition)->userPort);
        if ( accounts->users.at(enemyPosition)->player->stats.hitPoints <= 0 ) {
            //--TODO RIP & Spawn on_exit( default position
        }
        qDebug() << "emeny HP" << accounts->users.at(enemyPosition)->player->stats.hitPoints;
        } else {
            qDebug() << "Player inFight";
        }
    }

    it = accounts->users.begin();

    while ( it != accounts->users.end() ) {
        if ( (*it)->userIP == ip && (*it)->userPort == port ) {

            qDebug() << "User" << (*it)->player->nickname << " pidar";
        } else {
            qDebug() << "Send to: " << QString::fromStdString((*it)->userIP) << ":" << (*it)->userPort;
            socket.send(outPacket, (*it)->userIP, (*it)->userPort);
        }
        it++;
    }

    outPacket.clear();
}

void GameLogic::detach(int command, std::string ip, unsigned short port)
{
    accounts->setStatus(ip, port, 0);
    int ID = accounts->getUserID(ip, port);
    int position = accounts->getUserListPosition(ip, port);

    std::string nickname;
    nickname = accounts->users.at(position)->player->nickname.toStdString();
    qDebug()<< "NICKNAME LEAVE: " << QString::fromStdString(nickname);
    outPacket << mark << command << nickname;

//    if ( !accounts->disconnectUser(ID) ) {
//        qDebug() << "User delete error";
//    }
//    else {
        QList<User*>::iterator it = accounts->users.begin();

        while ( it != accounts->users.end() ) {
            if ( (*it)->userIP == ip && (*it)->userPort == port ) {

                qDebug() << "User" << (*it)->player->nickname << "leave";
            } else {
                qDebug() << "Send to: " << QString::fromStdString((*it)->userIP) << ":" << (*it)->userPort;
                socket.send(outPacket, (*it)->userIP, (*it)->userPort);
            }
            it++;
        }
        if ( !accounts->disconnectUser(ID) ) {
            qDebug() << "USR DISCONNECT ERROR!!";
        }

        outPacket.clear();


//        accounts->users.at(position)->player = nullptr;

//        accounts->users.at(position)->deletePlayer();

//        delete accounts->users.at(position);
//        accounts->users.removeAt(position);
//    }
}

