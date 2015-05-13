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

  }

}
