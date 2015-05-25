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
#include "util.hh"

#include <ixxx/ixxx.h>

namespace ixxx {

  namespace util {

    FD::FD()
    {
    }
    FD::FD(int fd)
      : fd_(fd)
    {
    }
    FD::FD(const char *filename, int flags)
      : fd_(ixxx::posix::open(filename, flags))
    {
    }
    FD::FD(const char *filename, int flags, mode_t mode)
      : fd_(ixxx::posix::open(filename, flags, mode))
    {
    }
    FD::FD(const std::string &filename, int flags)
      : fd_(ixxx::posix::open(filename, flags))
    {
    }
    FD::FD(const std::string &filename, int flags, mode_t mode)
      : fd_(ixxx::posix::open(filename, flags, mode))
    {
    }
    FD::FD(FD &dir_fd, const char *filename, int flags)
      : fd_(ixxx::posix::openat(dir_fd.get(), filename, flags))
    {
    }
    FD::FD(FD &dir_fd, const char *filename, int flags, mode_t mode)
      : fd_(ixxx::posix::openat(dir_fd.get(), filename, flags, mode))
    {
    }
    FD::FD(FD &dir_fd, const std::string &filename, int flags)
      : fd_(ixxx::posix::openat(dir_fd.get(), filename, flags))
    {
    }
    FD::FD(FD &dir_fd, const std::string &filename, int flags, mode_t mode)
      : fd_(ixxx::posix::openat(dir_fd.get(), filename, flags, mode))
    {
    }
    FD::FD(FD &&o)
      : fd_(o.fd_)
    {
      o.fd_ = -1;
    }
    FD::~FD()
    {
      try {
        if (fd_ > -1)
          ixxx::posix::close(fd_);
      } catch (...) {
      }
    }

    FD &FD::operator=(FD &&o)
    {
      fd_ = o.fd_;
      o.fd_ = -1;
      return *this;
    }
    FD::operator int() const
    {
      return fd_;
    }

    void FD::reset()
    {
      fd_ = -1;
    }
    int FD::release()
    {
      int r = fd_;
      reset();
      return r;
    }
    int FD::get() const
    {
      return fd_;
    }


    File::File()
    {
    }
    File::File(FILE *file)
      : file_(file)
    {
    }
    File::File(const char *filename, const char *mode)
      : file_(ixxx::ansi::fopen(filename, mode))
    {
    }
    File::File(const std::string &filename, const char *mode)
      : file_(ixxx::ansi::fopen(filename, mode))
    {
    }
    File::File(FD &fd, const char *mode)
      : file_(ixxx::posix::fdopen(fd.get(), mode))
    {
    }
    File::File(File &&o)
      : file_(o.file_)
    {
      o.file_ = nullptr;
    }
    File::~File()
    {
      try {
        if (file_)
          fclose(file_);
      } catch (...) {
      }
    }

    File &File::operator=(File &&o)
    {
      file_ = o.file_;
      o.file_ = nullptr;
      return *this;
    }
    File::operator FILE*() const
    {
      return file_;
    }

    void File::reset()
    {
      file_ = nullptr;
    }
    FILE *File::release()
    {
      FILE *r = file_;
      reset();
      return r;
    }
    FILE *File::get() const
    {
      return file_;
    }

  }

}
