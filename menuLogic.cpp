#include "menuLogic.h"

MenuLogic::MenuLogic(Accounts *_accounts)
{
    accounts = _accounts;
}

MenuLogic::~MenuLogic()
{

}

void MenuLogic::checkLogin(int command, std::string ip, unsigned short port, std::string login, std::string password)
{
    int ID = accounts->check(login, password);
    if ( ID < 0 ) {
        loginReject(command, ip, port);
    } else {
        accounts->addUser(ID, ip, port);
        loginAccept(command, ip, port);
        accounts->setStatus(ID, 0);
    }
//        if ( !accounts->isOnline(ID, ip, port) ) {
//            accounts->addUser(ID, ip, port);
//            loginAccept(command, ip, port);
//            accounts->setStatus(ID, 0);
//        } else {
//            if ( accounts->disconnectUser(ID) ) {
//            accounts->addUser(ID, ip, port);
//            loginAccept(command, ip, port);
//            accounts->setStatus(ID, 0);
//            } else {
//                loginReject(command, ip, port);
//            }
//        }
//    }
}

void MenuLogic::getUserCharacters(int command, std::string ip, unsigned short port)
{

    int position = accounts->getUserListPosition(ip, port);

    if ( position < 0 ) {
        qDebug() << "[ML:gUC] Characters not found";
        outPacket << mark << command << 0;
        socket.send(outPacket, ip, port);
    } else {
        QList<User::Character*> characters;
        characters = accounts->users.at(position)->getCharacters();
        if ( characters.size() <= 0 ) {
            qDebug() << "Characters empty";
            outPacket << mark << command << 0;
        } else {

        qDebug() << "Characters size out: " << characters.size();

        QList<User::Character*>::iterator it = characters.begin();
        QString characterName;
        unsigned short characterClass;

        outPacket << mark << command << characters.size();

            while ( it != characters.end() ) {
                characterName = (*it)->Nickname;
                qDebug() << "Char name: " << characterName;
                characterClass = (*it)->ClassId;
                std::cout << "Char class: " << characterClass << "\n";

                outPacket << characterName.toStdString() << characterClass;
                it++;
            }
        }
        characters.clear();
        socket.send(outPacket, ip, port);
    }
    qDebug() << "Send characters to: " << QString::fromStdString(ip) << ":" << port;

    outPacket.clear();
}

void MenuLogic::registration(int command, std::string ip, unsigned short port, std::string log, std::string password)
{
    bool isRegistered;

    outPacket << mark << command;

    if ( !accounts->newUser(log, password, ip, port) ) {
        isRegistered = false;
    } else {
        isRegistered = true;
    }

    outPacket << isRegistered;
    socket.send(outPacket, ip, port);

    outPacket.clear();
}

void MenuLogic::chooseUserCharacter(int command, std::string ip, unsigned short port, std::string nickname)
{
    QList<User*>::iterator it = accounts->users.begin();
    Player::Stats stats;
    sf::Vector2f pos;

    unsigned short classID;
    outPacket << mark << command << nickname;

    while ( it != accounts->users.end() ) {
        if ( (*it)->userIP == ip && (*it)->userPort == port ) {
            (*it)->chooseCharacter(nickname);
            stats = (*it)->player->stats;
            classID = (*it)->player->classID;
            pos = (*it)->player->position;
        } else {

        }
        it++;
    }
    outPacket << classID << pos.x << pos.y << stats.attack << stats.defence << stats.hitPoints << stats.manaPoints;
    socket.send(outPacket, ip, port);

    outPacket.clear();
}

void MenuLogic::deleteCharacter(int command, std::string ip, unsigned short port, std::string nickname)
{
    bool isDeleted;
    outPacket << mark << command;

    int position = accounts->getUserListPosition(ip, port);
    if ( position < 0 ) {
        qDebug() << "[ML:dC] Delete error";
    } else {
        if ( accounts->users.at(position)->removeCharacter(nickname) ) {
            isDeleted = true;
            accounts->users.at(position)->reloadCharacters();
        } else {
            isDeleted = false;
        }

        outPacket << isDeleted;
        socket.send(outPacket, ip, port);

        outPacket.clear();
        if ( isDeleted ) {
            getUserCharacters(1, ip, port);
            qDebug() << "OUT PACKET";
        }
    }

}

void MenuLogic::newCharacter(int command, std::string ip, unsigned short port, std::string nickname, int classID)
{
    bool isCreated;

    outPacket << mark << command;
    if ( accounts->createCharacter(ip, port, nickname, classID) ) {
        accounts->users.at(accounts->getUserListPosition(ip, port))->reloadCharacters();
        isCreated = true;
    } else {
        isCreated = false;
    }

    outPacket << isCreated;
    socket.send(outPacket, ip, port);

    outPacket.clear();
}

void MenuLogic::loginAccept(int command, std::string ip, unsigned short port)
{
    bool isLogin = true;
    outPacket << mark << command << isLogin;
    socket.send(outPacket, ip, port);
    qDebug() << "Out packet loginAccept";
    outPacket.clear();
}

void MenuLogic::loginReject(int command, std::string ip, unsigned short port)
{
    bool isLogin = false;
    outPacket << mark << command << isLogin;
    socket.send(outPacket, ip, port);
    qDebug() << "Out packet loginReject";

    outPacket.clear();
}



