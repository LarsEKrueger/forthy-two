#define BOOST_TEST_MODULE TestRuntime
#include <boost/test/unit_test.hpp>
#include <forth/runtime.hpp>
#include <forth/parser.hpp>

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
        BOOST_CHECK( kIntrinsics[kOpCodeIf] == &IntrIf);
        BOOST_CHECK( kIntrinsics[kOpCodeIfElse] == &IntrIfElse);
        BOOST_CHECK( kIntrinsics[kOpCodeEmit] == &IntrEmit);
        BOOST_CHECK( kIntrinsics[kOpCodeRead] == &IntrRead);
        BOOST_CHECK( kIntrinsics[kOpCodeExit] == &IntrExit);
      }
};

BOOST_AUTO_TEST_CASE(OpCodeIndices)
{
  TestRuntime::CheckIndices();
}

BOOST_AUTO_TEST_CASE(Basics)
{
  TestRuntime forth;

  forth.PushData( 0);
  BOOST_CHECK_EQUAL( forth.TestDataStackSize(), 1);

  forth.PushReturn( 0);
  forth.PushReturn( 1);
  BOOST_CHECK_EQUAL( forth.TestReturnStackSize(), 2);
}

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

static void
TestCompileCall(
  TestRuntime &a_forth,
  size_t a_line,
  TestRuntime::Cell a_opCode)
{
  a_forth.Compile( a_line, a_opCode);
  a_forth.Compile( a_line, TestRuntime::kOpCodeCall);
}

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

