#include <cstring>
#include <fstream>
#include <sstream>
#include <string>
#include "parser.hpp"

/// Length of the line we present to the user in case of a parsing error.
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
    // If we find a non-space in the line, we have to parse it. If not, we can
    // ignore the line as empty.
    std::string::size_type firstNonSpace = a_line.find_first_not_of( " \t");
    if (firstNonSpace == std::string::npos)
      return;

    // If the line starts with a hash mark, it's a comment and we can ignore
    // it.
    if (a_line.size() > 0)
    {
      if (a_line[0] == '#')
        return;
    }

    // Seems like a legit line. Read the numbers one by one.
    std::istringstream line( a_line);
    while ( !line.eof())
    {
      Runtime::Cell v;
      // If we can parse a number, it's a number
      if ( !(line >> v))
      {
        // Drop the error state to look ahead
        line.clear();
        // Clear the buffer
        char nextChars[CHARS_IN_ERROR + 1];
        memset( nextChars, 0, sizeof(nextChars));

        // Read some characters from the line
        line.read( nextChars, CHARS_IN_ERROR);
        std::ostringstream str;
        // Build the error message
        str << a_filename << "(" << a_lineNo << "): not a number at '" <<
          nextChars << "'";
        throw ParseError( str.str().c_str());
      }

      // If everything worked, we add the number to the program
      a_runtime.Compile( a_lineNo, v);
    }
  }

  void
  Parser::ParseFromStream(
    const char *  a_filename,
    std::istream &a_input,
    Runtime &a_runtime)
  {
    // Start counting the line numbers starting from 1
    size_t lineNo = 1;

    while ( !a_input.eof())
    {
      // Read a line
      std::string line;
      std::getline( a_input, line);

      // If the line can't be read, we quit
      if (!a_input.good())
        break;

      // If the line can be called, we process it
      if ( lineNo >= Runtime::kOpCodeFirstUser &&
           lineNo != Runtime::kOpCodeCall)
      {
        CompileLine( a_filename, lineNo, line, a_runtime);
      }
      ++lineNo;
    }
  }

}
