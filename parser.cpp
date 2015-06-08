#include "parser.h"

Parser::Parser()
{
//    qDebug() << "Create parser";
}

Parser::~Parser()
{

}

void Parser::receive(Task task)
{
    sf::Packet packet;

    packet = task.packet;

    int command;
    unsigned short _mark;
    unsigned short port;

    packet >> _mark;

    if ( _mark == mark ) {
        packet >> port;
        packet >> command;
        qDebug() << "Current command: " << command;
        switch (command) {
            case Commands::Login: {
                std::string login, pass;

                packet >> login >> pass;
                menuLogic->checkLogin(command, task.sender, port, login, pass);
            }
            break;

            case Commands::GetUserCharacters: {
                menuLogic->getUserCharacters(command, task.sender, port);
            }
            break;

            case Commands::Registration: {
                std::string log;
                std::string pass;

                packet >> log >> pass;
                menuLogic->registration(command, task.sender, port, log, pass);
            }
            break;

            case Commands::NewCharacter: {
                std::string characterNickname;
                int characterClass;

                packet >> characterNickname >> characterClass;
                menuLogic->newCharacter(command, task.sender, port, characterNickname, characterClass);
            }
            break;

            case Commands::ChooseCharacter: {
                std::string characterNickname;

                packet >> characterNickname;
                menuLogic->chooseUserCharacter(command, task.sender, port, characterNickname);
            }
            break;

            case Commands::DeleteCharacter: {
                std::string characterNickname;

                packet >> characterNickname;
                menuLogic->deleteCharacter(command, task.sender, port, characterNickname);
            }
            break;

            case Commands::InGame: {
                bool isOk;

                packet >> isOk; //--TODO Users status changing
                gameLogic->getEnemys(command, task.sender, port);
                gameLogic->sendNewPlayer(command, task.sender, port);
            }
            break;

            case Commands::PlayerMove: {
                sf::Vector2f vec;

                packet >> vec.x >> vec.y;
                gameLogic->playerMove(command, task.sender, port, vec);
            }
            break;

            case Commands::ExitGame: {
                gameLogic->deattach(task.sender, port);
            }
            break;

            default:
                qDebug() << "404";
                break;
        }
    }
}

void Parser::kickUser(unsigned short userID)
{
    if ( userID <= accounts->users.size() ) {
        accounts->users.removeAt(userID);
    } else {
        qDebug() << "User not fount";
    }
}


