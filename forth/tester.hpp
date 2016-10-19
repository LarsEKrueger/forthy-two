#ifndef FORTH_TESTER_H
#define FORTH_TESTER_H

#include <istream>
#include <stdexcept>
#include <vector>

#include "runtime.hpp"

namespace forth
{
  /** This class stores a list of test cases.
   */
  class Tester
  {
    public:

      /// Parsing exception
      class ParseError : public std::runtime_error
      {
        public:

          ParseError(
            const char * a_what)
            : std::runtime_error( a_what)
          {
          }

      };

      /** A test case defines input and output of a function call.
       */
      class TestCase
      {
        public:

          /// Construct a test case by name
          TestCase(
            const std::string &a_name)
            : m_name( a_name)
            , m_start_line( Runtime::kOpCodeFirstUser)
          {
          }

          /// Get the name of the test case
          const std::string &

          Name() const
          {
            return m_name;
          }

          /// Set the line to call for this test case
          void
          SetStartLine(
            Runtime::Cell a_line)
          {
            m_start_line = a_line;
          }

          /// Get the line to call for this test case
          size_t
          GetStartLine() const
          {
            return m_start_line;
          }

          /// Add a number to be used for input
          void
          AddInputNumber(
            Runtime::Cell a_nr)
          {
            m_input_stack.push_back( a_nr);
          }

          /// Add a number to be used for checking the output of this test case
          void
          AddOutputNumber(
            Runtime::Cell a_nr)
          {
            m_output_stack.push_back( a_nr);
          }

          /// Get the inputs of this test case
          const std::vector<Runtime::Cell> &

          GetInput() const
          {
            return m_input_stack;
          }

          /// Get the desired output of this test case
          const std::vector<Runtime::Cell> &

          GetOutput() const
          {
            return m_output_stack;
          }

        protected:

          /// Name of this test case
          std::string m_name;

          /// Line to start for this test case
          Runtime::Cell m_start_line;

          /// Values of the data stack to be used for the test
          std::vector<Runtime::Cell> m_input_stack;

          /// Values of the data stack to be expected after the function call
          std::vector<Runtime::Cell> m_output_stack;
      };

      /// Construct a tester
      Tester();

      /// Parse a test case description file
      void
      ParseFromFile(
        const char * a_filename);

      /// Return the number of test cases loaded
      size_t
      CountTestCases() const;

      /// Access a specific test case
      const TestCase &

      GetTestCase(
        size_t a_index) const;

    protected:

      /// List of test cases
      std::vector<TestCase> m_test_case;

      /// Parse a test case description file from a stream
      void
      ParseFromStream(
        const char *  a_filename,
        std::istream &a_input);

      /// Helper function to process a single line of the test case file
      void
      ProcessLine(
        const char * a_filename,
        size_t a_lineNo,
        const std::string &a_line);

      /// Throw a ParseError exception with a proper error message
      void
      ThrowParseError(
        const char * a_filename,
        size_t a_lineNo,
        const char * a_error_message);

  };

}

#endif
