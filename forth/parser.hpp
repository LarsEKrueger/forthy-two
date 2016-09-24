#ifndef _parser_hpp_
#define _parser_hpp_

#include <istream>
#include "runtime.hpp"

namespace forth
{
  struct Parser
  {
    class ParseError : public std::runtime_error
    {
      public:
        ParseError(
          const char * a_what)
          : std::runtime_error( a_what)
        {
        }
    };

    static void
    ParseFromFile(
      const char * a_filename,
      Runtime &a_runtime);

    protected:

      static void
      CompileLine(
        const char *       a_filename,
        size_t a_lineNo,
        const std::string &a_line,
        Runtime &a_runtime);

      static void
      ParseFromStream(
        const char *  a_filename,
        std::istream &a_input,
        Runtime &a_runtime);
  };

}

#endif
