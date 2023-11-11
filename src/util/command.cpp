#include "../include/command.h"

// List of keywords for database operations and their corresponding StatementType
// Contains SQL-like command directives, not all are implemented
const static WordMap keywords = {
        {"INSERT", STATEMENT_ADD},
        {"insert", STATEMENT_ADD},
        {"ADD", STATEMENT_ADD},
        {"add", STATEMENT_ADD},
        {"NEW", STATEMENT_ADD},
        {"new", STATEMENT_ADD},
        {"TRADE", STATEMENT_TRADE},
        {"trade", STATEMENT_TRADE},
        {"DELETE", STATEMENT_REMOVE},
        {"delete", STATEMENT_REMOVE},
        {"CANCEL", STATEMENT_REMOVE},
        {"cancel", STATEMENT_REMOVE},
        {"SELECT", STATEMENT_QUERY},
        {"select", STATEMENT_QUERY},
        {"EXIT", STATEMENT_EXIT},
        {"exit", STATEMENT_EXIT},
        {"q", STATEMENT_EXIT},
        {"Q", STATEMENT_EXIT},
        {"quit", STATEMENT_EXIT},
        {"QUIT", STATEMENT_EXIT}
};

// List of query fields and their corresponding QueryType
const static QueryMap query_fields = {
    {"ASK1P", ASK1P}, {"ASK1Q", ASK1Q},
    {"ASK2P", ASK2P}, {"ASK2Q", ASK2Q},
    {"ASK3P", ASK3P}, {"ASK3Q", ASK3Q},
    {"ASK4P", ASK4P}, {"ASK4Q", ASK4Q},
    {"ASK5P", ASK5P}, {"ASK5Q", ASK5Q},
    {"BID1P", BID1P}, {"BID1Q", BID1Q},
    {"BID2P", BID2P}, {"BID2Q", BID2Q},
    {"BID3P", BID3P}, {"BID3Q", BID3Q},
    {"BID4P", BID4P}, {"BID4Q", BID4Q},
    {"BID5P", BID5P}, {"BID5Q", BID5Q},
    {"ask1p", ASK1P}, {"ask1q", ASK1Q},
    {"ask2p", ASK2P}, {"ask2q", ASK2Q},
    {"ask3p", ASK3P}, {"ask3q", ASK3Q},
    {"ask4p", ASK4P}, {"ask4q", ASK4Q},
    {"ask5p", ASK5P}, {"ask5q", ASK5Q},
    {"bid1p", BID1P}, {"bid1q", BID1Q},
    {"bid2p", BID2P}, {"bid2q", BID2Q},
    {"bid3p", BID3P}, {"bid3q", BID3Q},
    {"bid4p", BID4P}, {"bid4q", BID4Q},
    {"bid5p", BID5P}, {"bid5q", BID5Q},
    {"LAST_TRADE_PRICE", LAST_TRADED_PRICE},
    {"last_trade_price", LAST_TRADED_PRICE},
    {"LAST_TRADE_QUANTITY", LAST_TRADED_QUANTITY},
    {"last_trade_quantity", LAST_TRADED_QUANTITY},
    {"*", ALL}, {"top", TOP_5}, {"TOP", TOP_5}
};

// Command class constructors
Command::Command(Table *table) { 
    this->type = STATEMENT_EMPTY;
    this->table = table;
    this->price = 0.0;
    this->quantity = 0;
    this->epoch = 0;
    this->last_epoch = 0;
}

// Getters and Setters
void Command::setType(StatementType type) { this->type = type; }
std::string Command::getText() { return this->text; }
StatementType Command::getType() { return this-> type; }

// Helper function to check if a string is a number
bool isNumber(const std::string &in) {
    return !in.empty() && std::find_if(in.begin(), 
        in.end(), [](unsigned char c) { return !std::isdigit(c); }) == in.end();
}

// Converts the text of the command to lowercase
void Command::toLower() {
    std::transform(this->text.begin(), this->text.end(), this->text.begin(),
                    [](unsigned char c) { return std::tolower(c); });
}

// Helper function to parse the command
void Command::parse() {
    // Checks to see if the command is a log or a query 
    // Differentiates the two by checking if the first token is a number
    if (isNumber(this->command_args[0])) {
        if (this->command_args.size() != 7) {
            this->type = STATEMENT_COMMAND_UNRECOGNIZED;
            return;
        }

        // If it is a log, then treat it as an add command and insert it into the order book
        this->epoch = std::stoull(this->command_args[0]);
        this->symbol = this->command_args[2];
        this->side = (this->command_args[3] == "BUY" || this->command_args[3] == "buy") ? BUY : SELL;
        this->type = (keywords.find(this->command_args[4]) == keywords.end()) ? STATEMENT_COMMAND_UNRECOGNIZED : keywords.at(this->command_args[4]);
        this->price = std::stof(this->command_args[5]);
        this->quantity = std::stoi(this->command_args[6]);
        return;
    }

    // Command must be a query
    if (keywords.find(this->command_args[0]) == keywords.end()) {
        this->type = STATEMENT_COMMAND_UNRECOGNIZED;
        return;
    }
    
    this->type = keywords.at(this->command_args[0]);

    // Parses the command based on the number of tokens
    switch (this->type) {
        case STATEMENT_QUERY: {
            this->symbol = this->command_args[1];

            if (this->command_args.size() == 3) {
                this->epoch = 0;

                // Increase length of command_args to 4 to account for the query field
                // Improvements to be made here
                this->command_args.push_back(this->command_args[2]);
                return;
            }

            this->epoch = std::stoull(this->command_args[2]);
            break;
        }

        /* To be implemented further */
        default:
            break;
    }
}

