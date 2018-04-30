// ConcurrentAsyncQueue.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <future>
#include <chrono>

template <class T>
class IConcurrentAsyncQueue<T>
{

	virtual void push(const T &item) = 0;
	virtual void push(T &&item) = 0;
	virtual bool try_push(const T &item, std::chrono::milliseconds timeout) = 0;
	virtual bool try_push(T &&item, std::chrono::milliseconds timeout) = 0;

	virtual T pop() = 0;
	virtual bool try_pop() = 0;

	virtual size_t count() = 0;
	virtual bool is_empty() = 0;

	virtual bool try_peek(T &item, std::chrono::milliseconds timeout) = 0;

	~IConcurrentAsyncQueue(){}
};

template <class T>
class ConcurrentAsyncQueue : IConcurrentAsyncQueue<T>
{
	void push(const T& item) override
	{
		
	}

	void push(T&& item) override;
	bool try_push(const T& item, std::chrono::milliseconds timeout) override;
	bool try_push(T&& item, std::chrono::milliseconds timeout) override;
	T pop() override;
	bool try_pop() override;
	size_t count() override;
	bool is_empty() override;
	bool try_peek(T& item, std::chrono::milliseconds timeout) override;

	virtual ConcurrentAsyncQueue
};