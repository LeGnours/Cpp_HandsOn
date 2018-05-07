// ConcurrentAsyncQueue.cpp : Defines the exported functions for the DLL application.
//

#pragma once
#include "stdafx.h"
#include <future>
#include <chrono>
#include <queue>
#include <mutex>

template <class T>
class IConcurrentAsyncQueue
{
public:
	virtual void push(const T &item) = 0;
	virtual void push(T &&item) = 0;
	virtual bool try_push(const T &item, const std::chrono::milliseconds &timeout) = 0;
	//virtual bool try_push(T &&item, const std::chrono::milliseconds &timeout) = 0;

	virtual size_t count() = 0;
	virtual bool is_empty() = 0;

	virtual T& pop() = 0;
	/*virtual bool try_pop() = 0;

	virtual bool try_peek(T &item, std::chrono::milliseconds timeout) = 0;*/

	virtual ~IConcurrentAsyncQueue()
	{}
};

template <class T>
class ConcurrentAsyncQueue : public IConcurrentAsyncQueue<T>
{
public:
	ConcurrentAsyncQueue() = default;

	explicit ConcurrentAsyncQueue(const size_t max_number_elements):
		max_number_elements_set_(true)
	{
		if (max_number_elements)
			max_number_elements_.store(max_number_elements);
	}

	void push(const T& item) override
	{
		std::unique_lock<std::timed_mutex> lock(queue_mutex_);
		if (max_number_elements_set_)
		{
			if (queue_.size() >= max_number_elements_.load())
			{
				capacity_changed_.wait(queue_mutex_,
					[this]
				{
					return queue_.size() < max_number_elements_.load();
				});
			}
		}
		queue_.push(std::move(item));
		capacity_changed_.notify_one();
	}

	void push(T&& item) override
	{
		std::unique_lock<std::timed_mutex> lock(queue_mutex_);
		if (max_number_elements_set_)
		{
			if (queue_.size() >= max_number_elements_.load())
			{
				capacity_changed_.wait(queue_mutex_,
					[this]
				{
					return queue_.size() < max_number_elements_.load();
				});
			}
		}
		queue_.push(std::move(item));
		capacity_changed_.notify_one();
	}

	bool try_push(const T& item, const std::chrono::milliseconds &timeout = std::chrono::hours(24)) override
	{
		std::unique_lock<std::timed_mutex> lock(queue_mutex_, std::defer_lock);

		if (lock.try_lock_for(timeout))
		{
			queue_.push(item);
			return true;
		}
		return false;
	}

	/*bool try_push(T&& item, const std::chrono::milliseconds &timeout) override
	{
		std::unique_lock<std::timed_mutex> lock(queue_mutex_, std::defer_lock);

		if (lock.try_lock_for(timeout))
		{
			queue_.push(std::move(item));
			return true;
		}
		return false;
	}*/

	size_t count() override
	{
		std::lock_guard<std::timed_mutex> lock(queue_mutex_);
		return queue_.size();
	}

	bool is_empty() override
	{
		std::lock_guard<std::timed_mutex> lock(queue_mutex_);
		return queue_.empty();
	}

	T& pop() override
	{
		std::unique_lock<std::timed_mutex> lock(queue_mutex_);
		if (queue_.size() == 0)
		{
			capacity_changed_.wait(queue_mutex_,
				[this]
			{
				return queue_.size() > 0;
			});
		}
		auto front_item = queue_.front();
		queue_.pop();
		capacity_changed_.notify_one();
		return front_item;
	}
	
	
	/*bool try_pop() override;
	
	bool try_peek(T& item, std::chrono::milliseconds timeout) override;*/

	virtual ~ConcurrentAsyncQueue()
	{}

protected:
	bool max_number_elements_set_ = false;
	std::atomic_size_t max_number_elements_ = ULLONG_MAX;
	std::condition_variable_any capacity_changed_;
	std::queue<T> queue_;
	std::timed_mutex queue_mutex_;

	std::timed_mutex &get_queue_mutex()
	{
		return queue_mutex_;
	}

	std::condition_variable_any &get_capacity_changed()
	{
		return capacity_changed_;
	}


};