#ifndef __ORDERBOOK_H
#define __ORDERBOOK_H
#pragma once

#include <iostream>
#include <unordered_map>
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

        /*
        *   printSide() - Prints a side of the orderbook
        *   PriceMap - The side of the orderbook to print
        *   bool - True to print Top 5, false to print all
        */
        void printSide(std::ostream &, const PriceMap&, bool) const;

    public:

        /*
        *   isEmpty() - Checks if the orderbook is empty
        */
        bool isEmpty() const;

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
        void topFive();

        /*
        *   print() - Prints the orderbook
        */
        friend std::ostream& operator<<(std::ostream &, const OrderBook &);
};


#endif