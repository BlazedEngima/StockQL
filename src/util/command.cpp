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
        {"Q", STATEMENT_EXIT}
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
Command::Command() { 
    this->type = STATEMENT_EMPTY;
    this->price = 0.0;
    this->quantity = 0;
}

Command::Command(OrderBookPtr order_book) {
    this->type = STATEMENT_EMPTY;
    this->order_book = order_book;
    this->price = 0.0;
    this->quantity = 0;
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
        this->side = (this->command_args[3] == "BUY") ? BUY : SELL;
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
            try {
                this->symbol = this->command_args[1];
                this->epoch = std::stoull(this->command_args[2]);
            } catch (const std::invalid_argument&) {
                this->type = STATEMENT_COMMAND_UNRECOGNIZED;
                return;
            }

            break;
        }

        /* For debugging */
        default:
            break;
    }
}

void Command::handle_query(std::ostream &os) {
    // Checks to see if the epoch exists in the order book
    // if (this->order_book->at(this->symbol).find(this->epoch) == this->order_book->at(this->symbol).end()) {
    //     std::cout << "No such epoch exists" << std::endl;
    //     return;
    // }


    for (size_t i = 3; i < this->command_args.size(); i++) {
        if (query_fields.find(this->command_args[i]) == query_fields.end()) {
            std::cout << "No such query field exists" << std::endl;
            return;
        }

        // std::cout << "Test" << std::endl;
        switch (query_fields.at(this->command_args[i])) {
            case ALL: {
                os << *(this->order_book) << std::endl;
                break;
            }

            case TOP_5: {
                this->order_book->topFive(os);
                break;
            }

            case LAST_TRADED_PRICE: {
                os << this->order_book->get_last_trade_price() << std::endl;
                break;
            }

            case LAST_TRADED_QUANTITY: {
                os << this->order_book->get_last_trade_quantity() << std::endl;
                break;
            }

            case ASK1P: {
                // std::cout << "Test" << std::endl;
                this->order_book->print_entry(os, true, SELL, 1);
                break;
            }

            case ASK1Q: {
                this->order_book->print_entry(os, false, SELL, 1);
                break;
            }

            case ASK2P: {
                this->order_book->print_entry(os, true, SELL, 2);
                break;
            }

            case ASK2Q: {
                this->order_book->print_entry(os, false, SELL, 2);
                break;
            }

            case ASK3P: {
                this->order_book->print_entry(os, true, SELL, 3);
                break;
            }

            case ASK3Q: {
                this->order_book->print_entry(os, false, SELL, 3);
                break;
            }

            case ASK4P: {
                this->order_book->print_entry(os, true, SELL, 4);
                break;
            }

            case ASK4Q: {
                this->order_book->print_entry(os, false, SELL, 4);
                break;
            }

            case ASK5P: {
                this->order_book->print_entry(os, true, SELL, 5);
                break;
            }

            case ASK5Q: {
                this->order_book->print_entry(os, false, SELL, 5);
                break;
            }

            case BID1P: {
                this->order_book->print_entry(os, true, BUY, 1);
                break;
            }
            
            case BID1Q: {
                this->order_book->print_entry(os, false, BUY, 1);
                break;
            }

            case BID2P: {
                this->order_book->print_entry(os, true, BUY, 2);
                break;
            }

            case BID2Q: {
                this->order_book->print_entry(os, false, BUY, 2);
                break;
            }

            case BID3P: {
                this->order_book->print_entry(os, true, BUY, 3);
                break;
            }

            case BID3Q: {
                this->order_book->print_entry(os, false, BUY, 3);
                break;
            }

            case BID4P: {
                this->order_book->print_entry(os, true, BUY, 4);
                break;
            }

            case BID4Q: {
                this->order_book->print_entry(os, false, BUY, 4);
                break;
            }

            case BID5P: {
                this->order_book->print_entry(os, true, BUY, 5);
                break;
            }

            case BID5Q: {
                this->order_book->print_entry(os, false, BUY, 5);
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
    os << *(command.order_book);
    return os;
}

// Executes the command
MetaCommandResult Command::executeStatement(std::ostream &os) {
    /* To be implmented further */

    switch (this->type) {
        case STATEMENT_EXIT: {
            return META_COMMAND_QUIT;
        }

        case STATEMENT_ADD: {
            this->order_book->add(this->price, this->quantity, this->side);
            return META_COMMAND_SUCCESS;
        }

        case STATEMENT_QUERY: {
            this->handle_query(os);
            return META_COMMAND_SUCCESS;
        }

        case STATEMENT_TRADE: {
            order_book->set_last_trade(this->price, this->quantity);
        }

        case STATEMENT_REMOVE: {
            this->order_book->remove(this->price, this->quantity, this->side);
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