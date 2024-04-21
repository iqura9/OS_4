#include <iostream>
#include <sys/ptrace.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <pid> <address>" << std::endl;
        return 1;
    }

    pid_t pid = atoi(argv[1]);
    void *address = reinterpret_cast<void *>(strtoul(argv[2], nullptr, 0));

    if (ptrace(PTRACE_ATTACH, pid, nullptr, nullptr) == -1) {
        perror("ptrace attach failed");
        return 1;
    }

    waitpid(pid, nullptr, 0);

    std::string str;
    char buffer;
    int offset = 0;

    while (true) {
        // Read one byte at a time from the target process
        long data = ptrace(PTRACE_PEEKDATA, pid, reinterpret_cast<void *>(reinterpret_cast<char *>(address) + offset), nullptr);
        if (data == -1) {
            perror("ptrace peekdata failed");
            ptrace(PTRACE_DETACH, pid, nullptr, nullptr);
            return 1;
        }

        buffer = static_cast<char>(data & 0xFF);
        str += buffer;

        if (buffer == '\0') {
            break;
        }

        ++offset;
    }

    std::cout << "String read from target process: " << str << std::endl;

    if (ptrace(PTRACE_DETACH, pid, nullptr, nullptr) == -1) {
        perror("ptrace detach failed");
        return 1;
    }

    return 0;
}
