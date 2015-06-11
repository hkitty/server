#ifndef COMMANDCONTROLLER_H
#define COMMANDCONTROLLER_H
#include <QCoreApplication>
#include <QDebug>
#include <iostream>
#include <messageController.h>
#include <settings.h>

class AdminInterface
{
public:
    AdminInterface();
    ~AdminInterface();

public:
    void authentication();
    void checkCommand();

private:
    std::string command;
    std::string IP;

    int id;
    int _port;

private:
    Settings *settings = new Settings;
    MessageController *msgCntr = new MessageController;

};

#endif // COMMANDCONTROLLER_H
