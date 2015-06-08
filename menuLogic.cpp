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
    QList<User::Character> characters;
    characters = accounts->getCharacters(ip, port);
    qDebug() << "Characters size out: " << characters.size();

    QList<User::Character>::iterator it = characters.begin();
    QString characterName;
    unsigned short characterClass;

    outPacket << mark << command << characters.size();

        while ( it != characters.end() ) {
            characterName = (*it).Nickname;
            qDebug() << "Char name: " << characterName;
            characterClass = (*it).ClassId;
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
        if ( (*it)->userIP != ip && (*it)->userPort != port ) {
            qDebug() << "[MenuLogic::chooseUserCharacter] Enter if";
        } else {
            qDebug() << "[MenuLogic::chooseUserCharacter] Enter else";
            (*it)->chooseCharacter(characterNickname);
            stats = (*it)->player->stats;
            characterClassID = (*it)->player->characterClass;
            pos = (*it)->player->position;
        }
        it++;
    }
    outPacket << characterClassID << pos.x << pos.y << stats.attack << stats.defence << stats.hitPoints << stats.manaPoints;
    socket.send(outPacket, ip, port);

    outPacket.clear();
}

void MenuLogic::deleteCharacter(int command, std::string ip, unsigned short port, std::string characterNickname)
{
    QList<User*>::iterator it = accounts->users.begin();
    bool isDeleted;
    std::string userLogin;

    outPacket << mark << command;

    while ( it != accounts->users.end() ) {
        if ( (*it)->userIP == ip && (*it)->userPort == port ) {
            userLogin = (*it)->userLog;
        } else {
            qDebug() << "User not found";
        }
    }

    QString pathToCharacter = QDir::currentPath() + "/Characters/" + QString::fromStdString(userLogin) + "_" + QString::fromStdString(characterNickname) + ".txt";
    QString pathToAccount = QDir::currentPath() + "/Accounts/" + QString::fromStdString(userLogin) + ".txt";
    QString line;
    QStringList currentUser;
    QStringList strList;

    QFile characterFile(pathToCharacter);
    if ( characterFile.exists() ) {
        if ( characterFile.remove() ) {
            QFile accountFile(pathToAccount);
            if ( accountFile.open(QIODevice::WriteOnly) ) {
                while ( !accountFile.atEnd() ) {
                    line = accountFile.readLine();
                    line = line.remove(line.size() - 1, 1);
                    currentUser = line.split(" ");
                    if ( currentUser.at(0) != QString::fromStdString(characterNickname) ) {
                        strList.append(line);
                    }
                }
                accountFile.close();
            }
        }
    } else {
        qDebug() << "Character file not found";
        isDeleted = false;
    }

    QFile accFile(pathToAccount);
    if ( !accFile.open(QIODevice::Append ) ) {
        qDebug() << "Account file open error";
        isDeleted = false;
    } else {
        QString sline;
        QStringList::iterator it = strList.begin();

        while ( it != strList.end() ) {
            sline = (*it).at(0);
            sline += " ";
            sline += (*it).at(1);
            accFile.write(QByteArray::fromStdString(sline.toStdString()));
            sline.clear();
            it++;
        }
        isDeleted = true;
    }
    accFile.close();
    outPacket << isDeleted;
    socket.send(outPacket, ip, port);

    outPacket.clear();
}

void MenuLogic::newCharacter(int command, std::string ip, unsigned short port, std::string characterNickname, int characterClassID)
{
    bool isCreated;

    outPacket << mark << command;
    if ( !accounts->createCharacter(ip, port, characterNickname, characterClassID) ) {
        isCreated = false;
    } else {
        isCreated = true;
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

