#pragma once
#include "../ConcurrentAsyncQueue/ConcurrentAsyncQueue.hpp"

class ConcurrentAsyncQueueTestHelper
{
public:
	static bool add_copy_elements(ConcurrentAsyncQueue<int> &queue, int thread_id, int nb_elements_to_add);
	static bool add_move_elements(ConcurrentAsyncQueue<int> &queue, int thread_id, int nb_elements_to_add);
	static bool pop_elements(ConcurrentAsyncQueue<int> &queue, int nb_elements_to_pop);
};
