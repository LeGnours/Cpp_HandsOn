#pragma once
#include <gtest/gtest.h>
#include <future>
#include "ConcurrentAsyncQueueMock.hpp"
#include "ConcurrentAsyncQueueTestHelper.h"

using namespace std::chrono_literals;

class ConcurrentAsyncQueuePush : public testing::TestWithParam<::testing::tuple<int, int>>
{
public:
	void SetUp() final
	{
		test_queue_ = new ConcurrentAsyncQueueMock<int>();
		nb_threads_ = ::testing::get<0>(GetParam());
		nb_values_to_add_ = ::testing::get<1>(GetParam());
	}

	void TearDown() final
	{
		delete test_queue_;
		test_queue_ = nullptr;
	}

protected:
	ConcurrentAsyncQueueMock<int> *test_queue_;
	std::vector<std::future<bool>> test_threads_result_;
	int nb_threads_;
	int nb_values_to_add_;
};
