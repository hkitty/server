#include "accounts.h"

Accounts::Accounts()
{
    qDebug() << "Create accounts";
}

Accounts::~Accounts()
{

}

void Accounts::addUser(std::string log, std::string pass, std::string ip, unsigned short port)
{
    User user(log, pass, ip, port);
    users.append(user);
}

void Accounts::newUser(std::string log, std::string pass)
{
    QFile accFile;

    if ( accFile.exists() ) {
        if ( !accFile.open(QIODevice::Append | QIODevice::Text) ) {
            qDebug() << "AccFile n o";
        }
        std::string user;
        user = log + " " + pass + "\n";
        accFile.write(QByteArray::fromStdString(user));
        accFile.close();

    } else {
        qDebug() << "AccFile n f";
    }
}

void Accounts::showUsers()
{
    if ( !users.isEmpty() ) {
    QListIterator<User> it(users);
    User user;// = new User(NULL, NULL, NULL, NULL);

        while ( it.hasNext() ) {
            user = it.next();
            std::cout << "Log: "<< user.userLog << " Pass: " << user.userPass << std::endl;
            //qDebug() << "Login: " << QString::fromStdString(it.next().userLogin) << " Pass: " << QString::fromStdString(it.next().userPassword);
        }
    } else {
        qDebug() << "Users is emp";
    }
}

QList<QString> Accounts::getCharacters(std::string IP)
{
    QList<QString> nicklist;

    if ( !users.isEmpty() ) {

    QList<User>::iterator it = users.begin();

    qDebug() << "Enter getUser()";

        while ( it != users.end() ) {
            qDebug() << "Enter while";
            if ( IP == (*it).userIP ) {
                nicklist = (*it).nickList;
            }
            it++;
        }

        if ( nicklist.isEmpty() ) {
            qDebug() << "NickList n f";
        }
    }
    return nicklist;
}

bool Accounts::check(std::string _log, std::string _pass, std::string IP, unsigned short port)
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
            //qDebug() << slist.at(0) << " [sl 0]";
            //qDebug() << slist.at(1) << " [sl 1]";


            if ( _log == slist.at(0).toStdString() && _pass == slist.at(1).toStdString() ) {
//                User user(slist.at(0).toStdString(), slist.at(1).toStdString(), IP, port);

//                users.append(user);

                accountsFile->close();
                return true;
            } else {
                //--TODO send error log\pass
            }
        }

    }
    else {
        qDebug() << "File not found";
    }
    accountsFile->close();
    return false;
}

