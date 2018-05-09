#pragma once
#include <future>
#include <gtest/gtest.h>
#include "ConcurrentAsyncQueueMock.hpp"

using namespace std::chrono_literals;

class ConcurrentAsyncQueueTryPush : public testing::TestWithParam<::testing::tuple<bool, int, int, int>>
{
public:
	void SetUp() final
	{
		size_queue_used_ = ::testing::get<0>(GetParam());
		nb_threads_ = ::testing::get<1>(GetParam());
		nb_values_to_add_ = ::testing::get<2>(GetParam());
		size_queue_ = ::testing::get<3>(GetParam());
		//nb_values_to_add_ = ::testing::get<4>(GetParam());
		test_queue_ = size_queue_used_ ? new ConcurrentAsyncQueueMock<int>(size_queue_) :
													new ConcurrentAsyncQueueMock<int>();
	}

	void TearDown() final
	{
		delete test_queue_;
		test_queue_ = nullptr;
		std::this_thread::sleep_for(250ms);
	}

protected:
	ConcurrentAsyncQueueMock<int> *test_queue_;
	std::vector<std::future<bool>> test_threads_result_;
	bool size_queue_used_;
	int nb_threads_;
	int nb_values_to_add_;
	int size_queue_;
};
