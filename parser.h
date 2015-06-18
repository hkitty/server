#ifndef PARSER_H
#define PARSER_H
#include <SFML/Network.hpp>
#include <QDebug>
#include <task.h>
#include <unistd.h>
#include <accounts.h>
#include <menuLogic.h>
#include <gameLogic.h>

class Parser
{
public:
    Parser();
    ~Parser();

public:
    int i = 0;
    unsigned short const mark = 1337;

    Accounts *accounts = new Accounts;

public:
    void receive(Task task);
    void kickUser(unsigned short userID);

private:
    enum Commands {
        Login,
        GetUserCharacters,
        Registration,
        NewCharacter,
        ChooseCharacter,
        DeleteCharacter,
        InGame,
        GetEnemys,
        PlayerMove,
        ExitGame,
        PlayerAttack,
        GetStats
    };

    MenuLogic *menuLogic = new MenuLogic(accounts);
    GameLogic *gameLogic = new GameLogic(accounts);
};

#endif // PARSER_H
