#include "stdafx.h"
#include "ConcurrentAsyncQueueTestHelper.h"

bool ConcurrentAsyncQueueTestHelper::add_copy_elements(ConcurrentAsyncQueue<int> &queue, int thread_id, int nb_elements_to_add)
{
	for (auto i = 0; i < nb_elements_to_add; ++i)
	{
		auto value = thread_id * i;
		queue.push(value);
	}
	return true;
}

bool ConcurrentAsyncQueueTestHelper::add_move_elements(ConcurrentAsyncQueue<int>& queue, int thread_id, int nb_elements_to_add)
{
	for (auto i = 0; i < nb_elements_to_add; ++i)
	{
		queue.push(thread_id * i);
	}
	return true;
}

bool ConcurrentAsyncQueueTestHelper::pop_elements(ConcurrentAsyncQueue<int>& queue, int nb_elements_to_pop)
{
	for (auto i = 0; i < nb_elements_to_pop; ++i)
	{
		queue.pop();
	}
	return true;
}
