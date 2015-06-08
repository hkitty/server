#include "user.h"

User::User()
{

}

User::User(std::string log, std::string pass, std::string ip, unsigned short port)
{
    qDebug() << "Load user: " << QString::fromStdString(log);
    QFile file(QDir::currentPath() + "/Accounts/" + QString::fromStdString(log) + ".txt");

    QString line;
    QString crutch;
    QStringList charac;
    Character character;

    if ( file.exists() ) {
        if ( !file.open(QIODevice::ReadOnly) ) {
            qDebug() << "F N O [users]";
        }

        while ( !file.atEnd() ) {


            line = file.readLine();
            line.remove(line.size() - 1, 1);
            charac = line.split(" ");
            character.Nickname = charac.at(0);
            crutch = charac.at(1);
            character.ClassId = crutch.toUShort();

            characters.append(character);
        }

    } else {
        qDebug() << "User file not found";
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
    if ( player != nullptr) {
        delete player;
    }
}

int User::getStatus()
{
    return userStatus;
}

void User::deletePlayer()
{
    delete player;
}

void User::newCharacter(std::string characterNickname, int characterClass)
{
    qDebug() << "Character class: " << characterClass;
    QString pathToCharacter = QDir::currentPath() + "/Characters/" + QString::fromStdString(userLog) + "_" + QString::fromStdString(characterNickname) + ".txt";
    QString pathToAccount = QDir::currentPath() + "/Accounts/" + QString::fromStdString(userLog) + ".txt";


    QFile character(pathToCharacter);
    if ( !character.exists() ) {
        qDebug() << "File not found [OK]";
        if ( !character.open(QIODevice::WriteOnly)) {
                qDebug() << "Character create error";
            }
            QTextStream out(&character);

            out << QString::number(characterClass);
            out.flush();
            out << "\n100";
            out.flush();
            out << "\n100";
            out.flush();
            out << "\n12";
            out.flush();
            out << "\n4";
            out.flush();
            out << "\n100";
            out.flush();
            out << "\n100";
            out.flush();

            character.close();
    } else {
        qDebug() << "Nickname used";
    }

    QFile account(pathToAccount);
    if ( !account.open(QIODevice::Append) ) {
        qDebug() << "Account character append error";
    }

    QTextStream outAcc(&account);
    outAcc << QByteArray::fromStdString(characterNickname) + " " + QByteArray::number(characterClass);

    account.close();
}


void User::chooseCharacter(std::string characterNickname)
{
    std::cout << "[User::chooseCharacter] Char nickname: " << characterNickname << "\n";
    player = new Player(characterNickname, userLog);
}

void User::setStatus(bool _status)
{
    userStatus = _status;
}

