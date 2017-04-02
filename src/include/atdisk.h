#ifndef ATDISK_H
#define ATDISK_H 1

#include <sqlite3.h>
#include <iostream>
#include "idatabase.h"

class AtDisk : public IDatabase {
 public:
  AtDisk();
  virtual ~AtDisk();

  virtual void list_groups(messagehandler& mshHand) override;
  virtual void create_group(messagehandler& mshHand,
                            std::string& name) override;
  virtual void delete_group(messagehandler& mshHand,
                            unsigned int& name) override;
  virtual void list_articles(messagehandler& mshHand,
                             unsigned int& id) override;
  virtual void create_article(messagehandler& msgHand, unsigned int& id,
                              std::string& t, std::string& a,
                              std::string& txt) override;
  virtual void delete_article(messagehandler& msgHand, unsigned int& grId,
                              unsigned int& artId) override;
  virtual void get_article(messagehandler& msgHand, unsigned int& grId,
                           unsigned int& artId) override;

 private:
  sqlite3* db;
};

#endif
