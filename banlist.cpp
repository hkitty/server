#include "banlist.h"

BanList::BanList()
{
    qDebug() << "Load BanList...";
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

        banListFile->close();
    } else {
        qDebug() << "File not found";
    }

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

//    QListIterator<std::string> it(bans);

    if ( bans.contains(userIP) ) {
        return true;
    } else {
        return false;
    }
//    qDebug() << "Checking... ";
//    while ( it.hasNext() ) {
//        if ( userIP == it.next() ) {
//            qDebug() << "IP detected: ";
//            qDebug() << QString::fromStdString(it.next());
//            return false;
//        }
//        qDebug() << "Free ip";
//        it.next();
//    }
    //    return true;
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
        //banListFile->flush()
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


