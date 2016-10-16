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

  const Runtime::Cell Runtime::kOpCodeEmit = 12;
  const Runtime::Cell Runtime::kOpCodeRead = 13;

  const Runtime::Cell Runtime::kOpCodeExit = 14;

  const Runtime::Cell Runtime::kOpCodeFirstUser = 21;

  const Runtime::Cell Runtime::kOpCodeCall = 42;

  /// Lookup table of the intrinsics. Keep in sync with the opcodes above.
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

    IntrEmit,
    IntrRead,

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
    // If the number to push is magic, take the top of the stack and call the
    // line
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
    // Handle an empty stack
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
  size_t
  Runtime::PopReturn()
  {
    // Handle an empty stack
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
    // If it's a valid line
    if ( a_row >= kOpCodeFirstUser && a_row != kOpCodeCall)
    {
      // Resize the program memory if required
      if ( m_program.size() <= a_row)
        m_program.resize( a_row + 1);
      // Append the number to the line
      m_program[a_row].push_back( a_number);
    }
  }
  void
  Runtime::DoOpcode(
    Cell a_opCode)
  {
    // Only handle legal, i.e. non-negative opcodes
    if (a_opCode >= 0)
    {
      // If it is an intrinsic, run it
      if (a_opCode < kOpCodeFirstUser)
        kIntrinsics[ a_opCode]( *this);
      else
      {
        // Otherwise, we remember where we are
        PushReturn( m_ipLine);
        PushReturn( m_ipCol);

        // And set the IP to the new position
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
    // If the IP is outside the program, we jump back to the beginning.
    if (m_ipLine < m_program.size())
    {
      // If we still have things to do on this line
      if (m_ipCol < m_program[m_ipLine].size())
      {
        // Read the value
        Cell v = m_program[m_ipLine][m_ipCol];
        // Advance the IP
        m_ipCol++;
        // Push the data or execute the code
        PushData( v);
      }
      else
      {
        // We are at the end of the line, pop the return stack and continue
        // where we left off
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
    // Get the two values and convert them to bools
    bool a = a_forth.PopData() != 0;
    bool b = a_forth.PopData() != 0;

    // Push the result back
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
    // Get the index of the value below the top of the stack
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

    // We check the value for being non-zero
    Cell v = a_forth.m_dataStack[tos];
    if (v == 0)
    {
      // If it is zero, drop it and continue with the next instruction
      a_forth.PopData();
    }
    else
    {
      // Start at the beginning of the line
      a_forth.m_ipCol = 0;
    }
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
