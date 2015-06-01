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

public:
    int mark = 1337;
    int i = 0;

public:
    void receive(Task task);

    Accounts *accounts = new Accounts;

};

#endif // PARSER_H
