#include "client.h"
#include "connection.h"
#include <iostream>
#include <vector>
#include <utility>
#include "messagehandler.h"
#include "connectionclosedexception.h"

using namespace std;


int main(int argc, char *argv[]) {

    if (argc != 3) {
        cerr << "Usage: myclient host-name port-number" << endl;
        exit(1);
    }

    int port = -1;
    try {
        port = stoi(argv[2]);
    } catch (exception &e) {
        cerr << "Wrong port number. " << e.what() << endl;
        exit(1);
    }

    Connection conn(argv[1], port);
    if (!conn.isConnected()) {
        cerr << "Connection attempt failed" << endl;
        exit(1);
    }

    cout << "Running client ..." << endl;
    messagehandler msgHand(conn);
    string input;

    while (getline(cin, input)) {
        try {
            if (input == "help") {
                cout
                        << " lgr - List Groups \n crgr - Create Group \n delgr - Delete Group \n lart - List Articles \n crart - Create Article \n delart - Delete Article \n getart - Get Article"
                        << endl;

            } else if (input == "lgr") {
                msgHand.sendCode(Protocol::COM_LIST_NG);
                msgHand.sendCode(Protocol::COM_END);

                cout << "Listing of newsgroups: " << endl;

                vector <pair<int, string>> groupList;
                int identifier = msgHand.receiveCode();
                if (identifier == Protocol::ANS_LIST_NG) {
                    int amount = msgHand.receiveIntParameter();
                    for (int i = 0; i < amount; i++) {
                        int id = msgHand.receiveIntParameter();
                        string title = msgHand.receiveString();
                        groupList.push_back(make_pair(id, title));
                    }
                    identifier = msgHand.receiveCode();
                    if (identifier == Protocol::ANS_END) {
                        if (groupList.empty()) {
                            cout << "There are no newsgroups in the database." << endl;
                        } else {
                            cout << "Group ID \t Title" << endl;
                            for (unsigned int j = 0; j < groupList.size(); j++) {
                                cout << groupList[j].first << " \t \t   " << groupList[j].second
                                     << endl;
                            }
                        }
                    }
                }
            } else if (input == "crgr") {
                cout << "Write the title of the newsgroup: ";
                string title;
                getline(cin, title);
                msgHand.sendCode(Protocol::COM_CREATE_NG);
                msgHand.sendString(title);
                msgHand.sendCode(Protocol::COM_END);

                int identifier = msgHand.receiveCode();
                if (identifier == Protocol::ANS_CREATE_NG) {
                    identifier = msgHand.receiveCode();
                    if (identifier == Protocol::ANS_ACK) {
                        cout << "Newsgroup created." << endl;
                    } else if (identifier == Protocol::ANS_NAK) {
                        if (msgHand.receiveCode() == Protocol::ERR_NG_ALREADY_EXISTS) {
                            cout << "Error: The newsgroup already exists."
                                 << endl;
                        }
                    }
                }
                msgHand.receiveCode();
                input.clear();
            } else if (input == "delgr") {
                msgHand.sendCode(Protocol::COM_DELETE_NG);
                cout << "ID of newsgroup to be deleted: ";
                string id;
                getline(cin, id);
                try {
                    int idNbr = stoi(id);
                    msgHand.sendIntParameter(idNbr);
                    msgHand.sendCode(Protocol::COM_END);

                    int identifier = msgHand.receiveCode();
                    if (identifier == Protocol::ANS_DELETE_NG) {
                        identifier = msgHand.receiveCode();
                        if (identifier == Protocol::ANS_ACK) {
                            cout << "Newsgroup deleted." << endl;
                        } else if (identifier == Protocol::ANS_NAK) {
                            msgHand.receiveCode();
                            cout << "Error: The newsgroup doesn't exist." << endl;
                        }
                    }
                    msgHand.receiveCode();

                } catch (exception &e) {
                    msgHand.sendIntParameter(0);
                    cout << "Error: Input must be an integer." << endl;

                    exit(1);
                }
                input.clear();
            } else if (input == "lart") {
                msgHand.sendCode(Protocol::COM_LIST_ART);
                cout << "ID of newsgroup: ";
                string id;
                getline(cin, id);
                try {
                    int idNbr = stoi(id);
                    msgHand.sendIntParameter(idNbr);
                    msgHand.sendCode(Protocol::COM_END);

                    vector <pair<int, string>> artList;
                    int identifier = msgHand.receiveCode();
                    if (identifier == Protocol::ANS_LIST_ART) {
                        identifier = msgHand.receiveCode();
                        if (identifier == Protocol::ANS_ACK) {
                            int amount = msgHand.receiveIntParameter();
                            for (int i = 0; i < amount; i++) {
                                int id = msgHand.receiveIntParameter();
                                string title = msgHand.receiveString();
                                artList.push_back(make_pair(id, title));
                            }
                            identifier = msgHand.receiveCode();
                            if (identifier == Protocol::ANS_END) {
                                if (artList.empty()) {
                                    cout << "There are no articles in this newsgroup." << endl;
                                } else {
                                    cout << "Article ID /t Title" << endl;
                                    for (unsigned int j = 0; j < artList.size(); j++) {
                                        cout << artList[j].first << " \t \t     " << artList[j].second
                                             << endl;
                                    }
                                }
                            }
                        } else if (identifier == Protocol::ANS_NAK) {
                            msgHand.receiveCode();
                            cout << "Error: The newsgroup doesn't exist." << endl;
                            msgHand.receiveCode();
                        }

                    }

                } catch (exception &e) {
                    msgHand.sendIntParameter(0);
                    cout << "Error: Input must be an integer." << endl;

                    exit(1);
                }


                input.clear();


            } else if (input == "crart") {
                msgHand.sendCode(Protocol::COM_CREATE_ART);
                cout << "ID of the newsgroup: ";
                string id;
                getline(cin, id);
                try {
                    int idNbr = stoi(id);
                    cout << "Choose title: ";
                    string title;
                    getline(cin, title);
                    cout << "Choose author: ";
                    string author;
                    getline(cin, author);
                    cout << "Article text: ";
                    string text;
                    getline(cin, text);

                    msgHand.sendIntParameter(idNbr);
                    msgHand.sendString(title);
                    msgHand.sendString(author);
                    msgHand.sendString(text);
                    msgHand.sendCode(Protocol::COM_END);

                    int identifier = msgHand.receiveCode();
                    if (identifier == Protocol::ANS_CREATE_ART) {
                        identifier = msgHand.receiveCode();
                        if (identifier == Protocol::ANS_ACK) {
                            cout << "Article created." << endl;
                        } else if (identifier == Protocol::ANS_NAK) {
                            msgHand.receiveCode();
                            cout << "Error: The newsgroup doesn't exist." << endl;
                        }
                        msgHand.receiveCode();
                    }
                    input.clear();


                } catch (exception &e) {
                    msgHand.sendIntParameter(0);
                    cout << "Error: Input must be an integer." << endl;

                    exit(1);
                }
            } else if (input == "delart") {
                msgHand.sendCode(Protocol::COM_DELETE_ART);
                cout << "ID of the newgroup: ";
                string nId;
                getline(cin, nId);
                cout << "ID of the article: ";
                string aId;
                getline(cin, aId);
                try {
                    int nIdNbr = stoi(nId);
                    int aIdNbr = stoi(aId);
                    msgHand.sendIntParameter(nIdNbr);
                    msgHand.sendIntParameter(aIdNbr);
                    msgHand.sendCode(Protocol::COM_END);

                    int identifier = msgHand.receiveCode();
                    if (identifier == Protocol::ANS_DELETE_ART) {
                        identifier = msgHand.receiveCode();
                        if (identifier == Protocol::ANS_ACK) {
                            cout << "The article has been deleted." << endl;
                        } else if (identifier == Protocol::ANS_NAK) {
                            identifier = msgHand.receiveCode();
                            if (identifier == Protocol::ERR_NG_DOES_NOT_EXIST) {
                                cout << "Error: The newsgroup doesn't exist." << endl;
                            }
                            if (identifier == Protocol::ERR_ART_DOES_NOT_EXIST) {
                                cout << "Error: The article doesn't exist." << endl;
                            }

                        }
                        msgHand.receiveCode();
                    }
                    input.clear();
                } catch (exception &e) {
                    msgHand.sendIntParameter(0);
                    cout << "Error: Input must be an integer." << endl;

                    exit(1);
                }
            } else if (input == "getart") {
                msgHand.sendCode(Protocol::COM_GET_ART);
                cout << "ID of newsgroup: ";
                string nId;
                getline(cin, nId);
                cout << "ID of article: ";
                string aId;
                getline(cin, aId);
                try {
                    int nIdNbr = stoi(nId);
                    int aIdNbr = stoi(aId);

                    msgHand.sendIntParameter(nIdNbr);
                    msgHand.sendIntParameter(aIdNbr);
                    msgHand.sendCode(Protocol::COM_END);

                    int identifier = msgHand.receiveCode();
                    if (identifier == Protocol::ANS_GET_ART) {
                        identifier = msgHand.receiveCode();
                        if (identifier == Protocol::ANS_ACK) {
                            string title = msgHand.receiveString();
                            string author = msgHand.receiveString();
                            string text = msgHand.receiveString();
                            cout << title << ", written by: " << author << endl;
                            cout << text << endl;
                        } else if (identifier == Protocol::ANS_NAK) {
                            identifier = msgHand.receiveCode();
                            if (identifier == Protocol::ERR_NG_DOES_NOT_EXIST) {
                                cout << "Error: The newsgroup doesn't exist." << endl;

                            }
                            if (identifier == Protocol::ERR_ART_DOES_NOT_EXIST) {
                                cout << "Error: The article doesn't exist." << endl;
                            }
                        }
                        msgHand.receiveCode();
                        input.clear();
                    }
                } catch (exception &e) {
                    msgHand.sendIntParameter(0);
                    cout << "Error: Input must be an integer." << endl;

                    exit(1);
                }
            } else {
                cout << "Wrong input" << endl;
            }


        } catch (ConnectionClosedException &) {
            cout << " no reply from server. Exiting." << endl;
            exit(1);
        }
    }
}
