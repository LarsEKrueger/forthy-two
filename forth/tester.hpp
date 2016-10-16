#ifndef FORTH_TESTER_H
#define FORTH_TESTER_H

#include <istream>
#include <stdexcept>

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

      Tester();

    protected:
      void ParseFromStream(
        const char *  a_filename,
        std::istream &a_input);

      void
      ProcessLine(
        const char * a_filename,
        size_t a_lineNo,
        const std::string &a_line
        );
  };

}

#endif
