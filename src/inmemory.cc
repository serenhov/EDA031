#include "inmemory.h"
#include "idatabase.h"
#include "protocol.h"

#include <iostream>

using namespace std;

/*
int readNumber(const shared_ptr<Connection>& conn) {
	unsigned char byte1 = conn->read();
	unsigned char byte2 = conn->read();
	unsigned char byte3 = conn->read();
	unsigned char byte4 = conn->read();
	return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}
*/

void InMemory::list_groups(const shared_ptr<Connection>& conn) {
	cout << "Listing groups ... " << endl;
	/*
	if(message_handler.readNumber() != Protocol::ANS_END)
		cout << "ABORT! No end for list groups" << endl;
		}
	message_handler.sendCode(Protocol::ANS_LIST_NG);
	message_handler.sendInt(groups.size());
	*/
	for(auto it = groups.begin(); it != groups.end(); ++it) {
		//message_handler.sendInt(it->id);
		//message_handler.sendString(it->name);
	}
	//message_handler.sendCode(Protocol::ANS_END);

}

void InMemory::create_group(const shared_ptr<Connection>& conn) {
	cout << "Creating group ... " << endl;
	/*
	string name = message_handler.receiveString();
	if(message_handler.readNumber() != Protocol::ANS_END)
		cout << "ABORT! No end for list groups" << endl;
		}
		*/
	string name = "group";
	unsigned int i = 1;
	if(groups.size() == 0) groups.push_back(NewsGroup(i, name));
	else groups.push_back(NewsGroup(groups.back().id, name));

}
