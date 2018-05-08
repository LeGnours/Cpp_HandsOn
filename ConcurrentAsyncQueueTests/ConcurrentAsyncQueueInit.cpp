#include "stdafx.h"
#include "ConcurrentAsyncQueueInit.h"
#define private public

INSTANTIATE_TEST_CASE_P(ConcurrentAsyncQueueInitParametized,
	ConcurrentAsyncQueueInit,
	Values(1, 3, 5, 7, 17, 23, 31)
);

TEST_P(ConcurrentAsyncQueueInit, ConstructorInitWithNoParams)
{
	test_queue_ = new ConcurrentAsyncQueueMock<int>();

	ASSERT_EQ(true, test_queue_->is_empty());
	ASSERT_EQ(0, test_queue_->count());
}

TEST_P(ConcurrentAsyncQueueInit, ConstructorInitWithCapacityMaxSet)
{
	ASSERT_EQ(true, test_queue_->is_empty());
	ASSERT_EQ(0, test_queue_->count());
	ASSERT_EQ(GetParam(), test_queue_->get_max_number_elements());
}