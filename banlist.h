#ifndef BANLIST_H
#define BANLIST_H
#include <iostream>
#include <user.h>
#include <QList>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QtSql>

class BanList
{
public:
    BanList(QSqlDatabase *_accountDB);
    ~BanList();

public:
    QList<std::string> bans;

public:
    bool add(std::string ip, std::string nickname);
    bool add(std::string ip, std::string nickname, std::string reason);
    bool check(std::string ip);

    void del(std::string ip);
    void showBans();

private:
    QSqlDatabase *accountsDB;
    QDateTime dateTime;

    enum Banlist {
        IP,
        Time,
        Nickname,
        Reason
    };
};

#endif // BANLIST_H
