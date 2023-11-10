#ifndef __COMMAND_H
#define __COMMAND_H 
#pragma once

#include <iostream>
#include <algorithm>
#include <cctype>
#include <string>
#include <unordered_map>

/* Typedefs */
typedef enum {META_COMMAND_SUCCESS, META_COMMAND_UNRECOGNIZED, META_COMMAND_QUIT} MetaCommandResult;
typedef enum {STATEMENT_INSERT, STATEMENT_DELETE, STATEMENT_QUERY, STATEMENT_UPDATE, STATEMENT_EXIT, STATEMENT_COMMAND_UNRECOGNIZED, STATEMENT_EMPTY} StatementType;
typedef std::unordered_map<std::string, StatementType> WordList;

/* Command Class */
class Command {
    private:
        std::string text; // The text of the command
        StatementType type; // The type of the command

        /*
        *   toLower() - Converts the text of the command to lowercase
        */
        void toLower();

    public:
        /*
        *   Command() - Default constructor for the Command class
        *   initialized text to an empty string and type to STATEMENT_EMPTY
        */
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