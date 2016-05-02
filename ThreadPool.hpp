//
// Created by Louis GARCZYNSKI on 4/30/16.
//


#pragma once

#include <thread>
#include <vector>
#include <atomic>
#include <condition_variable>

template <typename Data, typename Value>
class ThreadPool
{
	typedef std::function<Value(Data)> Call;
	typedef std::unique_lock<std::mutex> Lock;

public:
	ThreadPool(const ThreadPool& cpy);

	ThreadPool(int threadCount);
	~ThreadPool();

	std::vector<Value> run(Call* call, const std::vector<Data> &data);

private:

	void waitForData();

	bool isKill;

	Call* _call;
	std::mutex _startedMutex;
	std::mutex _finishedMutex;
	int _startedCounter;
	int _finishedCounter;
	std::mutex _condMutex;
	std::condition_variable _finished;
	std::vector<Data> _data;
	std::vector<Value> _values;
	std::vector<std::thread*> _threads;
};


template <typename Data, typename Value>
void ThreadPool<Data, Value>::waitForData()
{
	while (1)
	{
		int index;
		{
			Lock lock(_startedMutex);

			if (_startedCounter >= 0)
				--_startedCounter;
			index = _startedCounter;
		}

		if (isKill)
			return;

		if (index >= 0)
		{
			_values[index] = _call(_data[index]);
		}
		else
		{
			{
				Lock lock(_finishedMutex);
				_finishedCounter++;
			}

			_finished.notify_one();
		}
	}
}

template <typename Data, typename Value>
ThreadPool<Data, Value>::ThreadPool(int threadCount)
{
	isKill = false;

	//makes every future created threads wait instead of looking up _counter
	_startedMutex.lock();

	//creates threads
	_threads.resize(threadCount);
	for (int i = 0; i < threadCount; i++)
	{
		auto functor = std::bind(this, &ThreadPool::waitForData);
		_threads[i] = new std::thread(functor);
	}

}

template <typename Data, typename Value>
ThreadPool<Data, Value>::ThreadPool(const ThreadPool& cpy)
{
	throw std::logic_error("Copy constructor not implemented");
}

template <typename Data, typename Value>
ThreadPool<Data, Value>::~ThreadPool()
{
	//set destroyed flag to true
	isKill = true;

	//let every thread check for flag
	_startedMutex.unlock();

	//creates threads
	for (int i = 0; i < _threads.size(); i++)
	{
		_threads[i]->join();
		delete _threads[i];
	}

}

template <typename Data, typename Value>
std::vector<Value> ThreadPool<Data, Value>::run(Call* call, const std::vector<Data> &data)
{
	//loads data
	_call = call;
	_data = data;

	int size = _data.size();

	_values.resize(size);
	_startedCounter = size;
	_finishedCounter = 0;

	//release every threads waiting to read _counter;
	_startedMutex.unlock();

	{
		//make main thread wait until at least one thread is finished
		//thread will continue working, but will wait once task is over

		Lock lock(_condMutex);
		_finished.wait(lock);

		_startedMutex.lock();
	}
	{
		//wait until every thread is finished

		Lock lock(_condMutex);
		while (_finishedCounter != size)
			_finished.wait(lock);
	}
}

