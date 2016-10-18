#ifndef FORTH_RUNTIME_H
#define FORTH_RUNTIME_H

#include <vector>
#include <stdint.h>
#include <cstdlib>
#include <stdexcept>

namespace forth
{
  /** Execution environment for the language.
   *
   * It consists of a data stack, a return stack, and the program memory.
   * While the data stack and the return stack are readable and writable, the
   * program memory is only writable from the outside of the class.
   *
   * Program memory is organized in lines. Each line contains a list of
   * number. When executed, these numbers are pushed onto the data stack. The
   * only exception is number 42, which, when pushed to the stack, will take
   * the top number from the stack and try to execute the respective line from
   * the program memory.
   *
   * The first few lines are not user-programmable. They represent the basic
   * operations (e.g. adding numbers). These intrinsics are the basic building
   * blocks of the programs.
   */
  class Runtime
  {
    public:

      /// Contents of the data stack
      typedef int32_t Cell;

      /** @name Constants for the instrinsics. */
      /*@{*/

      /// Opcode for addition
      static const Cell kOpCodePlus;

      /// Opcode for difference
      static const Cell kOpCodeMinus;

      /// Opcode for multiplication
      static const Cell kOpCodeMult;

      /// Opcode for division
      static const Cell kOpCodeDiv;

      /// Opcode for modulus
      static const Cell kOpCodeMod;

      /// Opcode for logical and
      static const Cell kOpCodeAnd;

      /// Opcode for logical or
      static const Cell kOpCodeOr;

      /// Opcode for logical not
      static const Cell kOpCodeNot;

      /// Opcode for swapping the two top-most items
      static const Cell kOpCodeSwap;

      /// Opcode duplicating the op-most item on the data stack
      static const Cell kOpCodeDup;

      /// Opcode to remove the top-most item from the data stack
      static const Cell kOpCodeDrop;

      /// Opcode to restart the current line on condition
      static const Cell kOpCodeLoop;

      /// Opcode to write a single character to stdout
      static const Cell kOpCodeEmit;

      /// Opcode to read a single character from stdin
      static const Cell kOpCodeRead;

      /// Opcode to exit the program
      static const Cell kOpCodeExit;

      /// Opcode to duplicate the second item on the stack
      static const Cell kOpCodeOver;

      /// Number of the first user-programmable line
      static const Cell kOpCodeFirstUser;

      /// Opcode to interpret the top-most item on the stack as an opcode.
      static const Cell kOpCodeCall;
      /*@}*/

      /// Exception to be thrown when a taking a number from an empty stack.
      class StackUnderflow : public std::runtime_error
      {
        public:

          StackUnderflow(
            const char * a_what)
            : std::runtime_error( a_what)
          {
          }

      };

      /// Construct a runtime instance
      Runtime();

      /// Destruct a runtime instance
      ~Runtime();

      /** Push a number onto the data stack. If the number is 42, interpret
       * the top-most item on the data stack as a line to call.
       */
      void
      PushData(
        Cell a_data);

      /** Push a number onto the data stack, no special treatment for number
       * 42.
       */
      void
      PushDataNoExec(
        Cell a_data);

      /// Push a number onto the return stack
      void
      PushReturn(
        Cell a_data);

      /// Add a number to a given line
      void
      Compile(
        size_t a_row,
        Cell a_number);

      /// Set the instruction pointer to the first number in a given line
      void
      ResetIp(
        size_t a_line = kOpCodeFirstUser);

      /// Perform one step of computation.
      void
      ComputeStep();

      /// Set the name of the source file for error messages.
      void
      SetFileName(
        const char * a_filename);

      /// Get the number of lines in the program
      size_t
      CountProgramLines();

      /// Get the number of instructions in a line of the program
      size_t
      CountInstructionsInLine(
        size_t a_row);

      /// Check if the instruction pointer is at a row/column
      bool
      IsIpAt(
        size_t a_row,
        size_t a_col);

      /// Access the data stack read-only
      const std::vector<Cell> &

      GetDataStack() const;

    protected:

      /// Return stack
      std::vector<size_t> m_returnStack;

      /// Data stack
      std::vector<Cell> m_dataStack;

      /// File name for error messages
      std::string m_filename;

      /// Program memory
      std::vector< std::vector< Cell> > m_program;

      /// Take one number from the data stack
      Cell
      PopData();

      /// Take one number from the return stack
      size_t
      PopReturn();

      /// Execute the opcode
      void
      DoOpcode(
        Cell a_opCode);

      /// Prototype of a function to run an intrinsic
      typedef void (* Intrinsic)(
        Runtime &);

      /// Table of intrinsic
      static const Intrinsic kIntrinsics[];

      /// Intrinsic to add two numbers
      static void
      IntrPlus(
        Runtime &a_forth);

      /// Intrinsic to subtract two numbers
      static void
      IntrMinus(
        Runtime &a_forth);

      /// Intrinsic to multiply two numbers
      static void
      IntrMult(
        Runtime &a_forth);

      /// Intrinsic to divide two numbers
      static void
      IntrDiv(
        Runtime &a_forth);

      /// Intrinsic to compute the modulus of two numbers
      static void
      IntrMod(
        Runtime &a_forth);

      /// Intrinsic to compute the logical and
      static void
      IntrAnd(
        Runtime &a_forth);

      /// Intrinsic to compute the logical or
      static void
      IntrOr(
        Runtime &a_forth);

      /// Intrinsic to compute the logical inversion
      static void
      IntrNot(
        Runtime &a_forth);

      /// Intrinsic to swap the top two stack items
      static void
      IntrSwap(
        Runtime &a_forth);

      /// Intrinsic to duplicate the top stack item
      static void
      IntrDup(
        Runtime &a_forth);

      /// Intrinsic to remove the top item from the data stack
      static void
      IntrDrop(
        Runtime &a_forth);

      /// Intrinsic to loop to the beginning of the line
      static void
      IntrLoop(
        Runtime &a_forth);

      /// Intrinsic to print a single character
      static void
      IntrEmit(
        Runtime &a_forth);

      /// Intrinsic to read a single character
      static void
      IntrRead(
        Runtime &a_forth);

      /// Intrinsic to exit the program
      static void
      IntrExit(
        Runtime &a_forth);

        /// Intrinsic to duplicate the second item on the stack
        static void
        IntrOver(
          Runtime &a_forth);

      /// Instruction pointer, the line we're currently executing
      size_t m_ipLine;

      /// Instruction pointer, the index into the current line
      size_t m_ipCol;
  };

}

#endif
