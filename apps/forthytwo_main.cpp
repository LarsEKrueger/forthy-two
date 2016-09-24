#include <cstring>
#include <cstdlib>
#include <iostream>

#include <forth/runtime.hpp>
#include <forth/parser.hpp>

static void 
ErrorHelp( const char * msg)
{
  std::cerr << "forthytwo: " << msg << std::endl <<
    "forthytwo: Command line interpreter" << std::endl <<
    "USAGE: forthytwo [OPTIONS] <filename>" << std::endl <<
    std::endl <<
    "Options:" << std::endl <<
    std::endl <<
    "  -h" << std::endl <<
    "  --help -- Display help." << std::endl <<
    std::endl <<
    "Parameters:" << std::endl <<
    std::endl <<
    "  <filename> -- File name of forthytwo source file" << std::endl <<
    std::endl;

  exit( EXIT_FAILURE);
}


int main( int argc, char * * argv)
{
  int opti=1;
  while( opti<argc)
  {
    if( !strcmp( argv[opti], "-h") || !strcmp( argv[opti], "--help"))
      ErrorHelp( "Display help text.");
    else
      break;
  }

  if( opti<argc && argv[opti][0]=='-')
  {
    std::string msg( "Unknown option ");
    msg+=argv[opti];
    ErrorHelp( msg.c_str());
  }

  if( opti>=argc)
    ErrorHelp( "Too few parameter");

  const char * inputFileName = argv[opti];

  forth::Runtime forth;
  try
  {
  forth::Parser::ParseFromFile( inputFileName, forth);
  }
  catch (const std::exception & ex)
  {
    std::cerr << "forthytwo:" << ex.what() << std::endl;
  }
  catch( ...)
  {
    std::cerr << "forthytwo: Unknown exception caught" << std::endl;
  }

  return EXIT_SUCCESS;
}
