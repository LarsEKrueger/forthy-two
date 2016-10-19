#define BOOST_TEST_MODULE TestRuntime
#include <boost/test/unit_test.hpp>
#include <forth/runtime.hpp>
#include <forth/parser.hpp>

/** Interface to expose protected attributes and methods.
 */
class TestRuntime : public forth::Runtime
{
  public:

    TestRuntime() : forth::Runtime()
    {
    }

    size_t
    TestDataStackSize() const
    {
      return m_dataStack.size();
    }

    Cell
    TestDataStackAt(
      size_t a_ind) const
    {
      return m_dataStack[a_ind];
    }

    size_t
    TestReturnStackSize() const
    {
      return m_returnStack.size();
    }

    Cell
    TestPopData()
    {
      return PopData();
    }

    const std::vector< std::vector< Cell > > &

    TestGetProgram() const
    {
      return m_program;
    }

    size_t
    TestGetIpLine() const
    {
      return m_ipLine;
    }

    size_t
    TestGetIpCol() const
    {
      return m_ipCol;
    }

    /** Test function to check if the opcodes and the intrinsics match. This
     * is easier done here than exporting all the symbols and checking
     * outside.
     */
    static void
    CheckIndices()
    {
      BOOST_CHECK( kIntrinsics[kOpCodePlus] == &IntrPlus);
      BOOST_CHECK( kIntrinsics[kOpCodeMinus] == &IntrMinus);
      BOOST_CHECK( kIntrinsics[kOpCodeMult] == &IntrMult);
      BOOST_CHECK( kIntrinsics[kOpCodeDiv] == &IntrDiv);
      BOOST_CHECK( kIntrinsics[kOpCodeMod] == &IntrMod);
      BOOST_CHECK( kIntrinsics[kOpCodeAnd] == &IntrAnd);
      BOOST_CHECK( kIntrinsics[kOpCodeOr] == &IntrOr);
      BOOST_CHECK( kIntrinsics[kOpCodeNot] == &IntrNot);
      BOOST_CHECK( kIntrinsics[kOpCodeSwap] == &IntrSwap);
      BOOST_CHECK( kIntrinsics[kOpCodeDup] == &IntrDup);
      BOOST_CHECK( kIntrinsics[kOpCodeDrop] == &IntrDrop);
      BOOST_CHECK( kIntrinsics[kOpCodeLoop] == &IntrLoop);
      BOOST_CHECK( kIntrinsics[kOpCodeEmit] == &IntrEmit);
      BOOST_CHECK( kIntrinsics[kOpCodeRead] == &IntrRead);
      BOOST_CHECK( kIntrinsics[kOpCodeExit] == &IntrExit);
      BOOST_CHECK( kIntrinsics[kOpCodeOver] == &IntrOver);
    }

};

/// Tie the actual test into the framework
BOOST_AUTO_TEST_CASE(OpCodeIndices)
{
  TestRuntime::CheckIndices();
}

/// Check if the stacks work
BOOST_AUTO_TEST_CASE(Basics)
{
  TestRuntime forth;

  forth.PushData( 0);
  BOOST_CHECK_EQUAL( forth.TestDataStackSize(), 1);

  forth.PushReturn( 0);
  forth.PushReturn( 1);
  BOOST_CHECK_EQUAL( forth.TestReturnStackSize(), 2);
}

/// Test function for intrinsics with two operands
static void
TestIntrinsic2(
  forth::Runtime::Cell a_operand1,
  forth::Runtime::Cell a_operand2,
  forth::Runtime::Cell a_intrinsic,
  forth::Runtime::Cell a_result)
{
  TestRuntime forth;

  forth.PushData( a_operand1);
  forth.PushData( a_operand2);

  forth.PushData( a_intrinsic);
  forth.PushData( TestRuntime::kOpCodeCall);

  BOOST_CHECK_EQUAL( forth.TestPopData(), a_result);

  BOOST_CHECK_EQUAL( forth.TestDataStackSize(), 0);
}

/// Test function for intrinsics with one operand
static void
TestIntrinsic1(
  forth::Runtime::Cell a_operand,
  forth::Runtime::Cell a_intrinsic,
  forth::Runtime::Cell a_result)
{
  TestRuntime forth;

  forth.PushData( a_operand);

  forth.PushData( a_intrinsic);
  forth.PushData( TestRuntime::kOpCodeCall);

  BOOST_CHECK_EQUAL( forth.TestPopData(), a_result);

  BOOST_CHECK_EQUAL( forth.TestDataStackSize(), 0);
}

