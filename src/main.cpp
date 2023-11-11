#include <iostream>
#include <string>
#include "include/command.h"

// Main function that uses REPL loop
int main(int argc, char const *argv[]) {
    OrderBookPtr order_book = std::make_shared<OrderBook>();
    Command command(order_book);

    // std::string filename = "../data/SCH.log";
    // std::ifstream file(filename);

    // if (!file.is_open()) {
    //     std::cout << "Could not open file " << filename << std::endl;
    //     exit(EXIT_FAILURE);
    // }

    // command.load(file);

    while (true) {
        std::cout << "db >  ";
        std::cin >> command;

        switch (command.executeStatement(std::cout)) {
            case META_COMMAND_SUCCESS: {
                break;
            }
            
            case META_COMMAND_UNRECOGNIZED: {
                std::cout << "Unrecognized command " << command.getText() << std::endl;
                break;
            }

            case META_COMMAND_QUIT: {
                exit(EXIT_SUCCESS);
                return 0;
                break;
            }
        }
    }

    return 0;
}
