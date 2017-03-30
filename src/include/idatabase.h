#ifndef IDATABASE_H
#define IDATABASE_H 1

#include "messagehandler.h"
#include "connection.h"

#include <string>
#include <vector>
#include <memory>

class IDatabase {
public:
	virtual ~IDatabase() = default;
	virtual void list_groups(messagehandler& msgHand) = 0;
	virtual void create_group(messagehandler& msgHand, std::string& name) = 0;
	virtual void delete_group(messagehandler& msgHand, unsigned int& id) = 0;
	virtual void list_articles(messagehandler& msgHand, unsigned int& id) = 0;
	virtual void create_article(messagehandler& msgHand, unsigned int& id, std::string& t, std::string& a, std::string& txt) = 0;
	virtual void delete_article(messagehandler& msgHand, unsigned int& grId, unsigned int& artId) = 0;
	virtual void get_article(messagehandler& msgHand, unsigned int& grId, unsigned int& artId) = 0;

};

class Article {
public:
	explicit Article(unsigned int i, std::string s1, std::string s2, std::string s3) : id(i), title(s1), author(s2), text(s3) {}
	unsigned int id;
	std::string title;
	std::string author;
	std::string text;
};

class NewsGroup {
public:
	unsigned int id;
	std::string name;
	std::vector<Article> articles;

	NewsGroup(unsigned int& i, std::string& s) : id(i), name(s), articles() {}
	NewsGroup(unsigned int&& i, std::string& s) : id(i), name(s), articles() {}

};


#endif
