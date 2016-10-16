#define BOOST_TEST_MODULE TestTester
#include <boost/test/unit_test.hpp>
#include <forth/runtime.hpp>
#include <forth/parser.hpp>
#include <forth/tester.hpp>

class TestTester : public forth::Tester
{
  public:
    TestTester() : forth::Tester()
    {

    }

    void TestParseFromStream(
      const char *  a_filename,
      std::istream &a_input)
      {
        ParseFromStream( a_filename, a_input);
      }


};

BOOST_AUTO_TEST_CASE(TestTesterClass)
{
  std::stringstream file;

// A comment
  file << "= Test 0" << std::endl;

// Start function
  file << "@ 21" << std::endl;

// Initial stack
  file << "^ 0 1 2 3" << std::endl;

// Final stack
  file << "v 4 5 6 7" << std::endl;

  file << std::endl;

  TestTester tester;

  tester.TestParseFromStream( "file", file);

}
