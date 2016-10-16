#ifndef _parser_hpp_
#define _parser_hpp_

#include <istream>
#include "runtime.hpp"

namespace forth
{
  /** Parser functions to read the source files of forthytwo.
   *
   * The functions are implemented as static members of the Parser struct.
   * This way, we can make the internal functions protected. The testing rig
   * can then make them accessible again.
   */
  struct Parser
  {
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

    /// Public interface to parsing function
    static void
    ParseFromFile(
      const char * a_filename,
      Runtime &a_runtime);

    protected:
      /// Process a line from the stream
      static void
      CompileLine(
        const char * a_filename,
        size_t a_lineNo,
        const std::string &a_line,
        Runtime &a_runtime);

      /// Parse the stream
      static void
      ParseFromStream(
        const char *  a_filename,
        std::istream &a_input,
        Runtime &a_runtime);

  };

}

#endif
