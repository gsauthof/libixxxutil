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
#include <string>

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
        FD(FD &dir_fd, const char *filename, int flags);
        FD(FD &dir_fd, const char *filename, int flags, mode_t mode);
        FD(FD &dir_fd, const std::string &filename, int flags);
        FD(FD &dir_fd, const std::string &filename, int flags, mode_t mode);
        FD(const FD &) =delete;
        FD(FD &&o);
        ~FD();

        FD &operator=(const FD&) =delete;
        FD &operator=(FD &&o);
        operator int() const;

        void reset();
        int release();
        int get() const;
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
    };


  }

}

#endif
