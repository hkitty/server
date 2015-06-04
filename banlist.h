#ifndef BANLIST_H
#define BANLIST_H
#include <iostream>
#include <user.h>
#include <QList>
#include <QDebug>
#include <QFile>
#include <QDir>
class BanList
{
public:
    BanList();
    ~BanList();

public:
    QList<std::string> bans;

public:
    void add(std::string userIP);
    bool check(std::string userIP);
    void del(std::string userIP);
    void showBans();

private:
    QFile *banListFile = new QFile(QDir::currentPath() + "/BanList.txt");
};

#endif // BANLIST_H
