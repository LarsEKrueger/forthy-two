#include <cstring>
#include <fstream>
#include <sstream>
#include "parser.hpp"

#define CHARS_IN_ERROR 10

namespace forth
{

  void
  Parser::ParseFromFile(
    const char * a_filename,
    Runtime &a_runtime)
  {
    std::ifstream f( a_filename, std::ios_base::in);

    if ( !f.is_open())
    {
      std::ostringstream str;
      str << "Cannot open '" << a_filename << "'";
      throw ParseError( str.str().c_str());
    }

    ParseFromStream( a_filename, f, a_runtime);
  }

  void
  Parser::CompileLine(
    const char * a_filename,
    size_t a_lineNo,
    const std::string &a_line,
    Runtime &a_runtime)
  {
    std::istringstream line( a_line);

    while ( !line.eof())
    {
      Runtime::Cell v;
      if ( !(line >> v))
      {
        line.clear();
        char nextChars[CHARS_IN_ERROR + 1];
        memset( nextChars, 0, sizeof(nextChars));

        line.read( nextChars, CHARS_IN_ERROR);
        std::ostringstream str;
        str << a_filename << "(" << a_lineNo << "): not a number at '" <<
          nextChars << "'";
        throw ParseError( str.str().c_str());
      }
      a_runtime.Compile( a_lineNo, v);
    }
  }

  void
  Parser::ParseFromStream(
    const char *  a_filename,
    std::istream &a_input,
    Runtime &a_runtime)
  {
    size_t lineNo = 1;

    while ( !a_input.eof())
    {
      std::string line;
      std::getline( a_input, line);
      if (!a_input.good())
        break;

      if ( lineNo >= Runtime::kOpCodeFirstUser &&
           lineNo != Runtime::kOpCodeCall)
      {
        CompileLine( a_filename, lineNo, line, a_runtime);
      }
      ++lineNo;
    }
  }
}
