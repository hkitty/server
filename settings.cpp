#include "settings.h"

Settings::Settings()
{
    qDebug() << "Create settings";
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


