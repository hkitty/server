#include "adminInterface.h"
#include <unistd.h>

AdminInterface::AdminInterface()
{
//    qDebug() << "Create commandController";
}

AdminInterface::~AdminInterface()
{

}

void AdminInterface::authentication()
{
    std::string log;
    std::string pass;

    while ( true ) {
        std::cout << "Enter login: ";
        std::cin >> log;

        std::cout << "Enter pass: ";
        std::cin >> pass;

        if ( log == settings->rootLog.toStdString() && pass == settings->rootPass.toStdString() ) {
            qDebug() << "\nWelcome!";
            qDebug() << "(Use commands: Start, Stop, Kick, Ban, Unban, showBans, showUsers, \nsetPort, setRootLogin, setRootPass, Exit) \n";

            checkCommand();

        } else {
            qDebug() << "\nWrong login or pass! Try Again.";
        }
    }
}

void AdminInterface::checkCommand()
{  

    while ( true ) {

    std::cin >> command;

        if ( command == "Start" || command == "start" ) {
            qDebug() << "Starting server...";
            msgCntr->startReceiver(settings->getPort());
        }

        else if ( command == "Stop" || command == "stop") {
            qDebug() << "Server stopped";
            msgCntr->stop();

        } else if ( command == "Ban" || command == "ban" ) {
            std::string nickname;
            std::string reason;
            std::cout << "Enter IP: ";
            std::cin >> IP;
            std::cout << "Enter nickname: ";
            std::cin >> nickname;
            std::cout << "Enter reason: ";
            std::cin >> reason;
            msgCntr->parser->accounts->banlist->add(IP, nickname, reason);

        } else if ( command == "showAll" || command == "showall" || command == "sa" ) {
            msgCntr->parser->accounts->showAllUsers();

        } else if ( command == "unban" || command == "Unban" ) {
            std::cout << " Enter IP: ";
            std::cin >> IP;
            msgCntr->parser->accounts->banlist->del(IP);

        } else if ( command == "kick" || command == "Kick" ) {
            std::cout << "Enter ID: ";
            unsigned short userID;
            std::cin >> userID;
            msgCntr->parser->kickUser(userID);

        } else if ( command == "showbans" || command == "showBans" || command == "banlist" ) {
            msgCntr->parser->accounts->banlist->showBans();

        } else if ( command == "showusers" || command == "showUsers" || command == "online" || command == "Online"  ) {
            msgCntr->parser->accounts->showUsers();

        } else if ( command == "SetPort" || command == "setport" || command == "Setport" || command == "setPort") {
            std::cout << "Enter port: ";
            std::cin >> _port;
            settings->setPort(_port);

        } else if ( command == "Exit" || command == "exit") {
            exit(0);

        } else {
            qDebug() << "Command not found";
        }
    command.clear();

    sleep(1);
    }
}

