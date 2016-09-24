#ifndef FORTH_RUNTIME_H
#define FORTH_RUNTIME_H

#include <vector>
#include <stdint.h>
#include <cstdlib>

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
      static const Cell kOpCodeLoop;
      static const Cell kOpCodeIf;
      static const Cell kOpCodeIfElse;
      static const Cell kOpCodeEmit;

      static const Cell kOpCodeFirstUser;

      static const Cell kOpCodeCall;

      Runtime();

      ~Runtime();

      void PushData(
        Cell a_data);

      void PushReturn(
        Cell a_data);

    protected:
      std::vector<Cell> m_returnStack;
      std::vector<Cell> m_dataStack;

      Cell PopData();

      void DoOpcode();

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
      static void IntrLoop(
        Runtime &a_forth);
      static void IntrIf(
        Runtime &a_forth);
      static void IntrIfElse(
        Runtime &a_forth);
      static void IntrEmit(
        Runtime &a_forth);
      static void IntrExit(
        Runtime &a_forth);

      Cell m_ipLine;
      Cell m_ipCol;
  };

}

#endif
