#ifndef IXXX_UTIL_BOOST_HH
#define IXXX_UTIL_BOOST_HH

#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>

namespace ixxx {

  namespace util {

    class RO_Mapped_File {
      private:
        boost::interprocess::file_mapping  file_;
        boost::interprocess::mapped_region region_;
      public:
        RO_Mapped_File();
        RO_Mapped_File(const char *filename);
        RO_Mapped_File(const std::string &filename);
        RO_Mapped_File(const RO_Mapped_File &) =delete;
        RO_Mapped_File(RO_Mapped_File &&);

        RO_Mapped_File &operator=(const RO_Mapped_File &) =delete;
        RO_Mapped_File &operator=(RO_Mapped_File &&);

        const uint8_t *begin() const;
        const uint8_t *end() const;
        const char *sbegin() const;
        const char *send() const;

        size_t size() const;
    };

    class RW_Mapped_File {
      private:
        boost::interprocess::file_mapping  file_;
        boost::interprocess::mapped_region region_;
      public:
        RW_Mapped_File();
        RW_Mapped_File(const char *filename);
        RW_Mapped_File(const std::string &filename);
        RW_Mapped_File(const RW_Mapped_File &) =delete;
        RW_Mapped_File(RW_Mapped_File &&);

        RW_Mapped_File &operator=(const RW_Mapped_File &) =delete;
        RW_Mapped_File &operator=(RW_Mapped_File &&);

        uint8_t *begin();
        uint8_t *end();
        char *sbegin();
        char *send();
        size_t size() const;
    };

  }
}

#endif
