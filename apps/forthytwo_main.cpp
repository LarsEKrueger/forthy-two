#include <cstring>
#include <cstdlib>
#include <iostream>

#include <forth/runtime.hpp>
#include <forth/parser.hpp>

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
    "  --start <n> -- Start processing at line (default:21)." << std::endl <<
    "  --push <n>  -- Push <n> to the data stack." << std::endl <<
    "                 Can be used multiple times." << std::endl <<
    std::endl <<
    "Parameters:" << std::endl <<
    std::endl <<
    "  <filename> -- File name of forthytwo source file" << std::endl <<
    std::endl;

  exit( EXIT_FAILURE);
}

int
main(
  int argc, char * * argv)
{
  forth::Runtime forth;

  int opti = 1;
  while ( opti < argc)
  {
    if ( !strcmp( argv[opti], "-h") || !strcmp( argv[opti], "--help"))
      ErrorHelp( "Display help text.");
    else
    if ( !strcmp( argv[opti], "--start"))
    {
      ++opti;
      if ( opti >= argc)
        ErrorHelp( "Missing argument after --start");

      forth.ResetIp( atoi(argv[opti]));
      opti++;
    }
    else
    if ( !strcmp( argv[opti], "--push"))
    {
      ++opti;
      if ( opti >= argc)
        ErrorHelp( "Missing argument after --push");

      forth.PushDataNoExec( atoi( argv[opti] ));
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
    forth::Parser::ParseFromFile( inputFileName, forth);
    forth.SetFileName( inputFileName);
    for (;; )
    {
      forth.ComputeStep();
    }
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
