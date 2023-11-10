#include <iostream>
#include <string>
#include "include/command.h"

// Main function that uses REPL loop
int main(int argc, char const *argv[]) {
    Command command;

    while (true) {
        std::cout << "db >  ";
        std::cin >> command;

        switch (command.executeStatement()) {
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
