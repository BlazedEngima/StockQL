#include "../include/pager.h"

// Table constructor
Table::Table(const std::string &db_name) {
  this->db_name = db_name;
  this->order_book = OrderBook();

  // Create the file streams
  this->curr_db = std::fstream(DATA_PATH + db_name + ".db",
                               std::ios::in | std::ios::out | std::ios::app);
  this->epoch_db = std::fstream(DATA_PATH + db_name + "_epochs.db",
                                std::ios::in | std::ios::out | std::ios::app);
  if (!this->curr_db.is_open() || !this->epoch_db.is_open()) {
    throw std::runtime_error("Could not open file");
  }

  this->epochs = EpochCache(MAX_EPOCHS);

  std::cout << "Loading table..." << std::endl;

  // Load the table
  this->load_table();
}

// Load the table from the file
void Table::load_table() {
  // Parse through the .db file and load it to the order book
  std::vector<std::string> tokens;
  std::string line;

  // Go through the file while tokenizing and parsing every line
  while (std::getline(this->curr_db, line)) {
    // Tokenize and parse
    std::istringstream ss(line);

    tokens = std::vector<std::string>(std::istream_iterator<std::string>{ss},
                                      std::istream_iterator<std::string>());

    // When it reaches the end of bids indicator (//)
    if (tokens.size() < 2) {
      break;
    }

    // Skip if it is the header
    if (tokens[0] == "Price") {
      continue;
    }

    this->order_book.add(std::stof(tokens[0]), std::stoi(tokens[1]), BUY);
    tokens.clear();
  }

  // Load the asks
  while (std::getline(this->curr_db, line)) {
    std::istringstream ss(line);

    tokens = std::vector<std::string>(std::istream_iterator<std::string>{ss},
                                      std::istream_iterator<std::string>());

    // Skip if it is the header
    if (tokens[0] == "Price") {
      continue;
    }

    this->order_book.add(std::stof(tokens[0]), std::stoi(tokens[1]), SELL);
    tokens.clear();
  }
}

// Searches for the epoch in the cache and if it is not found, then it will
// search the file
RecordData Table::search(uint64_t epoch) {
  // Error handling for when the epoch is 0
  // Can be improved
  if (epoch == 0) {
    // Load all RecordData parameters with 0
    std::vector<std::string> tokens;
    for (size_t i = 0; i < 22; i++) {
      tokens.push_back("0");
    }
    return RecordData(tokens);
  }

  // Check to see if the key is in the cache and perform the appropriate action
  if (this->epochs.exists(epoch)) {
    return this->epochs.get(epoch);
  }

  // Move the fstream pointer to the beginning of the file
  this->epoch_db.seekp(0, std::ios::beg);

  // Search the file
  // Have not been tested
  std::string line;
  std::vector<std::string> tokens;

  while (std::getline(this->epoch_db, line)) {
    std::istringstream ss(line);

    tokens = std::vector<std::string>(std::istream_iterator<std::string>{ss},
                                      std::istream_iterator<std::string>());

    if (std::stoull(tokens[0]) == epoch) {
      RecordData data(tokens);
      this->epochs.put(epoch, data);
      return data;
    }

    tokens.clear();
  }

  throw std::runtime_error("Epoch not found");
}

// Writes the order book to the file
void Table::save() {
  // Writes the order book to the file
  this->curr_db.close();
  this->curr_db.open(DATA_PATH + this->db_name + ".db",
                     std::ios::out | std::ios::trunc);
  this->curr_db << this->order_book;
}

// Saves epoch to file
void Table::save_epoch(uint64_t epoch, const RecordData &data) {
  this->epoch_db.seekp(0, std::ios::end);
  this->epoch_db.clear();
  this->epoch_db << epoch << "\t" << data << std::endl;
}

// Add to order book
// epoch values can only be the same if it is continuous
// else epoch will always be new
// Even after querying, epoch will change so most recent epoch will determine
// if epoch is new or not
// Will leave the current instruction in cache and wont write to file until quit
// Issue: won't write out the last epoch to file (Needs refactoring to solve)
void Table::update(uint64_t epoch, uint64_t last_epoch, float price,
                   unsigned int quantity, Side side, bool add) {
  // Update order book
  if (epoch < last_epoch) {
    throw std::runtime_error(
        "New epoch is less than last added epoch which cannot happen");
  }

  // To be updated (will only work if the last added epoch is still in the
  // cache)
  if (epoch > last_epoch && last_epoch != 0) {
    if (this->epochs.exists(last_epoch))
      this->save_epoch(last_epoch, this->epochs.get(last_epoch));
  }

  // Add to the right side of the order book
  if (add)
    this->order_book.add(price, quantity, side);
  else
    this->order_book.remove(price, quantity, side);

  // Get the query fields
  std::vector<std::string> tokens;
  this->order_book.get_query_fields(tokens);
  RecordData data(tokens);

  if (this->epochs.size() == 0) {
    this->epochs.put(epoch, data);
    return;
  }

  // Check to see if the key is in the cache and perform the appropriate action
  if (this->epochs.exists(epoch)) {
    this->epochs.update(epoch, data);
  } else {
    this->epochs.put(epoch, data);
  }
}

// Table destructor
Table::~Table() {
  this->save();

  this->curr_db.close();
  this->epoch_db.close();
}

/* Incomplete functions, thus currently commented */

// Table& Pager::getTable(const std::string &db_name) {
//     // If the entry does not exist, add it
//     if (this->table.find(db_name) == this->table.end()) {
//         this->addTable(db_name);
//     }

//     return this->table[db_name];
// }

// void Pager::addTable(const std::string &db_name) {
//     // Create the entry
//     Table table(db_name);

//     // Add the entry to the table
//     this->table.emplace(db_name, table);
// }

// void Pager::loadTable(const std::string &db_name) {
//     // If entry is already in table, then do nothing
//     if (this->table.find(db_name) != this->table.end()) {
//         return;
//     }

//     // Create the entry and add it to the table
//     // Table table(curr_db, epoch_db);
//     this->table.emplace(db_name, table);
// }
