#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <limits>

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include "TaskRunner.h"

static int s_call_count = 0;
static int s_call_number = -1;
static int s_calls_remaining = -1;
static int s_stop_after_n_calls = std::numeric_limits<int>::max();

static bool test_callback(int call_number, int calls_remaining)
{
	s_call_number = call_number;
	s_calls_remaining = calls_remaining;
	s_call_count++;
	return s_call_count < s_stop_after_n_calls;
}

static TaskRunner m_task_runner(test_callback);

class TaskRunnerTest : public CppUnit::TestFixture  {

	CPPUNIT_TEST_SUITE(TaskRunnerTest);
	CPPUNIT_TEST(test_run_once_without_interval_with_default);
	CPPUNIT_TEST(test_run_once_without_interval);
	CPPUNIT_TEST(test_run_twice_without_interval);
	CPPUNIT_TEST(test_run_three_times_without_interval);
	CPPUNIT_TEST(test_run_once_with_interval);
	CPPUNIT_TEST(test_run_twice_with_interval);
	CPPUNIT_TEST(test_run_three_times_with_interval);
	CPPUNIT_TEST(test_run_twice_with_early_stop);
	CPPUNIT_TEST(test_run_three_times_with_early_stop);
	CPPUNIT_TEST_SUITE_END();

	void test_run_once_without_interval_with_default()
	{
		m_task_runner.run();
		m_task_runner.tick(0);
		CPPUNIT_ASSERT_EQUAL(1, s_call_count);
		CPPUNIT_ASSERT_EQUAL(0, s_call_number);
		CPPUNIT_ASSERT_EQUAL(0, s_calls_remaining);
	}

	void test_run_once_without_interval()
	{
		m_task_runner.run(1);
		m_task_runner.tick(0);
		CPPUNIT_ASSERT_EQUAL(1, s_call_count);
		CPPUNIT_ASSERT_EQUAL(0, s_call_number);
		CPPUNIT_ASSERT_EQUAL(0, s_calls_remaining);
	}

	void test_run_twice_without_interval()
	{
		m_task_runner.run(2);
		m_task_runner.tick(0);
		CPPUNIT_ASSERT_EQUAL(2, s_call_count);
		CPPUNIT_ASSERT_EQUAL(1, s_call_number);
		CPPUNIT_ASSERT_EQUAL(0, s_calls_remaining);
	}

	void test_run_three_times_without_interval()
	{
		m_task_runner.run(3);
		m_task_runner.tick(0);
		CPPUNIT_ASSERT_EQUAL(3, s_call_count);
		CPPUNIT_ASSERT_EQUAL(2, s_call_number);
		CPPUNIT_ASSERT_EQUAL(0, s_calls_remaining);
	}

	void test_run_once_with_interval()
	{
		m_task_runner.run(1, 100);
		m_task_runner.tick(99);
		CPPUNIT_ASSERT_EQUAL(0, s_call_count);
		CPPUNIT_ASSERT_EQUAL(-1, s_call_number);
		CPPUNIT_ASSERT_EQUAL(-1, s_calls_remaining);
		m_task_runner.tick(100);
		CPPUNIT_ASSERT_EQUAL(1, s_call_count);
		CPPUNIT_ASSERT_EQUAL(0, s_call_number);
		CPPUNIT_ASSERT_EQUAL(0, s_calls_remaining);
		m_task_runner.tick(100);
		CPPUNIT_ASSERT_EQUAL(1, s_call_count);
		CPPUNIT_ASSERT_EQUAL(0, s_call_number);
		CPPUNIT_ASSERT_EQUAL(0, s_calls_remaining);
	}

	void test_run_twice_with_interval()
	{
		m_task_runner.run(2, 100);
		m_task_runner.tick(99);
		m_task_runner.tick(100);
		m_task_runner.tick(101);
		CPPUNIT_ASSERT_EQUAL(1, s_call_count);
		CPPUNIT_ASSERT_EQUAL(0, s_call_number);
		CPPUNIT_ASSERT_EQUAL(1, s_calls_remaining);

		m_task_runner.tick(199);
		m_task_runner.tick(200);
		m_task_runner.tick(201);
		CPPUNIT_ASSERT_EQUAL(2, s_call_count);
		CPPUNIT_ASSERT_EQUAL(1, s_call_number);
		CPPUNIT_ASSERT_EQUAL(0, s_calls_remaining);
	}

	void test_run_three_times_with_interval()
	{
		m_task_runner.run(3, 100);
		m_task_runner.tick(99);
		m_task_runner.tick(100);
		m_task_runner.tick(101);
		CPPUNIT_ASSERT_EQUAL(1, s_call_count);
		CPPUNIT_ASSERT_EQUAL(0, s_call_number);
		CPPUNIT_ASSERT_EQUAL(2, s_calls_remaining);

		m_task_runner.tick(199);
		m_task_runner.tick(200);
		m_task_runner.tick(201);
		CPPUNIT_ASSERT_EQUAL(2, s_call_count);
		CPPUNIT_ASSERT_EQUAL(1, s_call_number);
		CPPUNIT_ASSERT_EQUAL(1, s_calls_remaining);

		m_task_runner.tick(299);
		m_task_runner.tick(300);
		m_task_runner.tick(301);
		CPPUNIT_ASSERT_EQUAL(3, s_call_count);
		CPPUNIT_ASSERT_EQUAL(2, s_call_number);
		CPPUNIT_ASSERT_EQUAL(0, s_calls_remaining);
	}

	void test_run_twice_with_early_stop()
	{
		s_stop_after_n_calls = 1;
		m_task_runner.run(2);
		m_task_runner.tick(99);
		m_task_runner.tick(100);
		m_task_runner.tick(101);
		CPPUNIT_ASSERT_EQUAL(1, s_call_count);
		CPPUNIT_ASSERT_EQUAL(0, s_call_number);
		CPPUNIT_ASSERT_EQUAL(1, s_calls_remaining);

		m_task_runner.tick(199);
		m_task_runner.tick(200);
		m_task_runner.tick(201);
		CPPUNIT_ASSERT_EQUAL(1, s_call_count);
		CPPUNIT_ASSERT_EQUAL(0, s_call_number);
		CPPUNIT_ASSERT_EQUAL(1, s_calls_remaining);
	}

	void test_run_three_times_with_early_stop()
	{
		s_stop_after_n_calls = 1;
		m_task_runner.run(2);
		m_task_runner.tick(99);
		m_task_runner.tick(100);
		m_task_runner.tick(101);
		CPPUNIT_ASSERT_EQUAL(1, s_call_count);
		CPPUNIT_ASSERT_EQUAL(0, s_call_number);
		CPPUNIT_ASSERT_EQUAL(1, s_calls_remaining);

		m_task_runner.tick(199);
		m_task_runner.tick(200);
		m_task_runner.tick(201);
		CPPUNIT_ASSERT_EQUAL(1, s_call_count);
		CPPUNIT_ASSERT_EQUAL(0, s_call_number);
		CPPUNIT_ASSERT_EQUAL(1, s_calls_remaining);
	}

public:
	void setUp(void)
	{
		s_call_number = -1;
		s_calls_remaining = -1;
		s_call_count = 0;
		s_stop_after_n_calls = std::numeric_limits<int>::max();
	}
};

int main()
{
   CppUnit::TextUi::TestRunner runner;
   
   CPPUNIT_TEST_SUITE_REGISTRATION( TaskRunnerTest );

   CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();

   runner.addTest( registry.makeTest() );
   runner.run();

   return 0;
}
