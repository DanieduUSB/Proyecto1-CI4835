// socket.hpp
// Clase Socket para manejar la comunicación de red

#pragma once

#include <unistd.h> // close()

class Socket {
    int fd_ = -1; // file descriptor. -1 Significa que no está abierto

public:
    // Constructor. Recibe un file descriptor y lo almacena en fd_
    explicit Socket(int fd) : fd_(fd) {}

    // Destructor. Cierra el file descriptor si está abierto
    ~Socket() {
        if (fd_ >= 0) ::close(fd_);
    }

    Socket(const Socket&) = delete; // No se puede copiar
    Socket& operator=(const Socket&) = delete; // No se puede asignar

    Socket(Socket&& other) noexcept : fd_(other.fd_) { // Constructor de movimiento
        other.fd_ = -1; // Evita que el otro Socket cierre el fd
    }

    // Retorna el file descriptor, para connect(), send(), recv(), etc.
    int get_fd() const { return fd_; }

    bool ok() const { return fd_ >= 0; } // Retorna true si el socket está abierto
};