void Command::handle_query(std::ostream &os) {
    // Search epoch
    RecordData query = this->table->search(this->epoch);

    // If not then search and load it from file
    for (size_t i = 3; i < this->command_args.size(); i++) {
        if (query_fields.find(this->command_args[i]) == query_fields.end()) {
            std::cout << "No such query field exists" << std::endl;
            return;
        }

        // Switch statment to handle the different query fields
        // Can be improved
        switch (query_fields.at(this->command_args[i])) {
            /* Not done, needs further implmentation */
            case ALL: {
                os << this->table->order_book << std::endl;
                break;
            }

            case TOP_5: {
                this->table->order_book.topFive(os);
                break;
            }

            case LAST_TRADED_PRICE: {
                os << query.last_traded_price << std::endl;
                break;
            }

            case LAST_TRADED_QUANTITY: {
                os << query.last_traded_quantity << std::endl;
                break;
            }

            case ASK1P: {
                os << query.ask1p << std::endl;
                break;
            }

            case ASK1Q: {
                os << query.ask1q << std::endl;
                break;
            }

            case ASK2P: {
                os << query.ask2p << std::endl;
                break;
            }

            case ASK2Q: {
                os << query.ask2q << std::endl;
                break;
            }

            case ASK3P: {
                os << query.ask3p << std::endl;
                break;
            }

            case ASK3Q: {
                os << query.ask3q << std::endl;
                break;
            }

            case ASK4P: {
                os << query.ask4p << std::endl;
                break;
            }

            case ASK4Q: {
                os << query.ask4q << std::endl;
                break;
            }

            case ASK5P: {
                os << query.ask5p << std::endl;
                break;
            }

            case ASK5Q: {
                os << query.ask5q << std::endl;
                break;
            }

            case BID1P: {
                os << query.bid1p << std::endl;
                break;
            }
            
            case BID1Q: {
                os << query.bid1q << std::endl;
                break;
            }

            case BID2P: {
                os << query.bid2p << std::endl;
                break;
            }

            case BID2Q: {
                os << query.bid2q << std::endl;
                break;
            }

            case BID3P: {
                os << query.bid3p << std::endl;
                break;
            }

            case BID3Q: {
                os << query.bid3q << std::endl;
                break;
            }

            case BID4P: {
                os << query.bid4p << std::endl;
                break;
            }

            case BID4Q: {
                os << query.bid4q << std::endl;
                break;
            }

            case BID5P: {
                os << query.bid5p << std::endl;
                break;
            }

            case BID5Q: {
                os << query.bid5q << std::endl;
                break;
            }
        }
    }
}

// Overloads the >> operator for the Command class
std::istream& operator>>(std::istream &istream, Command &command) {
    // Get string from input stream
    std::getline(istream, command.text);

    if (command.text.empty()) {
        command.setType(STATEMENT_EMPTY);
        return istream;
    }

    std::istringstream iss(command.text);

    // Tokenize the command into a vector
    command.command_args = StringVector(
        std::istream_iterator<std::string>{iss},
        std::istream_iterator<std::string>()
    );

    command.parse();

    return istream;
}

// Overloads the << operator for the Command class to print the order book
std::ostream& operator<<(std::ostream &os, const Command &command) {
    os << command.table->order_book;

    return os;
}

// Executes the command
MetaCommandResult Command::executeStatement(std::ostream &os) {

    // Execute statement based on the type of command
    // To be implemented further
    // Missing functionality for multiple symbols as Pager class is not complete

    switch (this->type) {
        case STATEMENT_EXIT: {
            this->table->~Table();
            return META_COMMAND_QUIT;
        }

        case STATEMENT_ADD: {
            this->table->update(this->epoch, this->last_epoch, this->price, this->quantity, this->side, true);
            this->last_epoch = this->epoch;
            return META_COMMAND_SUCCESS;
        }

        case STATEMENT_QUERY: {
            this->handle_query(os);
            return META_COMMAND_SUCCESS;
        }

        case STATEMENT_TRADE: {
            this->table->order_book.set_last_trade(this->price, this->quantity);
        }

        case STATEMENT_REMOVE: {
            this->table->update(this->epoch, this->last_epoch, this->price, this->quantity, this->side, false);
            return META_COMMAND_SUCCESS;
        }

        case STATEMENT_COMMAND_UNRECOGNIZED: {
            return META_COMMAND_UNRECOGNIZED;
        }

        default: {
            return META_COMMAND_SUCCESS;
        }
    }

    this->command_args.clear();

    return META_COMMAND_SUCCESS;
}

// Loads the database from a log file of commands and executes the commands
void Command::load(std::ifstream &file) {
    // To be implemented further
    // Needs proper command parsing
    while (file >> *this) {
        switch (this->executeStatement(std::cout)) {
            case META_COMMAND_SUCCESS: {
                break;
            }
            
            case META_COMMAND_UNRECOGNIZED: {
                std::cout << "Unrecognized command " << this->getText() << std::endl;
                break;
            }

            case META_COMMAND_QUIT: {
                exit(EXIT_SUCCESS);
                return;
                break;
            }
        }
    }

    file.close();
}