#include "idatabase.h"
#include "atdisk.h"
#include <sqlite3.h>
#include <algorithm>
#include <iostream>
#include "connection.h"
#include "connectionclosedexception.h"
#include "protocol.h"
#include "server.h"

using namespace std;

AtDisk::AtDisk() {
  int rc;

  rc = sqlite3_open("newsgroups.db", &db);

  if (rc) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
  } else {
    fprintf(stderr, "Opened database successfully\n");
  }
}

AtDisk::~AtDisk() { sqlite3_close(db); }

void AtDisk::list_groups(messagehandler& msgHand) {
  cout << "Listing groups ... " << endl;

  sqlite3_stmt* stmt;
  const char* sql = "SELECT count(*) FROM newsgroups";
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

  while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
    int count = sqlite3_column_int(stmt, 0);
    msgHand.sendIntParameter(count);
    cout << "sending: " << count << endl;
  }

  sqlite3_finalize(stmt);

  sql = "SELECT * FROM newsgroups";
  rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

  while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
    int id = sqlite3_column_int(stmt, 0);
    auto name = std::string(
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));

    msgHand.sendIntParameter(id);
    msgHand.sendString(name);
    cout << id << " " << name << endl;
  }

  sqlite3_finalize(stmt);
}

void AtDisk::create_group(messagehandler& msgHand, string& name) {
  cout << "Creating group ... " << endl;

  sqlite3_stmt* stmt;
  string sql = "INSERT INTO newsgroups (name) VALUES ('" + name + "')";
  int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

  rc = sqlite3_step(stmt);
  if (rc == SQLITE_DONE) {
    cout << "Succesfully added group" << endl;
    msgHand.sendCode(Protocol::ANS_ACK);
  } else {
    msgHand.sendCode(Protocol::ANS_NAK);
    msgHand.sendCode(Protocol::ERR_NG_ALREADY_EXISTS);
  }

  sqlite3_finalize(stmt);
}

void AtDisk::delete_group(messagehandler& msgHand, unsigned int& id) {
  cout << "Deleting group ... " << endl;

  sqlite3_stmt* stmt;
  string sql =
      "SELECT count(*) FROM newsgroups WHERE id = " + to_string(id);
  int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

  while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
    int count = sqlite3_column_int(stmt, 0);

		if(count != 1) {
			msgHand.sendCode(Protocol::ANS_NAK);
			msgHand.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
		}
  }

  sqlite3_finalize(stmt);

  sql = "DELETE FROM newsgroups WHERE id = " + to_string(id);
  rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

  rc = sqlite3_step(stmt);
  if (rc == SQLITE_DONE) {
    msgHand.sendCode(Protocol::ANS_ACK);
  }

  sqlite3_finalize(stmt);
}

void AtDisk::list_articles(messagehandler& msgHand, unsigned int& id) {
  cout << "Listing articles ... " << endl;

  sqlite3_stmt* stmt;
  string sql =
      "SELECT count(*) FROM articles WHERE newsgroup_id = " + to_string(id);
  int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

  while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
    int count = sqlite3_column_int(stmt, 0);
    msgHand.sendCode(Protocol::ANS_ACK);
    msgHand.sendIntParameter(count);
    cout << "sending: " << count << endl;
  }

  sqlite3_finalize(stmt);

  sql = "SELECT id, title FROM articles WHERE newsgroup_id = " + to_string(id);
  rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

  while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
    int id = sqlite3_column_int(stmt, 0);
    auto title = std::string(
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));

    msgHand.sendIntParameter(id);
    msgHand.sendString(title);
    cout << id << " " << title << endl;
  }

  sqlite3_finalize(stmt);
}

void AtDisk::create_article(messagehandler& msgHand, unsigned int& id,
                            std::string& t, std::string& a, std::string& txt) {
  cout << "Creating article ... " << endl;

  sqlite3_stmt* stmt;
  string sql =
      "INSERT INTO articles (newsgroup_id, title, author, text) VALUES (" +
      to_string(id) + ", '" + t + "', '" + a + "', '" + txt + "')";
  int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

  rc = sqlite3_step(stmt);
  if (rc == SQLITE_DONE) {
    cout << "Succesfully added article" << endl;
    msgHand.sendCode(Protocol::ANS_ACK);
  } else {
    msgHand.sendCode(Protocol::ANS_NAK);
    msgHand.sendCode(Protocol::ERR_NG_ALREADY_EXISTS);
  }

  sqlite3_finalize(stmt);
}

void AtDisk::delete_article(messagehandler& msgHand, unsigned int& grId,
                            unsigned int& artId) {
  cout << "Deleting article ... " << endl;

  sqlite3_stmt* stmt;
  string sql = "DELETE FROM articles WHERE id = " + to_string(artId) +
               " AND newsgroup_id = " + to_string(grId);
  int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

  rc = sqlite3_step(stmt);
  if (rc == SQLITE_DONE) {
    msgHand.sendCode(Protocol::ANS_ACK);
  } else {
    msgHand.sendCode(Protocol::ANS_NAK);
    msgHand.sendCode(Protocol::ERR_ART_DOES_NOT_EXIST);
  }

  sqlite3_finalize(stmt);
}

void AtDisk::get_article(messagehandler& msgHand, unsigned int& grId,
                         unsigned int& artId) {
  cout << "Getting article ... " << endl;

  sqlite3_stmt* stmt;
  string sql = "SELECT title, author, text FROM articles WHERE id = " +
        to_string(artId) + " AND newsgroup_id = " + to_string(grId);
  cout << sql << endl;
  int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

  rc = sqlite3_step(stmt);
  if (rc == SQLITE_ROW) {
    auto title = std::string(
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
    auto author = std::string(
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
    auto text = std::string(
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));

    msgHand.sendCode(Protocol::ANS_ACK);
    msgHand.sendString(title);
    msgHand.sendString(author);
    msgHand.sendString(text);
  } else {
    msgHand.sendCode(Protocol::ANS_NAK);
    msgHand.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
  }

  sqlite3_finalize(stmt);
}
