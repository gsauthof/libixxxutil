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
