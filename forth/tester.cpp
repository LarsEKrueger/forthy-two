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
    const std::string &a_line)
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
    if (a_line.size() < 2)
      ThrowParseError( a_filename, a_lineNo, "Missing line leader");

    if (a_line[1] != ' ')
      ThrowParseError( a_filename, a_lineNo, "Missing space in line leader");

    std::string param = a_line.substr( 2);
    switch (a_line[0])
    {
      case '=':
        // Create a new test case with param as the name
        m_test_case.push_back( TestCase( param));
        break;
      case '@':
        {
          // Set the starting line of this test
          if (m_test_case.size() == 0)
            ThrowParseError( a_filename, a_lineNo, "No test declared");

          TestCase &test_case = m_test_case[m_test_case.size() - 1];
          std::istringstream nr_parser(param);
          Runtime::Cell start_line = -1;
          if ( !(nr_parser >> start_line))
            ThrowParseError( a_filename, a_lineNo, "Can't parse start line");

          if ( start_line < Runtime::kOpCodeFirstUser)
            ThrowParseError( a_filename, a_lineNo, "Start line too low");
          test_case.SetStartLine( start_line);
        }
        break;
      case '^':
        {
          // Set the initial stack contents
          if (m_test_case.size() == 0)
            ThrowParseError( a_filename, a_lineNo, "No test declared");

          TestCase &test_case = m_test_case[m_test_case.size() - 1];
          std::istringstream nr_parser(param);
          while (!nr_parser.eof())
          {
            Runtime::Cell nr;
            if ( !(nr_parser >> nr))
              ThrowParseError( a_filename, a_lineNo, "Can't parse number");

            test_case.AddInputNumber( nr);
          }
        }
        break;
      case 'v':
        {
          // Set the final stack contents
          if (m_test_case.size() == 0)
            ThrowParseError( a_filename, a_lineNo, "No test declared");

          TestCase &test_case = m_test_case[m_test_case.size() - 1];
          std::istringstream nr_parser(param);
          while (!nr_parser.eof())
          {
            Runtime::Cell nr;
            if ( !(nr_parser >> nr))
              ThrowParseError( a_filename, a_lineNo, "Can't parse number");

            test_case.AddOutputNumber( nr);
          }
        }
        break;
      default:
        ThrowParseError( a_filename, a_lineNo, "Bad line leader");
        break;
    }
  }

  void
  Tester::ThrowParseError(
    const char * a_filename,
    size_t a_lineNo,
    const char * a_error_message)
  {
    std::ostringstream msg;
    msg << a_filename << ":"
        << a_lineNo << ": Parse error ("
        << a_error_message
        << ")";
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

  size_t
  Tester::CountTestCases() const
  {
    return m_test_case.size();
  }

}
