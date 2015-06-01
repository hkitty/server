#ifndef TASK
#define TASK
struct Task {
    std::string sender;
    unsigned short port;
    sf::Packet packet;
};
#endif // TASK

