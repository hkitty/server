#include "settings.h"

Settings::Settings()
{
    checkResources();

    qDebug() << "Load settings";
    port = 1234;
    rootLog  = "root";
    rootPass = "root";
    mark = 1337;
}

Settings::~Settings()
{

}

void Settings::setPort(int _port)
{
    port = _port;
}

void Settings::setRootLogin(QString login)
{
    rootLog = login;
}

void Settings::setRootPass(QString pass)
{
    rootPass = pass;
}

int Settings::getMark()
{
    return mark;
}

int Settings::getPort()
{
    return port;
}

void Settings::checkResources()
{
    if ( !QDir("Accounts").exists() ) {
        QDir().mkdir("Accounts");
    }

    if ( !QDir("Characters").exists() ) {
        QDir().mkdir("Characters");
    }

    if ( !QFile(QDir::currentPath() + "/AccountsList.txt").exists() ) {
        QFile file(QDir::currentPath() + "/AccountsList.txt");
        file.open(QIODevice::WriteOnly);
        file.close();
    }

    if ( !QFile(QDir::currentPath() + "/BanList.txt").exists() ) {
        QFile file(QDir::currentPath() + "/BanList.txt");
        file.open(QIODevice::WriteOnly);
        file.close();
    }
}


