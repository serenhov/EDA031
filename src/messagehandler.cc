#include "messagehandler.h"
#include <iostream>
using namespace std;


int messagehandler::readNumber(){

    unsigned char byte1 = conn.read();
    unsigned char byte2 = conn.read();
    unsigned char byte3 = conn.read();
    unsigned char byte4 = conn.read();
    return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}


void messagehandler:: sendString(string str){
    messagehandler::sendCode(Protocol:: PAR_STRING);
    messagehandler::sendInt(str.length());
    for(unsigned int i=0; i<str.length(); ++i){
        cout << str.at(i) << endl;
        messagehandler::sendByte(str.at(i));
    }
}
void messagehandler::sendCode(int code){
    conn.write(code);
}

void messagehandler::sendInt(int value){
    conn.write((value >> 24) & 0xFF);
    conn.write((value >> 16) & 0xFF);
    conn.write((value >> 8)	 & 0xFF);
    conn.write(value & 0xFF);

}
string messagehandler::receiveString(){
    string s;
   // char ch;

    int n = readNumber();
    cout << n << endl;
    for (int i =1; i<= n; i++) {
        char ch = (char) conn.read();
        s += ch;
    }
    return s;
}

void messagehandler::sendByte(int code){
    conn.write((char) code);
}
