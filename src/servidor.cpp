// servidor.cpp

#include <socket.hpp>

#include <arpa/inet.h> // sockaddr_in, htons(), inet_pton()
#include <sys/socket.h> // socket(), connect(), send(), recv(), listen(), accept()
#include <cstdio> // perror()
#include <iostream>
#include <string>
#include <thread>
#include <poll.h> // pollfd, poll()
#include <vector>

void log(const std::string& msg) {
    std::cout << msg << std::endl;
}

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

    Socket chat_listener = make_listener(std::stoi(argv[2]));
    if (!chat_listener.ok()) {
        perror("chat listener socket");
        return 1;
    }
    log("listening chat on port " + std::string(argv[2]));

    Socket http_listener = make_listener(std::stoi(argv[4]));
    if (!http_listener.ok()) {
        perror("http listener socket");
        return 1;
    }
    log("listening http on port " + std::string(argv[4]));

    Socket mail_listener = make_listener(std::stoi(argv[6]));
    if (!mail_listener.ok()) {
        perror("mail listener socket");
        return 1;
    }
    log("listening mail on port " + std::string(argv[6]));

    // Configura los pollfd para cada listener
    pollfd chat_pollfd{chat_listener.get_fd(), POLLIN, 0};

    pollfd http_pollfd{http_listener.get_fd(), POLLIN, 0};

    pollfd mail_pollfd{mail_listener.get_fd(), POLLIN, 0};

    // Crea un vector mutable de pollfd para poder usarlo en poll(). Este vector
    // también recibirá los pollfd de los sockets aceptados, que se irán agregando
    // dinámicamente.
    std::vector<pollfd> pollfds{chat_pollfd, http_pollfd, mail_pollfd};

    return 0;
}