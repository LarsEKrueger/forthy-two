#include <cstring>
#include <cstdlib>
#include <iostream>
#include <sstream>

#include <forth/runtime.hpp>
#include <forth/parser.hpp>
#include <forth/tester.hpp>

/// Display help text
static void
ErrorHelp(
  const char * msg)
{
  std::cerr << "forthytwo: " << msg << std::endl <<
    "forthytwo: Command line interpreter" << std::endl <<
    "USAGE: forthytwo [OPTIONS] <filename>" << std::endl <<
    std::endl <<
    "Options:" << std::endl <<
    std::endl <<
    "  -h" << std::endl <<
    "  --help -- Display help." << std::endl <<
    "  --test <testfile> -- Run all the tests in the given file" <<
    std::endl <<
    std::endl <<
    "Parameters:" << std::endl <<
    std::endl <<
    "  <filename> -- File name of forthytwo source file" << std::endl <<
    std::endl;

  exit( EXIT_FAILURE);
}

class TestException : public std::runtime_error
{
  public:

    TestException(
      const char * a_what)
      : std::runtime_error( a_what)
    {
    }

};

static void
RunTestCases(
  const char * a_test_file_name,
  const char * a_input_file_name)
{
  forth::Tester tester;
  tester.ParseFromFile( a_test_file_name);

  std::cout << "Running tests ..." << std::endl;

  for ( size_t test_case_ind = 0;
        test_case_ind < tester.CountTestCases();
        ++test_case_ind)
  {
    const forth::Tester::TestCase &test_case =
      tester.GetTestCase(
        test_case_ind);

    std::cout << "  " << test_case_ind << "/" << tester.CountTestCases()
              << ": " << test_case.Name() << std::endl;

    forth::Runtime forth;
    forth::Parser::ParseFromFile( a_input_file_name, forth);
    forth.SetFileName( a_input_file_name);

    // Compile an additional line with the input stack and a call to the test function
    size_t line_count = forth.CountProgramLines();
    size_t start_line = test_case.GetStartLine();
    if (start_line >= line_count)
    {
      std::ostringstream str;
      str << "Test case '" << test_case.Name() << "': Illegal start line";
      throw TestException( str.str().c_str());
    }
    const std::vector<forth::Runtime::Cell> &input = test_case.GetInput();

    for (size_t index = 0; index < input.size(); ++index)
      forth.Compile( line_count, input[index]);
    forth.Compile( line_count, start_line);
    forth.Compile( line_count, forth::Runtime::kOpCodeCall);

    // Run the program until we reach the last entry of the additional line
    size_t ip_col = forth.CountInstructionsInLine( start_line);
    forth.ResetIp( start_line);
    while ( !forth.IsIpAt( start_line, ip_col) )
    {
      forth.ComputeStep();
    }

    // Compare the data stack with the output stack of the test case
    bool result_identical = (forth.GetDataStack() == test_case.GetOutput());
    std::cout << "    " << (result_identical ? "pass" : "FAILED") <<
      std::endl;
  }
}

static void
RunSource(
  const char * a_input_file_name)
{
  forth::Runtime forth;

  forth::Parser::ParseFromFile( a_input_file_name, forth);
  forth.SetFileName( a_input_file_name);
  for (;; )
  {
    forth.ComputeStep();
  }
}

int
main(
  int argc,
  char * * argv)
{
  const char *  test_file_name = NULL;

  int opti = 1;

  while ( opti < argc)
  {
    if ( !strcmp( argv[opti], "-h") || !strcmp( argv[opti], "--help"))
      ErrorHelp( "Display help text.");
    else
    if ( !strcmp( argv[opti], "--test"))
    {
      ++opti;
      if ( opti >= argc)
        ErrorHelp( "Missing argument after --test");

      test_file_name = argv[opti];
      opti++;
    }
    else
      break;
  }

  if ( opti < argc && argv[opti][0] == '-')
  {
    std::string msg( "Unknown option ");
    msg += argv[opti];
    ErrorHelp( msg.c_str());
  }

  if ( opti >= argc)
    ErrorHelp( "Too few parameter");

  const char * inputFileName = argv[opti];

  try
  {
    if (test_file_name != 0)
      RunTestCases( test_file_name, inputFileName);
    else
      RunSource( inputFileName);
  }
  catch (const std::exception &ex)
  {
    std::cerr << ex.what() << std::endl;
    return EXIT_FAILURE;
  }
  catch ( ...)
  {
    std::cerr << "Unknown exception caught" << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
