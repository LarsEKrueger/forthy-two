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

    void
    TestParseFromStream(
      const char *  a_filename,
      std::istream &a_input)
    {
      ParseFromStream( a_filename, a_input);
    }

    const forth::Tester::TestCase &
    TestGetCase(
      size_t a_ind) const
    {
      return m_test_case[a_ind];
    }

};

BOOST_AUTO_TEST_CASE(GoodCase)
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

  // A second test
  file << "= Test 1" << std::endl;
  file << "@ 22" << std::endl;
  file << "^ 3 2 1 0" << std::endl;
  file << "v 7 6 5" << std::endl;

  TestTester tester;

  tester.TestParseFromStream( "file", file);

  BOOST_REQUIRE_EQUAL( tester.CountTestCases(), 2);
  BOOST_CHECK_EQUAL( tester.TestGetCase(0).Name(), "Test 0");

  BOOST_CHECK_EQUAL( tester.TestGetCase(0).GetInput().size(), 4);
  BOOST_CHECK_EQUAL( tester.TestGetCase(0).GetInput()[0], 0);
  BOOST_CHECK_EQUAL( tester.TestGetCase(0).GetInput()[1], 1);
  BOOST_CHECK_EQUAL( tester.TestGetCase(0).GetInput()[2], 2);
  BOOST_CHECK_EQUAL( tester.TestGetCase(0).GetInput()[3], 3);

  BOOST_CHECK_EQUAL( tester.TestGetCase(0).GetOutput().size(), 4);
  BOOST_CHECK_EQUAL( tester.TestGetCase(0).GetOutput()[0], 4);
  BOOST_CHECK_EQUAL( tester.TestGetCase(0).GetOutput()[1], 5);
  BOOST_CHECK_EQUAL( tester.TestGetCase(0).GetOutput()[2], 6);
  BOOST_CHECK_EQUAL( tester.TestGetCase(0).GetOutput()[3], 7);

  BOOST_CHECK_EQUAL( tester.TestGetCase(1).Name(), "Test 1");
  BOOST_CHECK_EQUAL( tester.TestGetCase(1).GetInput().size(), 4);
  BOOST_CHECK_EQUAL( tester.TestGetCase(1).GetInput()[0], 3);
  BOOST_CHECK_EQUAL( tester.TestGetCase(1).GetInput()[1], 2);
  BOOST_CHECK_EQUAL( tester.TestGetCase(1).GetInput()[2], 1);
  BOOST_CHECK_EQUAL( tester.TestGetCase(1).GetInput()[3], 0);

  BOOST_CHECK_EQUAL( tester.TestGetCase(1).GetOutput().size(), 3);
  BOOST_CHECK_EQUAL( tester.TestGetCase(1).GetOutput()[0], 7);
  BOOST_CHECK_EQUAL( tester.TestGetCase(1).GetOutput()[1], 6);
  BOOST_CHECK_EQUAL( tester.TestGetCase(1).GetOutput()[2], 5);
}
static void
TestErrorMessage(
  const char * a_file_content,
  const char * a_error_message)
{
  std::stringstream file;

  file << a_file_content;

  TestTester tester;

  bool correctExceptionWasCaught = false;
  try
  {
    tester.TestParseFromStream( "file", file);
  }
  catch ( forth::Tester::ParseError &parse_error)
  {
    BOOST_CHECK_EQUAL( parse_error.what(), a_error_message);
    correctExceptionWasCaught = true;
  }

  BOOST_CHECK( correctExceptionWasCaught);
}

BOOST_AUTO_TEST_CASE(ParseFail)
{
  TestErrorMessage(
    "@ 21\n",
    "file:1: Parse error (No test declared)");
  TestErrorMessage(
    "^ 21\n",
    "file:1: Parse error (No test declared)");
  TestErrorMessage(
    "v 21\n",
    "file:1: Parse error (No test declared)");
  TestErrorMessage(
    "= Test 0\n@ xxx\n",
    "file:2: Parse error (Can't parse start line)");
  TestErrorMessage(
    "= Test 0\n@ 12\n",
    "file:2: Parse error (Start line too low)");
  TestErrorMessage(
    "= Test 0\n^ 1 2 3 4 xx\n",
    "file:2: Parse error (Can't parse number)");
  TestErrorMessage(
    "= Test 0\nv 1 2 3 4 xx\n",
    "file:2: Parse error (Can't parse number)");
}
