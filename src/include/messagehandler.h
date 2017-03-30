#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H 1
#include "connection.h"
#include <string>
#include "protocol.h"

class messagehandler {
public:
    messagehandler(Connection &connect) : conn(connect) {}

    int receiveInt();
    void sendString(std::string str);
    void sendInt(int value) ;
    void sendCode(int Code);
    std::string receiveString();
    void sendByte(int code);
    int receiveCode();
    void sendIntParameter(int param);
    int receiveIntParameter();

private:
    Connection &conn;



};
#endif
