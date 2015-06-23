#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <string>

#include <ixxx/util.hh>
#include <ixxx/ixxx.h>

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

using namespace std;

BOOST_AUTO_TEST_SUITE(ixxx_)

  BOOST_AUTO_TEST_SUITE(util)

    BOOST_AUTO_TEST_SUITE(misc)

      BOOST_AUTO_TEST_CASE(basic)
      {
        char dir_template[1024] = "ixxxutil_XXXXXX";
        string dirname(ixxx::posix::mkdtemp(dir_template));
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
          BOOST_CHECK(fcntl(t, F_GETFD) != -1);
          ixxx::posix::read(fd, out, 11);
        }
        BOOST_CHECK(fcntl(t, F_GETFD) == -1);
        BOOST_CHECK_EQUAL(out, "Hello World");
        boost::filesystem::remove_all(dirname);
      }

    BOOST_AUTO_TEST_SUITE_END()

    BOOST_AUTO_TEST_SUITE(mapped_file_)

      BOOST_AUTO_TEST_CASE(basic)
      {
        char dir_template[1024] = "ixxxutil_XXXXXX";
        string dirname(ixxx::posix::mkdtemp(dir_template));
        string filename(dirname + "/foo");
        {
          ixxx::util::File f(filename, "w");
          const char inp[] = "Hello World";
          ixxx::ansi::fwrite(inp, 1, strlen(inp), f.get());
        }
        ixxx::util::Mapped_File f(filename);
        string out(f.s_begin(), f.s_end());
        BOOST_CHECK_EQUAL(out, "Hello World");
        boost::filesystem::remove_all(dirname);
      }

      BOOST_AUTO_TEST_CASE(unsigned_)
      {
        char dir_template[1024] = "ixxxutil_XXXXXX";
        string dirname(ixxx::posix::mkdtemp(dir_template));
        string filename(dirname + "/bar");
        {
          ixxx::util::File f(filename, "w");
          const char inp[] = "Hello World";
          ixxx::ansi::fwrite(inp, 1, strlen(inp), f.get());
        }
        ixxx::util::Mapped_File f(filename);
        const uint8_t *begin = f.begin();
        const uint8_t *end = f.end();
        string out(reinterpret_cast<const char *>(begin), reinterpret_cast<const char*>(end));
        BOOST_CHECK_EQUAL(out, "Hello World");
        boost::filesystem::remove_all(dirname);
      }

      BOOST_AUTO_TEST_CASE(create_new)
      {
        char dir_template[1024] = "ixxxutil_XXXXXX";
        string dirname(ixxx::posix::mkdtemp(dir_template));
        string filename(dirname + "/foo");
        {
          const char hw[] = "Hello World";
          ixxx::util::Mapped_File f(filename, false, true, sizeof(hw)-1);
          memcpy(f.begin(), hw, sizeof(hw)-1);
        }
        ixxx::util::Mapped_File f(filename);
        string out(f.s_begin(), f.s_end());
        BOOST_CHECK_EQUAL(out, "Hello World");
        boost::filesystem::remove_all(dirname);
      }

      BOOST_AUTO_TEST_CASE(create_new_rw)
      {
        char dir_template[1024] = "ixxxutil_XXXXXX";
        string dirname(ixxx::posix::mkdtemp(dir_template));
        string filename(dirname + "/foo");
        {
          BOOST_CHECKPOINT("Read/Write mapping of: " << filename);
          const char hw[] = "Hello World";
          ixxx::util::Mapped_File f(filename, true, true, sizeof(hw)-1);
          memcpy(f.begin(), hw, sizeof(hw)-1);
          BOOST_CHECKPOINT("Read directly after creation of: " << filename);
          string out(f.s_begin(), f.s_end());
          BOOST_CHECK_EQUAL(out, "Hello World");
        }
        ixxx::util::Mapped_File f(filename);
        string out(f.s_begin(), f.s_end());
        BOOST_CHECK_EQUAL(out, "Hello World");
        boost::filesystem::remove_all(dirname);
      }

    BOOST_AUTO_TEST_SUITE_END()

  BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
