#include "user.h"

User::User()
{

}

User::User(QSqlDatabase *_charactersDB, int ID, std::string ip, unsigned short port)
{
    charactersDB = _charactersDB;

    qDebug() << "Loading user " << QString::number(ID) << "...";

    userID = ID;
    userIP = ip;
    userPort = port;

    //--TODO Add new Character, load character nickname and classID, while ( hasNext() ) { add Character to characterList }

    if ( !charactersDB->open() ) {
        qDebug() << "[U:U]CharactersDB open error: " << charactersDB->lastError().text();
    } else {
        QSqlQuery QLoadCharacters(*charactersDB);

        QLoadCharacters.prepare("SELECT * FROM CharacterList WHERE ID=:ID");
        QLoadCharacters.bindValue(":ID", QString::number(ID));

        if ( !QLoadCharacters.exec() ) {
            qDebug() << "Load characters error: " << QLoadCharacters.lastError().text();
        } else {
            while ( QLoadCharacters.next() ) {
                Character *character = new Character;
                character->Nickname = QLoadCharacters.value(Characters::Nickname).toString();
                character->ClassId  = QLoadCharacters.value(Characters::ClassID).toUInt();
                qDebug() << "Character ID: " << character->ClassId;
                qDebug() << "Character nickname: " << character->Nickname;
                characters.append(character);
            }
        }
        charactersDB->close();
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

bool User::newCharacter(std::string nickname, int classID)
{
    if ( !charactersDB->open() ) {
        qDebug() << "CharactersDB open error: " << charactersDB->lastError().text();
        return false;
    }

    QSqlQuery QNewCharacter(*charactersDB);

    QNewCharacter.prepare("INSERT INTO CharacterList(nickname, classID" "VALUES(:nickname, :classID)");
    QNewCharacter.bindValue(":nickname", QString::fromStdString(nickname));
    QNewCharacter.bindValue(":classID", QString::number(classID));

    if ( !QNewCharacter.exec() ) {
        qDebug() << "Create new character error: " << QNewCharacter.lastError().text();
        charactersDB->close();
        return false;
    }
    qDebug() << "Character " << QString::fromStdString(nickname) << " created";
    charactersDB->close();
    return true;
}

void User::deleteCharacter(std::string nickname, unsigned short id)
{
    int i = 0;
    while ( i < characters.size() ) {
        if ( characters.at(i)->Nickname == QString::fromStdString(nickname) && characters.at(i)->ClassId == id ) {
            characters.removeAt(i);
        }
    }
}

void User::chooseCharacter(std::string characterNickname)
{
    std::cout << "[User::chooseCharacter] Char nickname: " << characterNickname << "\n";
    player = new Player(charactersDB, characterNickname);
}

QList<User::Character *> User::getCharacters()
{
    return characters;
//    QList<User::Character*> chars;

//    if ( !charactersDB->open() ) {
//        qDebug() << "[U:gC] CharactersDB open error: " << charactersDB->lastError().text();
//        return characters;
//    }

//    QSqlQuery QGetCharacters(*charactersDB);

//    QGetCharacters.prepare("SELECT * FROM CharacterList WHERE ID=:ID");
//    QGetCharacters.bindValue(":ID", QString::number(ID));

//    if ( !QGetCharacters.exec() ) {
//        qDebug() << "Get characters error: " << QGetCharacters.lastError().text();
//    } else {
//        int i = 0;
//        while ( QGetCharacters.next() ) {
//            chars.at(i)->Nickname = QGetCharacters.value(Characters::Nickname).toString();
//            chars.at(i)->ClassId = QGetCharacters.value(Characters::ClassID).toUInt();
//            i++;
//        }
//        charactersDB->close();
//        return chars;
//    }
}

void User::setStatus(bool _status)
{
    userStatus = _status;
}

