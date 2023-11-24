#ifndef __LRU_H
#define __LRU_H
#pragma once

#include <iostream>
#include <iterator>
#include <list>
#include <unordered_map>
#include <utility>
#include <vector>

// Data structure for the record data used to store query values for each epoch
typedef struct RecordData {
  float ask1p, ask2p, ask3p, ask4p, ask5p;
  float bid1p, bid2p, bid3p, bid4p, bid5p;
  unsigned int ask1q, ask2q, ask3q, ask4q, ask5q;
  unsigned int bid1q, bid2q, bid3q, bid4q, bid5q;
  float last_traded_price;
  unsigned int last_traded_quantity;

  RecordData(const std::vector<std::string> &);
  friend std::ostream &operator<<(std::ostream &, const RecordData &);

} RecordData;

class LRUCache {
 private:
  /*
   *   A doubly linked-list of key-value pairs, serves as the cache
   */
  std::list<std::pair<uint64_t, RecordData>> cache_list;

  /*
   *   A map of key to the corresponding node in the cache_list serves as a
   * lookup table
   */
  std::unordered_map<uint64_t,
                     std::list<std::pair<uint64_t, RecordData>>::iterator>
      cache_map;

  /*
   *   The maximum capacity of the cache
   */
  size_t capacity;

  /*
   *   Helper function that moves performs cleanup to ensure
   *   that the cache does not exceed its capacity
   */
  void clean();

 public:
  /*
   *   Constructors for the LRUCache
   *   @param capacity: the maximum capacity of the cache
   *   @default constructor: sets the capacity to 1000
   */
  LRUCache(size_t);
  LRUCache();

  /*
   *   get() returns the value of the key
   */
  RecordData get(const uint64_t &);

  /*
   *   exists() checks if the key exists in the cache
   */
  bool exists(const uint64_t &);

  /*
   *   put() adds a key-value pair to the cache
   */
  void put(const uint64_t &, const RecordData &);

  /*
   *   size() returns the size of the cache
   */
  unsigned int size() const;

  /*
   *   update() updates the value of the key in the cache
   */
  void update(const uint64_t &, const RecordData &);

  /*
   *   get_oldest_key() returns the least recently used key in the cache
   */
  uint64_t get_oldest_key() const;

  /*
   *   get_recent_key() returns the most recently used key in the cache
   */
  uint64_t get_recent_key() const;
};

#endif
