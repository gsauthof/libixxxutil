Utility classes for scope based file management.

Provided are classes to manage file descriptors, `FILE`
pointers and memory mapped files (cf. [RAII][1]).

[![Build Status](https://travis-ci.org/gsauthof/libixxxutil.svg?branch=master)](https://travis-ci.org/gsauthof/libixxxutil)

2015-2018, Georg Sauthoff <mail@gms.tf>, BSD-2-Clause

## Design

- Conversion operators are provided to implicitly convert to
  file descriptors/FILE pointers
- The [ixxx][2] library is used such that system errors are
  reported via exceptions
- Memory mappings are implemented via `mmap()` on Linux/Unix and
  via win32 API on Windows

## Example

Write to a file descriptor:

    #include <ixxx/ixxx.h>
    #include <ixxx/util.hh>

    using namespace ixxx;

    void Foo::dump()
    {
      util::FD fd(filename, O_CREAT | O_WRONLY | O_TRUNC, 0777);
      posix::write(fd, array_.data(), array_.size());
      // ...
    }

Compare two files via memory mappings:

    #include <algorithm>
    #include <ixxx/util.hh>

    bool files_equal(const std::string &name_a, const std::string name_b)
    {
      auto a = ixxx::util::mmap_file(name_a);
      auto b = ixxx::util::mmap_file(name_b);
      bool are_equal = std::equal(a.begin(), a.end(), b.begin(), b.end());
      return are_equal;
    }

## Compile

    $ git submodule update --init
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make

This creates the library and also the unittests. Execute the unittests via:

    $ ./ut

## Portability

Tested on:

- Linux (e.g. Fedora 23)
- Solaris 10/SPARC (gcc 4.9)
- Mac OS X
- Debian 8, PowerPC64
- Windows 7 (mingw64 and mingw32)

## Licence

2-clause BSD

[1]: https://en.wikipedia.org/wiki/Resource_Acquisition_Is_Initialization
[2]: https://github.com/gsauthof/libixxx


