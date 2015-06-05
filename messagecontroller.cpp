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

    while ( isStarted ) {
        if(socket.receive(packet, sender, port) == sf::Socket::Done) {
            qDebug() << QString::fromStdString(sender.toString()) << " connected";
            if ( !banlist->bans.contains(sender.toString()) ) {
                queue->addTask(sender.toString(), port, packet);
            }

            packet.clear();
        }
    }
    qDebug() << "While exit";
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
    qDebug() << "start parser";
    while ( isStarted ) { //--true
//        qDebug() << "Parser while start";
        if (!queue->Tasks.isEmpty() ) {
            qDebug() << "";
            parser->receive(queue->getTask());
//            qDebug() << "";
        }
    }
}
