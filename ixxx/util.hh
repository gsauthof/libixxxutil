// Copyright (c) 2015, Georg Sauthoff <mail@georg.so>

/* {{{ 2-clause BSD

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

}}} */
#ifndef IXXX_UTIL_HH
#define IXXX_UTIL_HH

#include <stdio.h>
#include <sys/types.h>
#include <stdint.h>
#include <string>
#include <deque>

namespace ixxx {

  namespace util {

    class FD {
      private:
        int fd_ {-1};
      public:
        FD();
        FD(int fd);
        FD(const char *filename, int flags);
        FD(const char *filename, int flags, mode_t mode);
        FD(const std::string &filename, int flags);
        FD(const std::string &filename, int flags, mode_t mode);
#if (defined(__APPLE__) && defined(__MACH__))
#else
        FD(FD &dir_fd, const char *filename, int flags);
        FD(FD &dir_fd, const char *filename, int flags, mode_t mode);
        FD(FD &dir_fd, const std::string &filename, int flags);
        FD(FD &dir_fd, const std::string &filename, int flags, mode_t mode);
#endif
        FD(const FD &) =delete;
        FD(FD &&o);
        ~FD();

        FD &operator=(const FD&) =delete;
        FD &operator=(FD &&o);
        operator int() const;

        void reset();
        int release();
        int get() const;

        // allow to throw
        void close();
    };

    class File {
      private:
        FILE *file_ {nullptr};
      public:
        File();
        File(FILE *file);
        File(const char *filename, const char *mode);
        File(const std::string &filename, const char *mode);
        File(FD &fd, const char *mode);
        File(const File &) =delete;
        File(File &&o);
        ~File();

        File &operator=(const File &) =delete;
        File &operator=(File &&o);
        operator FILE*() const;

        void reset();
        FILE *release();
        FILE *get() const;

        // allow to throw
        void close();
    };

    class Mapping {
      private:
        void *addr_ {nullptr};
        size_t length_ {0u};
      public:
        Mapping();
        Mapping(FD &fd, size_t length, int prot, int flags);
        Mapping(Mapping &&);
        Mapping(const Mapping &) = delete;
        ~Mapping();

        // allow to throw
        void unmap();

        const uint8_t *begin() const;
        const uint8_t *end() const;
        uint8_t *begin();
        uint8_t *end();
        const char *s_begin() const;
        const char *s_end() const;
        char *s_begin();
        char *s_end();

        Mapping &operator=(Mapping &&);
        Mapping &operator=(const Mapping &) = delete;
    };

    class Mapped_File {
      private:
        FD fd_;
        Mapping mapping_;
      public:
        Mapped_File(const std::string &filename,
            bool read = true, bool write = false,
            size_t size = 0u);
        Mapped_File(const Mapped_File &) =delete;
        //Mapped_File(Mapped_File &&);

        // allow to throw
        void close();

        FD fd();

        const uint8_t *begin() const;
        const uint8_t *end() const;
        uint8_t *begin();
        uint8_t *end();
        const char *s_begin() const;
        const char *s_end() const;
        char *s_begin();
        char *s_end();

        size_t size() const;

        //Mapped_File &operator=(Mapped_File &&);
        Mapped_File &operator=(const Mapped_File &) =delete;
    };

    std::string which(const std::deque<std::string> &path,
        const std::string &filename);


  }

}

#endif
