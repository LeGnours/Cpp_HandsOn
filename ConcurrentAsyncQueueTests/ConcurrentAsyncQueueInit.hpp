#pragma once
#include "pch.h"
#include "../ConcurrentAsyncQueue/ConcurrentAsyncQueue.hpp"
#include "ConcurrentAsyncQueueMock.hpp"

using ::testing::TestWithParam;
using ::testing::Bool;
using ::testing::Values;
using ::testing::Combine;

class ConcurrentAsyncQueueInit : public TestWithParam<int>
{
public:
	void SetUp() final
	{
		test_queue_ = new ConcurrentAsyncQueueMock<int>(GetParam());
	}

	void TearDown() final
	{
		delete test_queue_;
		test_queue_ = nullptr;
	}

protected:
	ConcurrentAsyncQueueMock<int> *test_queue_;
};

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
	ASSERT_EQ(GetParam(), test_queue_->get_capacity_changed());
}