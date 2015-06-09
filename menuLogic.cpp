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
    if ( accounts->check(login, password) ) {
        accounts->addUser(login, password, ip, port);
        loginAccept(command, ip, port);
    } else {
        loginReject(command, ip, port);
    }
}

void MenuLogic::getUserCharacters(int command, std::string ip, unsigned short port)
{
    QList<User::Character*> characters;
    characters = accounts->getCharacters(ip, port);
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

    socket.send(outPacket, ip, port);
    qDebug() << "Send characters to: " << QString::fromStdString(ip) << ":" << port;

    outPacket.clear();
}

void MenuLogic::registration(int command, std::string ip, unsigned short port, std::string log, std::string pass)
{
    bool isRegistered;
    outPacket << mark << command;
    if ( log.size() != 0 && pass.size() != 0 ) { //--TODO Check double nickname
        if ( !accounts->accLogins.contains(QString::fromStdString(log)) ) {
            if ( !accounts->newUser(log, pass) ) {
                isRegistered = false;
            } else {
                isRegistered = true;
            }
        } else {
            qDebug() << "This username is already used"; //--TODO Send error to sender
            isRegistered = false;
        }
    } else {
        qDebug() << "Incorrect login or passworg [Log or pass ..size() == 0]";
        isRegistered = false;
    }

    outPacket << isRegistered;
    socket.send(outPacket, ip, port);

    outPacket.clear();
}

void MenuLogic::chooseUserCharacter(int command, std::string ip, unsigned short port, std::string characterNickname)
{
    QList<User*>::iterator it = accounts->users.begin();
    Player::Stats stats;
    sf::Vector2f pos;

    unsigned short characterClassID;
    outPacket << mark << command << characterNickname;

    while ( it != accounts->users.end() ) {
        if ( (*it)->userIP == ip && (*it)->userPort == port ) {
            (*it)->chooseCharacter(characterNickname);
            stats = (*it)->player->stats;
            characterClassID = (*it)->player->characterClass;
            pos = (*it)->player->position;
        } else {

        }
        it++;
    }
    outPacket << characterClassID << pos.x << pos.y << stats.attack << stats.defence << stats.hitPoints << stats.manaPoints;
    socket.send(outPacket, ip, port);

    outPacket.clear();
}

bool MenuLogic::deleteFromUserFiles(std::string ip, unsigned short port, std::string nickname, unsigned short id)
{
    std::string userLogin;
    QStringList strList;
    QStringList accs;
    int i = 0;
    QList<User*>::iterator it = accounts->users.begin();
    while ( it != accounts->users.end() ) {
        if ( (*it)->userIP == ip && (*it)->userPort == port ) {
            userLogin = (*it)->userLog;
            (*it)->deleteCharacter(nickname, id);
        } else {
            qDebug() << "User not found";
        }
        it++;
    }


    QString pathToAccount = QDir::currentPath() + "/Accounts/" + QString::fromStdString(userLogin) + ".txt";
    qDebug() << "Path to delete account: " << pathToAccount;

    QFile accFile(pathToAccount);

    if ( !accFile.open(QIODevice::ReadOnly) ) {
        qDebug() << "File open error";
        return false;
    } else {
        QString line;
        QList<QString>::iterator it = strList.begin();

        while ( it != strList.end() ) {
            line = accFile.readLine();
            line = line.remove(line.size() - 1, 1);
            strList = line.split(" ");
                if ( strList.at(0) != QString::fromStdString(nickname) ) {

                } else {
                    accs.append(line);
                }
         it++;
        }
        accFile.close();

        if ( !accFile.open(QIODevice::WriteOnly | QIODevice::Truncate) ) {
            qDebug() << "Rewrite error";
            return false;
        } else {
            while ( i < accs.size() ) {
                accFile.write(accs.at(i).toUtf8());
                i++;
            }
            accFile.close();

            deleteFromCharacters(userLogin, nickname);
            return true;
        }
    }
    return true;
}

void MenuLogic::deleteFromCharacters(std::string userLogin, std::string characterNickname)
{
    QString pathToCharacter = QDir::currentPath() + "/Characters/" + QString::fromStdString(userLogin) + "_" + QString::fromStdString(characterNickname) + ".txt";
    QFile file(pathToCharacter);
    if ( !file.remove() ) {
        qDebug() << "CharacterFile remove error";
    } else {
        qDebug() << "CharacterFile removed";
    }
}

void MenuLogic::deleteCharacter(int command, std::string ip, unsigned short port, std::string characterNickname, unsigned short id)
{
    bool isDeleted;
    outPacket << mark << command;
    if ( deleteFromUserFiles(ip, port, characterNickname, id) ) {
        isDeleted = true;
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

void MenuLogic::newCharacter(int command, std::string ip, unsigned short port, std::string characterNickname, int characterClassID)
{
    bool isCreated;

    outPacket << mark << command;
    if ( accounts->createCharacter(ip, port, characterNickname, characterClassID) ) {
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



