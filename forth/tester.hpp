#ifndef FORTH_TESTER_H
#define FORTH_TESTER_H

#include <istream>
#include <stdexcept>
#include <vector>

#include "runtime.hpp"

namespace forth
{
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

      class TestCase
      {
        public:

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

          void
          SetStartLine(
            Runtime::Cell a_line)
          {
            m_start_line = a_line;
          }

          void
          AddInputNumber(
            Runtime::Cell a_nr)
          {
            m_input_stack.push_back( a_nr);
          }

          void
          AddOutputNumber(
            Runtime::Cell a_nr)
          {
            m_output_stack.push_back( a_nr);
          }

          const std::vector<Runtime::Cell> &

          GetInput() const
          {
            return m_input_stack;
          }

          const std::vector<Runtime::Cell> &

          GetOutput() const
          {
            return m_output_stack;
          }

        protected:
          std::string m_name;
          Runtime::Cell m_start_line;
          std::vector<Runtime::Cell> m_input_stack;
          std::vector<Runtime::Cell> m_output_stack;
      };

      Tester();

      void
      ParseFromFile(
        const char * a_filename);

      size_t
      CountTestCases() const;

    protected:

      std::vector<TestCase> m_test_case;

      void
      ParseFromStream(
        const char *  a_filename,
        std::istream &a_input);

      void
      ProcessLine(
        const char * a_filename,
        size_t a_lineNo,
        const std::string &a_line);

      void
      ThrowParseError(
        const char * a_filename,
        size_t a_lineNo,
        const char * a_error_message);

  };

}

#endif
