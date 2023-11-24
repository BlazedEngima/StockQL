#ifndef __PAGER_H
#define __PAGER_H
#pragma once

#include <fstream>
#include <iterator>
#include <list>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

#include "../include/LRU.h"
#include "../include/orderbook.h"

#define DATA_PATH "data/db/"
#define MAX_EPOCHS 1000000

// Typedefs
typedef std::pair<uint64_t, RecordData> Node;
typedef std::list<Node> EpochList;
typedef std::list<uint64_t> EpochQueue;
typedef LRUCache EpochCache;

class Table {
 private:
  /*
   *   db_name - The name of the database file
   *   epochs - The cache of epochs
   *   curr_db, epoch_db - The file streams for the database and epochs
   */
  std::string db_name;
  EpochCache epochs;
  std::fstream curr_db, epoch_db;

  /*
   *   save() - Saves the order book to the database file
   */
  void save();

  /*
   *   load_table() - Loads the table from the database file
   */
  void load_table();

  /*
   *   save_epoch() - Saves the epoch to the epoch database file
   */
  void save_epoch(uint64_t, const RecordData &);

 public:
  /*
   *   order_book - The order book of the table
   *   Ideally placed in private but not enough time
   *   to properly provide functionality for the Table class
   *   to access the order book without making it public
   */
  OrderBook order_book;

  /*
   *   Table constructor - will load the table from the database file
   *   If the file does not exist, it will create a new one
   */
  Table(const std::string &);

  /*
   *   update() - Updates the order book with the given data
   *   uint64_t - The epoch and last epoch of the data
   *   float - The price
   *   unsigned int - The quantity
   *   Side - The side of the order
   *   bool - True if the order is a trade, false otherwise
   *
   *   To do: Update to work for multiple symbols, solve bug
   */
  void update(uint64_t, uint64_t, float, unsigned int, Side, bool);

  /*
   *   search() - Searches for the given epoch in the cache
   *   uint64_t - The epoch to search for
   *   Searches the data sequentially from the start of the file
   *
   *   To do: Make searching more efficient (Use B-Trees)
   */
  RecordData search(uint64_t);

  /*
   *   ~Table() - Destructor for the table
   *   Saves the order book to the database file on destruction
   */
  ~Table();
};

/* Pager class to load different tables of different symbols */
/* Not complete, thus currently unused */

// typedef std::unordered_map<std::string, Table> TableMap;

// class Pager {
//     private:
//         TableMap table;

//     public:
//         Table& getTable(const std::string &);

//         void addTable(const std::string &);
//         void loadTable(const std::string &);

// };

#endif
