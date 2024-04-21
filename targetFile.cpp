#include <iostream>
#include <unistd.h>
#include <cstring>

int main() {
    pid_t pid = getpid();

    while (true) {
        std::cout << "Enter a value (press 'q' to quit): ";
        char input[1000];

        std::cin.getline(input, 1000);

        if (strcmp(input, "q") == 0) {
            break;
        }

        std::cout << "PID: " << pid << ", Input Value: " << input << std::endl;
        std::cout << "Address of the 'input' variable: " << static_cast<void*>(&input) << std::endl; // Corrected to use address-of operator
    }

    return 0;
}
