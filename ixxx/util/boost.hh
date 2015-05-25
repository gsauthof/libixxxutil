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
