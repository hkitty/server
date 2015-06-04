#include "user.h"

User::User()
{

}

User::User(std::string log, std::string pass, std::string ip, unsigned short port)
{
    qDebug() << "Create user";
//    qDebug() << QString::fromStdString(log);
    QFile file(QDir::currentPath() + "/Accounts/" + QString::fromStdString(log) + ".txt");
    Character character;

    QString line;
    QString crutch;
    QStringList charac;

    if ( file.exists() ) {
        if ( !file.open(QIODevice::ReadOnly) ) {
            qDebug() << "F N O [users]";
        }

        while ( !file.atEnd() ) {
            line = file.readLine();
            line.remove(line.size() - 1, 1);
            qDebug() << "[User::User] " << "Line: " << line;
            charac = line.split(" ");
            character.Nickname = charac.at(0);
            qDebug() << "[User::User] " << "charac.at(0): " << charac.at(0);
            qDebug() << "[User::User] " << "charac.at(1): " << charac.at(1);
            crutch = charac.at(1);
            qDebug() << "[User::User] " << "Crutch: " << crutch;
            character.ClassId = crutch.toUShort();
            qDebug() << "[User::User] " << "character.Nickname: " << character.Nickname;
            qDebug() << "[User::User] " << "character.ClassId: " << character.ClassId;

            characters.append(character);
        }

    } else {
        qDebug() << " F N F [users]";
    }

    file.close();

    userLog = log;
    userPass = pass;
    userIP = ip;
    userPort = port;

    QListIterator<Character> it(characters);

    while ( it.hasNext() ) {
        nickList.append(it.next().Nickname);
    }
}

User::~User()
{

}

int User::getStatus()
{
    return userStatus;
}

void User::newCharacter(std::string characterNickname, unsigned short characterClass)
{
    QString pathToCharacter = QDir::currentPath() + "/Characters/" + QString::fromStdString(userLog) + "_" + QString::fromStdString(characterNickname) + ".txt";
    QString pathToAccount = QDir::currentPath() + "/Accounts/" + QString::fromStdString(userLog) + ".txt";
    QFile character(pathToCharacter);
    if ( !character.exists() ) {
        if ( !character.open(QIODevice::Append | QIODevice::Text) ) {
            qDebug() << "Character create error";
        }
        character.write(characterClass + "\n");
        character.write("100\n");
        character.write("100\n");
        character.write("12\n");
        character.write("4");
        character.close();

        QFile account(pathToAccount);
        if ( !account.open(QIODevice::Append | QIODevice::Text) ) {
            qDebug() << "Account character append error";
        }
        account.write(QByteArray::fromStdString(characterNickname) + " " + characterClass);
        account.close();
    } else {
        qDebug() << "Nickname used";
    }
}


void User::chooseCharacter(std::string characterNickname)
{
    player = new Player(characterNickname, userLog);
}

