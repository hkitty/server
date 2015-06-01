#ifndef QUEUE_H
#define QUEUE_H
#include <SFML/Network.hpp>
#include <QQueue>
#include <QDebug>
#include <task.h>

class Queue
{
public:
    Queue();
    ~Queue();

public:


    QQueue<Task> Tasks;

public:
    void addTask(std::string _sender, unsigned short _port, sf::Packet _packet);
    Task getTask();
};

#endif // QUEUE_H
