#include <iostream>
#include <string>

// Main function that uses REPL loop
int main(int argc, char const *argv[]) {
    std::string in;

    while (true) {
        std::cout << "db >  ";
        getline(std::cin, in);

        if (in == ".exit" || in == "q") {
            exit(EXIT_SUCCESS);
            return 0;
        } else {
            std::cout << "Unrecognized command " << in << std::endl;
        }
    }

    return 0;
}
