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

	std::mutex _startedCounterMutex;
	int _startedCounter;
	std::mutex _finishedCounterMutex;
	int _finishedCounter;


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
	for (size_t i = 0; i < _threads.size(); i++)
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

			//get current counter then increment it for next thread
			{
				Lock lock(_startedCounterMutex);

				index = _startedCounter++;
			}

			//if index is under data size
			// calculate data/values couple
			//else
			// set thread as finished
			if (index < _data.size())
			{
				_values[index] = _call(_data[index]);
			}
			else
			{
				//set another thread as finished
				//if all of them are, notify main thread to stop waiting
				{
					Lock lock(_finishedCounterMutex);

					_finishedCounter--;
					if (_finishedCounter <= 0)
						_finished.notify_one();
				}
				//could potentially be fired twice, but shouldn't be a problem
				//BUG possible, pas de notify
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

	_values.resize(_data.size());
	_startedCounter = 0;
	_finishedCounter = _threads.size();

	//release every threads waiting start loop
	_started.notify_all();
	{
		//make main thread wait until notify_one is called from waitForData
		Lock lock(_condMutexFinished);
		_finished.wait(lock);//BUG possible: wait apres notify
	}
	return (_values);
}