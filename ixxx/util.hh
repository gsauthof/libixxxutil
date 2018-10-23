// Copyright (c) 2015, Georg Sauthoff <mail@georg.so>

// SPDX-License-Identifier: BSD-2-Clause

#ifndef IXXX_UTIL_HH
#define IXXX_UTIL_HH

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdint.h>
#include <string>
#include <deque>
#include <memory>
#include <iterator>
#include <array>
#include <vector>

namespace ixxx {

  namespace util {

    class FD {
      private:
        int fd_ {-1};
        bool keep_open_ {false};
      public:
        FD();
        FD(int fd, bool keep_open = false);
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

        void set_keep_open(bool b);

        // throws
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

    class MMap {
      public:
        MMap();
        MMap(void *addr, size_t length, int prot, int flags,
            int fd, off_t offset);
        MMap(MMap &&);
        MMap(const MMap &) =delete;
        MMap &operator=(MMap &&);
        MMap &operator=(const MMap &) = delete;

        void unmap(); // allows to throw
        ~MMap();
        const unsigned char *begin() const;
        const unsigned char *end() const;
        unsigned char *begin();
        unsigned char *end();
        const char *s_begin() const;
        const char *s_end() const;
        char *s_begin();
        char *s_end();
        size_t size() const;
      private:
        void *addr_ {nullptr};
        size_t length_ {0};
        unsigned char empty_[0];
    };
    MMap mmap_file(FD &fd, bool read = true, bool write = false,
        size_t length = 0, size_t off = 0);
    MMap mmap_file(const std::string &filename,
        bool read = true, bool write = false,
        size_t length = 0, size_t off = 0);

    class Directory {
      public:
        Directory();
        Directory(const char *name);
        Directory(const std::string &name);
        ~Directory();
        Directory(const Directory &) =delete;
        Directory &operator=(const Directory &) =delete;
        Directory(Directory &&);
        Directory &operator=(Directory &&);
        const struct dirent *read();
        const struct dirent &entry() const;
        void close();
      private:
        DIR *dir_ {nullptr};
        struct dirent *entry_ {nullptr};
    };

    // See also boost::filesystem::directory_iterator (bdi) for a boost
    // implementation: http://www.boost.org/doc/libs/1_64_0/libs/filesystem/doc/reference.html#Class-directory_iterator
    // bdi also is a single-pass iterator and also uses a shared-pointer
    // in addition, it uses boost iterator facade
    // cf. http://www.boost.org/doc/libs/1_64_0/libs/iterator/doc/iterator_facade.html
    class Directory_Iterator {
      public:
        // boost category: single-pass iterator
        using iterator_category = std::input_iterator_tag;
        using difference_type   = ssize_t;
        using value_type        = struct dirent;
        using pointer           = value_type*;
        using reference         = value_type&;
        Directory_Iterator();
        Directory_Iterator(const char *name);
        Directory_Iterator(const std::string &name);
        bool                operator==(const Directory_Iterator &o) const;
        bool                operator!=(const Directory_Iterator &o) const;
        Directory_Iterator &operator++();
        const struct dirent &operator*() const;
      private:
        std::shared_ptr<Directory> d_;
    };

    // a pair of iterators for e.g. a range-loop can be obtained like this:
    //
    // #include <boost/range/iterator_range_core.hpp>
    //
    // boost::iterator_range<ixxx::util::Directory_Iterator>(
    //     ixxx::util::Directory_Iterator(name),
    //     ixxx::util::Directory_Iterator())

    const char *getenv(const char *name, const char *dflt);

    std::string which(const std::deque<std::string> &path,
        const std::string &filename);

    size_t write_retry(int fd, const void *buf, size_t n);
    size_t write_all(int fd, const void *buf, size_t n);
    size_t write_all(int fd, const std::vector<char> &buf);
    size_t write_all(int fd, const std::vector<unsigned char> &buf);

    size_t read_retry(int fd, void *buf, size_t n);
    size_t read_all(int fd, void *buf, size_t n);
    template <typename T, std::size_t N>
        size_t read_all(int fd, std::array<T, N> &buf)
        {
            return read_all(fd, buf.data(), buf.size());
        }
    template <typename T, typename A>
        size_t read_all(int fd, std::vector<T, A> &buf)
        {
            size_t r = read_all(fd, buf.data(), buf.size());
            buf.resize(r);
            return r;
        }

  }

}

#endif
