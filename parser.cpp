#include "parser.h"

Parser::Parser()
{
    qDebug() << "Create parser";
}

Parser::~Parser()
{

}

void Parser::receive(Task task)
{

    qDebug() << " Enter Receicve";
    sf::Packet packet;

    packet = task.packet;
    qDebug() << "Packet size: " << packet.getDataSize();
    int command;
    unsigned short _mark;
    unsigned short port;

    packet >> _mark;
    qDebug() << _mark << "[mark]";

    if ( _mark == mark ) {
        packet >> port;
        qDebug() << "User port: " <<  port;
        packet >> command;
        qDebug() << "Current command: " << command;
        switch (command) {
            case Commands::Login: {
                std::string login, pass;
                packet >> login >> pass;
                qDebug() << "Mark: " << _mark << "Comm: " << command << "Log: " << QString::fromStdString(login) << "Pass: " << QString::fromStdString(pass);
                qDebug() << "Log size: " << login.size() << "Pass size: " << pass.size();
                if ( accounts->check(login, pass) ) {
                    accounts->addUser(login, pass, task.sender, port);
                    loginAccept(command, task.sender, port);
                } else {
                    loginReject(command, task.sender, port);
                }
            }
            break;

            case Commands::GetUserCharacters: {
                    getUserCharacters(command, task.sender, port);
            }
            break;

            case Commands::Registration: {
                    std::string log;
                    std::string pass;

                    packet >> log >> pass;
                    registration(command, task.sender, port, log, pass);
            }
            break;

            case Commands::NewCharacter: {
                    std::string characterNickname;
                    unsigned short characterClass;

                    packet >> characterNickname >> characterClass;
                    newCharacter(command, task.sender, port, characterNickname, characterClass);

            }
            break;

            case Commands::ChooseCharacter: {
                    std::string characterNickname;
                    packet >> characterNickname;
                    std::cout << "packet in Nickname: " << characterNickname << "\n";
                    chooseUserCharacter(command, task.sender, port, characterNickname);
            }
            break;

            case Commands::DeleteCharacter: {

            }
            break;

            case Commands::InGame: {
                    bool isOk;
                    packet >> isOk; //--TODO Users status changing
                    getEnemys(task.sender, port);
                    sendNewPlayer(task.sender, port);
            }
            break;

            case Commands::PlayerMove: {
                    sf::Vector2f vec;

                    packet >> vec.x >> vec.y;
                    std::cout << "Vec.x: " << vec.x << " Vec.y: " << vec.y << "\n";
                    playerMove(command, task.sender, port, vec);
            }
            break;

            default:
                qDebug() << "404";
                break;
        }
    }
}

void Parser::loginAccept(int command, std::string ip, unsigned short port)
{
    bool isLogin = true;
    qDebug() << "Command log acc: " << command;
    outPacket << mark << command << isLogin;
    qDebug() << "Data size: " << outPacket.getDataSize();
    socket.send(outPacket, ip, port);
    qDebug() << "Out packet Log OK";
    outPacket.clear();
}

void Parser::loginReject(int command, std::string ip, unsigned short port)
{
    bool isLogin = false;
    outPacket << mark << command << isLogin;
    socket.send(outPacket, ip, port);
    qDebug() << "Out packet Log FALSE";
    outPacket.clear();
}

bool Parser::registration(int command, std::string ip, unsigned short port, std::string log, std::string pass)
{
    qDebug() << "Command: " << command;
    if ( log.size() != 0 && pass.size() != 0 ) { //--TODO Check double nickname
        if ( !accounts->accLogins.contains(QString::fromStdString(log)) ) {
        accounts->newUser(log, pass);
        return true;
        } else {
            qDebug() << "This username is already used"; //--TODO Send error to sender
        }
    } else {
        qDebug() << "Incorrect login or passworg [Log or pass ..size() == 0]";
    }

    //--TODO Send answer to new user
}

void Parser::getUserCharacters(int command, std::string ip, unsigned short port)
{
    QList<User::Character> characters;

    characters = accounts->getCharacters(ip);

    qDebug() <<"Characters count: "<< characters.size();

        QList<User::Character>::iterator it = characters.begin();
        QString characterName;
        unsigned short characterClass;

        qDebug() << "Command: " << command;
        qDebug() << "Characters size: " << characters.size();
//        qDebug() << "Char 1 name: " << characters.at(0).Nickname;
        outPacket << mark << command << characters.size();

            while ( it != characters.end() ) {
                characterName = (*it).Nickname;
                characterClass = (*it).ClassId;

                outPacket << characterName.toStdString() << characterClass;
                it++;
            }

        socket.send(outPacket, ip, port);
        qDebug() << "Send characters to: " << QString::fromStdString(ip) << ":" << port;
        outPacket.clear();
}

