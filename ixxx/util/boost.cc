#include "boost.hh"

#include <utility>

namespace ixxx {

  namespace util {

    RO_Mapped_File::RO_Mapped_File()
    {
    }
    RO_Mapped_File::RO_Mapped_File(const char *filename)
      :
        file_(filename, boost::interprocess::read_only),
        region_(file_, boost::interprocess::read_only)
    {
    }
    RO_Mapped_File::RO_Mapped_File(const std::string &filename)
      : RO_Mapped_File(filename.c_str())
    {
    }
    RO_Mapped_File::RO_Mapped_File(RO_Mapped_File &&o)
      :
        file_  (std::move(o.file_)),
        region_(std::move(o.region_))
    {
    }

    RO_Mapped_File &RO_Mapped_File::operator=(RO_Mapped_File &&o)
    {
      file_   = std::move(o.file_);
      region_ = std::move(o.region_);
      return *this;
    }

    const uint8_t *RO_Mapped_File::begin() const
    {
      return static_cast<const uint8_t*>(region_.get_address());
    }
    const uint8_t *RO_Mapped_File::end() const
    {
      return begin() + region_.get_size();
    }
    const char *RO_Mapped_File::sbegin() const
    {
      return static_cast<const char *>(region_.get_address());
    }
    const char *RO_Mapped_File::send() const
    {
      return sbegin() + region_.get_size();
    }
    size_t RO_Mapped_File::size() const
    {
      return region_.get_size();
    }


    RW_Mapped_File::RW_Mapped_File()
    {
    }
    RW_Mapped_File::RW_Mapped_File(const char *filename)
      :
        file_(filename, boost::interprocess::read_write),
        region_(file_, boost::interprocess::read_write)
    {
    }
    RW_Mapped_File::RW_Mapped_File(const std::string &filename)
      : RW_Mapped_File(filename.c_str())
    {
    }
    RW_Mapped_File::RW_Mapped_File(RW_Mapped_File &&o)
      :
        file_  (std::move(o.file_)),
        region_(std::move(o.region_))
    {
    }

    RW_Mapped_File &RW_Mapped_File::operator=(RW_Mapped_File &&o)
    {
      file_   = std::move(o.file_);
      region_ = std::move(o.region_);
      return *this;
    }

    uint8_t *RW_Mapped_File::begin()
    {
      return static_cast<uint8_t*>(region_.get_address());
    }
    uint8_t *RW_Mapped_File::end()
    {
      return begin() + region_.get_size();
    }
    char *RW_Mapped_File::sbegin()
    {
      return static_cast<char *>(region_.get_address());
    }
    char *RW_Mapped_File::send()
    {
      return sbegin() + region_.get_size();
    }
    size_t RW_Mapped_File::size() const
    {
      return region_.get_size();
    }

  }

}
