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
