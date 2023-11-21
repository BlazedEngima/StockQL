#include "../include/orderbook.h"

// Checks to see if orderbook is empty
bool OrderBook::isEmpty() const {
  return this->bids.empty() && this->asks.empty();
}

// Sets last trade price and quantity
void OrderBook::set_last_trade(float price, unsigned int quantity) {
  this->last_traded_price = price;
  this->last_traded_quantity = quantity;
}

// Getters for last trade price and quantity
float OrderBook::get_last_trade_price() const {
  return this->last_traded_price;
}
unsigned int OrderBook::get_last_trade_quantity() const {
  return this->last_traded_quantity;
}

// Add value to the orderbook
void OrderBook::add(float price, unsigned int quantity, Side side) {
  // Add to the appropriate side
  PriceMap *map = (side == BUY) ? &(this->bids) : &(this->asks);

  // If the price does not exist, then make a new key-value pair
  (*map)[price] += quantity;
}

// Remove value from the orderbook
void OrderBook::remove(float price, unsigned int quantity, Side side) {
  // Select the appropriate side
  PriceMap *map = (side == BUY) ? &(this->bids) : &(this->asks);

  // Perform check to see if the price exists and if there is enough quantity to
  // remove
  if (map->find(price) == map->end()) {
    std::cout << "No such price exists" << std::endl;
    return;
  }

  if (map->at(price) < quantity) {
    std::cout << "Not enough quantity to remove" << std::endl;
    return;
  }

  // Remove the quantity from the price
  map->at(price) -= quantity;
  if (map->at(price) == 0) {
    map->erase(price);
  }
}

// Helper function to print a side of the orderbook
void OrderBook::printSide(std::ostream &os, Side side, bool topFive) const {
  const PriceMap *map = (side == BUY) ? &(this->bids) : &(this->asks);

  if (map->empty()) {
    os << "\tNo orders" << std::endl;
    return;
  }

  os << "\tPrice\tQuantity" << std::endl;

  // Print the top five or all of the orders
  int count = (topFive) ? 5 : map->size();

  // Print largest to smallest if side is BUY, vice versa for SELL
  if (side == BUY) {
    for (auto it = map->rbegin(); it != map->rend() && count > 0;
         it++, count--) {
      os << "\t" << it->first << "\t" << it->second << std::endl;
    }
  } else {
    for (auto it = map->begin(); it != map->end() && count > 0; it++, count--) {
      os << "\t" << it->first << "\t" << it->second << std::endl;
    }
  }
}

// Wrapper function to print the top five bids and asks
void OrderBook::topFive(std::ostream &os) const {
  if (this->isEmpty()) {
    std::cout << "Orderbook is empty" << std::endl;
    return;
  }

  os << "Top 5 bids:" << std::endl;
  this->printSide(os, BUY, true);

  os << std::endl;

  os << "Top 5 asks:" << std::endl;
  this->printSide(os, SELL, true);
}

// Print a specified entry from the order book
void OrderBook::print_entry(std::ostream &os, bool price, Side side,
                            unsigned int index) const {
  os << this->get_entry(price, side, index) << std::endl;
}

// Get a specified entry from the order book
std::string OrderBook::get_entry(bool price, Side side,
                                 unsigned int index) const {
  const PriceMap *map = (side == BUY) ? &(this->bids) : &(this->asks);

  if (map->empty()) {
    return "0";
  }

  // Choose to begin at the start or at the end of the map
  if (side == BUY) {
    auto it = map->rbegin();
    std::advance(it, index - 1);

    // Print price or quantity
    if (price) {
      return std::to_string(it->first);
    } else {
      return std::to_string(it->second);
    }

  } else {
    auto it = map->begin();
    std::advance(it, index - 1);

    if (price) {
      return std::to_string(it->first);
    } else {
      return std::to_string(it->second);
    }
  }
}

// Get the query fields for the order book
void OrderBook::get_query_fields(std::vector<std::string> &tokens) const {
  tokens.push_back(this->get_entry(true, SELL, 1));
  tokens.push_back(this->get_entry(true, SELL, 2));
  tokens.push_back(this->get_entry(true, SELL, 3));
  tokens.push_back(this->get_entry(true, SELL, 4));
  tokens.push_back(this->get_entry(true, SELL, 5));
  tokens.push_back(this->get_entry(true, BUY, 1));
  tokens.push_back(this->get_entry(true, BUY, 2));
  tokens.push_back(this->get_entry(true, BUY, 3));
  tokens.push_back(this->get_entry(true, BUY, 4));
  tokens.push_back(this->get_entry(true, BUY, 5));
  tokens.push_back(this->get_entry(false, SELL, 1));
  tokens.push_back(this->get_entry(false, SELL, 2));
  tokens.push_back(this->get_entry(false, SELL, 3));
  tokens.push_back(this->get_entry(false, SELL, 4));
  tokens.push_back(this->get_entry(false, SELL, 5));
  tokens.push_back(this->get_entry(false, BUY, 1));
  tokens.push_back(this->get_entry(false, BUY, 2));
  tokens.push_back(this->get_entry(false, BUY, 3));
  tokens.push_back(this->get_entry(false, BUY, 4));
  tokens.push_back(this->get_entry(false, BUY, 5));
  tokens.push_back(std::to_string(this->get_last_trade_price()));
  tokens.push_back(std::to_string(this->get_last_trade_quantity()));
}

// Overloads the << operator to print the whole OrderBook
std::ostream &operator<<(std::ostream &os, const OrderBook &orderbook) {
  // To be changed so it prints out all query fields instead
  if (orderbook.isEmpty()) {
    os << "Orderbook is empty" << std::endl;
    return os;
  }

  orderbook.printSide(os, BUY, false);

  // Indicator between bids and asks
  os << "//" << std::endl;

  orderbook.printSide(os, SELL, false);
  return os;
}
