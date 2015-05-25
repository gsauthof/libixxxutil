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
