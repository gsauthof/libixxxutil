Utility classes for scope based file management.

Provided are classes to manage file descriptors, `FILE`
pointers and memory mapped files (cf. [RAII][1]).

## Design

- Conversion operators are provided to implicitly convert to
  file descriptors/FILE pointers
- The [ixxx][2] library is used such that system errors are
  reported via exceptions
- The convenience classes for memory mapped files internally
  use the classes from [Boost Interprocess][3]

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
      util::RO_Mapped_File a(filename_a);
      util::RO_Mapped_File b(filename_b);
      bool are_equal = std::equal(r.begin(), r.end(), o.begin(), o.end());
      if (!are_equal) {
        // report differences ...
      }
    }

## Compile

    $ mkdir build
    $ cd build
    $ cmake ..
    $ make

This creates the library and also the unittests. Execute the unittests via:

    $ ./ut

## Licence

2-clause BSD

[1]: https://en.wikipedia.org/wiki/Resource_Acquisition_Is_Initialization
[2]: https://github.com/gsauthof/libixxx
[3]: http://www.boost.org/doc/libs/1_58_0/doc/html/interprocess/sharedmemorybetweenprocesses.html#interprocess.sharedmemorybetweenprocesses.mapped_file


