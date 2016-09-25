#include "runtime.hpp"

#include <iostream>
#include <sstream>

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

  const Runtime::Cell Runtime::kOpCodeSwap = 8;
  const Runtime::Cell Runtime::kOpCodeDup = 9;
  const Runtime::Cell Runtime::kOpCodeDrop = 10;

  const Runtime::Cell Runtime::kOpCodeLoop = 11;
  const Runtime::Cell Runtime::kOpCodeIf = 12;
  const Runtime::Cell Runtime::kOpCodeIfElse = 13;

  const Runtime::Cell Runtime::kOpCodeEmit = 14;
  const Runtime::Cell Runtime::kOpCodeRead = 15;

  const Runtime::Cell Runtime::kOpCodeExit = 16;

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

    IntrSwap,
    IntrDup,
    IntrDrop,

    IntrLoop,
    IntrIf,
    IntrIfElse,

    IntrEmit,
    IntrRead,

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
    {
      Cell opCode = PopData();
      DoOpcode( opCode);
    }
    else
      PushDataNoExec( a_data);
  }

  void
  Runtime::PushDataNoExec(
    Cell a_data)
  {
    m_dataStack.push_back( a_data);
  }

  Runtime::Cell
  Runtime::PopData()
  {
    if ( m_dataStack.empty())
    {
      std::ostringstream str;
      str << m_filename << "(" << m_ipLine << "): data stack underflow";
      throw StackUnderflow( str.str().c_str());
    }

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
    if ( m_returnStack.empty())
    {
      std::ostringstream str;
      str << m_filename << "(" << m_ipLine << "): return stack underflow";
      throw StackUnderflow( str.str().c_str());
    }

    Cell res = m_returnStack.back();

    m_returnStack.pop_back();
    return res;
  }

  void
  Runtime::Compile(
    size_t a_row,
    Cell a_number)
  {
    if ( a_row >= kOpCodeFirstUser && a_row != kOpCodeCall)
    {
      if ( m_program.size() <= a_row)
        m_program.resize( a_row + 1);
      m_program[a_row].push_back( a_number);
    }
  }

  void
  Runtime::DoOpcode(
    Cell a_opCode)
  {
    if (a_opCode >= 0)
    {
      if (a_opCode < kOpCodeFirstUser)
        kIntrinsics[ a_opCode]( *this);
      else
      {
        PushReturn( m_ipLine);
        PushReturn( m_ipCol);

        m_ipLine = a_opCode;
        m_ipCol = 0;
      }
    }
  }

  void
  Runtime::ResetIp(
    size_t a_line)
  {
    m_ipLine = a_line;
    m_ipCol = 0;
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

    a_forth.PushDataNoExec( a + b);
  }

  void
  Runtime::IntrMinus(
    Runtime &a_forth)
  {
    Cell a = a_forth.PopData();
    Cell b = a_forth.PopData();

    a_forth.PushDataNoExec( b - a);
  }

  void
  Runtime::IntrMult(
    Runtime &a_forth)
  {
    Cell a = a_forth.PopData();
    Cell b = a_forth.PopData();

    a_forth.PushDataNoExec( a * b);
  }

  void
  Runtime::IntrDiv(
    Runtime &a_forth)
  {
    Cell a = a_forth.PopData();
    Cell b = a_forth.PopData();

    a_forth.PushDataNoExec( b / a);
  }

  void
  Runtime::IntrMod(
    Runtime &a_forth)
  {
    Cell a = a_forth.PopData();
    Cell b = a_forth.PopData();

    a_forth.PushDataNoExec( b % a);
  }

  void
  Runtime::IntrAnd(
    Runtime &a_forth)
  {
    bool a = a_forth.PopData() != 0;
    bool b = a_forth.PopData() != 0;

    a_forth.PushDataNoExec( (a && b) ? 1 : 0);
  }

  void
  Runtime::IntrOr(
    Runtime &a_forth)
  {
    bool a = a_forth.PopData() != 0;
    bool b = a_forth.PopData() != 0;

    a_forth.PushDataNoExec( (a || b) ? 1 : 0);
  }

  void
  Runtime::IntrNot(
    Runtime &a_forth)
  {
    bool a = a_forth.PopData() != 0;

    a_forth.PushDataNoExec( (!a) ? 1 : 0);
  }

  void
  Runtime::IntrSwap(
    Runtime &a_forth)
  {
    if ( a_forth.m_dataStack.size() < 2)
      throw StackUnderflow( "Swap");
    size_t tos1 = a_forth.m_dataStack.size() - 2;
    std::swap( a_forth.m_dataStack[tos1], a_forth.m_dataStack[tos1 + 1]);
  }

  void
  Runtime::IntrDup(
    Runtime &a_forth)
  {
    if ( a_forth.m_dataStack.size() == 0)
      throw StackUnderflow( "Dup");
    size_t tos = a_forth.m_dataStack.size() - 1;
    a_forth.PushDataNoExec( a_forth.m_dataStack[tos]);
  }

  void
  Runtime::IntrDrop(
    Runtime &a_forth)
  {
    a_forth.PopData();
  }

  void
  Runtime::IntrLoop(
    Runtime &a_forth)
  {
    if ( a_forth.m_dataStack.size() == 0)
      throw StackUnderflow( "Dup");
    size_t tos = a_forth.m_dataStack.size() - 1;
    Cell v = a_forth.m_dataStack[tos];
    if (v == 0)
    {
      a_forth.PopData();
    }
    else
    {
      a_forth.m_ipCol = 0;
    }
  }

  void
  Runtime::IntrIf(
    Runtime &a_forth)
  {
    Cell opCode = a_forth.PopData();
    Cell cond = a_forth.PopData();

    if (cond != 0)
      a_forth.DoOpcode( opCode);
  }

  void
  Runtime::IntrIfElse(
    Runtime &a_forth)
  {
    Cell opCodeNo = a_forth.PopData();
    Cell opCodeYes = a_forth.PopData();
    Cell cond = a_forth.PopData();

    a_forth.DoOpcode( (cond != 0) ? opCodeYes : opCodeNo);
  }

  void
  Runtime::IntrEmit(
    Runtime &a_forth)
  {
    Cell v = a_forth.PopData();

    if (0 <= v && v < 255)
      std::cout << char(v);
  }

  void
  Runtime::IntrRead(
    Runtime &a_forth)
  {
    char c;

    std::cin >> c;

    // We can't use PushData here or every * will trigger something
    a_forth.PushDataNoExec( c);
  }

  void
  Runtime::IntrExit(
    Runtime &a_forth)
  {
    Cell v = a_forth.PopData();

    exit( v);
  }

  void
  Runtime::SetFileName(
    const char * a_filename)
  {
    m_filename = a_filename;
  }
}
