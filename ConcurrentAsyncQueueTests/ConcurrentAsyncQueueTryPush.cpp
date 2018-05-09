#include "stdafx.h"
#include "ConcurrentAsyncQueueTryPush.h"
#include "ConcurrentAsyncQueueTestHelper.h"



INSTANTIATE_TEST_CASE_P(ConcurrentAsyncQueueTryPushParametized,
	ConcurrentAsyncQueueTryPush,
	/*testing::Combine(testing::Bool(), // Set size queue
	testing::Values(17), // threads
	testing::Values(17), // nb_value_to_add
	testing::Values(23))*/
	testing::Combine(testing::Bool(), // Set size queue
					 testing::Values(1, 3, 5, 7, 17, 23, 31), // threads
					 testing::Values(1, 3, 5, 7, 17, 23, 31), // nb_value_to_add
					 testing::Values(10, 3, 5, 7, 17, 23, 31))); // size_queue

TEST_P(ConcurrentAsyncQueueTryPush, TryAddCopyElementsSizeQueue)
{
	ASSERT_EQ(true, test_queue_->is_empty());
	ASSERT_EQ(0, test_queue_->count());
	for (auto i = 0; i < nb_threads_; ++i)
	{
		test_threads_result_.push_back(std::async(std::launch::async, [this, i]
		{
			return ConcurrentAsyncQueueTestHelper::try_add_copy_elements(*test_queue_, i, nb_values_to_add_, 100ms);
		}));
	}

	std::this_thread::sleep_for(500ms); // wait for enough time to process everything.

	auto result_async = true;
	for (auto &thread_result : test_threads_result_)
	{
		result_async &= thread_result.get();
	}

	auto all_thread_resolved = !size_queue_used_ || (nb_threads_ * nb_values_to_add_) <= size_queue_;
	const auto expect_size_queue = all_thread_resolved || !size_queue_used_ ? nb_threads_ * nb_values_to_add_ : size_queue_;
	std::cout << "nb_threads_[" << nb_threads_ << "] nb_values_to_add[" << nb_values_to_add_ << "] size_queue_[" << size_queue_ << "]" << std::endl;

	ASSERT_EQ(false, test_queue_->is_empty());
	ASSERT_EQ(expect_size_queue, test_queue_->count());
	ASSERT_EQ(all_thread_resolved, result_async);
}

// TODO : Test method not enough space, remove elements when waiting.