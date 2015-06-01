#include <QCoreApplication>

#include <QDebug>
#include <SFML/Network.hpp>
#include <iostream>
#include "adminInterface.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    AdminInterface admInterface;

//    admInterface.authentication();

    sf::Thread TAdmInterface(&AdminInterface::authentication, &admInterface);
    TAdmInterface.launch();

    return a.exec();
}
