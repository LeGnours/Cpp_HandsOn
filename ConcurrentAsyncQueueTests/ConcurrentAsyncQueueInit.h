#pragma once
#include "../ConcurrentAsyncQueue/ConcurrentAsyncQueue.hpp"
#include "ConcurrentAsyncQueueMock.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

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