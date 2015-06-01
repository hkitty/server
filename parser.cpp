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
//        qDebug() << "Mark == mark";
    packet >> command;

        switch (command) {
            case 0: {
                std::string login, pass;
                packet >> login >> pass;

                if ( accounts->check(login, pass, task.sender, task.port) ) {
                    qDebug() << "Login OK";

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

            case 1: {
//                    User *user;

                    QList<QString> users;
                    users = accounts->getUser(task.sender);
                    qDebug() << " 3";

                    QList<QString>::iterator it = users.begin();

                    qDebug() <<"Size: "<< users.size();
                    outPacket << mark << command << users.size();

                        while ( it != users.end() ) {
                            outPacket << (*it).toStdString();
                            it++;
                        }

                    socket.send(outPacket, task.sender, task.port);
                    outPacket.clear();
            }
            break;

            case 2: {
                    qDebug() << "Cmd 2";
                    std::string log;
                    std::string pass;

                    packet >> log >> pass;
                    accounts->newUser(log, pass);
            }
            break;

            default:
                qDebug() << "404";
                break;
        }
    }
}


