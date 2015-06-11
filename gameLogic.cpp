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
            nickname = (*it)->player->nickname.toStdString();

            outPacket << mark << command << nickname << (*it)->player->classID << (*it)->player->position.x << (*it)->player->position.y;
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

void GameLogic::playerAttack(int command, std::string ip, unsigned short port)
{
//    QList<User*>::iterator it = accounts->users.begin();
//    User *user;

//    while ( it != accounts->users.end() ) {
//        if ( (*it)->userIP == ip && (*it)->userPort == port ) {
//            (*it)->player->TStartAttack.launch();
//            characterNickname = user->player->nickname.toStdString();
//        } else {
//            //--TODO n f
//        }
//        it++;
    //    }
}

void GameLogic::deattach(std::string ip, unsigned short port)
{
    accounts->setStatus(ip, port, 0);

    QList<User*>::iterator it = accounts->users.begin();
    unsigned short userID;

    while ( it != accounts->users.end() ) {
        if ( (*it)->userIP == ip && (*it)->userPort == port ) {
            userID = (*it)->userID;
            qDebug() << "UserID in user" <<(*it)->userID;
        } else {
        }
        it++;
    }
    qDebug() << "UserID" << userID;
    if ( userID <= accounts->users.size() ) {
        accounts->users.removeAt(userID);
    } else {
        qDebug() << "User not fount";
    }
}

