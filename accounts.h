#ifndef ACCOUNTS_H
#define ACCOUNTS_H
#include <iostream>
#include <QList>
#include <QDebug>
#include <user.h>
#include <QtSql/QSql>
#include <QtSql>
#include <banlist.h>

class Accounts
{
public:
    Accounts();
    ~Accounts();

public:
    QList<User*> users;

    QSqlDatabase accountsDB;
    QSqlDatabase charactersDB;

    QString accountsConnection = "a_connection" + QString::number(rand());
    QString charactersConnection = "c_connection" + QString::number(rand());

    BanList *banlist;

public:
    bool addUser(int ID, std::string ip, unsigned short port);
    bool createCharacter(std::string ip, unsigned short port, std::string nickname, int classID);
    bool newUser(std::string login, std::string password, std::string ip, unsigned short port);

    void showUsers();
    void showAllUsers();

    int check(std::string login, std::string password);
    int getUserID(std::string ip, unsigned short port);

    void getStatus(std::string login);

    bool setStatus(int ID, int status);
    bool setStatus(std::string login, int status);
    bool setStatus(std::string ip, unsigned short port, int status);

    QList<User::Character*> getCharacters(std::string ip, unsigned short port);

private:
    enum Users {
        UserID,
        Login,
        Password,
        AccessRights,
        Status,
        LastIP,
        LastPort,
        Warnings
    };
};

#endif // ACCOUNTS_H
