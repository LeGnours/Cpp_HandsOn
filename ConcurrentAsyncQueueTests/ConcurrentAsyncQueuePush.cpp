#include "stdafx.h"
#include "ConcurrentAsyncQueuePush.h"
#include "ConcurrentAsyncQueueTestHelper.h"

using namespace std::chrono_literals;

INSTANTIATE_TEST_CASE_P(ConcurrentAsyncQueuePushParametized,
	ConcurrentAsyncQueuePush,
	testing::Combine(testing::Values(1, 3, 5, 7, 17, 23, 31),
		testing::Values(1, 3, 5, 7, 17, 23, 31)));

TEST_P(ConcurrentAsyncQueuePush, AddCopyElements)
{
	ASSERT_EQ(true, test_queue_->is_empty());
	ASSERT_EQ(0, test_queue_->count());
	for (auto i = 0; i < nb_threads_; ++i)
	{
		test_threads_result_.push_back(std::async(std::launch::async, [this, i]
		{
			return ConcurrentAsyncQueueTestHelper::add_copy_elements(*test_queue_, i, nb_values_to_add_);
		}));
	}

	auto result_async = true;
	for (auto &thread_result : test_threads_result_)
	{
		result_async &= thread_result.get();
	}

	ASSERT_EQ(false, test_queue_->is_empty());
	ASSERT_EQ(nb_threads_ * nb_values_to_add_, test_queue_->count());
	ASSERT_EQ(true, result_async);
}

TEST_P(ConcurrentAsyncQueuePush, AddMoveElements)
{
	ASSERT_EQ(true, test_queue_->is_empty());
	ASSERT_EQ(0, test_queue_->count());
	for (auto i = 0; i < nb_threads_; ++i)
	{
		test_threads_result_.push_back(std::async(std::launch::async, [this, i]
		{
			return ConcurrentAsyncQueueTestHelper::add_move_elements(*test_queue_, i, nb_values_to_add_);
		}));
	}

	auto result_async = true;
	for (auto &thread_result : test_threads_result_)
	{
		result_async &= thread_result.get();
	}

	ASSERT_EQ(false, test_queue_->is_empty());
	ASSERT_EQ(nb_threads_ * nb_values_to_add_, test_queue_->count());
	ASSERT_EQ(true, result_async);
}
