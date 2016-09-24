#define BOOST_TEST_MODULE TestRuntime
#include <boost/test/unit_test.hpp>
#include <forth/runtime.hpp>

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
};

BOOST_AUTO_TEST_CASE(Basics)
{
  TestRuntime forth;

  forth.PushData( 0);
  BOOST_CHECK_EQUAL( forth.TestDataStackSize(), 1);

  forth.PushReturn( 0);
  forth.PushReturn( 1);
  BOOST_CHECK_EQUAL( forth.TestReturnStackSize(), 2);
}

static void TestIntrinsic2( 
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

static void TestIntrinsic1( 
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