BOOST_AUTO_TEST_CASE(Calling)
{
  TestRuntime forth;

  TestCompileCall( forth,
    TestRuntime::kOpCodeFirstUser,
    TestRuntime::kOpCodeFirstUser + 1);

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

BOOST_AUTO_TEST_CASE(Dropt)
{
  TestRuntime forth;

  forth.PushData( 2);

  BOOST_CHECK_EQUAL( forth.TestDataStackAt( 0), 2);

  forth.PushData( TestRuntime::kOpCodeDrop);
  forth.PushData( TestRuntime::kOpCodeCall);

  BOOST_REQUIRE_EQUAL( forth.TestDataStackSize(), 0);
}

BOOST_AUTO_TEST_CASE(Looping)
{
  TestRuntime forth;

  // 21: 5 7 22 call
  forth.Compile( TestRuntime::kOpCodeFirstUser, 5);
  forth.Compile( TestRuntime::kOpCodeFirstUser, 7);
  TestCompileCall( forth,
    TestRuntime::kOpCodeFirstUser,
    TestRuntime::kOpCodeFirstUser + 1);

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

BOOST_AUTO_TEST_CASE(If)
{
  TestRuntime forth;

  // We run the following program
  // 21: 5 dup 2 mod 22 if dup 2 mod 22 23 ifElse
  // 22: 1 +
  // 23: 2 +

  // The if will enter line 22 because 5 % 2 = 1 -> true
  // Line 22 will increment TOS to 6
  // The ifElse will enter line 23 because 6 % 2 = -> false
  // Line 23 will increment TOS to 8

  // 21
  forth.Compile( TestRuntime::kOpCodeFirstUser, 5);
  TestCompileCall( forth,
    TestRuntime::kOpCodeFirstUser,
    TestRuntime::kOpCodeDup);
  forth.Compile( TestRuntime::kOpCodeFirstUser, 2);
  TestCompileCall( forth,
    TestRuntime::kOpCodeFirstUser,
    TestRuntime::kOpCodeMod);
  forth.Compile( TestRuntime::kOpCodeFirstUser,
    TestRuntime::kOpCodeFirstUser + 1);
  TestCompileCall( forth,
    TestRuntime::kOpCodeFirstUser,
    TestRuntime::kOpCodeIf);

  TestCompileCall( forth,
    TestRuntime::kOpCodeFirstUser,
    TestRuntime::kOpCodeDup);
  forth.Compile( TestRuntime::kOpCodeFirstUser, 2);
  TestCompileCall( forth,
    TestRuntime::kOpCodeFirstUser,
    TestRuntime::kOpCodeMod);

  forth.Compile( TestRuntime::kOpCodeFirstUser,
    TestRuntime::kOpCodeFirstUser + 1);
  forth.Compile( TestRuntime::kOpCodeFirstUser,
    TestRuntime::kOpCodeFirstUser + 2);
  TestCompileCall( forth,
    TestRuntime::kOpCodeFirstUser,
    TestRuntime::kOpCodeIfElse);

  // 22
  forth.Compile( TestRuntime::kOpCodeFirstUser + 1, 1);
  TestCompileCall( forth,
    TestRuntime::kOpCodeFirstUser + 1,
    TestRuntime::kOpCodePlus);

  // 23
  forth.Compile( TestRuntime::kOpCodeFirstUser + 2, 2);
  TestCompileCall( forth,
    TestRuntime::kOpCodeFirstUser + 2,
    TestRuntime::kOpCodePlus);

  forth.ResetIp();

  // After 9 steps, we should be in line 22
  for (unsigned i = 0; i < 9; ++i)
    forth.ComputeStep();
  BOOST_CHECK_EQUAL( forth.TestGetIpLine(),
    TestRuntime::kOpCodeFirstUser + 1);
  BOOST_CHECK_EQUAL( forth.TestGetIpCol(), 0);

  // 13 steps later, we should be in line 23
  for (unsigned i = 0; i < 13; ++i)
    forth.ComputeStep();
  BOOST_CHECK_EQUAL( forth.TestGetIpLine(),
    TestRuntime::kOpCodeFirstUser + 2);
  BOOST_CHECK_EQUAL( forth.TestGetIpCol(), 0);

  // Four more steps and we are at the end.
  for (unsigned i = 0; i < 4; ++i)
    forth.ComputeStep();
  BOOST_CHECK_EQUAL( forth.TestGetIpLine(),
    TestRuntime::kOpCodeFirstUser);
  BOOST_CHECK_EQUAL( forth.TestGetIpCol(), 18);

  BOOST_CHECK_EQUAL( forth.TestPopData(), 8);
}

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

BOOST_AUTO_TEST_CASE(ParseOk)
{
  std::stringstream file;

  // Add the header
  for (unsigned i = 1; i < TestRuntime::kOpCodeFirstUser; ++i)
    file << std::endl;

  // Add the program
  // 21: 5 dup 2 mod 22 if dup 2 mod 22 23 ifElse
  file << "5  9 42  2  4 42  22  12 42  9 42  2  4 42  22  23  13 42" <<
    std::endl;
  // 22: 1 +
  file << "1 0 42" << std::endl;
  // 23: 2 +
  file << "2 0 42" << std::endl;

  TestRuntime forth;
  TestParser::TestParseFromStream( "file", file, forth);

  const std::vector< std::vector< TestRuntime::Cell > > &prg =
    forth.TestGetProgram();

  BOOST_REQUIRE_EQUAL( prg.size(), TestRuntime::kOpCodeFirstUser + 3);
  BOOST_REQUIRE_EQUAL( prg[TestRuntime::kOpCodeFirstUser].size(), 18);

  BOOST_CHECK_EQUAL( prg[TestRuntime::kOpCodeFirstUser][0], 5);
  BOOST_CHECK_EQUAL( prg[TestRuntime::kOpCodeFirstUser][1], 9);
  BOOST_CHECK_EQUAL( prg[TestRuntime::kOpCodeFirstUser][17], 42);

  BOOST_REQUIRE_EQUAL( prg[TestRuntime::kOpCodeFirstUser + 1].size(), 3);
  BOOST_REQUIRE_EQUAL( prg[TestRuntime::kOpCodeFirstUser + 2].size(), 3);

  forth.ResetIp();

  // After 9 steps, we should be in line 22
  for (unsigned i = 0; i < 9; ++i)
    forth.ComputeStep();
  BOOST_CHECK_EQUAL( forth.TestGetIpLine(),
    TestRuntime::kOpCodeFirstUser + 1);
  BOOST_CHECK_EQUAL( forth.TestGetIpCol(), 0);

  // 13 steps later, we should be in line 23
  for (unsigned i = 0; i < 13; ++i)
    forth.ComputeStep();
  BOOST_CHECK_EQUAL( forth.TestGetIpLine(),
    TestRuntime::kOpCodeFirstUser + 2);
  BOOST_CHECK_EQUAL( forth.TestGetIpCol(), 0);

  // Four more steps and we are at the end.
  for (unsigned i = 0; i < 4; ++i)
    forth.ComputeStep();
  BOOST_CHECK_EQUAL( forth.TestGetIpLine(),
    TestRuntime::kOpCodeFirstUser);
  BOOST_CHECK_EQUAL( forth.TestGetIpCol(), 18);

  BOOST_CHECK_EQUAL( forth.TestPopData(), 8);
}

BOOST_AUTO_TEST_CASE(ParseSpaceAndComments)
{
  std::stringstream file;

  // Add the header
  for (unsigned i = 1; i < TestRuntime::kOpCodeFirstUser; ++i)
    file << std::endl;

  // Add the program
  // 21: 5 dup 2 mod 23 if dup 2 mod 23 26 ifElse
  file << "5  9 42  2  4 42  23  12 42  9 42  2  4 42  23  26  13 42" <<
    std::endl;
  // 22:
  file << "   \t  " << std::endl;
  // 23: 1 +
  file << "1 0 42" << std::endl;
  // 24:
  file << std::endl;
  // 25: # Comment
  file << "# Comment" << std::endl;
  // 26: 2 +
  file << "2 0 42" << std::endl;

  TestRuntime forth;
  TestParser::TestParseFromStream( "file", file, forth);

  const std::vector< std::vector< TestRuntime::Cell > > &prg =
    forth.TestGetProgram();

  BOOST_REQUIRE_EQUAL( prg.size(), TestRuntime::kOpCodeFirstUser + 6);
  BOOST_REQUIRE_EQUAL( prg[TestRuntime::kOpCodeFirstUser].size(), 18);

  BOOST_CHECK_EQUAL( prg[TestRuntime::kOpCodeFirstUser][0], 5);
  BOOST_CHECK_EQUAL( prg[TestRuntime::kOpCodeFirstUser][1], 9);
  BOOST_CHECK_EQUAL( prg[TestRuntime::kOpCodeFirstUser][17], 42);

  BOOST_REQUIRE_EQUAL( prg[TestRuntime::kOpCodeFirstUser + 2].size(), 3);
  BOOST_REQUIRE_EQUAL( prg[TestRuntime::kOpCodeFirstUser + 5].size(), 3);

  forth.ResetIp();

  // After 9 steps, we should be in line 23
  for (unsigned i = 0; i < 9; ++i)
    forth.ComputeStep();
  BOOST_CHECK_EQUAL( forth.TestGetIpLine(),
    TestRuntime::kOpCodeFirstUser + 2);
  BOOST_CHECK_EQUAL( forth.TestGetIpCol(), 0);

  // 13 steps later, we should be in line 26
  for (unsigned i = 0; i < 13; ++i)
    forth.ComputeStep();
  BOOST_CHECK_EQUAL( forth.TestGetIpLine(),
    TestRuntime::kOpCodeFirstUser + 5);
  BOOST_CHECK_EQUAL( forth.TestGetIpCol(), 0);

  // Four more steps and we are at the end.
  for (unsigned i = 0; i < 4; ++i)
    forth.ComputeStep();
  BOOST_CHECK_EQUAL( forth.TestGetIpLine(),
    TestRuntime::kOpCodeFirstUser);
  BOOST_CHECK_EQUAL( forth.TestGetIpCol(), 18);

  BOOST_CHECK_EQUAL( forth.TestPopData(), 8);
}

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
  bool hasCaughtTheRightException = false;
  try
  {
    TestParser::TestParseFromStream( "file", file, forth);
  }
  catch ( forth::Parser::ParseError ex)
  {
    BOOST_REQUIRE_EQUAL( ex.what(), "file(21): not a number at 'xx 9 '");
    hasCaughtTheRightException = true;
  }
  catch (...)
  {
  }

  BOOST_CHECK( hasCaughtTheRightException);
}
