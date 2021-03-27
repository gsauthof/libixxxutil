// Copyright (c) 2015, Georg Sauthoff <mail@georg.so>

// SPDX-License-Identifier: BSD-2-Clause

#include "util.hh"

#include <ixxx/ixxx.hh>

#include <unistd.h>

#if (defined(__MINGW32__) || defined(__MINGW64__))
  #include <windows.h>
  // cf.
  // mingw/include/io.h       _get_osfhandle()
  // include/memoryapi.h      CreateFileMapping()
  // include/errhandlingapi.h GetLastError()
#else
  #include <sys/mman.h>
#endif

#include <stdlib.h>
#include <errno.h>

namespace ixxx {

  namespace util {

    FD::FD()
    {
    }
    FD::FD(int fd, bool keep_open)
      : fd_(fd), keep_open_(keep_open)
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
#if (defined(__APPLE__) && defined(__MACH__))
#elif (defined(__MINGW32__) || defined(__MINGW64__))
#else
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
#endif

    // XXX destruct i.e. 'close' before move ...
    // also for move-assigment and other classes ...
    FD::FD(FD &&o)
      :
        fd_(o.fd_),
        keep_open_(o.keep_open_)
    {
      o.fd_ = -1;
    }
    FD::~FD()
    {
      if (keep_open_)
        return;
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
      FD::close();
      fd_ = o.fd_;
      keep_open_ = o.keep_open_;
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

    void FD::set_keep_open(bool b)
    {
      keep_open_ = b;
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
      File::close();
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


    MMap::MMap() =default;
    MMap::MMap(void *addr, size_t length, int prot, int flags,
        int fd, off_t offset)
      :
        length_(length)
    {
      if (length_) {
#if (defined(__MINGW32__) || defined(__MINGW64__))
        HANDLE h = reinterpret_cast<HANDLE>(_get_osfhandle(fd));
        if (h == INVALID_HANDLE_VALUE)
          throw std::runtime_error("mapping failed: invalid handle");
        HANDLE fm = CreateFileMapping(h, NULL,
            prot ? PAGE_READWRITE : PAGE_READONLY, 0, 0, NULL);
        if (!fm)
          throw std::runtime_error("Create mapping failed"); // GetLastError()
        addr_ = MapViewOfFile(fm, prot ? FILE_MAP_WRITE : FILE_MAP_READ,
            0, /*off*/ 0, length);
        CloseHandle(fm);
        if (!addr_)
          throw std::runtime_error("Could not create view"); // GetLastError()
#else
        addr_ = ixxx::posix::mmap(addr, length_, prot, flags, fd, offset);
#endif
      } else {
        addr_ = empty_;
      }
    }
    MMap::MMap(MMap &&o)
      :
        addr_(o.addr_),
        length_(o.length_)
    {
      o.addr_ = nullptr;
      o.length_ = 0;
    }
    MMap &MMap::operator=(MMap &&o)
    {
      addr_ = o.addr_;
      length_ = o.length_;
      o.addr_ = nullptr;
      o.length_ = 0;
      return *this;
    }
    void MMap::sync()
    {
#if (defined(__MINGW32__) || defined(__MINGW64__))
        // XXX windows doesn't really have a direct msync() equivalent
        // https://stackoverflow.com/q/618046/427158
        // > To flush all the dirty pages plus the metadata for the file and
        // > ensure that they are physically written to disk, call
        // > FlushViewOfFile and then call the FlushFileBuffers function.
        // https://msdn.microsoft.com/en-us/library/windows/desktop/aa366563(v=vs.85).aspx
        auto r = FlushViewOfFile(addr_, size());
        if (!r)
            throw std::runtime_error("flush sync failed"); // GetLastError() -> code
        // XXX
#if 0
        {
        auto r = FlushFileBuffers(h_);
        if (!r)
            throw std::runtime_error("flush file buffers failed");
        }
#endif
#else
        ixxx::posix::msync(begin(), size(), MS_SYNC);
#endif
    }
    void MMap::unmap()
    {
      if (addr_ && length_) {
#if (defined(__MINGW32__) || defined(__MINGW64__))
        auto r = UnmapViewOfFile(addr_);
        if (!r)
          throw std::runtime_error("unmap failed"); // GetLastError() -> code
#else
        ixxx::posix::munmap(addr_, length_);
#endif
        addr_ = nullptr;
        length_ = 0;
      }
    }
    MMap::~MMap()
    {
      try {
        unmap();
      } catch (...) {
      }
    }
    const unsigned char *MMap::begin() const
    {
      return static_cast<const unsigned char*>(addr_);
    }
    const unsigned char *MMap::end() const
    {
      return static_cast<const unsigned char*>(addr_) + length_;
    }
    unsigned char *MMap::begin()
    {
      return static_cast<unsigned char *>(addr_);
    }
    unsigned char *MMap::end()
    {
      return static_cast<unsigned char*>(addr_) + length_;
    }
    const char *MMap::s_begin() const
    {
      return static_cast<const char*>(addr_);
    }
    const char *MMap::s_end() const
    {
      return static_cast<const char*>(addr_) + length_;
    }
    char *MMap::s_begin()
    {
      return static_cast<char*>(addr_);
    }
    char *MMap::s_end()
    {
      return static_cast<char*>(addr_) + length_;
    }
    size_t MMap::size() const
    {
      return length_;
    }

    MMap mmap_file(FD &fd, bool read, bool write,
        size_t length, size_t off)
    {
#if (defined(__MINGW32__) || defined(__MINGW64__))
      int flags = 0;
      int prot = 0;
      if (write)
        prot = 1;
#else
      // for PROT_READ only mapping we can also use MAP_PRIVATE
      // but it shouldn't make a difference
      int flags = MAP_SHARED;
      int prot = 0;
      if (read)
        prot |= PROT_READ;
      if (write)
        prot |= PROT_WRITE;
#endif
      return MMap(nullptr, length, prot, flags, fd, off);
    }
    MMap mmap_file(const std::string &filename, bool read, bool write,
        size_t length, size_t off)
    {
      MMap m;
      mode_t mode = write ? 0666 : 0444;
      // Even for just PROT_WRITE, POSIX requires that fd is
      // opened with O_RDWR. This requirement is e.g. enforced
      // by Linux.
      int open_flags = write ? O_CREAT | O_RDWR : O_RDONLY;
      FD fd(filename, open_flags, mode);
      if (write && !off && length)
        ixxx::posix::ftruncate(fd, length);
      if (!off && !length) {
        struct stat s;
        ixxx::posix::fstat(fd, s);
        length = s.st_size;
      }
      m = mmap_file(fd, read, write, length, off);
      fd.close();
      return m;
    }


    Directory::Directory() =default;
    Directory::Directory(const char *name)
      :
        dir_(ixxx::posix::opendir(name))
    {
    }
    Directory::Directory(const std::string &name)
      :
        dir_(ixxx::posix::opendir(name))
    {
    }
    Directory::~Directory()
    {
      if (dir_) // destructor must not throw ...
        ::closedir(dir_);
    }
    Directory::Directory(Directory &&o)
      : dir_(o.dir_)
    {
      o.dir_ = nullptr;
    }
    Directory &Directory::operator=(Directory &&o)
    {
      close();
      dir_ = o.dir_;
      o.dir_ = nullptr;
      return *this;
    }
    const struct dirent *Directory::read()
    {
      if (!dir_)
        return nullptr;
      entry_ = ixxx::posix::readdir(dir_);
      return entry_;
    }
    const struct dirent &Directory::entry() const
    {
      return *entry_;
    }
    void Directory::close()
    {
      if (dir_)
        ixxx::posix::closedir(dir_);
    }

    Directory_Iterator::Directory_Iterator() =default;
    Directory_Iterator::Directory_Iterator(const char *name)
      :
        d_(std::make_shared<Directory>(name))
    {
      ++(*this);
    }
    Directory_Iterator::Directory_Iterator(const std::string &name)
      :
        d_(std::make_shared<Directory>(name))
    {
      ++(*this);
    }
    bool Directory_Iterator::operator==(const Directory_Iterator &o) const
    {
      return d_.get() == o.d_.get();
    }
    bool Directory_Iterator::operator!=(const Directory_Iterator &o) const
    {
      return !(*this == o);
    }
    Directory_Iterator &Directory_Iterator::operator++()
    {
      auto r = d_->read();
      if (!r)
        d_.reset();
      return *this;
    }
    const struct dirent &Directory_Iterator::operator*() const
    {
      return d_->entry();
    }

    const char *getenv(const char *name, const char *dflt)
    {
      const char *r = ::getenv(name);
      return r ? r : dflt;
    }

    std::string which(const std::deque<std::string> &path,
        const std::string &filename)
    {
      for (auto &p : path) {
        try {
          std::string s(p);
          s += '/';
          s += filename;
          struct stat buf {0};
          ixxx::posix::stat(s, &buf);
          return s;
        } catch (const ixxx::sys_error &e) {
          continue;
        }
      }
      throw std::range_error("Not found in given path - file: " + filename);
      return "";
    }

    size_t write_retry(int fd, const void *buf, size_t n)
    {
      for (;;) {
        try {
          ssize_t r = ixxx::posix::write(fd, buf, n);
          return r;
        } catch (const ixxx::sys_error &e) {
          if (e.code() == EINTR)
            continue;
          throw;
        }
        break;
      }
      return 0; // never reached
    }
    size_t write_all(int fd, const void *buf, size_t n)
    {
      size_t m = n;
      size_t off = 0;
      while (m) {
        size_t r = write_retry(fd, static_cast<const char*>(buf)+off, m);
        off += r;
        m   -= r;
      }
      return off;
    }
    size_t write_all(int fd, const std::vector<char> &buf)
    {
        return write_all(fd, buf.data(), buf.size());
    }
    size_t write_all(int fd, const std::vector<unsigned char> &buf)
    {
        return write_all(fd, buf.data(), buf.size());
    }

    size_t read_retry(int fd, void *buf, size_t n)
    {
      for (;;) {
        try {
          ssize_t r = ixxx::posix::read(fd, buf, n);
          return r;
        } catch (const ixxx::sys_error &e) {
          if (e.code() == EINTR)
            continue;
          throw;
        }
        break;
      }
      return 0; // never reached
    }
    size_t read_all(int fd, void *buf, size_t n)
    {
      size_t m = n;
      size_t off = 0;
      while (m) {
        size_t r = read_retry(fd, static_cast<char*>(buf)+off, m);
        off += r;
        if (!r)
          return off;
        m   -= r;
      }
      return off;
    }



  }

}
