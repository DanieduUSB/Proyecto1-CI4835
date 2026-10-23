// servidor.cpp

#include <socket.hpp>

#include <cstdio> // perror()
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 7) {
        std::cerr << "Usage: " << argv[0] << " --chat <port> --http <port> --buzones <file>\n";
        return 1;
    }

    return 0;
}