#include <cassert>
#include <sstream>
#include "tester.hpp"

namespace forth
{
  Tester::Tester()
  {
  }

  void
  Tester::ProcessLine(
    const char * a_filename,
    size_t a_lineNo,
    const std::string &a_line
    )
  {
    // If we find a non-space in the line, we have to parse it. If not, we can
    // ignore the line as empty.
    std::string::size_type firstNonSpace = a_line.find_first_not_of( " \t");
    if (firstNonSpace == std::string::npos)
      return;

    // If the line starts with a hash mark, it's a comment and we can ignore
    // it.
    assert (a_line.size() > 0);
    if (a_line[0] == '#')
      return;

    // Ensure that the second character is a space, then get the parameters
    if (a_line.size() >= 2)
    {
      if (a_line[1] == ' ')
      {
        std::string param = a_line.substr( 2);

        switch (a_line[0]) {
          case '=':
          // Create a new test case with param as the name
          return;
          break;
          case '@':
          // Set the starting line of this test
          return;
          break;
          case '^':
          // Set the initial stack contents
          return;
          break;
          case 'v':
          // Set the final stack contents
          return;
          break;
        }
      }
    }

    std::ostringstream msg;
    msg << a_filename << ":" << a_lineNo << ": Parse error";
      throw ParseError( msg.str().c_str());
  }

  void
  Tester::ParseFromStream(
    const char *  a_filename,
    std::istream &a_input)
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

      ProcessLine( a_filename, lineNo, line);

      ++lineNo;

    }
  }
}
