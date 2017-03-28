#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H 1
#include "connection.h"
#include <string>
#include "protocol.h"

class messagehandler {
public:
    messagehandler(Connection &connect) : conn(connect) {}

    int readNumber();
    void sendString(std::string str);
    void sendInt(int value) ;
    void sendCode(int Code);
    std::string receiveString();
    void sendByte(int code);

private:
    Connection &conn;



};
#endif
