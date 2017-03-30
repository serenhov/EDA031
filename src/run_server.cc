/* myserver.cc: sample server program */
#include "idatabase.h"
#include "inmemory.h"
#include "messagehandler.h"

#include "protocol.h"
#include "server.h"
#include "connection.h"
#include "connectionclosedexception.h"

#include <memory>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>

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

		IDatabase* database = new InMemory();

    while (true) {
        auto conn = server.waitForActivity();

        if (conn != nullptr) {
            try {
                messagehandler msgHand(*conn.get());
                int code = msgHand.receiveCode();
                switch (code) {
                    case Protocol::COM_LIST_NG : {
                        if (msgHand.receiveCode() == Protocol::COM_END) {
                            msgHand.sendCode(Protocol::ANS_LIST_NG);
														database->list_groups(msgHand);
                            msgHand.sendCode(Protocol::ANS_END);
                        }
                        break;
                    }
                    case Protocol::COM_CREATE_NG : {
                        string name = msgHand.receiveString();
                        if (msgHand.receiveCode() == Protocol::COM_END) {
                            msgHand.sendCode(Protocol::ANS_CREATE_NG);
														database->create_group(msgHand, name);
                            msgHand.sendCode(Protocol::ANS_END);
                        }
                        break;
                    }
                    case Protocol::COM_DELETE_NG : {
                        unsigned int id = msgHand.receiveIntParameter();
                        if (msgHand.receiveCode() == Protocol::COM_END) {
                            msgHand.sendCode(Protocol::ANS_DELETE_NG);
														database->delete_group(msgHand, id);
                            msgHand.sendCode(Protocol::ANS_END);
                        }
                        break;
                    }
                    case Protocol::COM_LIST_ART : {
                        unsigned int id = msgHand.receiveIntParameter();
                        if (msgHand.receiveCode() == Protocol::COM_END) {
                            msgHand.sendCode(Protocol::ANS_LIST_ART);
														database->list_articles(msgHand, id);
                            msgHand.sendCode(Protocol::ANS_END);
                        }
                        break;
                    }
                    case Protocol::COM_CREATE_ART : {
                        unsigned int id = msgHand.receiveIntParameter();
                        string title = msgHand.receiveString();
                        string author = msgHand.receiveString();
                        string text = msgHand.receiveString();
                        if (msgHand.receiveCode() == Protocol::COM_END) {
                            msgHand.sendCode(Protocol::ANS_CREATE_ART);
														database->create_article(msgHand, id, title, author, text);
                            msgHand.sendCode(Protocol::ANS_END);
                        }
                        break;
                    }
                    case Protocol::COM_DELETE_ART : {
                        unsigned int grId = msgHand.receiveIntParameter();
                        unsigned int artId = msgHand.receiveIntParameter();
                        if (msgHand.receiveCode() == Protocol::COM_END) {
                            msgHand.sendCode(Protocol::ANS_DELETE_ART);
														database->delete_article(msgHand, grId, artId);
                            msgHand.sendCode(Protocol::ANS_END);
                        }
                        break;
                    }
                    case Protocol::COM_GET_ART : {
                        unsigned int grId = msgHand.receiveIntParameter();
                        unsigned int artId = msgHand.receiveIntParameter();
                        if (msgHand.receiveCode() == Protocol::COM_END) {
                            msgHand.sendCode(Protocol::ANS_GET_ART);
														database->get_article(msgHand, grId, artId);
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
