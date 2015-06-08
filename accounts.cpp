#include "accounts.h"

Accounts::Accounts()
{
    QStringList slist;
    QString line;
    QString msgError;

    if ( accountsFile->exists() ) {
        if ( !accountsFile->open(QIODevice::ReadOnly | QIODevice::Text) ) {
            qDebug() << "Can`t open accountsFile";
            msgError = accountsFile->errorString();
            qDebug() << msgError << " [Error]";
        }

        while ( !accountsFile->atEnd() ) {
            line = accountsFile->readLine();

            line = line.remove(line.size() - 1, 1);
            slist = line.split(" ");

            accLogins.append(slist.at(0));
                accountsFile->close();
        }
    accountsFile->close();
    qDebug() << "Load accounts";
    } else {
        qDebug() << "[Accounts::Accounts] File not found";
    }
}

Accounts::~Accounts()
{

}

void Accounts::addUser(std::string log, std::string pass, std::string ip, unsigned short port)
{
    User *user = new User(log, pass, ip, port);
//    qDebug() << "[Accounts::addUser] Login: " << QString::fromStdString(log) << " Pass: " << QString::fromStdString(pass);
    user->userID = users.size();
    users.append(user);
}

bool Accounts::newUser(std::string log, std::string pass)
{
    if ( accountsFile->exists() ) {
        if ( !accountsFile->open(QIODevice::Append | QIODevice::Text) ) {
            qDebug() << "AccountsList file not open";
            return false;
        } else {
            std::string user;
            user = log + " " + pass + "\n";
            accountsFile->write(QByteArray::fromStdString(user));
            accountsFile->close();
        }
    } else {
        qDebug() << "AccountsList file not found";
    }

    QString pathToCharacter = QDir::currentPath() + "/Accounts/" + QString::fromStdString(log) + ".txt";
    QFile file(pathToCharacter);

    if ( !file.open(QIODevice::WriteOnly) ) {
        qDebug() << "Error create new user accout file";
        return false;
    }
    file.close();
    return true;
}

void Accounts::showUsers()
{
    if ( !users.isEmpty() ) {
    QListIterator<User*> it(users);
    User *user;

        while ( it.hasNext() ) {
            user = it.next();
            std::cout << "UserID: " << user->userID << " Log: "<< user->userLog << " Pass: " << user->userPass << std::endl;
        }
    } else {
        qDebug() << "Users is emp";
    }
}

QList<User::Character> Accounts::getCharacters(std::string ip, unsigned short port)
{
    QList<User::Character> characters;

    if ( !users.isEmpty() ) {

    QList<User*>::iterator it = users.begin();

    qDebug() << "Enter getUser()";

        while ( it != users.end() ) {
//            qDebug() << "Enter while";
            if ( ip != (*it)->userIP && port != (*it)->userPort ) {

            } else {
                characters = (*it)->characters;
            }
            it++;
        }

        if ( characters.isEmpty() ) {
            qDebug() << "NickList is empty";
        }
    }
    return characters;
}

bool Accounts::createCharacter(std::string ip, unsigned short port, std::string characterNickname, int ClassId)
{
    qDebug() << "Create new character" << QString::fromStdString(characterNickname);
    User::Character charact;
    charact.Nickname = QString::fromStdString(characterNickname);
    charact.ClassId = ClassId;

    QList<User*>::iterator it = users.begin();
    while ( it != users.end() ) {
//        qDebug() << "Enter while";
        if ( (*it)->userIP == ip && (*it)->userPort == port ) {
            (*it)->newCharacter(characterNickname, ClassId);
            (*it)->characters.append(charact);
            return true;
        }
        it++;
    }
    return false;
}

bool Accounts::check(std::string _log, std::string _pass)
{

    QStringList slist;
    QString line;
    QString msgError;

    if ( accountsFile->exists() ) {
        if ( !accountsFile->open(QIODevice::ReadOnly | QIODevice::Text) ) {
            qDebug() << "Can`t open accountsFile";
            msgError = accountsFile->errorString();
            qDebug() << msgError << " [Error]";
        }

        while ( !accountsFile->atEnd() ) {
            line = accountsFile->readLine();

            line = line.remove(line.size() - 1, 1);
            slist = line.split(" ");

            if ( _log == slist.at(0).toStdString() && _pass == slist.at(1).toStdString() ) {
                QListIterator<User*> it(users);   //--TODO uncomment

                while ( it.hasNext() ) {
                    if ( _log == it.next()->userLog ) {
                        accountsFile->close();
                        return false;
                    }
                }
                accountsFile->close();
                return true;

            } else {
                //--TODO send error log\pass
            }
        }

    } else {
        qDebug() << "File not found";
    }
    accountsFile->close();
    return false;
}
