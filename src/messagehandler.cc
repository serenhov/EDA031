#include "messagehandler.h"
#include <iostream>

using namespace std;


int messagehandler::receiveInt() {

    unsigned char byte1 = conn.read();
    unsigned char byte2 = conn.read();
    unsigned char byte3 = conn.read();
    unsigned char byte4 = conn.read();

    return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}


void messagehandler::sendString(string str) {
    messagehandler::sendCode(Protocol::PAR_STRING);
    messagehandler::sendInt(str.length());
    for (unsigned int i = 0; i < str.length(); ++i) {
        messagehandler::sendByte(str.at(i));
    }
}

void messagehandler::sendCode(int code) {
    conn.write(code);
}

void messagehandler::sendInt(int value) {
    conn.write((value >> 24) & 0xFF);
    conn.write((value >> 16) & 0xFF);
    conn.write((value >> 8) & 0xFF);
    conn.write(value & 0xFF);
}

void messagehandler::sendIntParameter(int param) {
    messagehandler::sendCode(Protocol::PAR_NUM);
    conn.write((param >> 24) & 0xFF);
    conn.write((param >> 16) & 0xFF);
    conn.write((param >> 8) & 0xFF);
    conn.write(param & 0xFF);
}

int messagehandler::receiveIntParameter() {
    if (messagehandler::receiveCode() == Protocol::PAR_NUM) {
        unsigned char byte1 = conn.read();
        unsigned char byte2 = conn.read();
        unsigned char byte3 = conn.read();
        unsigned char byte4 = conn.read();

        return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
    }
    return 0;
}

string messagehandler::receiveString() {
    string s;
    if (messagehandler::receiveCode() == Protocol::PAR_STRING) {
        int n = receiveInt();
        for (int i = 1; i <= n; i++) {
            char ch = static_cast<char>(conn.read());
            s += ch;
        }
    }
    return s;
}

int messagehandler::receiveCode() {
    int n = conn.read();
    return n;
}

void messagehandler::sendByte(int code) {
    conn.write(static_cast<char>(code));
}
