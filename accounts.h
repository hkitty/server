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
//    struct User {
//        std::string userLogin;
//        std::string userPassword;
//    };

public:
    QList<User> users;

public:
    QFile *accountsFile = new QFile("/home/hkitty/Projects/Qt/Test/s2/Accounts.txt");

public:
    void newUser(std::string log, std::string pass);
    void showUsers();
    QList<QString> getUser(std::string IP);

    bool check(std::string _log, std::string _pass, std::string IP, unsigned short port);
};

#endif // ACCOUNTS_H