/// Compile a call to an intrinsic
static void
TestCompileCall(
  TestRuntime &a_forth,
  size_t a_line,
  TestRuntime::Cell a_opCode)
{
  a_forth.Compile( a_line, a_opCode);
  a_forth.Compile( a_line, TestRuntime::kOpCodeCall);
}

/// Test the individual intrinsics
BOOST_AUTO_TEST_CASE(Intrinsics)
{
  TestIntrinsic2( 2, 3, TestRuntime::kOpCodePlus, 5);
  TestIntrinsic2( 2, 3, TestRuntime::kOpCodeMinus, -1);

  TestIntrinsic2( 2, 3, TestRuntime::kOpCodeMult, 6);

  TestIntrinsic2( 2, 3, TestRuntime::kOpCodeDiv, 0);
  TestIntrinsic2( 4, 2, TestRuntime::kOpCodeDiv, 2);

  TestIntrinsic2( 3, 2, TestRuntime::kOpCodeMod, 1);

  TestIntrinsic2( 0, 0, TestRuntime::kOpCodeAnd, 0);
  TestIntrinsic2( 1, 0, TestRuntime::kOpCodeAnd, 0);
  TestIntrinsic2( 0, 1, TestRuntime::kOpCodeAnd, 0);
  TestIntrinsic2( 1, 1, TestRuntime::kOpCodeAnd, 1);

  TestIntrinsic2( 0, 0, TestRuntime::kOpCodeOr, 0);
  TestIntrinsic2( 1, 0, TestRuntime::kOpCodeOr, 1);
  TestIntrinsic2( 0, 1, TestRuntime::kOpCodeOr, 1);
  TestIntrinsic2( 1, 1, TestRuntime::kOpCodeOr, 1);

  TestIntrinsic1( 2, TestRuntime::kOpCodeNot, 0);
  TestIntrinsic1( 0, TestRuntime::kOpCodeNot, 1);
}

/// Test the compiler
BOOST_AUTO_TEST_CASE(Compiler)
{
  TestRuntime forth;

  forth.Compile( TestRuntime::kOpCodeFirstUser, 2);
  forth.Compile( TestRuntime::kOpCodeFirstUser, 3);
  TestCompileCall( forth,
    TestRuntime::kOpCodeFirstUser,
    TestRuntime::kOpCodePlus);

  const std::vector< std::vector< TestRuntime::Cell > > &prg =
    forth.TestGetProgram();

  BOOST_REQUIRE_EQUAL( prg.size(), TestRuntime::kOpCodeFirstUser + 1);
  BOOST_CHECK_EQUAL( prg[TestRuntime::kOpCodeFirstUser].size(), 4);
  BOOST_CHECK_EQUAL( prg[TestRuntime::kOpCodeFirstUser][0], 2);
  BOOST_CHECK_EQUAL( prg[TestRuntime::kOpCodeFirstUser][1], 3);
  BOOST_CHECK_EQUAL( prg[TestRuntime::kOpCodeFirstUser][2],
    TestRuntime::kOpCodePlus);
  BOOST_CHECK_EQUAL( prg[TestRuntime::kOpCodeFirstUser][3],
    TestRuntime::kOpCodeCall);
}

/// Compile a simple sequence and run it step by step.
BOOST_AUTO_TEST_CASE(Running)
{
  TestRuntime forth;

  forth.Compile( TestRuntime::kOpCodeFirstUser, 2);
  forth.Compile( TestRuntime::kOpCodeFirstUser, 3);

  TestCompileCall( forth,
    TestRuntime::kOpCodeFirstUser,
    TestRuntime::kOpCodePlus);

  forth.ResetIp();
  forth.ComputeStep();
  BOOST_CHECK_EQUAL( forth.TestDataStackSize(), 1);
  forth.ComputeStep();
  BOOST_CHECK_EQUAL( forth.TestDataStackSize(), 2);
  forth.ComputeStep();
  BOOST_CHECK_EQUAL( forth.TestDataStackSize(), 3);
  forth.ComputeStep();
  BOOST_CHECK_EQUAL( forth.TestDataStackSize(), 1);
}

