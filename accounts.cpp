#include "accounts.h"

Accounts::Accounts()
{
    qDebug() << "Create accounts";
//    qDebug() << "Load Accounts...";
//    QString msgError;
//    QString line;
//    QStringList slist;
//    std::string log, pass;
//    if ( accountsFile->exists() ) {
//        if ( !accountsFile->open(QIODevice::ReadOnly | QIODevice::Text) ) {
//            qDebug() << "Can`t open accountsFile";
//            msgError = accountsFile->errorString();
//            qDebug() << msgError << " [Error]";
//        }


//        while ( !accountsFile->atEnd() ) {
//            User *user = new User;

//            line = accountsFile->readLine();
//            slist = line.split(" ");

//            log = slist.at(0).toStdString();
//            //qDebug() << "userLog";
//            pass = slist.at(1).toStdString();
//            pass = pass.erase(pass.size() - 1);
//            user->userLogin = log;
//            user->userPassword = pass;
//            //qDebug() << "userPass";
//            //std::cout << "Log: " << log << " Pass: " << pass << std::endl;
//            //qDebug() << "Log: " << QString::fromStdString(user->userLogin) << "Pass: " << QString::fromStdString(user->userPassword);
//            users.append(*user);
//            delete user;
//            //qDebug() << users.size() << " [Users size]";
//        }

//        accountsFile->close();
//    } else {
//        qDebug() << "File not found";
//    }
}

Accounts::~Accounts()
{

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
    User *user;// = new User(NULL, NULL, NULL, NULL);

        while ( it.hasNext() ) {
            *user = it.next();
            std::cout << "Log: "<< user->userLog << " Pass: " << user->userPass << std::endl;
            //qDebug() << "Login: " << QString::fromStdString(it.next().userLogin) << " Pass: " << QString::fromStdString(it.next().userPassword);
        }
    }
}

QList<QString> Accounts::getUser(std::string IP)
{
    QList<User>::iterator it = users.begin();

    qDebug() << "Enter getUser()";

    while ( it != users.end() ) {
        qDebug() << "Enter while";
        if ( IP == (*it).userIP ) {
            return (*it).nickList;
        }
        it++;
    }
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
                User *user = new User(slist.at(0).toStdString(), slist.at(1).toStdString(), IP, port);

                users.append(*user);

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

