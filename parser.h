#ifndef PARSER_H
#define PARSER_H
#include <SFML/Network.hpp>
#include <QDebug>
#include <task.h>
#include <unistd.h>
#include <accounts.h>

class Parser
{
public:
    Parser();
    ~Parser();

private:
    int mark = 1337;
    int i = 0;

public:
    void receive(Task task);

public:
    Accounts *accounts = new Accounts;

private:
    enum Commands {
        Login,
        GetUserCharacters,
        Registration,
        NewCharacter
    };

};

#endif // PARSER_H
