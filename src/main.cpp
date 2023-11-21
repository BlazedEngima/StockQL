#include <iostream>
#include <string>

#include "include/command.h"

// Main function that uses REPL loop
int main(int argc, char const *argv[]) {
  std::string filename;

  if (argc == 1) {
    filename = "database";
  } else if (argc == 2) {  // Name of database file is passed as argument
    filename = argv[1];
  } else {
    std::cout << "Usage: " << argv[0] << " [database file]" << std::endl;
    exit(EXIT_FAILURE);
  }

  Table table(filename);
  Command command(&table);

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
