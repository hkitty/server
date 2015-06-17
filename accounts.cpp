#include "accounts.h"

Accounts::Accounts()
{
    accountsDB = QSqlDatabase::addDatabase("QSQLITE", accountsConnection);
    charactersDB = QSqlDatabase::addDatabase("QSQLITE", charactersConnection);

    accountsDB.setDatabaseName("Accounts.sqlite");
    charactersDB.setDatabaseName("Characters.sqlite");

    if ( !accountsDB.open() ) {
        qDebug() << "[Accounts] Database connection error";
        qDebug() << accountsDB.lastError().text();
    } else {
        qDebug() << "Connected";
    }

    if ( !charactersDB.open() ) {
        qDebug() << "CharactersDB open error: " << charactersDB.lastError().text();
    }

    banlist = new BanList(&accountsDB);
}

Accounts::~Accounts()
{
    accountsDB.close();
    charactersDB.close();
    accountsDB.removeDatabase(accountsConnection);
    charactersDB.removeDatabase(charactersConnection);
    accountsDB.~QSqlDatabase();
    charactersDB.~QSqlDatabase();
}

bool Accounts::isOnline(int ID, std::string ip, unsigned short port)
{
    int position = getUserListPosition(ip, port);

    if ( position < 0 ) {
        qDebug() << "User is offline";
        return false;
    }

    return true;
}

bool Accounts::addUser(int ID, std::string ip, unsigned short port)
{
    if ( ID < 0 ) {
        qDebug() << "Add user error: user with ID " << ID << " not found";
        return false;
    } else {
        qDebug() << "[A:aU]ID: " << ID;
        qDebug() << "[A:aU]ip: " << QString::fromStdString(ip);
        qDebug() << "[A:aU]port: " << port;
        User *user = new User(&charactersDB, ID, ip, port);
        users.append(user);
        return true;
    }
}

bool Accounts::disconnectUser(int ID)
{
    int position = getUserListPosition(ID);
    if ( position < 0 ) {
        qDebug() << "User not found";
        return false;
    }
//    users.removeAt(position);
    users.at(position)->deletePlayer();
    qDebug() << "PL DELETED";
    delete users.at(position);
    qDebug() << "USER DELETED";
    users.removeAt(position);
    setStatus(ID, 0);

    return true;
}

bool Accounts::newUser(std::string login, std::string password, std::string ip, unsigned short port)
{

    QSqlQuery QNewUser(accountsDB);
    QNewUser.prepare("INSERT INTO Users(login, password, lastIP, lastPort)"
                       "VALUES(:login, :password, :lastIP, :lastPort)");
    QNewUser.bindValue(":login", QString::fromStdString(login));
    QNewUser.bindValue(":password", QString::fromStdString(password));
    QNewUser.bindValue(":lastIP", QString::fromStdString(ip));
    QNewUser.bindValue(":lastPort", QString::number(port));

    if ( !QNewUser.exec() ) {
        qDebug() << "QNewUser error: " << QNewUser.lastError().text();
        return false;
    }

    return true;
}

void Accounts::showUsers()
{
    //--TODO select * from Users Where status==1


//    QSqlQuery QShowOnline(*accountsDB);

    //QShowOnline.prepare("SELECT * FROM );

    if ( !users.isEmpty() ) {
    QList<User*>::iterator it = users.begin();

        while ( it != users.end() ) {
//            std::cout << "Pos: " << it << "ID: " << (*it)->userID << "Nickname: " << (*it)->player->nickname;
        it++;
        }
    }
}

void Accounts::showAllUsers()
{
    QSqlQuery QShowAllUsers(accountsDB);

    QShowAllUsers.prepare("SELECT * FROM Users");

    if ( !QShowAllUsers.exec() ) {
        qDebug() << "Show all users error: " << QShowAllUsers.lastError().text();
    } else {
        qDebug() << "ID  Login    Password   AccRghts   LastIP        LastPort   Warnings";
        while ( QShowAllUsers.next() ) {
            qDebug() << QShowAllUsers.value(Users::UserID).toInt() << " " << QShowAllUsers.value(Users::Login).toString() << " "
                     << QShowAllUsers.value(Users::Password).toString() << "   " << QShowAllUsers.value(Users::AccessRights).toInt() << "        "
                      << QShowAllUsers.value(Users::LastIP).toString() << " " << QShowAllUsers.value(Users::LastPort).toInt() << "     "
                      << QShowAllUsers.value(Users::Warnings).toInt();
            qDebug() << "---------------------------------------------------------------------";
//            qDebug() << QShowAllUsers.value(Users::Login).toString();
//            qDebug() << QShowAllUsers.value(Users::Password).toString();
//            qDebug() << QShowAllUsers.value(Users::AccessRights).toInt();
//            qDebug() << QShowAllUsers.value(Users::Status).toInt();
//            qDebug() << QShowAllUsers.value(Users::LastIP).toString();
//            qDebug() << QShowAllUsers.value(Users::LastPort).toInt();
//            qDebug() << QShowAllUsers.value(Users::Warnings).toInt();
        }
    }
}

QList<User::Character*> Accounts::getCharacters(std::string ip, unsigned short port)
{

}

