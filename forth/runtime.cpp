#include "runtime.hpp"

namespace forth
{
  const Runtime::Cell Runtime::kOpCodePlus = 0;
  const Runtime::Cell Runtime::kOpCodeMinus = 1;
  const Runtime::Cell Runtime::kOpCodeMult = 2;
  const Runtime::Cell Runtime::kOpCodeDiv = 3;
  const Runtime::Cell Runtime::kOpCodeMod = 4;
  const Runtime::Cell Runtime::kOpCodeAnd = 5;
  const Runtime::Cell Runtime::kOpCodeOr = 6;
  const Runtime::Cell Runtime::kOpCodeNot = 7;
  const Runtime::Cell Runtime::kOpCodeLoop = 8;
  const Runtime::Cell Runtime::kOpCodeIf = 9;
  const Runtime::Cell Runtime::kOpCodeIfElse = 10;
  const Runtime::Cell Runtime::kOpCodeEmit = 12;

  const Runtime::Cell Runtime::kOpCodeFirstUser = 21;

  const Runtime::Cell Runtime::kOpCodeCall = 42;

  const Runtime::Intrinsic Runtime::kIntrinsics[ Runtime::kOpCodeFirstUser] =
  {
    IntrPlus,
    IntrMinus,
    IntrMult,
    IntrDiv,
    IntrMod,
    IntrAnd,
    IntrOr,
    IntrNot,
    IntrLoop,
    IntrIf,
    IntrIfElse,
    IntrEmit,
    IntrExit,
    IntrExit,
    IntrExit,
    IntrExit,
    IntrExit,
    IntrExit,
    IntrExit,
    IntrExit,
    IntrExit,
  };

  Runtime::Runtime()
  {

  }

  Runtime::~Runtime()
  {

  }

  void
  Runtime::PushData(
    Cell a_data)
  {
    if ( a_data == kOpCodeCall)
      DoOpcode();
    else
      m_dataStack.push_back( a_data);
  }

  Runtime::Cell
  Runtime::PopData()
  {
    if( m_dataStack.empty())
      throw StackUnderflow( "PopData");

    Cell res = m_dataStack.back();

    m_dataStack.pop_back();
    return res;
  }

  void
  Runtime::PushReturn(
    Cell a_data)
  {
    m_returnStack.push_back( a_data);
  }

  Runtime::Cell
  Runtime::PopReturn()
  {
    if( m_returnStack.empty())
      throw StackUnderflow( "PopReturn");

    Cell res = m_returnStack.back();

    m_returnStack.pop_back();
    return res;
  }

  void
  Runtime::Compile(
    size_t a_row,
    Cell   a_number)
  {
    if ( a_row >= kOpCodeFirstUser && a_row != kOpCodeCall)
    {
      if ( m_program.size() <= a_row)
        m_program.resize( a_row + 1);
      m_program[a_row].push_back( a_number);
    }
  }

  void
  Runtime::DoOpcode()
  {
    Cell opCode = PopData();

    if (opCode < kOpCodeFirstUser)
      kIntrinsics[ opCode]( *this);
    else
    {
      PushReturn( m_ipLine);
      PushReturn( m_ipCol);

      m_ipLine = opCode;
      m_ipCol = 0;
    }
  }

  void
  Runtime::ComputeStep()
  {
    if (m_ipLine < m_program.size())
    {
      if (m_ipCol < m_program[m_ipLine].size())
      {
        Cell v = m_program[m_ipLine][m_ipCol];
        m_ipCol++;
        PushData( v);
      }
      else
      {
        // Pop the return stack
        m_ipCol = PopReturn();
        m_ipLine = PopReturn();
      }
    }
    else
    {
      // Go to start
      m_ipLine = kOpCodeFirstUser;
      m_ipCol = 0;
    }
  }

  void
  Runtime::IntrPlus(
    Runtime &a_forth)
  {
    Cell a = a_forth.PopData();
    Cell b = a_forth.PopData();

    a_forth.PushData( a + b);
  }

  void
  Runtime::IntrMinus(
    Runtime &a_forth)
  {
    Cell a = a_forth.PopData();
    Cell b = a_forth.PopData();

    a_forth.PushData( b - a);
  }

  void
  Runtime::IntrMult(
    Runtime &a_forth)
  {
    Cell a = a_forth.PopData();
    Cell b = a_forth.PopData();

    a_forth.PushData( a * b);
  }

  void
  Runtime::IntrDiv(
    Runtime &a_forth)
  {
    Cell a = a_forth.PopData();
    Cell b = a_forth.PopData();

    a_forth.PushData( b / a);
  }

  void
  Runtime::IntrMod(
    Runtime &a_forth)
  {
    Cell a = a_forth.PopData();
    Cell b = a_forth.PopData();

    a_forth.PushData( b % a);
  }

  void
  Runtime::IntrAnd(
    Runtime &a_forth)
  {
    bool a = a_forth.PopData() != 0;
    bool b = a_forth.PopData() != 0;

    a_forth.PushData( (a && b) ? 1 : 0);
  }

  void
  Runtime::IntrOr(
    Runtime &a_forth)
  {
    bool a = a_forth.PopData() != 0;
    bool b = a_forth.PopData() != 0;

    a_forth.PushData( (a || b) ? 1 : 0);
  }

  void
  Runtime::IntrNot(
    Runtime &a_forth)
  {
    bool a = a_forth.PopData() != 0;

    a_forth.PushData( (!a) ? 1 : 0);
  }

  void
  Runtime::IntrLoop(
    Runtime &a_forth)
  {
  }

  void
  Runtime::IntrIf(
    Runtime &a_forth)
  {
  }

  void
  Runtime::IntrIfElse(
    Runtime &a_forth)
  {
  }

  void
  Runtime::IntrEmit(
    Runtime &a_forth)
  {
  }

  void
  Runtime::IntrExit(
    Runtime &a_forth)
  {
  }

}
