#include "user.h"

User::User(std::string log, std::string pass, std::string ip, unsigned short port)
{
    qDebug() << "Create user";
    QFile file(QString::fromStdString(log));

    QString nick;

    if ( file.exists() ) {
        if ( !file.open(QIODevice::ReadOnly) ) {
            qDebug() << "F N O [users]";
        }

        while ( !file.atEnd() ) {
            nick = file.readLine();
            nick.remove(nick.size() - 1, 1);

            nickList.append(nick);
        }

    } else {
        qDebug() << " F N F [users]";
    }

    file.close();

    userLog = log;
    userPass = pass;
    userIP = ip;
    userPort = port;

    QListIterator<QString> it(nickList);

    while ( it.hasNext() ) {
        qDebug() << it.next();
    }
}

User::~User()
{

}

int User::getStatus()
{
    return userStatus;
}

void User::chooseCharacter(std::string characterNickname)
{
    player = new Player(characterNickname, userLog);
}

