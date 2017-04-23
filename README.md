# C++-project - News system #

## Dependencies

### [SQLite](https://www.sqlite.org)

An older version of SQLite that works with this project comes included with every distribution of macOS. For Ubuntu the following should work:

``` bash
sudo apt-get install libsqlite3-dev
```

SQLite should be included in most package managers for different Linux distributions.

## Instructions

The program is compiled and built with `make`. `make install` moves the executables to the _bin_ directory. `make clean` removes the object files and executables and clears the database. 

To run the server execute: `./bin/run_server port-number server-version`. Here server-version should be _mem_ for the in-memory version and _disk_ for the disk version. 

To run the client execute: `./bin/client hostname port-number`.
