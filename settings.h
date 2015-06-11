#ifndef SETTINGS_H
#define SETTINGS_H

#include <qfile.h>
#include <qtextstream.h>
#include <QDebug>
#include <QDir>

class Settings
{
public:
    Settings();
    ~Settings();

public:
    int mark;
    int port;
    QString rootLog;
    QString rootPass;

public:
    void setPort(int _port);
    void setMark(int _mark);
    void setRootLogin(QString login);
    void setRootPass(QString password);

    int getMark();
    int getPort();

private:
    void checkResources();
};

#endif // SETTINGS_H
