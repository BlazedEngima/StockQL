#include "../include/command.h"

// List of keywords for database operations and their corresponding StatementType
const static WordList keywords = {
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

// Getters and Setters for Command class as well as its Constructors
Command::Command() { this->type = STATEMENT_EMPTY; }
Command::Command(OrderBookPtr order_book) { this->type = STATEMENT_EMPTY; this->order_book = order_book; }
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

// Helper function to parse the command
void Command::parse() {
    // Transforms string to istringstream then tokenizes it into a vector
    std::istringstream iss(this->text);
    std::vector<std::string> values(
        std::istream_iterator<std::string>{iss},
        std::istream_iterator<std::string>()
    );
    
    // Parses the command based on the number of tokens
    switch (values.size()) {
        case 7: {
            this->epoch = std::stoull(values[0]);
            this->symbol = values[2];
            this->side = (values[3] == "BUY") ? BUY : SELL;
            this->type = (keywords.find(values[4]) == keywords.end()) ? STATEMENT_COMMAND_UNRECOGNIZED : keywords.at(values[4]);
            this->price = std::stof(values[5]);
            this->quantity = std::stoi(values[6]);
            break;
        }
        
        /* For debugging */
        case 1: {
            this->type = (keywords.find(values[0]) == keywords.end()) ? STATEMENT_COMMAND_UNRECOGNIZED : keywords.at(values[0]);    
            break;
        }

        case 2: {
            this->type = (keywords.find(values[0]) == keywords.end()) ? STATEMENT_COMMAND_UNRECOGNIZED : keywords.at(values[0]);

            /* Can be improved with another switch statement with dictionary parsing and enum */
            if (values[1] == "*") {
                std::cout << *(this->order_book) << std::endl;
            } else if (values[1] == "top") {
                this->order_book->topFive();
            } else {
                std::cout << "Invalid symbol" << std::endl;
            }
            break;
        }

        default:
            break;
    }
}

// Overloads the >> operator for the Command class
std::istream& operator>>(std::istream &istream, Command &command) {
    std::getline(istream, command.text);
    if (command.text.empty()) {
        command.setType(STATEMENT_EMPTY);
        return istream;
    }

    command.parse();

    return istream;
}

// Executes the command
MetaCommandResult Command::executeStatement() {
    /* To be implmented further */

    switch (this->type) {
        case STATEMENT_EXIT: {
            return META_COMMAND_QUIT;
        }

        case STATEMENT_ADD: {
            this->order_book->add(this->price, this->quantity, this->side);
            return META_COMMAND_SUCCESS;
        }

        case STATEMENT_TRADE: {
            this->last_traded_price = this->price;
            this->last_traded_quantity = this->quantity;
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

    return META_COMMAND_SUCCESS;
}

// Loads the database from a log file of commands and executes the commands
void Command::load(std::ifstream &file) {
    // To be implemented further
    // Needs proper command parsing
    while (file >> *this) {
        switch (this->executeStatement()) {
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