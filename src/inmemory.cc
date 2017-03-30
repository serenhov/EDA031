#include "inmemory.h"
#include "idatabase.h"
#include "protocol.h"

#include "server.h"
#include "connection.h"
#include "connectionclosedexception.h"

#include <iostream>
#include <algorithm>

using namespace std;

void InMemory::list_groups(messagehandler& msgHand) {
	cout << "Listing groups ... " << endl;
	msgHand.sendIntParameter(groups.size());
	for(auto it = groups.begin(); it != groups.end(); ++it) {
		msgHand.sendIntParameter(it->id);
		cout << it->id << endl;
		cout << it->name << endl;
		msgHand.sendString(it->name);
	}
}

void InMemory::create_group(messagehandler& msgHand, string& name) {
	cout << "Creating group ... " << endl;
	if(groups.size() == 0) {
		cout << "Succesfully added group" << endl;
		groups.push_back(NewsGroup(1, name));
		msgHand.sendCode(Protocol::ANS_ACK);
	}
	else if(find_if(groups.begin(), groups.end(), [name] (const NewsGroup& ng) {return ng.name == name;}) != groups.end()) {
		msgHand.sendCode(Protocol::ANS_NAK);
		msgHand.sendCode(Protocol::ERR_NG_ALREADY_EXISTS);
	}
	else {
		cout << "Succesfully added group" << endl;
		groups.push_back(NewsGroup(groups.back().id+1, name));
		msgHand.sendCode(Protocol::ANS_ACK);
	}
}

void InMemory::delete_group(messagehandler& msgHand, unsigned int& id) {
	cout << "Deleting group ... " << endl;
	auto it = find_if(groups.begin(), groups.end(), [id] (const NewsGroup& ng) {return ng.id == id;});
	if(it != groups.end()) {
		groups.erase(it);
		msgHand.sendCode(Protocol::ANS_ACK);
	}
	else {
		msgHand.sendCode(Protocol::ANS_NAK);
		msgHand.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
	}
}

//Should be possible to make a template list function for groups vs. articles?
void InMemory::list_articles(messagehandler& msgHand, unsigned int& id) {
	cout << "Listing articles ... " << endl;
	auto this_group = find_if(groups.begin(), groups.end(), [id] (const NewsGroup& ng) {return ng.id == id;});
	if(this_group != groups.end()) {
		cout << "About to send the articles ..." << endl;
		msgHand.sendCode(Protocol::ANS_ACK);
		msgHand.sendIntParameter(this_group->articles.size());
		for(auto it = this_group->articles.begin(); it != this_group->articles.end(); ++it) {
			msgHand.sendIntParameter(it->id);
			msgHand.sendString(it->title);
		}
	}
	else {
		msgHand.sendCode(Protocol::ANS_NAK);
		msgHand.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
	}
}


void InMemory::create_article(messagehandler& msgHand, unsigned int& id, std::string& t, std::string& a, std::string& txt) {
	cout << "Creating article ... " << endl;
	auto this_group = find_if(groups.begin(), groups.end(), [id] (const NewsGroup& ng) {return ng.id == id;});
	if(this_group != groups.end()) {
		if(this_group->articles.size() == 0) {
			cout << "Succesfully added article" << endl;
			this_group->articles.push_back(Article(1,t,a,txt));
			msgHand.sendCode(Protocol::ANS_ACK);
		}
		else {
			cout << "Succesfully added article" << endl;
			this_group->articles.push_back(Article(this_group->articles.back().id+1,t,a,txt));
			msgHand.sendCode(Protocol::ANS_ACK);
		}
	}
	else {
		msgHand.sendCode(Protocol::ANS_NAK);
		msgHand.sendCode(Protocol::ERR_NG_ALREADY_EXISTS);
	}

}
void InMemory::delete_article(messagehandler& msgHand, unsigned int& grId, unsigned int& artId) {
	cout << "Deleting article ... " << endl;
	auto this_group = find_if(groups.begin(), groups.end(), [grId] (const NewsGroup& ng) {return ng.id == grId;});
	if(this_group != groups.end()) {
		auto this_article = find_if(this_group->articles.begin(), this_group->articles.end(), [artId] (const Article& art) {return art.id == artId;});
		if(this_article != this_group->articles.end()) {
			this_group->articles.erase(this_article);
			msgHand.sendCode(Protocol::ANS_ACK);
		}
		else {
			msgHand.sendCode(Protocol::ANS_NAK);
			msgHand.sendCode(Protocol::ERR_ART_DOES_NOT_EXIST);
		}
	}
	else {
		msgHand.sendCode(Protocol::ANS_NAK);
		msgHand.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
	}
}


//Could write a function which checks for group/article existing (some repetition here)
void InMemory::get_article(messagehandler& msgHand, unsigned int& grId, unsigned int& artId) {
	cout << "Getting article ... " << endl;
	auto this_group = find_if(groups.begin(), groups.end(), [grId] (const NewsGroup& ng) {return ng.id == grId;});
	if(this_group != groups.end()) {
		auto this_article = find_if(this_group->articles.begin(), this_group->articles.end(), [artId] (const Article& art) {return art.id == artId;});
		if(this_article != this_group->articles.end()) {
			msgHand.sendCode(Protocol::ANS_ACK);
			msgHand.sendString(this_article->title);
			msgHand.sendString(this_article->author);
			msgHand.sendString(this_article->text);
		}
		else {
			msgHand.sendCode(Protocol::ANS_NAK);
			msgHand.sendCode(Protocol::ERR_ART_DOES_NOT_EXIST);
		}
	}
	else {
		msgHand.sendCode(Protocol::ANS_NAK);
		msgHand.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
	}
}