/// Compile a simple program where one line calls the other, and then run it.
BOOST_AUTO_TEST_CASE(Calling)
{
  TestRuntime forth;

  // Compile to call the second line
  TestCompileCall( forth,
    TestRuntime::kOpCodeFirstUser,
    TestRuntime::kOpCodeFirstUser + 1);

  // For the second line, compile two numbers and the opcode to add them
  forth.Compile( TestRuntime::kOpCodeFirstUser + 1, 2);
  forth.Compile( TestRuntime::kOpCodeFirstUser + 1, 3);
  TestCompileCall( forth,
    TestRuntime::kOpCodeFirstUser + 1,
    TestRuntime::kOpCodePlus);

  forth.ResetIp();
  forth.ComputeStep();
  forth.ComputeStep();
  forth.ComputeStep();
  forth.ComputeStep();
  forth.ComputeStep();
  forth.ComputeStep();
  BOOST_CHECK_EQUAL( forth.TestDataStackSize(), 1);
}

/// Test the swap intrinsic
BOOST_AUTO_TEST_CASE(Swap)
{
  TestRuntime forth;

  forth.PushData( 2);
  forth.PushData( 3);

  BOOST_CHECK_EQUAL( forth.TestDataStackAt( 0), 2);
  BOOST_CHECK_EQUAL( forth.TestDataStackAt( 1), 3);

  forth.PushData( TestRuntime::kOpCodeSwap);
  forth.PushData( TestRuntime::kOpCodeCall);

  BOOST_REQUIRE_EQUAL( forth.TestDataStackSize(), 2);
  BOOST_CHECK_EQUAL( forth.TestDataStackAt( 0), 3);
  BOOST_CHECK_EQUAL( forth.TestDataStackAt( 1), 2);
}

/// Test the dup intrinsic
BOOST_AUTO_TEST_CASE(Dup)
{
  TestRuntime forth;

  forth.PushData( 2);

  BOOST_CHECK_EQUAL( forth.TestDataStackAt( 0), 2);

  forth.PushData( TestRuntime::kOpCodeDup);
  forth.PushData( TestRuntime::kOpCodeCall);

  BOOST_REQUIRE_EQUAL( forth.TestDataStackSize(), 2);
  BOOST_CHECK_EQUAL( forth.TestDataStackAt( 0), 2);
  BOOST_CHECK_EQUAL( forth.TestDataStackAt( 1), 2);
}

/// Test the drop intrinsic
BOOST_AUTO_TEST_CASE(Drop)
{
  TestRuntime forth;

  forth.PushData( 2);

  BOOST_CHECK_EQUAL( forth.TestDataStackAt( 0), 2);

  forth.PushData( TestRuntime::kOpCodeDrop);
  forth.PushData( TestRuntime::kOpCodeCall);

  BOOST_REQUIRE_EQUAL( forth.TestDataStackSize(), 0);
}

/// Test the looping mechanism
BOOST_AUTO_TEST_CASE(Looping)
{
  TestRuntime forth;

  // Place 5 and 7 on the stack, then call the second line
  // 21: 5 7 22 call
  forth.Compile( TestRuntime::kOpCodeFirstUser, 5);
  forth.Compile( TestRuntime::kOpCodeFirstUser, 7);
  TestCompileCall( forth,
    TestRuntime::kOpCodeFirstUser,
    TestRuntime::kOpCodeFirstUser + 1);

  // Expects an accumulator and an iteration counter.
  // add 4 to the accumulator, then decrement the counter.
  // Jump back to the beginning if the counter is non-zero
  // 22: swap 4 + swap 1 - loop
  TestCompileCall( forth,
    TestRuntime::kOpCodeFirstUser + 1,
    TestRuntime::kOpCodeSwap);
  forth.Compile( TestRuntime::kOpCodeFirstUser + 1, 4);
  TestCompileCall( forth,
    TestRuntime::kOpCodeFirstUser + 1,
    TestRuntime::kOpCodePlus);
  TestCompileCall( forth,
    TestRuntime::kOpCodeFirstUser + 1,
    TestRuntime::kOpCodeSwap);
  forth.Compile( TestRuntime::kOpCodeFirstUser + 1, 1);
  TestCompileCall( forth,
    TestRuntime::kOpCodeFirstUser + 1,
    TestRuntime::kOpCodeMinus);
  TestCompileCall( forth,
    TestRuntime::kOpCodeFirstUser + 1,
    TestRuntime::kOpCodeLoop);

  forth.ResetIp();

  // After 4 + 7 * ( 2 + 1 + 2 + 2 + 1 + 2 + 2) = 88 operations we should be
  // done
  for (unsigned i = 0; i < 88; ++i)
    forth.ComputeStep();

  BOOST_CHECK_EQUAL( forth.TestDataStackSize(), 1);
  BOOST_CHECK_EQUAL( forth.TestPopData(), 5 + 7 * 4);
}

