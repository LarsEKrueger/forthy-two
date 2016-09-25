#ifndef FORTH_RUNTIME_H
#define FORTH_RUNTIME_H

#include <vector>
#include <stdint.h>
#include <cstdlib>
#include <stdexcept>

namespace forth
{
  class Runtime
  {
    public:

      typedef int32_t Cell;

      static const Cell kOpCodePlus;
      static const Cell kOpCodeMinus;
      static const Cell kOpCodeMult;
      static const Cell kOpCodeDiv;
      static const Cell kOpCodeMod;
      static const Cell kOpCodeAnd;
      static const Cell kOpCodeOr;
      static const Cell kOpCodeNot;

      static const Cell kOpCodeSwap;
      static const Cell kOpCodeDup;
      static const Cell kOpCodeDrop;

      static const Cell kOpCodeLoop;

      static const Cell kOpCodeEmit;
      static const Cell kOpCodeRead;

      static const Cell kOpCodeExit;

      static const Cell kOpCodeFirstUser;

      static const Cell kOpCodeCall;

      class StackUnderflow : public std::runtime_error
      {
        public:
          StackUnderflow(
            const char * a_what)
            : std::runtime_error( a_what)
          {
          }
      };

      Runtime();

      ~Runtime();

      void PushData(
        Cell a_data);

      void PushDataNoExec(
        Cell a_data);

      void PushReturn(
        Cell a_data);

      void Compile(
        size_t a_row,
        Cell a_number);

      void ResetIp(
        size_t a_line = kOpCodeFirstUser);

      void ComputeStep();

      void SetFileName(
        const char * a_filename);

    protected:
      std::vector<Cell> m_returnStack;
      std::vector<Cell> m_dataStack;

      std::string m_filename;

      std::vector< std::vector< Cell> > m_program;

      Cell PopData();
      Cell PopReturn();

      void DoOpcode(
        Cell a_opCode);

      typedef void (* Intrinsic)(
        Runtime &);

      static const Intrinsic kIntrinsics[];

      static void IntrPlus(
        Runtime &a_forth);
      static void IntrMinus(
        Runtime &a_forth);
      static void IntrMult(
        Runtime &a_forth);
      static void IntrDiv(
        Runtime &a_forth);
      static void IntrMod(
        Runtime &a_forth);
      static void IntrAnd(
        Runtime &a_forth);
      static void IntrOr(
        Runtime &a_forth);
      static void IntrNot(
        Runtime &a_forth);

      static void IntrSwap(
        Runtime &a_forth);
      static void IntrDup(
        Runtime &a_forth);
      static void IntrDrop(
        Runtime &a_forth);

      static void IntrLoop(
        Runtime &a_forth);

      static void IntrEmit(
        Runtime &a_forth);
      static void IntrRead(
        Runtime &a_forth);

      static void IntrExit(
        Runtime &a_forth);

      size_t m_ipLine;
      size_t m_ipCol;
  };

}

#endif
