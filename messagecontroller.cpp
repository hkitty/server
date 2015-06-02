#include "messagecontroller.h"

MessageController::MessageController()
    : TStartReceiver(&MessageController::receiver, this), TStartParser(&MessageController::startParser, this)
{
    qDebug() << "Create messageController";
}

MessageController::~MessageController()
{

}

void MessageController::receiver() {
    if (socket.bind(port) != sf::Socket::Done){
        throw std::runtime_error("Failed to bind");
    }

    TStartParser.launch();
    //socket.setBlocking(false);

    while ( isStarted ) {
        if(socket.receive(packet, sender, port) == sf::Socket::Done) {
            qDebug() << QString::fromStdString(sender.toString()) << " user connected";
            if ( !banlist->bans.contains(sender.toString()) ) {
//            qDebug() << "[Enter packet. Add to queue]";
            queue->addTask(sender.toString(), port, packet);
            }

//            data.clear();
            packet.clear();
        }
    }
}

void MessageController::startReceiver(int _port) {
    port = _port;
    isStarted = true;
    TStartReceiver.launch();
}

void MessageController::stop() {
    isStarted = false;
    socket.unbind();
}

void MessageController::startParser()
{
//    int i = 0;
    qDebug() << "start parser";
    while ( isStarted ) { //--true
        if (!queue->Tasks.isEmpty() ) {
//            if ( i != 1) {
//            size = queue->Tasks.size();
//            i++;
//            }
//            qDebug() << size;
            qDebug() << "Send Task to Parser";
        parser->receive(queue->getTask());
        }
    }
}
