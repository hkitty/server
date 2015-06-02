#ifndef ACCOUNTS_H
#define ACCOUNTS_H
#include <iostream>
#include <QList>
#include <QDebug>
#include <QFile>
#include <user.h>

class Accounts
{
public:
    Accounts();
    ~Accounts();

public:
    QList<User> users;

private:
    QFile *accountsFile = new QFile(QDir::currentPath() + "/AccountsList.txt");

public:
    void addUser(std::string log, std::string pass, std::string ip, unsigned short port);
    void newUser(std::string log, std::string pass);
    void showUsers();
    QList<QString> getCharacters(std::string IP);

    bool check(std::string _log, std::string _pass, std::string IP, unsigned short port);
};

#endif // ACCOUNTS_H
