#ifndef ACCOUNTS_H
#define ACCOUNTS_H
#include <iostream>
#include <QList>
#include <QDebug>
#include <QFile>
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
    QStringList accLogins;
    QSqlDatabase accountsDB;
    QSqlDatabase charactersDB;
    QString accountsConnection = "a_connection" + QString::number(rand());
    QString charactersConnection = "c_connection" + QString::number(rand());

    BanList *banlist;// = new BanList(&accountsDB);

public:
    bool addUser(int ID, std::string ip, unsigned short port);
    bool newUser(std::string log, std::string pass, std::string ip, unsigned short port);
    void showUsers();
    void showAllUsers();

    int check(std::string log, std::string pass);

    QList<User::Character*> getCharacters(std::string ip, unsigned short port);
    bool createCharacter(std::string ip, unsigned short port, std::string characterNickname, int classID);
    int getUserID(std::string ip, unsigned short port);
    void getStatus(std::string login);
    bool setStatus(std::string login, int status);
    bool setStatus(std::string ip, unsigned short port, int status);
    bool setStatus(int id, int status);

private:
    QFile *accountsFile = new QFile(QDir::currentPath() + "/AccountsList.txt");
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
