#include "../include/orderbook.h"

// Checks to see if orderbook is empty
bool OrderBook::isEmpty() const {
    return this->bids.empty() && this->asks.empty();
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

    // Perform check to see if the price exists and if there is enough quantity to remove
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
    if (this->bids[price] == 0) { map->erase(price); }

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
    if (side == BUY) {
        for (auto it = map->rbegin(); it != map->rend() && count > 0; it++, count--) {
            os << "\t" << it->first << "\t" << it->second << std::endl;
        }
    } else {
        for (auto it = map->begin(); it != map->end() && count > 0; it++, count--) {
            os << "\t" << it->first << "\t" << it->second << std::endl;
        } 
    }
}

// Wrapper function to print the top five bids and asks
void OrderBook::topFive() {
    if (this->isEmpty()) {
        std::cout << "Orderbook is empty" << std::endl;
        return;
    }

    std::cout << "Top 5 bids:" << std::endl;
    this->printSide(std::cout, BUY, true);

    std::cout << std::endl;

    std::cout << "Top 5 asks:" << std::endl;
    this->printSide(std::cout, SELL, true);
}

// Overloads the << operator to print the whole OrderBook
std::ostream& operator<<(std::ostream &os, const OrderBook &orderbook) {
    // To be changed so it prints out all query fields instead
    if (orderbook.isEmpty()) {
        os << "Orderbook is empty" << std::endl;
        return os;
    }

    os << "Bids:" << std::endl;
    orderbook.printSide(os, BUY, false);

    os << std::endl;

    os << "Asks:" << std::endl;
    orderbook.printSide(os, SELL, false);
    return os;
}