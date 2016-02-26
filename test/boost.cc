#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <string>

#include <ixxx/util.hh>
#include <ixxx/util/boost.hh>
#include <ixxx/ixxx.h>

#include "helper.hh"

using namespace std;

BOOST_AUTO_TEST_SUITE(ixxx_)

  BOOST_AUTO_TEST_SUITE(util)

    BOOST_AUTO_TEST_SUITE(boost_)

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
        {
        ixxx::util::RO_Mapped_File f(filename);
        string out(f.sbegin(), f.send());
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
        ixxx::util::RO_Mapped_File f(filename);
        const uint8_t *begin = f.begin();
        const uint8_t *end = f.end();
        string out(reinterpret_cast<const char *>(begin), reinterpret_cast<const char*>(end));
        BOOST_CHECK_EQUAL(out, "Hello World");
        }
        boost::filesystem::remove_all(dirname);
      }

    BOOST_AUTO_TEST_SUITE_END()

  BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
