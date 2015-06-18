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
                std::string login;
                std::string password;

                packet >> login >> password;
                menuLogic->registration(command, task.sender, port, login, password);
            }
            break;

            case Commands::NewCharacter: {
                std::string nickname;
                int classID;

                packet >> nickname >> classID;
                menuLogic->newCharacter(command, task.sender, port, nickname, classID);
                menuLogic->getUserCharacters(Commands::GetUserCharacters, task.sender, port);
            }
            break;

            case Commands::ChooseCharacter: {
                std::string characterNickname;

                packet >> characterNickname;
                menuLogic->chooseUserCharacter(command, task.sender, port, characterNickname);
            }
            break;

            case Commands::DeleteCharacter: {
                qDebug() << "Enter del cahr";
                std::string nickname;

                std::cout << "Pack size berore: " << packet.getDataSize();
                packet >> nickname;
                std::cout << "Pack size after: " << packet.getDataSize();
                std::cout << "NICKNAMEEEE::: " << nickname;
                menuLogic->deleteCharacter(command, task.sender, port, nickname);
            }
            break;

            case Commands::InGame: {
                bool isOk;

                packet >> isOk; //--TODO Users status changing
                command += 1;
                gameLogic->getEnemys(command, task.sender, port);
                gameLogic->sendNewPlayer(command, task.sender, port);
                accounts->setStatus(task.sender, port, 1);
            }
            break;

            case Commands::PlayerMove: {
                sf::Vector2f vec;

                packet >> vec.x >> vec.y;
                gameLogic->playerMove(command, task.sender, port, vec);
            }
            break;

            case Commands::ExitGame: {
                gameLogic->detach(command, task.sender, port);
            }
            break;

            case Commands::PlayerAttack: {
                std::string attackPlayer;
                packet >> attackPlayer;
                gameLogic->playerAttack(command, task.sender, port, attackPlayer);

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


