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

	std::vector<Value> run(Call call, const std::vector<Data> &data);

private:

	void waitForData();

	bool isKill;

	Call _call;

	std::atomic<int> _finishedCounter;
	std::mutex _startedCounterMutex;
	int _startedCounter;


	std::mutex _condMutexFinished;
	std::mutex _condMutexStarted;
	std::condition_variable _finished;
	std::condition_variable _started;

	std::vector<Data> _data;
	std::vector<Value> _values;
	std::vector<std::thread*> _threads;
};

template <typename Data, typename Value>
ThreadPool<Data, Value>::ThreadPool(int threadCount)
{
	isKill = false;

	//creates threads
	_threads.resize(threadCount);
	for (int i = 0; i < threadCount; i++)
	{
		auto functor = std::bind(&ThreadPool::waitForData, this);
		_threads[i] = new std::thread(functor);
	}

}

template <typename Data, typename Value>
ThreadPool<Data, Value>::~ThreadPool()
{
	//set destroyed flag to true
	isKill = true;

	_started.notify_all();

	//creates threads
	for (int i = 0; i < _threads.size(); i++)
	{
		_threads[i]->join();
		delete _threads[i];
	}

}


template <typename Data, typename Value>
void ThreadPool<Data, Value>::waitForData()
{
	while (1)
	{
		//wait for started thread
		{
			Lock lock(_condMutexStarted);

			_started.wait(lock);
		}
		//kill if thread started by destructor
		if (isKill)
			return;

		while (1)
		{
			//will contain the thread data/values index for this loop
			int index;

			//if startedCounter is above or equal to 0 (not every data element was consumed)
			// decrement startedCounter
			//set index as startedcounter

			//if startedCounter == size
			// index = size - 1;
			//if startedCounter > 0
			// index >= 0
			//if startedCounter == 0
			// index = -1;
			//if startedCounter == -1
			// index = -1
			{
				Lock lock(_startedCounterMutex);

				if (_startedCounter >= 0)
					--_startedCounter;
				index = _startedCounter;
			}

			//if index is not -1
			// calculate data/values couple
			//else
			// set another thread as finished
			if (index >= 0)
			{
				_values[index] = _call(_data[index]);
			}
			else
			{
				//set another thread as finished
				//if all of them are, notify main thread to stop waiting
				_finishedCounter--;
				if (_finishedCounter == 0)
					_finished.notify_one();//could potentially be fired twice, but shouldn't be a problem
				break;
			}
		}
	}
}

template <typename Data, typename Value>
std::vector<Value> ThreadPool<Data, Value>::run(Call call, const std::vector<Data> &data)
{
	//loads data
	_call = call;
	_data = data;

	int size = _data.size();

	_values.resize(size);
	_startedCounter = size;
	_finishedCounter.store(_threads.size());

	//release every threads waiting start loop
	_started.notify_all();
	{
		//make main thread wait until notify_one is called from waitForData
		Lock lock(_condMutexFinished);
		_finished.wait(lock);
	}
	return (_values);
}

