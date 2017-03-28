#ifndef INMEMORY_H
#define INMEMORY_H 1

#include "idatabase.h"

class InMemory : public IDatabase{
public:
	InMemory() : IDatabase() {}

	virtual ~InMemory() {}

	virtual void list_groups(const std::shared_ptr<Connection>& conn) override;
	virtual void create_group(const std::shared_ptr<Connection>& conn) override;

private:
	std::vector<NewsGroup> groups;

};


#endif