/// Test the over instruction
BOOST_AUTO_TEST_CASE(Over)
{
  TestRuntime forth;

  forth.PushDataNoExec( 1);
  forth.PushDataNoExec( 2);

  forth.PushData( TestRuntime::kOpCodeOver);
  forth.PushData( TestRuntime::kOpCodeCall);

  BOOST_CHECK_EQUAL( forth.TestDataStackSize(), 3);
  BOOST_CHECK_EQUAL( forth.TestPopData(), 1);
  BOOST_CHECK_EQUAL( forth.TestPopData(), 2);
  BOOST_CHECK_EQUAL( forth.TestPopData(), 1);
}

/** Interface to expose the protected methods in the parser class.
 * This is not in its own test file because we need access to the private
 * methods of the runtime.
 */
class TestParser : public forth::Parser
{
  public:

    static void
    TestParseFromStream(
      const char *  a_filename,
      std::istream &a_input,
      forth::Runtime &a_runtime)
    {
      ParseFromStream(
        a_filename,
        a_input,
        a_runtime);
    }

};

/// Parse and run a simple program
BOOST_AUTO_TEST_CASE(ParseOk)
{
  std::stringstream file;

  // Add the header
  for (unsigned i = 1; i < TestRuntime::kOpCodeFirstUser; ++i)
    file << std::endl;

  // Add the program. The program is equivalent to:
  // a = 5
  // if (a % 2 == 0) a += 2; else a += 1;
  // if (a % 2 == 0) a += 2; else a += 1;
  // We do, however, use the result of the modulus operation as an offset to
  // call line 22 on even, and line 23 on odd values.
  // 21:   5  dup   2  mod   22  plus  call dup   2  mod   22 plus call
  file << "5  9 42  2  4 42  22  0 42  42   9 42  2  4 42  22 0 42 42" <<
    std::endl;

  // 22: 2 +
  file << "2 0 42" << std::endl;

  // 23: 1 +
  file << "1 0 42" << std::endl;

  TestRuntime forth;
  TestParser::TestParseFromStream( "file", file, forth);

  const std::vector< std::vector< TestRuntime::Cell > > &prg =
    forth.TestGetProgram();

  BOOST_REQUIRE_EQUAL( prg.size(), TestRuntime::kOpCodeFirstUser + 3);
  BOOST_REQUIRE_EQUAL( prg[TestRuntime::kOpCodeFirstUser].size(), 19);

  BOOST_CHECK_EQUAL( prg[TestRuntime::kOpCodeFirstUser][0], 5);
  BOOST_CHECK_EQUAL( prg[TestRuntime::kOpCodeFirstUser][1], 9);
  BOOST_CHECK_EQUAL( prg[TestRuntime::kOpCodeFirstUser][18], 42);

  BOOST_REQUIRE_EQUAL( prg[TestRuntime::kOpCodeFirstUser + 1].size(), 3);
  BOOST_REQUIRE_EQUAL( prg[TestRuntime::kOpCodeFirstUser + 2].size(), 3);

  forth.ResetIp();

  // We should be in line 23
  for (unsigned i = 0; i < 10; ++i)
    forth.ComputeStep();
  BOOST_CHECK_EQUAL( forth.TestGetIpLine(),
    TestRuntime::kOpCodeFirstUser + 2);
  BOOST_CHECK_EQUAL( forth.TestGetIpCol(), 0);

  // We should be in line 22
  for (unsigned i = 0; i < 13; ++i)
    forth.ComputeStep();
  BOOST_CHECK_EQUAL( forth.TestGetIpLine(),
    TestRuntime::kOpCodeFirstUser + 1);
  BOOST_CHECK_EQUAL( forth.TestGetIpCol(), 0);

  // Four more steps and we are at the end.
  for (unsigned i = 0; i < 4; ++i)
    forth.ComputeStep();
  BOOST_CHECK_EQUAL( forth.TestGetIpLine(),
    TestRuntime::kOpCodeFirstUser);
  BOOST_CHECK_EQUAL( forth.TestGetIpCol(), 19);

  BOOST_CHECK_EQUAL( forth.TestPopData(), 8);
}

