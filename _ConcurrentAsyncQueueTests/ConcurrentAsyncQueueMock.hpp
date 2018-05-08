#pragma once
#include "gmock/gmock.h"
#include "../ConcurrentAsyncQueue/ConcurrentAsyncQueue.hpp"

template <typename T>
class ConcurrentAsyncQueueMock : public ConcurrentAsyncQueue<T>
{
public:
	using ConcurrentAsyncQueue<T>::ConcurrentAsyncQueue;

	MOCK_METHOD0(get_queue_mutex, std::timed_mutex&());
	MOCK_METHOD0(get_capacity_changed, std::condition_variable_any&());
};
