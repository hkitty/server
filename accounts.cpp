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

bool Accounts::addUser(int ID, std::string ip, unsigned short port)
{
    if ( ID < 0 ) {
        qDebug() << "Add user error: user with ID " << ID << " not found";
        return false;
    } else {
        User *user = new User(&charactersDB, ID, ip, port);
        users.append(user);
        return true;
    }
}

bool Accounts::newUser(std::string log, std::string pass, std::string ip, unsigned short port)
{

    QSqlQuery QNewUser(accountsDB);
    QNewUser.prepare("INSERT INTO Users(login, password, lastIP, lastPort)"
                       "VALUES(:login, :password, :lastIP, :lastPort)");
    QNewUser.bindValue(":login", QString::fromStdString(log));
    QNewUser.bindValue(":password", QString::fromStdString(pass));
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

//    if ( !users.isEmpty() ) {
//    QListIterator<User*> it(users);
//    User *user;

//        while ( it.hasNext() ) {
//            user = it.next();
//            std::cout << "UserID: " << user->userID << " Log: "<< user->userLog << " Pass: " << user->userPass << std::endl;
//        }
//    } else {
//        qDebug() << "Users is emp";
//    }
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
//    int ID = getUserID(ip, port);
//    users.at(ID)->getCharacters();


//    QList<User::Character*> characters;
//    int id = getUserID(ip, port);
//    QSqlQuery QGetCharacters(accountsDB);

//    QGetCharacters.prepare("SELECT * FROM Characters WHERE id=:id");
//    QGetCharacters.bindValue(":id", QString::number(id));

//    if ( !QGetCharacters.exec() ) {
//        qDebug() << "Get characters error: " << QGetCharacters.lastError().text();
//    } else {
//        int i = 0;
//        while ( QGetCharacters.next() ) {
//            characters.at(i)->Nickname = QGetCharacters.value(Characters::Nickname).toString();
//            characters.at(i)->ClassId = QGetCharacters.value(Characters::ClassID).toUInt();
//            i++;
//        }
//        return characters;
//    }

//    return characters;
}

bool Accounts::createCharacter(std::string ip, unsigned short port, std::string characterNickname, int classID)
{
    int id = getUserID(ip, port);

    if ( id < 0 ) {
        qDebug() << "Create character error: User not found";
    } else {
        QSqlQuery QCreateCharacter(accountsDB);

        QCreateCharacter.prepare("INSERT INTO Characters(ID, nickname, classID" "VALUES(:ID, :nickname, :classID)");
        QCreateCharacter.bindValue(":ID", QString::number(id));
        QCreateCharacter.bindValue(":nickname", QString::fromStdString(characterNickname));
        QCreateCharacter.bindValue(":classID", QString::number(classID));

        if ( !QCreateCharacter.exec() ) {
            qDebug() << "Create character error: " << QCreateCharacter.lastError().text();
            return false;
        } else {
            qDebug() << "Character " << QString::fromStdString(characterNickname) << " created";
            return true;
        }
    }
}

int Accounts::getUserID(std::string ip, unsigned short port)
{

    QList<User*>::iterator it = users.begin();

    while ( it != users.end() ) {
        if ( (*it)->userIP == ip && (*it)->userPort == port ) {
            return (*it)->userID;
        }
        it ++;
    }

    return -1;


//    QSqlQuery QGetUserID(accountsDB);
//    QGetUserID.prepare("SELECT ID FROM Users WHERE ip=:ip");
//    qDebug() << QString::fromStdString(ip) << " connected";
//    QGetUserID.bindValue(":ip", QString::fromStdString(ip));

//    if ( !QGetUserID.exec() ) {
//        qDebug() << "Get userID error: " << QGetUserID.lastError().text();
//        return -4;
//    } else {
//        while ( QGetUserID.next() ) {
//            if ( port == QGetUserID.value(Users::LastPort).toInt() ) {
//                int i = QGetUserID.value(Users::UserID).toInt();
//                qDebug() << "ID= " << QGetUserID.value(Users::UserID).toInt();
//                return i;
//            } else {
//                return -5;
//            }
//        }
//    }
//    qDebug() << "getUserID(std::string, unsigned short) runtime error";
//    return -3;
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
    int i = getUserID(ip, port);

    if ( i >= 0 ) {
    QSqlQuery QSetStatus(accountsDB);

    QSetStatus.prepare("UPDATE Users SET status=:status WHERE id=:id");
    QSetStatus.bindValue(":status", QString::number(status));
    QSetStatus.bindValue(":id", QString::number(i));

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

bool Accounts::setStatus(int id, int status)
{
    if ( id >= 0 ) {
    QSqlQuery QSetStatus(accountsDB);

    QSetStatus.prepare("UPDATE Users SET status=:status WHERE id=:id");
    QSetStatus.bindValue(":status", QString::number(status));
    QSetStatus.bindValue(":id", QString::number(id));

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

int Accounts::check(std::string log, std::string pass)
{ 
    QSqlQuery QCheck(accountsDB);

//    QCheck.prepare("SELECT login(" + QString::fromStdString(log) + ") FROM Users");
    QCheck.prepare("SELECT * FROM Users WHERE login=:login");
    QCheck.bindValue(":login", QString::fromStdString(log));

    if ( !QCheck.exec() ) {
        qDebug() << "Check user error: " << QCheck.lastError().text();
        return -2;
    } else {
        while ( QCheck.next() ) {
            qDebug() << "Check value: " << QCheck.value(Users::Login).toString();
            if ( QString::fromStdString(pass) == QCheck.value(Users::Password).toString() ) {
                int ID = QCheck.value(Users::UserID).toInt();
                qDebug() << "User " << QString::fromStdString(log) << " execute";
                return ID;
            } else {
                qDebug() << "User " << QString::fromStdString(log) << " not found";
                return -3;
            }
        }
    }
    qDebug() << "check(std::string, std::string) runtime error ";
    return -1;
}
