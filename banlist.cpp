#include "banlist.h"

BanList::BanList(QSqlDatabase *_accountDB)
{
    accountsDB = _accountDB;

    QSqlQuery QLoadBanlist(*accountsDB);

    QLoadBanlist.prepare("SELECT * FROM Banlist");
    if ( !QLoadBanlist.exec() ) {
        qDebug() << "Load banlist error: " << QLoadBanlist.lastError().text();
    } else {
        qDebug() << "Load banlist";
        while ( QLoadBanlist.next() ) {
            bans.append(QLoadBanlist.value(Banlist::IP).toString().toStdString());
        }
    }
}

BanList::~BanList()
{

}

bool BanList::add(std::string ip, std::string nickname)
{

    QSqlQuery QBan(*accountsDB);

    QBan.prepare("INSERT INTO Banlist(IP, time, nickname)" "VALUES(:ip, :time, :nickname)");
    QBan.bindValue(":ip", QString::fromStdString(ip));
    QBan.bindValue(":time", QDateTime::currentDateTime().toString());
    QBan.bindValue(":nickname", QString::fromStdString(nickname));

    if ( !QBan.exec() ) {
        qDebug() << "QBan error: " << QBan.lastError().text();
        return false;
    } else {
        qDebug() << "IP: " << QString::fromStdString(ip) << " add to banlist";
        return true;
    }
}

bool BanList::add(std::string ip, std::string nickname, std::string reason)
{
    QSqlQuery QBan(*accountsDB);

    QBan.prepare("INSERT INTO Banlist(IP, time, nickname, reason)" "VALUES(:ip, :time, :nickname, :reason)");
    QBan.bindValue(":ip", QString::fromStdString(ip));
    QBan.bindValue(":time", QDateTime::currentDateTime().toString());
    QBan.bindValue(":nickname", QString::fromStdString(nickname));
    QBan.bindValue(":reason", QString::fromStdString(reason));

    if ( !QBan.exec() ) {
        qDebug() << "QBan error: " << QBan.lastError().text();
        return false;
    } else {
        qDebug() << "IP: " << QString::fromStdString(ip) << " add to banlist." << " Reason: " << QString::fromStdString(reason);
        return true;
    }
}

bool BanList::check(std::string ip)
{
    if ( bans.contains(ip) ) {
        return true;
    } else {
        return false;
    }
}

void BanList::del(std::string ip)
{

    QSqlQuery QUnban(*accountsDB);

    QUnban.prepare("DELETE FROM Banlist WHERE ip=:ip");
    QUnban.bindValue(":ip", QString::fromStdString(ip));

    if ( !QUnban.exec() ) {
        qDebug() << "Unban error: " << QUnban.lastError().text();
    } else {
        qDebug() << "IP: " << QString::fromStdString(ip) << " remove from banlist";
    }
    int pos;
    int from = 0;

    pos = bans.indexOf(ip, from);
    bans.removeAt(pos);
}

void BanList::showBans()
{
    QListIterator<std::string> it(bans);
    while ( it.hasNext() ) {
        qDebug() << QString::fromStdString(it.next());
    }
}


