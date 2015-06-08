#include "banlist.h"

BanList::BanList()
{
    qDebug() << "Load banlist";
    QString msgError;
    std::string string;

    if ( banListFile->exists() ) {
        if ( !banListFile->open(QIODevice::ReadOnly | QIODevice::Text) ) {
            qDebug() << "Can`t open banListFile";
            msgError = banListFile->errorString();
            qDebug() << msgError << " [Error]";
        }

        while ( !banListFile->atEnd() ) {
            string = banListFile->readLine().toStdString();
            string.erase(string.size() - 1);
            bans.append(string);
        }

    } else {
        qDebug() << "[BanList::BanList] File not found";
    }
banListFile->close();
}

BanList::~BanList()
{

}

void BanList::add(std::string userIP)
{
    bans.append(userIP);

    if ( banListFile->exists() ) {
        if ( !banListFile->open(QIODevice::Append | QIODevice::Text) ) {
            qDebug() << "File not open";
        }
        std::string banIP;
        banIP = userIP+"\n";
        banListFile->write(QByteArray::fromStdString(banIP));
        banListFile->close();
    }
}

bool BanList::check(std::string userIP)
{
    if ( bans.contains(userIP) ) {
        return true;
    } else {
        return false;
    }
}

void BanList::del(std::string userIP)
{
    int pos;
    int from = 0;
    std::string string;
    pos = bans.indexOf(userIP, from);
    bans.removeAt(pos);

    if ( banListFile->exists() ) {
        if ( !banListFile->open(QIODevice::WriteOnly | QIODevice::Truncate) ) {

        }

        QListIterator<std::string> it(bans);
        while ( it.hasNext() ) {
            string = it.next() + "\n";
            banListFile->write(QByteArray::fromStdString(string));
        }
        banListFile->close();
    }
}

void BanList::showBans()
{
    QListIterator<std::string> it(bans);
    while ( it.hasNext() ) {
        qDebug() << QString::fromStdString(it.next());
    }
}


