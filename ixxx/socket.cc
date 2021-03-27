// Copyright (c) 2015, Georg Sauthoff <mail@georg.so>

// SPDX-License-Identifier: BSD-2-Clause

#include "util.hh"

#include <ixxx/ixxx.hh>
#include <ixxx/socket.hh>

#include <netdb.h>
#include <netinet/tcp.h> // TCP_NODELAY
#include <unistd.h>

namespace ixxx {

  namespace util {

    Getaddrinfo::Getaddrinfo(const char *node, const char *service,
                   const struct addrinfo *hints)
    {
        ixxx::posix::getaddrinfo(node, service, hints, &x);
    }
    Getaddrinfo::~Getaddrinfo()
    {
        if (x)
            freeaddrinfo(x);
    }
    Getaddrinfo::Getaddrinfo(Getaddrinfo &&o)
        :
            x(o.x)
    {
        o.x = nullptr;
    }
    Getaddrinfo &Getaddrinfo::operator=(Getaddrinfo && o)
    {
        x = o.x;
        o.x = nullptr;
        return *this;
    }

    int connect(const char *host, const char *port, const addrinfo *hints)
    {
        struct addrinfo default_hints = {
            .ai_family   = AF_UNSPEC,
            .ai_socktype = SOCK_STREAM
        };
        Getaddrinfo ga(host, port, hints ? hints : &default_hints);
        int fd = -1;
        for (const addrinfo *a = ga.result(); a; a = a->ai_next) {
            try {
                fd = ixxx::posix::socket(a->ai_family, a->ai_socktype, a->ai_protocol);
                ixxx::posix::connect(fd, a->ai_addr, a->ai_addrlen);
                int i = 1;
                ixxx::posix::setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &i, sizeof i);
                return fd;
            } catch (const ixxx::socket_error &) {
                // ignore socket errors and try next
            } catch (const ixxx::connect_error &) {
                close(fd); // i.e. try next one
            }
        }
        return -1;
    }

  }

}
