#include "queue.h"

Queue::Queue()
{
//    qDebug() << "Create queue";
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
    delete task;
}

Task Queue::getTask()
{
    if ( !Tasks.isEmpty() ) {
        return Tasks.dequeue();
    }
}


