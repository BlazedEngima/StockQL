#ifndef __ORDERBOOK_H
#define __ORDERBOOK_H
#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <map>
#include <fstream>

/* Typedefs */
typedef enum{BUY, SELL} Side;
typedef enum{NEW, CANCEL, TRADE} OrderType;
typedef std::unordered_map<std::string, Side> SideMap;
typedef std::unordered_map<std::string, OrderType> OrderTypeMap;
typedef std::map<float, unsigned int> PriceMap;

/* OrderBook class */
class OrderBook {
    private:
        /*
        *   bids - A map (price, quantity) of the top bids
        *   asks - A map (price, quantity) of the top asks
        */
        PriceMap bids, asks;
        float last_traded_price; // The price of the last trade
        unsigned int last_traded_quantity; // The quantity of the last trade

        /*
        *   printSide() - Prints a side of the orderbook
        *   PriceMap - The side of the orderbook to print
        *   bool - True to print Top 5, false to print all
        */
        void printSide(std::ostream &, Side side, bool) const;

    public:
        /*
        *   isEmpty() - Checks if the orderbook is empty
        */
        bool isEmpty() const;

        /*
        *   set_last_trade() - Sets the last trade price and quantity
        */
        void set_last_trade(float, unsigned int);

        /*
        *   get_last_trade_price() - Returns the last traded price
        */
        float get_last_trade_price() const;

        /*
        *   get_last_trade_quantity() - Returns the last traded quantity
        */
        unsigned int get_last_trade_quantity() const;

        /*
        *   add() - Adds an order to the orderbook
        */
        void add(float, unsigned int, Side);

        /*
        *   remove() - Removes an order from the orderbook
        */
        void remove(float, unsigned int, Side);

        /*
        *   topFive() - Prints the top five bids and asks
        */
        void topFive(std::ostream &) const;

        /*
        *   print_entry() - Prints a specified entry from the order book
        *   Side - The side of the orderbook to print
        *   bool - Choice to print price or quantity, true for price, false for quantity
        *   unsigned int - The index of the entry to print (1 - 5 corresponding to lowest/highest bid/ask)
        */
        void print_entry(std::ostream &, bool, Side, unsigned int) const;

        /*
        *   get_entry() - Returns a specified entry from the order book
        */
        std::string get_entry(bool, Side, unsigned int) const;

        /*
        *   get_query_fields() - Returns a vector of the fields to be queried
        */
        void get_query_fields(std::vector<std::string> &) const;

        /*
        *   print() - Prints the orderbook
        */
        friend std::ostream& operator<<(std::ostream &, const OrderBook &);
};


#endif