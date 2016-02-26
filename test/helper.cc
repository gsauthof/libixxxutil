#include "helper.hh"

#include <boost/filesystem.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <string.h>

#include <ixxx/util.hh>

using namespace std;

char *portable_mkdtemp(char *dir_template)
{
#if (defined(__MINGW32__) || defined(__MINGW64__))
  string t(dir_template);
  boost::replace_all(t, "X", "%");
  auto p = boost::filesystem::unique_path(t);
  string r(p.generic_string());
  if (r.size() != strlen(dir_template))
    throw logic_error("mkdtemp path too long");
  strcpy(dir_template, r.c_str());
  boost::filesystem::create_directories(p);
  return dir_template;
#else
  return mkdtemp(dir_template);
#endif
}

