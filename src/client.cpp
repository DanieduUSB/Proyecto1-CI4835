// client.cpp

#include "socket.hpp"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <host> <port>\n";
        return 1;
    }

    // Abre un socket TCP por IPv4
    Socket sock(socket(AF_INET, SOCK_STREAM, 0));
    if (!sock.ok()) {
        perror("socket");
        return 1;
    }

    return 0;
}