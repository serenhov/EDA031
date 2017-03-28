#ifndef IDATABASE_H
#define IDATABASE_H 1

#include <string>
#include <vector>
#include <memory>
#include "connection.h"

class IDatabase {
public:
	virtual ~IDatabase() = default;
	virtual void list_groups(const std::shared_ptr<Connection>& conn) = 0;
	virtual void create_group(const std::shared_ptr<Connection>& conn) = 0;
	/*
	virtual void delete_group();
	virtual void list_articles();
	virtual void create_article();
	virtual void delete_article();
	virtual void get_article();
	*/

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

	NewsGroup(unsigned int& i, std::string& s) : id(i), name(s) {}

};


#endif
