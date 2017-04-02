DROP TABLE IF EXISTS articles;
DROP TABLE IF EXISTS newsgroups;

CREATE TABLE newsgroups (
    id INTEGER PRIMARY KEY,
    name VARCHAR(100) UNIQUE
);

CREATE TABLE articles(
    id INTEGER PRIMARY KEY,
    title VARCHAR(100),
    author VARCHAR(100),
    text VARCHAR(100),
    newsgroup_id INTEGER,
    FOREIGN KEY(newsgroup_id) REFERENCES newsgroups(id)
);
