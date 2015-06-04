#include "queue.h"

Queue::Queue()
{
    qDebug() << "Create queue";
}

Queue::~Queue()
{

}

void Queue::addTask(std::string _sender, unsigned short _port, sf::Packet _packet)
{
    Task *task = new Task;

    task->sender = _sender;
    task->port = _port;
    task->packet = _packet;

    Tasks.enqueue(*task);
    qDebug() << Tasks.size() << " size tasks";
    qDebug() << QString::fromStdString(_sender) << ":" << _port;
    delete task;
}

Task Queue::getTask()
{
    return Tasks.dequeue();
}


