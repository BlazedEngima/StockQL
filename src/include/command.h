#ifndef __COMMAND_H
#define __COMMAND_H 
#pragma once

#include "../include/pager.h"
#include <algorithm>
#include <memory>
#include <cctype>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>

/* Typedefs */
typedef enum {META_COMMAND_SUCCESS, META_COMMAND_UNRECOGNIZED, META_COMMAND_QUIT} MetaCommandResult;
typedef enum {
    STATEMENT_ADD, STATEMENT_REMOVE, STATEMENT_TRADE, STATEMENT_QUERY, 
    STATEMENT_EXIT, STATEMENT_COMMAND_UNRECOGNIZED, STATEMENT_EMPTY    
} StatementType;
typedef enum {
    ASK1P, ASK1Q, ASK2P, ASK2Q, ASK3P, ASK3Q, ASK4P, ASK4Q, ASK5P, ASK5Q,
    BID1P, BID1Q, BID2P, BID2Q, BID3P, BID3Q, BID4P, BID4Q, BID5P, BID5Q,
    LAST_TRADED_PRICE, LAST_TRADED_QUANTITY, ALL, TOP_5
} QueryType;
typedef std::unordered_map<std::string, StatementType> WordMap;
typedef std::unordered_map<std::string, Side> SideMap;
typedef std::unordered_map<std::string, QueryType> QueryMap;
typedef std::shared_ptr<OrderBook> OrderBookPtr;
typedef std::vector<std::string> StringVector;


/* Command Class */
class Command {
    private:
        std::string text; // The text of the command
        uint64_t epoch; // The epoch of the current command
        uint64_t last_epoch; // The epoch of the last add/remove command
        std::string symbol; // The name of tradable instruments
        Side side; // The side of the order
        StatementType type; // The type of the command
        float price; // The price of the order
        unsigned int quantity; // The quantity of the order

        /*
        *   command_args - a vector to store the arguments of the command
        *   command arguments could be data fields to be added to the order book
        *   or a query list of fields to be printed
        */
        StringVector command_args;

        /*
        *   table - A pointer to a Table class
        *   Currently effectively only an unordered map to tables
        */
        Table *table;

        /*
        *   toLower() - Converts the text of the command to lowercase
        */
        void toLower();

        /*
        *   parse() - Parses the command
        */
        void parse();

        /*
        *   handle_query(std::ostream &) - Handles the query command
        */
        void handle_query(std::ostream &);

    public:
        /*
        *   Command() - Default constructor for the Command class
        *   initialized text to an empty string and type to STATEMENT_EMPTY
        *   order_book is added via dependency injection otherwise it is nullptr
        */
        Command(Table *table);

        /* Getters and Setters */
        void setType(StatementType);
        std::string getText();
        StatementType getType();

        /*
        *   executeStatement() - Executes the command
        *   Returns a MetaCommandResult enum
        *   META_COMMAND_SUCCESS - The command was executed successfully
        *   META_COMMAND_UNRECOGNIZED - The command was not recognized
        *   META_COMMAND_QUIT - The command was quit
        *   (to be implemented further)
        */
        MetaCommandResult executeStatement(std::ostream &);

        /*
        *   load() - Loads the database from a log file of commands and executes the commands
        */
        void load(std::ifstream &);
        
        /*
        *   operator>>() - Overloads the >> operator for the Command class
        */
        friend std::istream& operator>>(std::istream &, Command &);

        /*
        *   operator<<() - Overloads the << operator for the Command class
        */
        friend std::ostream& operator<<(std::ostream &, const Command &);

};

/*
*   isNumber() - Checks if a string is a number
*   @param in - The string to check
*   @return - True if the string is a number, false otherwise
*/
bool isNumber(const std::string&);

#endif