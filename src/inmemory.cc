#include "inmemory.h"
#include "idatabase.h"

#include <iostream>
#include "server.h"
#include "connection.h"
#include "connectionclosedexception.h"

#include <memory>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>
#include "messagehandler.h"



using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: myserver port-number" << endl;
        exit(1);
    }

    int port = -1;
    try {
        port = stoi(argv[1]);
    } catch (exception &e) {
        cerr << "Wrong port number. " << e.what() << endl;
        exit(1);
    }

    Server server(port);
    if (!server.isReady()) {
        cerr << "Server initialization error." << endl;
        exit(1);
    }

    while (true) {
        auto conn = server.waitForActivity();

        if (conn != nullptr) {
            try {
                messagehandler msgHand(*conn.get());
                int code = msgHand.receiveCode();
                switch (code) {
                    case Protocol::COM_LIST_NG : {
                        if (msgHand.receiveCode() == Protocol::COM_END) {
                            //SKRIV KOD HÄR
                            msgHand.sendCode(Protocol::ANS_LIST_NG);
                            msgHand.sendIntParameter(0); // SKICKA ÖVER ANTALET GRUPPER
                            //ITERERA ÖVER ALL GRUPPER
                            //msgHand.sendInt(0) ID FÖR GRUPPEN
                            //msgHand.sendString("") TITEL PÅ GRUPPEN
                            //Skicka i sorterad ordning
                            msgHand.sendCode(Protocol::ANS_END);
                        }
                        break;
                    }
                    case Protocol::COM_CREATE_NG : {
                        string title = msgHand.receiveString();
                        if (msgHand.receiveCode() == Protocol::COM_END) {
                            msgHand.sendCode(Protocol::ANS_CREATE_NG);
                            msgHand.sendCode(Protocol::ANS_NAK);
                            msgHand.sendCode(Protocol::ERR_NG_ALREADY_EXISTS);
                            //SKRIV KOD HÄR
                            /*SKICKA ANTINGEN:
                            msgHand.sendCode(Protocol::ANS_ACK) OM DET GICK
                            ELLER:
                            msgHand.sendCode(Protocol::ANS_NAK);
                            msgHand.sendCode(Protocol::ERR_NG_ALREADY_EXISTS); OM DET INTE GICK
                            */

                            msgHand.sendCode(Protocol::ANS_END);
                        }
                        break;
                    }
                    case Protocol::COM_DELETE_NG : {
                        int id = msgHand.receiveIntParameter();
                        if (msgHand.receiveCode() == Protocol::COM_END) {
                            //SKRIV KOD HÄR
                            msgHand.sendCode(Protocol::ANS_DELETE_NG);
                            msgHand.sendCode(Protocol::ANS_ACK); //RADERA EFTER IMPLEMENTERING
                            /* SKICKA ANTINGEN:
                            msgHand.sendCode(Protocol::ANS_ACK); OM DET GICK
                            ELLER
                            msgHand.sendCode(Protocol::ANS_NAK);
                            msgHand.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST); OM DET INTE GICK
                            */

                            msgHand.sendCode(Protocol::ANS_END);
                        }
                        break;
                    }
                    case Protocol::COM_LIST_ART : {
                        int id = msgHand.receiveIntParameter();
                        if (msgHand.receiveCode() == Protocol::COM_END) {
                            //SKRIV KOD HÄR
                            msgHand.sendCode(Protocol::ANS_LIST_ART);
                            msgHand.sendCode(Protocol::ANS_ACK); //RADERA EFTER IMPLEMENTERING
                            msgHand.sendIntParameter(0); //RADERA EFTER IMPLEMENTERING
                        /* SKICKA ANTINGEN:
                        msgHand.sendCode(Protocol::ANS_ACK);
                        SKICKA ÖVER ANTALET ARTIKLAR
						ITERERA ÖVER ALL ARTIKLAR
						msgHand.sendInt(0) ID FÖR ARTIKELN
						msgHand.sendString("") TITEL PÅ ARTIKELN
						Skicka i sorterad ordning		OM DET GICK
						ELLER
						msgHand.sendCode(Protocol::ANS_NAK);
						msgHand.sendCode(Protocol:: ERR_NG_DOES_NOT_EXIST); OM DET INTE GICK
						*/

                            msgHand.sendCode(Protocol::ANS_END);
                        }
                        break;
                    }
                    case Protocol::COM_CREATE_ART : {
                        int id = msgHand.receiveIntParameter();
                        string title = msgHand.receiveString();
                        string author = msgHand.receiveString();
                        string text = msgHand.receiveString();
                        if (msgHand.receiveCode() == Protocol::COM_END) {
                            //SKRIV KOD HÄR
                            msgHand.sendCode(Protocol::ANS_CREATE_ART);

                            msgHand.sendCode(Protocol::ANS_ACK); //RADERA EFTER IMPLEMENTERING

                            /* SKICKA ANTINGEN:
                            msgHand.sendCode(Protocol::ANS_ACK); SKAPAT ARTIKELN
                            ELLER:
                            msgHand.sendCode(Protocol::ANS_NAK);
                            msgHand.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST); GICK INTE SKAPA ARTIKELN
                            */

                            msgHand.sendCode(Protocol::ANS_END);

                        }
                        break;
                    }
                    case Protocol::COM_DELETE_ART : {
                        int grId = msgHand.receiveIntParameter();
                        int artId = msgHand.receiveIntParameter();
                        if (msgHand.receiveCode() == Protocol::COM_END) {
                            //SKRIV KOD HÄR
                            msgHand.sendCode(Protocol::ANS_DELETE_ART);

                            msgHand.sendCode(Protocol::ANS_ACK); //RADERA EFTER IMPLEMENTERING

                            /* SKICKA ANTINGEN:
                            msgHand.sendCode(Protocol::ANS_ACK); OM ARTIKELN HAR RADERATS
                            ELLER
                            msgHand.sendCode(Protocol::ANS_NAK);
                            OCH ANTINGEN:
                                msgHand.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST); OM NEWSGROUP INTE EXISTERAR
                                ELLER
                                msgHand.sendCode(Protocol::ERR_ART_DOES_NOT_EXIST); OM ARTICLE INTE EXISTERAR
                                */

                            msgHand.sendCode(Protocol::ANS_END);
                        }
                        break;
                    }
                    case Protocol::COM_GET_ART : {
                        int grId = msgHand.receiveIntParameter();
                        int artId = msgHand.receiveIntParameter();
                        if (msgHand.receiveCode() == Protocol::COM_END) {
                            //SKRIV KOD HÄR

                            msgHand.sendCode(Protocol::ANS_GET_ART);

                            msgHand.sendCode(Protocol::ANS_ACK);
                            string title = "C++ Projektgrupp";
                            string author = "Gruppen";
                            string text = "Härligt, det funkar!";
                            msgHand.sendString(title);
                            msgHand.sendString(author);
                            msgHand.sendString(text);


                            /* SKICKA ANTINGEN:
                            msgHand.sendCode(Protocol::ANS_ACK);
                            string title;
                            string author;
                            string text;
                            msgHand.sendString(title);
                            msgHand.sendString(author);
                            msgHand.sendString(text); OM ARTIKELN FANNS
                            ELLER
                            msgHand.sendCode(Protocol::ANS_NAK);
                            OCH ANTINGEN
                                msgHand.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST); OM NEWSGROUP INTE FINNS
                                ELLER
                                msgHand.sendCode(Protocol::ERR_ART_DOES_NOT_EXIST); OM ARTICLE INTE FINNS
                                */

                            msgHand.sendCode(Protocol::ANS_END);
                        }
                        break;
                    }
                }
            } catch (ConnectionClosedException &) {
                server.deregisterConnection(conn);
                cout << "Client closed connection" << endl;
            }
        } else {
            conn = make_shared<Connection>();
            server.registerConnection(conn);
            cout << "New client connects" << endl;
        }
    }
}
