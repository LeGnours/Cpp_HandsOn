// ConcurrentAsyncQueue.cpp : Defines the exported functions for the DLL application.
//

#pragma once
#include "stdafx.h"
#include <future>
#include <chrono>
#include <queue>
#include <mutex>
#include <iostream>

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

	virtual T pop() = 0;
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
		std::cout << "In ConcurrentAsyncQueue Ctor" << std::endl;
		if (max_number_elements_set_)
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

	T pop() override
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

	bool try_push(const T& item, const std::chrono::milliseconds &timeout = std::chrono::hours(24)) override
	{
		std::unique_lock<std::timed_mutex> lock(queue_mutex_, std::defer_lock);

		if (lock.try_lock_for(timeout))
		{
			if (max_number_elements_set_)
			{
				if (queue_.size() >= max_number_elements_.load())
				{
					auto can_add = capacity_changed_.wait_for(queue_mutex_, timeout,
						[this]
					{
						return queue_.size() < max_number_elements_.load();
					});
					if (!can_add)
					{
						std::cout << "Cannot add..." << std::endl;
						return false;
					}
						
				}
			}
			//std::cout << "[try_push method] queue_size : " << queue_.size() << " - max_number_elements_set_ : " << max_number_elements_set_;
			//std::cout << " - max_number_elements_ : " << max_number_elements_.load() << std::endl;
			queue_.push(item);
			capacity_changed_.notify_one();
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
	
	/*bool try_pop() override;
	
	bool try_peek(T& item, std::chrono::milliseconds timeout) override;*/

	virtual ~ConcurrentAsyncQueue()
	{}

protected:
	bool max_number_elements_set_ = false;
	std::atomic_size_t max_number_elements_ = ULONG_MAX;
	std::condition_variable_any capacity_changed_;
	std::queue<T> queue_;
	std::timed_mutex queue_mutex_;

	virtual std::timed_mutex &get_queue_mutex()
	{
		return queue_mutex_;
	}

	virtual size_t get_max_number_elements() const
	{
		return max_number_elements_.load();
	}


};