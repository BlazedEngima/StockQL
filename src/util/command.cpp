#include "../include/command.h"

// List of keywords for database operations and their corresponding StatementType
WordList keywords = {
        {"insert", STATEMENT_INSERT},
        {"delete", STATEMENT_DELETE},
        {"select", STATEMENT_QUERY},
        {"exit", STATEMENT_EXIT},
        {"q", STATEMENT_EXIT},
        {"update", STATEMENT_UPDATE}
};

// Getters and Setters for Command class as well as its Constructor
Command::Command() { this->type = STATEMENT_EMPTY; this->text = ""; }
void Command::setType(StatementType type) { this->type = type; }
std::string Command::getText() { return this->text; }
StatementType Command::getType() { return this-> type; }

// Helper function to get the first word of a string
std::string getFirstWord(const std::string &in) {
    size_t start = in.find_first_not_of(" \t");
    size_t end = in.find_first_of(" \t", start);

    return in.substr(start, end - start);
}

// Converts the text of the command to lowercase
void Command::toLower() {
    std::transform(this->text.begin(), this->text.end(), this->text.begin(),
                    [](unsigned char c) { return std::tolower(c); });
}

// Overloads the >> operator for the Command class
std::istream& operator>>(std::istream &istream, Command &command) {
    std::string first_word;
    std::getline(istream, command.text);

    if (command.text.empty()) {
        command.setType(STATEMENT_EMPTY);
        return istream;
    }

    command.toLower();
    first_word = getFirstWord(command.getText());

    if (keywords.find(first_word) == keywords.end()) { command.setType(STATEMENT_COMMAND_UNRECOGNIZED); }
    else { command.setType(keywords.at(first_word)); }

    return istream;
}

// Executes the command
MetaCommandResult Command::executeStatement() {
    /* To be implmented further */
    if (this->type == STATEMENT_EXIT) { return META_COMMAND_QUIT; }

    return (this->type == STATEMENT_COMMAND_UNRECOGNIZED) ? META_COMMAND_UNRECOGNIZED : META_COMMAND_SUCCESS;
}