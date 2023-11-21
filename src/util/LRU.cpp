#include "../include/LRU.h"

// Constructor for RecordData struct
RecordData::RecordData(const std::vector<std::string> &tokens) {
  if (tokens.size() != 22) {
    throw std::invalid_argument("Invalid number of tokens");
  }

  // Parse the tokens
  this->ask1p = std::stof(tokens[0]);
  this->ask2p = std::stof(tokens[1]);
  this->ask3p = std::stof(tokens[2]);
  this->ask4p = std::stof(tokens[3]);
  this->ask5p = std::stof(tokens[4]);
  this->bid1p = std::stof(tokens[5]);
  this->bid2p = std::stof(tokens[6]);
  this->bid3p = std::stof(tokens[7]);
  this->bid4p = std::stof(tokens[8]);
  this->bid5p = std::stof(tokens[9]);
  this->ask1q = std::stoi(tokens[10]);
  this->ask2q = std::stoi(tokens[11]);
  this->ask3q = std::stoi(tokens[12]);
  this->ask4q = std::stoi(tokens[13]);
  this->ask5q = std::stoi(tokens[14]);
  this->bid1q = std::stoi(tokens[15]);
  this->bid2q = std::stoi(tokens[16]);
  this->bid3q = std::stoi(tokens[17]);
  this->bid4q = std::stoi(tokens[18]);
  this->bid5q = std::stoi(tokens[19]);
  this->last_traded_price = std::stof(tokens[20]);
  this->last_traded_quantity = std::stoi(tokens[21]);
}

// Overload the << operator for RecordData for printing to file
std::ostream &operator<<(std::ostream &os, const RecordData &data) {
  os << data.ask1p << " ";
  os << data.ask2p << " ";
  os << data.ask3p << " ";
  os << data.ask4p << " ";
  os << data.ask5p << " ";
  os << data.bid1p << " ";
  os << data.bid2p << " ";
  os << data.bid3p << " ";
  os << data.bid4p << " ";
  os << data.bid5p << " ";
  os << data.ask1q << " ";
  os << data.ask2q << " ";
  os << data.ask3q << " ";
  os << data.ask4q << " ";
  os << data.ask5q << " ";
  os << data.bid1q << " ";
  os << data.bid2q << " ";
  os << data.bid3q << " ";
  os << data.bid4q << " ";
  os << data.bid5q << " ";
  os << data.last_traded_price << " ";
  os << data.last_traded_quantity;

  return os;
}

// Constructor for LRUCache
LRUCache::LRUCache(size_t capacity) { this->capacity = capacity; }
LRUCache::LRUCache() { this->capacity = 1000; }

unsigned int LRUCache::size() const {
  return this->cache_map.size();
}  // Get size of LRUCache

// Helper function to clean the cache
void LRUCache::clean() {
  while (this->cache_map.size() > this->capacity) {
    // Get the key of the last element
    auto last = this->cache_list.end();
    last--;
    this->cache_map.erase(last->first);
    this->cache_list.pop_back();
  }
}

// Get the value of the key
RecordData LRUCache::get(const uint64_t &key) {
  // Check if the key exists in the cache
  auto it = cache_map.find(key);
  if (it == cache_map.end()) {
    throw std::runtime_error("Key does not exist");
  }

  // Move the key to the front of the list
  this->cache_list.splice(this->cache_list.begin(), this->cache_list,
                          it->second);

  // Return the value
  return it->second->second;
}

// Check if the key exists in the cache
bool LRUCache::exists(const uint64_t &key) {
  // First checks the most recent key
  if (this->cache_list.front().first == key) {
    return true;
  }

  return this->cache_map.find(key) != this->cache_map.end();
}

// Add a key-value pair to the cache
void LRUCache::put(const uint64_t &key, const RecordData &value) {
  // Check if the key exists in the cache
  auto it = cache_map.find(key);
  if (it != cache_map.end()) {
    // Erase the value from the cache to update
    cache_list.erase(it->second);
    cache_map.erase(it);
  }

  // Insert the updated key-value pair
  cache_list.emplace_front(key, value);
  cache_map.emplace(key, cache_list.begin());
  this->clean();
}

// Update the value of the key in the cache
void LRUCache::update(const uint64_t &key, const RecordData &value) {
  // Check if the key exists in the cache
  auto it = cache_map.find(key);
  if (it == cache_map.end()) {
    std::cout << "Key does not exist" << std::endl;
    return;
  }

  // Update the value
  it->second->second = value;

  // Move the key to the front of the list
  this->cache_list.splice(this->cache_list.begin(), this->cache_list,
                          it->second);
}

// Return the least recently used key in the cache
uint64_t LRUCache::get_oldest_key() const {
  return this->cache_list.back().first;
}

// Return the most recently used key in the cache
uint64_t LRUCache::get_recent_key() const {
  return this->cache_list.front().first;
}
