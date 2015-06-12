#include "user.h"

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
                qDebug() << "Character classID: " << character->ClassId;
                qDebug() << "Character nickname: " << character->Nickname;
                characters.append(character);
            }
            qDebug() << "Characters size() " << characters.size();
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

bool User::newCharacter(int ID, std::string nickname, int classID)
{
    if ( !charactersDB->open() ) {
        qDebug() << "CharactersDB open error: " << charactersDB->lastError().text();
        return false;
    }

    QSqlQuery QNewCharacter(*charactersDB);

    QNewCharacter.prepare("INSERT INTO CharacterList(ID, nickname, classID" "VALUES(:ID, :nickname, :classID)");
    QNewCharacter.bindValue(":ID", QString::number(ID));
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

bool User::removeCharacter(std::string nickname)
{
    if ( !charactersDB->open() ) {
        qDebug() << "CharactersDB open error: " << charactersDB->lastError().text();
        return false;
    }

    QSqlQuery QRemoveCharacter(*charactersDB);
    QRemoveCharacter.prepare("DELETE * FROM CharacterList WHERE nickname=:nickname");
    QRemoveCharacter.bindValue(":nickname", QString::fromStdString(nickname));

    if ( !QRemoveCharacter.exec() ) {
        qDebug() << "Character remove error" << QRemoveCharacter.lastError().text();
        charactersDB->close();
        return false;
    }
    charactersDB->close();
    return true;
}

void User::chooseCharacter(std::string nickname)
{
    std::cout << "[User::chooseCharacter] Char nickname: " << nickname << "\n";
    player = new Player(charactersDB, nickname);
}

QList<User::Character *> User::getCharacters()
{
    QList<User::Character* >::iterator it = characters.begin();

    while ( it != characters.end() ) {
        qDebug() << "it: " << *it;
        qDebug() << "ID: " << (*it)->ClassId;
        qDebug() << "nickname: " << (*it)->Nickname;
    it++;
    }
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

