#ifndef __COMMAND_H
#define __COMMAND_H 
#pragma once

#include "../include/orderbook.h"
#include <algorithm>
#include <memory>
#include <cctype>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>

/* Typedefs */
typedef enum {META_COMMAND_SUCCESS, META_COMMAND_UNRECOGNIZED, META_COMMAND_QUIT} MetaCommandResult;
typedef enum {STATEMENT_ADD, STATEMENT_REMOVE, STATEMENT_QUERY, STATEMENT_EXIT, STATEMENT_COMMAND_UNRECOGNIZED, STATEMENT_EMPTY} StatementType;
typedef std::unordered_map<std::string, StatementType> WordList;
typedef std::shared_ptr<OrderBook> OrderBookPtr;

/* Command Class */
class Command {
    private:
        std::string text; // The text of the command
        uint64_t epoch; // The epoch of the command
        std::string symbol; // The name of tradable instruments
        Side side; // The side of the order
        StatementType type; // The type of the command
        float price; // The price of the order
        unsigned int quantity; // The quantity of the order

        /*
        *   order_book - A pointer to the order book
        *   Added via dependency injection
        */
        OrderBookPtr order_book;

        /*
        *   toLower() - Converts the text of the command to lowercase
        */
        void toLower();

        /*
        *   parse() - Parses the command
        */
        void parse();

    public:
        /*
        *   Command() - Default constructor for the Command class
        *   initialized text to an empty string and type to STATEMENT_EMPTY
        *   order_book is added via dependency injection otherwise it is nullptr
        */
        Command(OrderBookPtr order_book);
        Command();

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
        MetaCommandResult executeStatement();

        /*
        *   load() - Loads the database from a log file of commands and executes the commands
        */
        void load(std::ifstream &);
        
        /*
        *   operator>>() - Overloads the >> operator for the Command class
        */
        friend std::istream& operator>>(std::istream &, Command &);

};

/*
*   getFirstWord() - Returns the first word of a string
*   @param in - The string to get the first word of
*   @return - The first word of the string
*   Helper function used to determine the type of command
*/
std::string getFirstWord(const std::string&);

#endif