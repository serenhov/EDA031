#include "client.h"
#include "connection.h"
#include <iostream>
#include "messagehandler.h"
#include "connectionclosedexception.h"
using namespace std;



int main(int argc, char* argv[]) {

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
			if (input == "test") {
				msgHand.sendInt(5);
				string reply;
                reply = msgHand.receiveString();
                cout << "nu kommer det snat" << endl;
				cout << reply;
			}

				/*if (input == "help"){
                    cout<< "lgr - List Groups \n crgr - Create Group \n delgr - Delete Group \n lart - List Articles \n crart - Create Article \n delart - Deleet Article \n getart - Get Article"

                }
                if (input=="lgr") {
                    msgHand.sendCode(Protocol::COM_LIST_NG);
                    msgHand.sendCode(Protocol::COM_END);
                    cout << "Listing of newsgroups: " << endl;

                    vector <Pair<int, string>> groupList;
                    int identifier = msgHand.receiveCode();
                    if (identifier == Protocol::ANS_LIST_NG) {
                        int amount = msgHand.receiveInt();
                        for (int i = 0; i < amount; i++) {
                            int id = msgHand.receiveInt();
                            string title = msgHand.receiveString();
                            groupList.make_pair(id, title);
                        }
                        identifier = msgHand.receiveCode();
                        if (identifier == Protocol::ANS_END) {
                            if (groupList.empty()) {
                                cout << "There are no newsgroups in the database" << endl;
                            } else {
                                for (int j = 0; j < groupList.length(); j++) {
                                    cout << "Group id: " << groupList[j].first << " title: " << groupList[j].second
                                         << endl;
                                }
                            }
                        }
                    }
                }

                else if (input=="crgr") {
                    cout << "Write the title of the newsgroup: " << endl;
                    string title;
                    cin >> title;
                    msgHand.sendCode(Protocol::COM_CREATE_NG)
                    msgHand.sendString(title);
                    msgHand.sendCode(Protocol::COM_END);

                    int identifier = msgHand.receiveCode();
                    if (identifier == Protocol::ANS_CREATE_NG) {
                        identifier = msgHand.receiveCode();
                        if (identifier == Protocol::ANS_ACK) {
                            cout << "Newsgroup created" << endl;
                        } else if (identifier == Protocol::ANS_NAK) {
                            cout << "Error: The newsgroup already exists"
                                 << endl; //kanske behöver läsa in ERR code och ANS_END
                        }
                    }
                    //clean up msgHand.

                }
                else if(input=="delgr") {
                    msgHand.sendCode(Protocol::COM_DELETE);
                    cout << "Id of newsgroup to be deleted: ";
                    int id;
                    cin >> id;
                    msgHand.sendInt(id);
                    msgHand.sendCode(Protocol::COM_END);

                    int identifier = msgHand.receiveCode();
                    if (identifier == Protocol::ANS_DELETE_NG) {
                        identifier = msgHand.receiveCode();
                        if (identifier == Protocol::ANS_ACK) {
                            cout << "Newsgroup deleted." << endl;
                        } else if (identifier == Protocol::ANS_NAK) {
                            cout << "Error: The newsgroup doesn't exist" << endl;
                        }

                    }
                    //clear msgHand
                }

                else if(input=="lart") {
                    msgHand.sendCode(Protocol::COM_LIST_ART);
                    cout << "Newsgroup id: ";
                    int id;
                    cin >> id;
                    msgHand.sendInt(id);
                    msgHand.sendCode(Protocol::COM_END);

                    vector <Pair<int, string>> artList;
                    int identifier = msgHand.receiveCode();
                    if (identifier == Protocol::ANS_LIST_ART) {
                        identifier = msgHand.receiveCode();
                        if (identifier == Protocol::ANS_ACK) {
                            int amount = msgHand.receiveInt();
                            for (int i = 0; i < amount; i++) {
                                int id = msgHand.receiveInt();
                                string title = msgHand.receiveString;
                                artList[i].make_pair(id, title);
                            }
                            identifier = msgHand.receiveCode();
                            if (identifier == Protocol::ANS_END) {
                                if (artList.empty()) {
                                    cout << "There are no articles in this newsgroup." << endl;
                                } else {
                                    for (int j = 0; j < artList.length(); j++) {
                                        cout << "Article id: " << artList[j].first << " title: " << artList[j].second
                                             << endl;
                                    }
                                }
                            }
                        } else if (identifier == Protocol::ANS_NAK) {
                            cout << "Error: The newsgroup doesn't exist" << endl;
                        }
                    }
                    //clean msgHand

                }
                else if(input=="crart") {
                    msgHand.sendCode(Protocol::COM_CREATE_ART);
                    cout << "Newsgroup id: ";
                    int id;
                    cin >> id;
                    cout << "Choose title: ";
                    string title;
                    cin >> title;
                    cout << "Choose author: ";
                    string author;
                    cin >> author;
                    cout << "Article text: ";
                    string text;
                    cin >> text;

                    msgHand.sendInt(id);
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
                            cout << "Error: The newsgroup doesn't exist." << endl;
                        }
                    }
                    //clean

                }
                else if(input=="delart") {
                    msgHand.sendCode(Protocol::COM_DELETE_ART);
                    cout << "Newsgroup id: ";
                    int nId;
                    cin << nId;
                    cout << "Article id: ";
                    int aId;
                    cin << aId;

                    msgHand.sendInt(nId);
                    msgHand.sendInt(aId);
                    msgHand.sendCode(Protocol::COM_END);

                    int identifier = msgHand.receiveCode();
                    if (identifier == Protocol::ANS_DELETE_ART) {
                        identifier = msgHand.receiveCode();
                        if (identifier == Protocol::ANS_ACK) {
                            cout << "The article has been deleted" << endl;
                        } else if (identifier == Protocol::ANS_NAK) {
                            identifier = msgHand.receiveCode();
                            if (identifier == Protocol::ERR_NG_DOES_NOT_EXIST) {
                                cout << "Error: The newsgroup doesn't exist." << endl;
                            }
                            if (identifier == Protocol::ERR_ART_DOES_NOT_EXIST) {
                                cout << "Error: The article doesn't exist." << endl;
                            }
                        }
                    }
                    //clean
                }

                else if(input=="getart") {
                        msgHand.sendCode(Protocol::COM_GET_ART);
                        cout << "Newsgroup id: ";
                        int nId;
                        cin << nId;
                        cout << "Article id: ";
                        int aId;
                        cin << aId;

                        msgHand.sendInt(nId);
                        msgHand.sendInt(aId);
                        msgHand.sendCode(Protocol::COM_END);

                        int identifier = msgHand.receiveCode();
                        if (identifier == Protocol::ANS_GET_ART) {
                            identifier = msgHand.receiveCode();
                            if (identifier == Protocol::ANS_ACK) {
                                cout << msgHand.receiveString() << " written by: " << msgHand.receiveString() << endl;
                                cout << msgHand.receiveString() << endl;
                            } else if (identifier == Protocol::ANS_NAK) {
                                identifier = msgHand.receiveCode();
                                if (identifier == Protocol::ERR_NG_DOES_NOT_EXIST) {
                                    cout << "Error: The newsgroup doesn't exist." << endl;

                                }
                                if (identifier == Protocol::ERR_ART_DOES_NOT_EXIST) {
                                    cout << "Error: The article doesn't exist." << endl;
                                }
                            }
                        }
                    }*/
				//clean

			else {
				cout << "Wrong input" << endl;
			}


		} catch (ConnectionClosedException &) {
			cout << " no reply from server. Exiting." << endl;
			exit(1);
		}
	}
}
