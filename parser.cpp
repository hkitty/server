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
    sf::Packet outPacket;
    sf::UdpSocket socket;

    packet = task.packet;

    int command;
    int _mark;

    packet >> _mark;
    qDebug() << _mark << "[mark]";

    if ( _mark == mark ) {
        packet >> command;

        switch (command) {
            case Commands::Login: {
                std::string login, pass;
                packet >> login >> pass;

                if ( accounts->check(login, pass, task.sender, task.port) ) {
                    qDebug() << "Login OK";

                    accounts->addUser(login, pass, task.sender, task.port);
                    bool isLogin = true;
                    outPacket << mark << command << isLogin;
                    socket.send(outPacket, task.sender, task.port);
                    outPacket.clear();
                } else {
                    qDebug() << "Login false";

                    bool isLogin = false;
                    outPacket << mark << command << isLogin;
                    socket.send(outPacket, task.sender, task.port);
                    outPacket.clear();
                }
//                qDebug() << "Login: " << QString::fromStdString(login) << " Pass: " << QString::fromStdString(pass);

//                packet.clear();

            }
            break;

            case Commands::GetUserCharacters: {
//                    User *user;

                    QList<QString> characters;

                    characters = accounts->getCharacters(task.sender);
                    qDebug() << "7148";
                    qDebug() <<"Size: "<< characters.size();
                    if ( !characters.isEmpty() ) {
                        qDebug() << "Enter if";

                        QListIterator<QString> it(characters);
                        QString characterName;
                        qDebug() << "Create it";

                        outPacket << mark << command << characters.size();
                        qDebug() << "Out pack";
                            while ( it.hasNext() ) {
                                qDebug() << "Enter while";
                                characterName = it.next();
                                outPacket << characterName.toStdString();
                                qDebug() << "add to packet";
                            }

                        socket.send(outPacket, task.sender, task.port);
                        qDebug() << "send";
                        outPacket.clear();
                        qDebug() << "Clear";
                    } else {
                        qDebug() << "Users is empty";
                    }
            }
            break;

            case Commands::Registration: {
                    qDebug() << "Cmd 2";
                    std::string log;
                    std::string pass;

                    packet >> log >> pass;
                    accounts->newUser(log, pass);
            }
            break;

            case Commands::NewCharacter: {
                    std::string characterNickname;
                    std::string characterClass;
                    User user;

            }

            default:
                qDebug() << "404";
                break;
        }
    }
}


