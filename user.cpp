#include "user.h"

User::User()
{

}

User::User(std::string log, std::string pass, std::string ip, unsigned short port)
{
    qDebug() << "Create user";
//    qDebug() << QString::fromStdString(log);
    QFile file(QDir::currentPath() + "/Accounts/" + QString::fromStdString(log) + ".txt");

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

void User::newCharacter(std::string characterNickname, std::string characterClass)
{
    QString pathToCharacter = QDir::currentPath() + "/Characters/" + QString::fromStdString(userLog) + "_" + QString::fromStdString(characterNickname) + ".txt";
    QString pathToAccount = QDir::currentPath() + "/Accounts/" + QString::fromStdString(userLog) + ".txt";
    QFile character(pathToCharacter);
    if ( !character.open(QIODevice::Append) ) {
        qDebug() << "Character create error";
    }
    character.write("100");
    character.write("100");
    character.write("12");
    character.write("4");
    character.close();

    QFile account(pathToAccount);
    if ( !account.open(QIODevice::Append) ) {
        qDebug() << "Account character append error";
    }
    account.write(QByteArray::fromStdString(characterNickname));
    account.close();
}


void User::chooseCharacter(std::string characterNickname)
{
    player = new Player(characterNickname, userLog);
}

