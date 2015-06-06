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
    QList<User*> users;
    QStringList accLogins;

private:
    QFile *accountsFile = new QFile(QDir::currentPath() + "/AccountsList.txt");

public:
    void addUser(std::string log, std::string pass, std::string ip, unsigned short port);
    void newUser(std::string log, std::string pass);
    void showUsers();

    bool check(std::string _log, std::string _pass);

    QList<User::Character> getCharacters(std::string IP);
    bool createCharacter(std::string ip, unsigned short port, std::string characterNickname, unsigned short ClassId);
};

#endif // ACCOUNTS_H