void Parser::chooseUserCharacter(int command, std::string ip, unsigned short port, std::string characterNickname)
{
    QList<User*>::iterator it = accounts->users.begin();
    Player::Stats stats;
    sf::Vector2f pos;

    unsigned short characterClassID;

    std::cout << "[Parser::chooseUserCharacter] Character nickname: " << characterNickname << "\n";
    while ( it != accounts->users.end() ) {
        qDebug() << "Enter while";
        if ( (*it)->userIP == ip && (*it)->userPort == port ) {
            (*it)->chooseCharacter(characterNickname);
            stats = (*it)->player->stats;
            characterClassID = (*it)->player->characterClass;
            pos = (*it)->player->position;
        }
        it++;
    }
    qDebug() << "Parser::chooseUserCharacter: " << command;
    outPacket << mark << command << characterClassID << pos.x << pos.y << stats.attack << stats.defence << stats.hitPoints << stats.manaPoints;
    socket.send(outPacket, ip, port);
    outPacket.clear();
}

void Parser::newCharacter(int command, std::string ip, unsigned short port, std::string characterNickname, unsigned short characterClassID)
{
    bool isOk = accounts->createCharacter(ip, port, characterNickname, characterClassID);

    outPacket << mark << command << isOk ;
    socket.send(outPacket, ip, port);
    outPacket.clear();
}

void Parser::getEnemys(std::string ip, unsigned short port)
{
    qDebug() << "Get enemys";
    QList<User*>::iterator it = accounts->users.begin();
    std::string characterNickname;

    int _command = Commands::GetEnemys;

    while ( it != accounts->users.end() ) {
        qDebug() << "Enter while";
        if ( (*it)->userIP == ip && (*it)->userPort == port ) {
            qDebug() << "enter if";
        } else {
            qDebug() << "enter else";
            qDebug() << (*it)->player->nickname;
            characterNickname = (*it)->player->nickname.toStdString();
            std::cout << "Character nickname: " << characterNickname << "\n";
            outPacket << mark << _command << characterNickname << (*it)->player->characterClass << (*it)->player->position.x << (*it)->player->position.y;
            socket.send(outPacket, ip, port);
            qDebug() << "Send pack";
            outPacket.clear();
        }
    it++;
    }
}

void Parser::sendNewPlayer(std::string ip, unsigned short port)
{
    std::cout << "IP: " << ip << " Port: " << port << "\n";
    qDebug() << "Send new player";
    QList<User*>::iterator it = accounts->users.begin();
    std::string characterNickname;
    User *user;

    int _command = Commands::GetEnemys;

    while ( it != accounts->users.end() ) {
        qDebug() << "Users size: " << accounts->users.size();
        if ( (*it)->userIP == ip && (*it)->userPort == port ) {
            std::cout << "[Parser::sednNewPlayer] Player ok" << "\n";
            user = *it;
            characterNickname = user->player->nickname.toStdString();
            std::cout << "[Parser::sednNewPlayer] Character nick: " << characterNickname << "\n";
        } else {
            //--TODO n f
            qDebug() << "[Parser::sednNewPlayer] file N F " << "\n";
        }
        it++;
    }

    it = accounts->users.begin();

    outPacket << mark << _command << characterNickname << user->player->characterClass << user->player->position.x << user->player->position.y;

    while ( it != accounts->users.end() ) {
        std::cout << "[sendNewPlayer()] User ip: " << (*it)->userIP << "User port: " << (*it)->userPort << "\n";
        if ( (*it)->userIP == ip && (*it)->userPort == port) {
                qDebug() << "Errory";
        } else {
            std::cout << "[sendNewPlayer()] Enter if!=ip&&port";
            socket.send(outPacket, (*it)->userIP, (*it)->userPort);
        }
    it++;
    }
    outPacket.clear();
    //    accounts->users.at(id).setStatus(true);
}

void Parser::playerMove(int command, std::string ip, unsigned short port, sf::Vector2f vect)
{
    QList<User*>::iterator it = accounts->users.begin();
    std::string characterNickname;
    User *user;

    while ( it != accounts->users.end() ) {
        if ( (*it)->userIP == ip && (*it)->userPort == port ) {
            user = *it;
            (*it)->player->position = vect;
            characterNickname = user->player->nickname.toStdString();
        } else {
            //--TODO n f
        }
        it++;
    }

    it = accounts->users.begin();

    outPacket << mark << command << characterNickname << vect.x << vect.y;

    while ( it != accounts->users.end() ) {
        if ( (*it)->userIP == ip && (*it)->userPort == port ) {
            std::cout << "WTF???\n";
        } else {
            socket.send(outPacket, (*it)->userIP, (*it)->userPort);
        }
        it++;
    }
    outPacket.clear();
}

void Parser::kickUser(unsigned short userID)
{
    if ( userID <= accounts->users.size() ) {
        accounts->users.removeAt(userID);
    } else {
        qDebug() << "User n f";
    }
}


