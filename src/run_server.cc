/* myserver.cc: sample server program */
#include "idatabase.h"
#include "inmemory.h"

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

/*
 * Read an integer from a client.
 */
int readNumber(const shared_ptr<Connection>& conn) {
	unsigned char byte1 = conn->read();
	unsigned char byte2 = conn->read();
	unsigned char byte3 = conn->read();
	unsigned char byte4 = conn->read();
	return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

void read_message(const shared_ptr<Connection>& conn) {
	int i = 0;
	i = readNumber(conn);
	cout << "Number read = " << i << endl;
}


/*
 * Send a string to a client.
 */
void writeString(const shared_ptr<Connection>& conn, const string& s) {
	for (char c : s) {
		conn->write(c);
	}
	conn->write('$');
}

int main(int argc, char* argv[]){
	if (argc != 2) {
		cerr << "Usage: myserver port-number" << endl;
		exit(1);
	}

	int port = -1;
	try {
		port = stoi(argv[1]);
	} catch (exception& e) {
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
				//read_message(conn);

				//messagehandler msgHand(conn);
				int i = readNumber(conn);
				string result;
				switch(i) {
					case Protocol::COM_LIST_NG:	cout << "listing groups" << endl;
									//result = database->list_groups(const shared_ptr<Connection>& conn);
									writeString(conn, result);
									break;
					case 2:	cout << "Creating group" << endl;
									result = "Creating group";
									writeString(conn, result);
									break;
					default:	cout << "Default" << endl;
										result = "Default";
										writeString(conn, result);
										break;
				}
				/*
				int nbr = readNumber(conn);
				string result;
				if (nbr > 0) {
					result = "positive";
				} else if (nbr == 0) {
					result = "zero";
				} else {
					result = "negative";
				}
				writeString(conn, result);
				*/
			} catch (ConnectionClosedException&) {
				server.deregisterConnection(conn);
				cout << "Client closed connection" << endl;
			}
		} else {
			conn = make_shared<Connection>();
			server.registerConnection(conn);
			cout << "New client connects" << endl;
		}
	}

	delete database;
}
