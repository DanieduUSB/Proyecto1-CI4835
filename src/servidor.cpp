// servidor.cpp

#include <socket.hpp>

#include <arpa/inet.h> // sockaddr_in, htons(), inet_pton()
#include <sys/socket.h> // socket(), connect(), send(), recv(), listen(), accept()
#include <cstdio> // perror()
#include <iostream>
#include <string>
#include <mutex>
#include <thread>

// Crea el socket bidireccional
Socket make_listener(int port) {
    int on = 1, off = 0;
    // Intenta con IPv6 primero
    Socket s(socket(AF_INET6, SOCK_STREAM, 0));
    if (s.ok()) {
        // Permite reusar la dirección y el puerto
        setsockopt(s.get_fd(), SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

        // Acepta IPv4 e IPv6
        setsockopt(s.get_fd(), IPPROTO_IPV6, IPV6_V6ONLY, &off, sizeof(off));

        sockaddr_in6 addr{};
        addr.sin6_family = AF_INET6;
        addr.sin6_port = htons(port);
        addr.sin6_addr = in6addr_any; // Acepta conexiones de cualquier dirección
        if (bind(s.get_fd(), reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) return Socket(-1);
    } else { // Si IPv6 falla, intenta con IPv4
        s = Socket(socket(AF_INET, SOCK_STREAM, 0));
        if (!s.ok()) return s;

        setsockopt(s.get_fd(), SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = INADDR_ANY; // Acepta conexiones de cualquier dirección
        if (bind(s.get_fd(), reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) return Socket(-1);
    }

    if (listen(s.get_fd(), SOMAXCONN) < 0) return Socket(-1);
    return s;
}

int main(int argc, char* argv[]) {
    if (argc != 7) {
        std::cerr << "Usage: " << argv[0] << " --chat <port> --http <port> --buzones <file>\n";
        return 1;
    }

    return 0;
}