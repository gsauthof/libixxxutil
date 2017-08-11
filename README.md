Utility classes for scope based file management.

Provided are classes to manage file descriptors, `FILE`
pointers and memory mapped files (cf. [RAII][1]).

[![Build Status](https://travis-ci.org/gsauthof/libixxxutil.svg?branch=master)](https://travis-ci.org/gsauthof/libixxxutil)

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
      util::FD fd(filename, O_CREAT | O_WRONLY, 0777);
      posix::write(fd, array.data(), array.size());
      // ...
    }

Compare two files via memory mappings:

    #include <algorithm>

    // ...

    void Bar::compare()
    {
      util::Mapped_File a(filename_a);
      util::Mapped_File b(filename_b);
      bool are_equal = std::equal(a.begin(), a.end(), b.begin(), b.end());
      if (!are_equal) {
        // report differences ...
      }
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


