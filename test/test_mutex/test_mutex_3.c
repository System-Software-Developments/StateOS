#include "test.h"

static void proc4()
{
	unsigned event;

	event = mtx_wait(mtx2);                      assert_success(event);
	event = tsk_kill(tsk3);                      assert_success(event);
	event = mtx_give(mtx2);                      assert_success(event);
	        tsk_stop();
}

static void proc3()
{
	                                             assert_stopped(tsk4);
	        tsk_startFrom(tsk4, proc4);
	                                             assert(!"test program cannot be caught here");
}

static void proc2()
{
	unsigned event;

	event = mtx_wait(mtx2);                      assert_success(event);
	event = mtx_wait(mtx1);                      assert_success(event);
	event = tsk_kill(tsk1);                      assert_success(event);
	event = mtx_give(mtx1);                      assert_success(event);
	event = mtx_give(mtx2);                      assert_success(event);
	        tsk_stop();
}

static void proc1()
{
	                                             assert_stopped(tsk2);
	        tsk_startFrom(tsk2, proc2);
	                                             assert(!"test program cannot be caught here");
}

static void test()
{
	unsigned event;

	event = mtx_wait(mtx1);                      assert_success(event);
	                                             assert_stopped(tsk1);
	        tsk_startFrom(tsk1, proc1);
	                                             assert_stopped(tsk3);
	        tsk_startFrom(tsk3, proc3);
	event = mtx_give(mtx1);                      assert_success(event);
	event = tsk_join(tsk3);                      assert_success(event);
	event = tsk_join(tsk1);                      assert_success(event);
}

void test_mutex_3()
{
	int i;
	TEST_Notify();
	mtx_init(mtx1, mtxPrioInherit, 0);
	mtx_init(mtx2, mtxPrioInherit, 0);
	for (i = 0; i < PASS; i++)
		test();
}
