//
// Created by Louis GARCZYNSKI on 4/30/16.
//


#pragma once

#include <thread>
#include <vector>
#include <atomic>
#include <iostream>
#include <condition_variable>
#include <functional>

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

	void waitForData(int i);
	void display(std::string str);

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

	std::mutex _displayMutex;

	std::vector<Data> _data;
	std::vector<Value> _values;
	std::vector<std::thread*> _threads;
};

template <typename Data, typename Value>
void ThreadPool<Data, Value>::display(std::string str)
{
	//Lock lock(_displayMutex);
	//std::cout << str << std::endl;
}

template <typename Data, typename Value>
ThreadPool<Data, Value>::ThreadPool(int threadCount)
{
	isKill = false;

	//creates threads
	_threads.resize(threadCount);
	display("main thread creating children");
	for (int i = 0; i < threadCount; i++)
	{
		auto functor = std::bind(&ThreadPool::waitForData, this, i);
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

	display("main thread deleting children");
	for (size_t i = 0; i < _threads.size(); i++)
	{
		_threads[i]->join();
		delete _threads[i];
	}
	display("main thread deleted children");

}

template <typename Data, typename Value>
void ThreadPool<Data, Value>::waitForData(int i)
{
	display(std::to_string(i) + ": thread starts");
	while (1)
	{
		//wait for started thread
		{
			Lock lock(_condMutexStarted);

			display(std::to_string(i) + ": thread falls asleep");
			_started.wait(lock);

			display(std::to_string(i) + ": thread wake up");
		}
		//kill if thread started by destructor
		if (isKill)
		{
			display(std::to_string(i) + ": thread returns");
			return;
		}
		{
			Lock lock(_finishedCounterMutex);

			_finishedCounter++;
		}

		while (1)
		{
			//will contain the thread data/values index for this loop
			int index;

			//get current counter then increment it for next thread
			{
				Lock lock(_startedCounterMutex);

				index = _startedCounter;
				_startedCounter++;

				bool isCorrect = static_cast<size_t>(index) < _data.size();
				display(std::to_string(i) + ": thread assigned work id: " + std::to_string(index) + ", id is " + (isCorrect ? "correct" : "incorrect"));
			}

			//if index is under data size
			// calculate data/values couple
			//else
			// set thread as finished
			if (static_cast<size_t>(index) < _data.size())
			{
				display(std::to_string(i) + ": thread starts working on id: " + std::to_string(index));
				_values[index] = _call(_data[index]);
				display(std::to_string(i) + ": thread ends work on id: " + std::to_string(index));
			}
			else
			{
				//set another thread as finished
				//if all of them are, notify main thread to stop waiting
				{
					Lock lock(_finishedCounterMutex);

					_finishedCounter--;

					display(std::to_string(i) + ": thread finished work, " + std::to_string(_finishedCounter) + " left working");

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
	_finishedCounter = 0;

	//release every threads waiting start loop

	display("main thread loads " + std::to_string(_values.size()) + " elements");
	display("main thread notifying start");
	_started.notify_all();

	{
		//make main thread wait until notify_one is called from waitForData
		Lock lock(_condMutexFinished);
		display("main thread waiting for finish");
		_finished.wait(lock);//BUG possible: wait after notify
	}

	display("main thread data marked as finished");
	return (_values);
}