/** Same program as before, but with a whitespace-only line and a comment line
 * before the two incrementing lines.
 */
BOOST_AUTO_TEST_CASE(ParseSpaceAndComments)
{
  std::stringstream file;

  // Add the header
  for (unsigned i = 1; i < TestRuntime::kOpCodeFirstUser; ++i)
    file << std::endl;

  // Add the program
  // 21:   5  dup   2  mod   24  plus  call dup   2  mod   24 plus call
  file << "5  9 42  2  4 42  24  0 42  42   9 42  2  4 42  24 0 42 42" <<
    std::endl;

  // 22:
  file << "   \t  " << std::endl;

  // 23: # Comment
  file << "Comment" << std::endl;

  // 24: 2 +
  file << "2 0 42" << std::endl;

  // 25: 1 +
  file << "1 0 42" << std::endl;

  TestRuntime forth;

  bool hasCaughtAnException = false;
  try
  {
    TestParser::TestParseFromStream( "file", file, forth);
  }
  catch (...)
  {
    hasCaughtAnException = true;
  }

  BOOST_REQUIRE_EQUAL( hasCaughtAnException, false);

  const std::vector< std::vector< TestRuntime::Cell > > &prg =
    forth.TestGetProgram();

  BOOST_REQUIRE_EQUAL( prg.size(), TestRuntime::kOpCodeFirstUser + 5);
  BOOST_REQUIRE_EQUAL( prg[TestRuntime::kOpCodeFirstUser].size(), 19);

  BOOST_CHECK_EQUAL( prg[TestRuntime::kOpCodeFirstUser][0], 5);
  BOOST_CHECK_EQUAL( prg[TestRuntime::kOpCodeFirstUser][1], 9);
  BOOST_CHECK_EQUAL( prg[TestRuntime::kOpCodeFirstUser][18], 42);

  BOOST_REQUIRE_EQUAL( prg[TestRuntime::kOpCodeFirstUser + 3].size(), 3);
  BOOST_REQUIRE_EQUAL( prg[TestRuntime::kOpCodeFirstUser + 4].size(), 3);

  forth.ResetIp();

  // We should be in line 25
  for (unsigned i = 0; i < 10; ++i)
    forth.ComputeStep();
  BOOST_CHECK_EQUAL( forth.TestGetIpLine(),
    TestRuntime::kOpCodeFirstUser + 4);
  BOOST_CHECK_EQUAL( forth.TestGetIpCol(), 0);

  // We should be in line 24
  for (unsigned i = 0; i < 13; ++i)
    forth.ComputeStep();
  BOOST_CHECK_EQUAL( forth.TestGetIpLine(),
    TestRuntime::kOpCodeFirstUser + 3);
  BOOST_CHECK_EQUAL( forth.TestGetIpCol(), 0);

  // Four more steps and we are at the end.
  for (unsigned i = 0; i < 4; ++i)
    forth.ComputeStep();
  BOOST_CHECK_EQUAL( forth.TestGetIpLine(),
    TestRuntime::kOpCodeFirstUser);
  BOOST_CHECK_EQUAL( forth.TestGetIpCol(), 19);

  BOOST_CHECK_EQUAL( forth.TestPopData(), 8);
}

/// Test if a non-number can be detected by the parser.
BOOST_AUTO_TEST_CASE(ParseFail)
{
  std::stringstream file;

  // Add the header
  for (unsigned i = 1; i < TestRuntime::kOpCodeFirstUser; ++i)
    file << std::endl;

  // Add the program
  // 21: 5 dup 2 mod 22 if dup 2 mod 23 22 ifElse
  file << "5  xx 9 " << std::endl;

  TestRuntime forth;
  bool hasCaughtAnException = false;
  try
  {
    TestParser::TestParseFromStream( "file", file, forth);
  }
  catch (...)
  {
    hasCaughtAnException = true;
  }

  BOOST_REQUIRE_EQUAL( hasCaughtAnException, false);
  const std::vector< std::vector< TestRuntime::Cell > > &prg =
    forth.TestGetProgram();

  BOOST_REQUIRE_EQUAL( prg.size(), TestRuntime::kOpCodeFirstUser + 1);
  BOOST_CHECK_EQUAL( prg[TestRuntime::kOpCodeFirstUser].size(), 1);
  BOOST_CHECK_EQUAL( prg[TestRuntime::kOpCodeFirstUser][0], 5);
}
