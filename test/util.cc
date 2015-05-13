#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <string>

#include <ixxx/util.hh>
#include <ixxx/ixxx.h>

#include <unistd.h>
#include <fcntl.h>

using namespace std;

BOOST_AUTO_TEST_SUITE(ixxx_)

  BOOST_AUTO_TEST_SUITE(util)

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
        int t = -1;
        char out[12] = {0};
        {
        ixxx::util::FD fd(filename, O_RDONLY);
        t = fd.get();
        BOOST_CHECK(fcntl(t, F_GETFD) != -1);
        ixxx::posix::read(fd.get(), out, 11);
        }
        BOOST_CHECK(fcntl(t, F_GETFD) == -1);
        BOOST_CHECK_EQUAL(out, "Hello World");
        boost::filesystem::remove_all(dirname);
      }


  BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