bool Accounts::createCharacter(std::string ip, unsigned short port, std::string nickname, int classID)
{
    int ID = getUserID(ip, port);
    qDebug() << "[A::cC]User ID: " << ID;

    if ( ID < 0 ) {
        qDebug() << "Create character error: User not found";
    } else {
        if ( !charactersDB.open() ) {
            qDebug() << "[A:cC]CharactersDB open error: " << charactersDB.lastError().text();
            return false;
        }

        QSqlQuery QCreateCharacter(charactersDB);

        QCreateCharacter.prepare("INSERT INTO CharacterList(ID, nickname, classID)"
                                 "VALUES(:ID, :nickname, :classID)");
        QCreateCharacter.bindValue(":ID", QString::number(ID));
        QCreateCharacter.bindValue(":nickname", QString::fromStdString(nickname));
        QCreateCharacter.bindValue(":classID", QString::number(classID));

        if ( !QCreateCharacter.exec() ) {
            qDebug() << "Create character error: " << QCreateCharacter.lastError().text();
//            charactersDB.close();
            return false;
        } else {
            qDebug() << "Character " << QString::fromStdString(nickname) << " created";
//            charactersDB.close();
            return true;
        }
    }
}

int Accounts::getUserID(std::string ip, unsigned short port)
{
    int position = getUserListPosition(ip, port);
    if ( position < 0 ) {
        qDebug() << "[A:gUID] Error";
        return -1;
    }
    int ID = users.at(position)->userID;
    return ID;
}

int Accounts::getUserListPosition(std::string ip, unsigned short port)
{
    if ( !users.isEmpty() ) {
        QList<User*>::iterator it = users.begin();
        qDebug() << "USER IP " << QString::fromStdString(ip);
        qDebug() << port;
        while ( it != users.end() ) {
            if ( (*it)->userIP == ip && (*it)->userPort == port ) {
                int index = it - users.begin();
                qDebug() << "[ip:port]Return position: " << index;
                return index;
            }
        it ++;
        }
    } else {
    qDebug() << "[ip:port]Get user list position error";
    return -1;
    }
}

int Accounts::getUserListPosition(int ID)
{
    if ( !users.isEmpty() ) {
        QList<User*>::iterator it = users.begin();

        while ( it != users.end() ) {
            if ( (*it)->userID == ID ) {
                int index = it - users.begin();
                qDebug() << "[ID]Return position: " << index;
                return index;
            }
        it++;
        }
    } else {
    qDebug() << "[ID]Get user list position error";
    return -1;
    }
}

bool Accounts::setStatus(std::string login, int status)
{
    QSqlQuery QSetStatus(accountsDB);

    QSetStatus.prepare("UPDATE Users SET status=:status WHERE login=:login");
    QSetStatus.bindValue(":status", QString::number(status));
    QSetStatus.bindValue(":login", QString::fromStdString(login));

    if ( !QSetStatus.exec() ) {
        qDebug() << "Status update error: " << QSetStatus.lastError().text();
        return false;
    } else {
        qDebug() << "Status update ok: ";
        return true;
    }
    qDebug() << "setStatus(std::string, int) runtime error ";
    return -2;
}

bool Accounts::setStatus(std::string ip, unsigned short port, int status)
{
    int ID = getUserID(ip, port);

    if ( ID >= 0 ) {
    QSqlQuery QSetStatus(accountsDB);

    QSetStatus.prepare("UPDATE Users SET status=:status WHERE ID=:ID");
    QSetStatus.bindValue(":status", QString::number(status));
    QSetStatus.bindValue(":ID", QString::number(ID));

        if ( !QSetStatus.exec() ) {
            qDebug() << "Status update error: " << QSetStatus.lastError().text();
            return false;
        } else {
            qDebug() << "Status update ok: ";
            return true;
        }
    }
    qDebug() << "setStatus(std::string, unsigned short, int) runtime error ";
    return -2;
}

bool Accounts::setStatus(int ID, int status)
{
    if ( ID >= 0 ) {
    QSqlQuery QSetStatus(accountsDB);

    QSetStatus.prepare("UPDATE Users SET status=:status WHERE ID=:ID");
    QSetStatus.bindValue(":status", QString::number(status));
    QSetStatus.bindValue(":ID", QString::number(ID));

        if ( !QSetStatus.exec() ) {
            qDebug() << "Status update error: " << QSetStatus.lastError().text();
            return false;
        } else {
            qDebug() << "Status update ok: ";
            return true;
        }
    }
    qDebug() << "setStatus(int, int) runtime error ";
    return -2;
}

int Accounts::check(std::string login, std::string password)
{ 
    QSqlQuery QCheck(accountsDB);

//    QCheck.prepare("SELECT login(" + QString::fromStdString(log) + ") FROM Users");
    QCheck.prepare("SELECT * FROM Users WHERE login=:login");
    QCheck.bindValue(":login", QString::fromStdString(login));

    if ( !QCheck.exec() ) {
        qDebug() << "Check user error: " << QCheck.lastError().text();
        return -2;
    } else {
        while ( QCheck.next() ) {
            qDebug() << "Check value: " << QCheck.value(Users::Login).toString();
            if ( QString::fromStdString(password) == QCheck.value(Users::Password).toString() ) {
                int ID = QCheck.value(Users::UserID).toInt();
                qDebug() << "User " << QString::fromStdString(login) << " execute";
                return ID;
            } else {
                qDebug() << "User " << QString::fromStdString(login) << " not found";
                return -3;
            }
        }
    }
    qDebug() << "check(std::string, std::string) runtime error ";
    return -1;
}
