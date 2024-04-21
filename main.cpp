#include <iostream>
#include <unistd.h>

int main() {
    // Get the PID of the current process
    pid_t pid = getpid();

    // Print the PID
    std::cout << "PID: " << pid << std::endl;

    return 0;
}
