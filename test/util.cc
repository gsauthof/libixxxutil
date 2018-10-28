#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range_core.hpp>

#include <algorithm>
#include <string>
#include <string.h>

#include <ixxx/util.hh>
#include <ixxx/ixxx.hh>

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#include "helper.hh"

using namespace std;

BOOST_AUTO_TEST_SUITE(ixxx_)

  BOOST_AUTO_TEST_SUITE(util)

    BOOST_AUTO_TEST_SUITE(misc)

      BOOST_AUTO_TEST_CASE(basic)
      {
        char dir_template[1024] = "ixxxutil_XXXXXX";
        string dirname(portable_mkdtemp(dir_template));
        string filename(dirname + "/foo");
        {
          ixxx::util::File f(filename, "w");
          const char inp[] = "Hello World";
          ixxx::ansi::fwrite(inp, 1, strlen(inp), f);
        }
        int t = -1;
        char out[12] = {0};
        {
          ixxx::util::FD fd(filename, O_RDONLY);
          t = fd;
#if (defined(__MINGW32__) || defined(__MINGW64__))
          (void)t;
#else
          BOOST_CHECK(fcntl(t, F_GETFD) != -1);
#endif
          ixxx::posix::read(fd, out, 11);
        }
#if !(defined(__MINGW32__) || defined(__MINGW64__))
        BOOST_CHECK(fcntl(t, F_GETFD) == -1);
#endif
        BOOST_CHECK_EQUAL(out, "Hello World");
        boost::filesystem::remove_all(dirname);
      }

      BOOST_AUTO_TEST_CASE(no_auto_close)
      {
        char dir_template[1024] = "ixxxutil_XXXXXX";
        string dirname(portable_mkdtemp(dir_template));
        string filename(dirname + "/foo");
        {
          ixxx::util::File f(filename, "w");
          const char inp[] = "Hello World";
          ixxx::ansi::fwrite(inp, 1, strlen(inp), f);
        }
        int t = -1;
        char out[12] = {0};
        {
          auto x = ixxx::posix::open(filename, O_RDONLY);
          ixxx::util::FD fd(x);
          fd.set_keep_open(true);
          t = fd;
#if !(defined(__MINGW32__) || defined(__MINGW64__))
          BOOST_CHECK(fcntl(t, F_GETFD) != -1);
#endif
          ixxx::posix::read(fd, out, 11);
        }
#if !(defined(__MINGW32__) || defined(__MINGW64__))
        BOOST_CHECK(fcntl(t, F_GETFD) != -1);
#endif
        ixxx::posix::close(t);
        BOOST_CHECK_EQUAL(out, "Hello World");
        boost::filesystem::remove_all(dirname);
      }

#if !(defined(__MINGW32__) || defined(__MINGW64__))
      BOOST_AUTO_TEST_CASE(close_because_of_move)
      {
        char dir_template[1024] = "ixxxutil_XXXXXX";
        string dirname(portable_mkdtemp(dir_template));
        ixxx::util::FD f(dirname + "/foo", O_CREAT | O_WRONLY, 0666);
        int fd = f.get();
        BOOST_CHECK(fcntl(fd, F_GETFD) != -1);
        ixxx::util::FD g(dirname + "/bar", O_CREAT | O_WRONLY, 0666);
        int gd = g.get();
        BOOST_CHECK(fcntl(gd, F_GETFD) != -1);
        f = std::move(g);
        BOOST_CHECK(fcntl(fd, F_GETFD) == -1);
        BOOST_CHECK(fcntl(gd, F_GETFD) != -1);
        BOOST_CHECK_EQUAL(g.get(), -1);
      }

      BOOST_AUTO_TEST_CASE(close_because_of_move_file)
      {
        char dir_template[1024] = "ixxxutil_XXXXXX";
        string dirname(portable_mkdtemp(dir_template));
        ixxx::util::File f(dirname + "/foo", "w");
        int fd = ixxx::posix::fileno(f.get());
        BOOST_CHECK(fcntl(fd, F_GETFD) != -1);
        ixxx::util::File g(dirname + "/bar", "w");
        int gd = ixxx::posix::fileno(g.get());
        BOOST_CHECK(fcntl(gd, F_GETFD) != -1);
        f = std::move(g);
        BOOST_CHECK(fcntl(fd, F_GETFD) == -1);
        BOOST_CHECK(fcntl(gd, F_GETFD) != -1);
        BOOST_CHECK(g.get() == nullptr);
      }
