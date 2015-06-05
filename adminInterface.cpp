#include "adminInterface.h"
#include <unistd.h>

AdminInterface::AdminInterface()
{
    qDebug() << "Create commandController";
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
            qDebug() << "(Use commands: Start, Stop, Ban, Unban, showBans, showUsers, setPort, Exit) \n";

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
            std::cout << "Enter IP: ";
            std::cin >> IP;
            std::cout << IP << " IP banned" << std::endl;
            msgCntr->banlist->add(IP);

        } else if ( command == "unban" || command == "Unban" ) {
            std::cout << " Enter IP: ";
            std::cin >> IP;
            msgCntr->banlist->del(IP);

        } else if ( command == "kick" || command == "Kick" ) {
            std::cout << "Enter ID: ";
            unsigned short userID;
            std::cin >> userID;
            msgCntr->parser->kickUser(userID);

        } else if ( command == "showbans" || command == "showBans" || command == "banlist" ) {
            msgCntr->banlist->showBans();

        } else if ( command == "showusers" || command == "showUsers"  ) {
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

