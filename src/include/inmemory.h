#ifndef INMEMORY_H
#define INMEMORY_H 1

#include "idatabase.h"

class InMemory : public IDatabase{
public:
	InMemory() : IDatabase() {}

	virtual ~InMemory() {}

	virtual void list_groups(messagehandler& mshHand) override;
	virtual void create_group(messagehandler& mshHand, std::string& name) override;
	virtual void delete_group(messagehandler& mshHand, unsigned int& name) override;
	virtual void list_articles(messagehandler& mshHand, unsigned int& id) override;
	virtual void create_article(messagehandler& msgHand, unsigned int& id, std::string& t, std::string& a, std::string& txt) override;
	virtual void delete_article(messagehandler& msgHand, unsigned int& grId, unsigned int& artId) override;
	virtual void get_article(messagehandler& msgHand, unsigned int& grId, unsigned int& artId) override;

private:
	std::vector<NewsGroup> groups;

};


#endif
