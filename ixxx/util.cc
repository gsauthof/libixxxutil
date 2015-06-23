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
#include <sys/mman.h>

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
        FD::close();
      } catch (...) {
      }
    }
    void FD::close()
    {
      if (fd_ > -1) {
        ixxx::posix::close(fd_);
        fd_ = -1;
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
        File::close();
      } catch (...) {
      }
    }
    void File::close()
    {
      if (file_) {
        fclose(file_);
        file_ = nullptr;
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


    Mapping::Mapping()
    {
    }
    Mapping::Mapping(FD &fd, size_t length, int prot, int flags)
      :
        length_(length)
    {
      addr_ = ixxx::posix::mmap(nullptr, length_, prot, flags, fd, 0);
    }
    Mapping::Mapping(Mapping &&o)
      :
        addr_(o.addr_),
        length_(o.length_)
    {
      o.addr_ = nullptr;
      o.length_ = 0;
    }
    Mapping::~Mapping()
    {
      try {
        unmap();
      } catch (...) {
      }
    }
    void Mapping::unmap()
    {
      if (addr_) {
        ixxx::posix::munmap(addr_, length_);
        addr_ = nullptr;
      }
    }
    Mapping &Mapping::operator=(Mapping &&o)
    {
      addr_ = o.addr_;
      o.addr_ = nullptr;
      length_ = o.length_;
      o.length_ = 0;
      return *this;
    }

    const uint8_t *Mapping::begin() const
    {
      return static_cast<const uint8_t*>(addr_);
    }
    const uint8_t *Mapping::end() const
    {
      return static_cast<const uint8_t*>(addr_) + length_;
    }
    uint8_t *Mapping::begin()
    {
      return static_cast<uint8_t*>(addr_);
    }
    uint8_t *Mapping::end()
    {
      return static_cast<uint8_t*>(addr_) + length_;
    }
    const char *Mapping::s_begin() const
    {
      return static_cast<const char*>(addr_);
    }
    const char *Mapping::s_end() const
    {
      return static_cast<const char*>(addr_) + length_;
    }
    char *Mapping::s_begin()
    {
      return static_cast<char*>(addr_);
    }
    char *Mapping::s_end()
    {
      return static_cast<char*>(addr_) + length_;
    }

    Mapped_File::Mapped_File(const std::string &filename,
        bool read,
        bool write,
        size_t size)
    {
      int prot = 0;
      int flags = 0;
      int open_flags = 0;

      if (read) {
        prot |= PROT_READ;
        flags = MAP_PRIVATE;
        open_flags = O_RDONLY;
      }
      if (write) {
        prot |= PROT_WRITE;
        flags = MAP_SHARED;
        if (read) {
          open_flags = O_CREAT | O_RDWR;
        } else {
          //open_flags = O_CREAT | O_WRONLY;
          // required at least on Linux 4
          open_flags = O_CREAT | O_RDWR;
        }
      }
      mode_t mode = 0666;
      fd_ = FD(filename, open_flags, mode);
      if (write && size) {
        ixxx::posix::ftruncate(fd_, size);
      }
      if (!size) {
        struct stat s;
        ixxx::posix::fstat(fd_, s);
        size = s.st_size;
      }
      mapping_ = Mapping(fd_, size, prot, flags);
    }
    void Mapped_File::close()
    {
      mapping_.unmap();
      fd_.close();
    }
    const uint8_t *Mapped_File::begin() const
    {
      return mapping_.begin();
    }
    const uint8_t *Mapped_File::end() const
    {
      return mapping_.end();
    }
    uint8_t *Mapped_File::begin()
    {
      return mapping_.begin();
    }
    uint8_t *Mapped_File::end()
    {
      return mapping_.end();
    }
    const char *Mapped_File::s_begin() const
    {
      return mapping_.s_begin();
    }
    const char *Mapped_File::s_end() const
    {
      return mapping_.s_end();
    }
    char *Mapped_File::s_begin()
    {
      return mapping_.s_begin();
    }
    char *Mapped_File::s_end()
    {
      return mapping_.s_end();
    }



  }

}