#endif

      BOOST_AUTO_TEST_CASE(which_throw)
      {
        deque<string> path = { "/bin", "/usr/bin" };
        BOOST_CHECK_THROW(
            ixxx::util::which(path, "___does_no_exist___"), std::range_error);
      }
      BOOST_AUTO_TEST_CASE(which_basic)
      {
        deque<string> path = { "/bin", "/usr/bin" };
        string r(ixxx::util::which(path, "sh"));
        BOOST_CHECK(r == "/bin/sh" || r == "/usr/bin/sh");
      }

    BOOST_AUTO_TEST_SUITE_END() // misc

    BOOST_AUTO_TEST_SUITE(mapped_file_)

      BOOST_AUTO_TEST_CASE(basic)
      {
        char dir_template[1024] = "ixxxutil_XXXXXX";
        string dirname(portable_mkdtemp(dir_template));
        string filename(dirname + "/foo");
        {
          ixxx::util::File f(filename, "w");
          const char inp[] = "Hello World";
          ixxx::ansi::fwrite(inp, 1, strlen(inp), f.get());
        }
        { // make sure that the file is closed/unmapped
          // before the remove - otherwise we get a sharing
          // violation on Windows ...
        auto f = ixxx::util::mmap_file(filename);
        string out(f.s_begin(), f.s_end());
        BOOST_CHECK_EQUAL(out, "Hello World");
        }
        boost::filesystem::remove_all(dirname);
      }

      BOOST_AUTO_TEST_CASE(unsigned_)
      {
        char dir_template[1024] = "ixxxutil_XXXXXX";
        string dirname(portable_mkdtemp(dir_template));
        string filename(dirname + "/bar");
        {
          ixxx::util::File f(filename, "w");
          const char inp[] = "Hello World";
          ixxx::ansi::fwrite(inp, 1, strlen(inp), f.get());
        }
        {
        auto f = ixxx::util::mmap_file(filename);
        const uint8_t *begin = f.begin();
        const uint8_t *end = f.end();
        string out(reinterpret_cast<const char *>(begin), reinterpret_cast<const char*>(end));
        BOOST_CHECK_EQUAL(out, "Hello World");
        }
        boost::filesystem::remove_all(dirname);
      }

      BOOST_AUTO_TEST_CASE(create_new)
      {
        char dir_template[1024] = "ixxxutil_XXXXXX";
        string dirname(portable_mkdtemp(dir_template));
        string filename(dirname + "/foo");
        {
          const char hw[] = "Hello World";
          auto f = ixxx::util::mmap_file(filename, false, true, sizeof(hw)-1);
          memcpy(f.begin(), hw, sizeof(hw)-1);
        }
        {
        auto f = ixxx::util::mmap_file(filename);
        string out(f.s_begin(), f.s_end());
        BOOST_CHECK_EQUAL(out, "Hello World");
        }
        boost::filesystem::remove_all(dirname);
      }

      BOOST_AUTO_TEST_CASE(create_new_sync)
      {
        char dir_template[1024] = "ixxxutil_XXXXXX";
        string dirname(portable_mkdtemp(dir_template));
        string filename(dirname + "/foo");
        {
          const char hw[] = "Hello World";
          auto f = ixxx::util::mmap_file(filename, false, true, sizeof(hw)-1);
          memcpy(f.begin(), hw, sizeof(hw)-1);
          f.sync();
        }
        {
        auto f = ixxx::util::mmap_file(filename);
        string out(f.s_begin(), f.s_end());
        BOOST_CHECK_EQUAL(out, "Hello World");
        }
        boost::filesystem::remove_all(dirname);
      }

      BOOST_AUTO_TEST_CASE(create_new_rw)
      {
        char dir_template[1024] = "ixxxutil_XXXXXX";
        string dirname(portable_mkdtemp(dir_template));
        string filename(dirname + "/foo");
        {
          BOOST_TEST_CHECKPOINT("Read/Write mapping of: " << filename);
          const char hw[] = "Hello World";
          auto f = ixxx::util::mmap_file(filename, true, true, sizeof(hw)-1);
          memcpy(f.begin(), hw, sizeof(hw)-1);
          BOOST_TEST_CHECKPOINT("Read directly after creation of: " << filename);
          string out(f.s_begin(), f.s_end());
          BOOST_CHECK_EQUAL(out, "Hello World");
        }
        {
        auto f = ixxx::util::mmap_file(filename);
        string out(f.s_begin(), f.s_end());
        BOOST_CHECK_EQUAL(out, "Hello World");
        }
        boost::filesystem::remove_all(dirname);
      }


    BOOST_AUTO_TEST_SUITE_END() // mapped_file_

    BOOST_AUTO_TEST_SUITE(directory)

      BOOST_AUTO_TEST_CASE(basic)
      {
        char dir_tpl[32] = "ixxxutil_XXXXXX";
        string dname(portable_mkdtemp(dir_tpl));
        auto fns = { "12342", "abc", "xyz"};
        for (auto fn : fns)
          ixxx::util::FD(dname + string("/")+ fn, O_CREAT | O_WRONLY, 0644);
        ixxx::util::Directory_Iterator begin(dname);
        ixxx::util::Directory_Iterator end;
        vector<string> v;
        for (; begin != end; ++begin)
          v.push_back((*begin).d_name);
        BOOST_REQUIRE_EQUAL(v.size(), 5);
        sort(v.begin(), v.end());
        BOOST_CHECK_EQUAL(v[0], ".");
        BOOST_CHECK_EQUAL(v[1], "..");
        BOOST_CHECK_EQUAL_COLLECTIONS(v.begin()+2, v.end(),
            fns.begin(), fns.end());
        for (auto fn : fns)
          ixxx::posix::unlink(dname+string("/")+fn);
        ixxx::posix::rmdir(dname);
      }

      BOOST_AUTO_TEST_CASE(range_for)
      {
        char dir_tpl[32] = "ixxxutil_XXXXXX";
        string dname(portable_mkdtemp(dir_tpl));
        auto fns = { "12342", "abc", "xyz"};
        for (auto fn : fns)
          ixxx::util::FD(dname + string("/")+ fn, O_CREAT | O_WRONLY, 0644);
        vector<string> v;
        for (auto &entry : boost::iterator_range<
            ixxx::util::Directory_Iterator>(
              ixxx::util::Directory_Iterator(dname),
              ixxx::util::Directory_Iterator()))
          v.push_back(entry.d_name);
        BOOST_REQUIRE_EQUAL(v.size(), 5);
        sort(v.begin(), v.end());
        BOOST_CHECK_EQUAL(v[0], ".");
        BOOST_CHECK_EQUAL(v[1], "..");
        BOOST_CHECK_EQUAL_COLLECTIONS(v.begin()+2, v.end(),
            fns.begin(), fns.end());
        for (auto fn : fns)
          ixxx::posix::unlink(dname+string("/")+fn);
        ixxx::posix::rmdir(dname);
      }

      BOOST_AUTO_TEST_CASE(in_algo)
      {
        char dir_tpl[32] = "ixxxutil_XXXXXX";
        string dname(portable_mkdtemp(dir_tpl));
        auto fns = { "12342", "abc", "xyz"};
        for (auto fn : fns)
          ixxx::util::FD(dname + string("/")+ fn, O_CREAT | O_WRONLY, 0644);
        vector<string> v;
        std::for_each(
            ixxx::util::Directory_Iterator(dname),
            ixxx::util::Directory_Iterator(),
            [&v](const struct dirent &d){ v.push_back(d.d_name); });
        BOOST_REQUIRE_EQUAL(v.size(), 5);
        sort(v.begin(), v.end());
        BOOST_CHECK_EQUAL(v[0], ".");
        BOOST_CHECK_EQUAL(v[1], "..");
        BOOST_CHECK_EQUAL(v[2], *(fns.begin()+0));
        BOOST_CHECK_EQUAL(v[3], *(fns.begin()+1));
        for (auto fn : fns)
          ixxx::posix::unlink(dname+string("/")+fn);
        ixxx::posix::rmdir(dname);
      }

    BOOST_AUTO_TEST_SUITE_END() // directory

  BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
