#include <cstring>
#include <cstdlib>
#include <iostream>

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

static void
RunTestCases(
  const char * a_test_file_name,
  const char * a_input_file_name)
{
  forth::Tester tester;
  tester.ParseFromFile( a_test_file_name);

  for( size_t test_case_ind=0; test_case_ind < tester.CountTestCases(); ++test_case_ind)
  {
    forth::Runtime forth;
    forth::Parser::ParseFromFile( a_input_file_name, forth);
    forth.SetFileName( a_input_file_name);

    // Compile an additional line with the input stack and a call to the test function

    // Run the program until we reach the last entry of the additional line

    // Compare the data stack with the output stack of the test case

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
  }
  catch ( ...)
  {
    std::cerr << "Unknown exception caught" << std::endl;
  }

  return EXIT_SUCCESS;
}
