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

    int command;
    unsigned short _mark;
    unsigned short port;

    packet >> _mark;
    qDebug() << _mark << "[mark]";

    if ( _mark == mark ) {
        packet >> port;
        qDebug() << "User port: " <<  port;
        packet >> command;

        switch (command) {
            case Commands::Login: {
                std::string login, pass;
                packet >> login >> pass;

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
//                    qDebug() << "Enter New Char";
                    std::string characterNickname;
                    unsigned short characterClass;

                    packet >> characterNickname >> characterClass;

                    accounts->createCharacter(task.sender, port, characterNickname, characterClass);

                    outPacket << 0;
                    socket.send(outPacket, task.sender, port);
                    outPacket.clear();
            }
            break;

            case Commands::ChooseCharacter: {
                    std::string characterNickname;
                    packet >> characterNickname;
                    chooseUserCharacter(command, task.sender, port, characterNickname);
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
    qDebug() << "Login OK";


    qDebug() << ";lasdk";
    bool isLogin = true;
    outPacket << mark << command << isLogin;
    socket.send(outPacket, ip, port);
    qDebug() << "Out packet Log OK";
    outPacket.clear();
}

void Parser::loginReject(int command, std::string ip, unsigned short port)
{
    qDebug() << "Login false";

    bool isLogin = false;
    outPacket << mark << command << isLogin;
    socket.send(outPacket, ip, port);
    qDebug() << "Out packet Log FALSE";
    outPacket.clear();
}

void Parser::registration(int command, std::string ip, unsigned short port, std::string log, std::string pass)
{
    qDebug() << "Command: " << command;
    if ( log.size() != 0 && pass.size() != 0 ) { //--TODO Check double nickname
        if ( !accounts->accLogins.contains(QString::fromStdString(log)) ) {
        accounts->newUser(log, pass);
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
        qDebug() << "Char 1 name: " << characters.at(0).Nickname;
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
    QList<User>::iterator it = accounts->users.begin();
    Player::Stats stats;

    while ( it != accounts->users.end() ) {
        qDebug() << "Enter while";
        if ( (*it).userIP == ip && (*it).userPort == port ) {
            (*it).chooseCharacter(characterNickname);
            stats = (*it).player->stats;
        }
        it++;
    }

    outPacket << stats.attack << stats.defence << stats.hitPoints << stats.manaPoints;
    socket.send(outPacket, ip, port);
}


