//
// Created by Louis GARCZYNSKI on 4/30/16.
//

#include "ThreadPool.hpp"


void ThreadPool::waitForData()
{
	while (1)
	{
		//wait for start
		_start.wait

		_mutex.lock();

		if (_counter > 0)
		{
			_counter--;
			int index = _counter;
			_mutex.unlock();

			_values[index] = _call(_data[index]);
		}
		else if (_counter == 0)
		{
			_counter--;
			_mutex.unlock();
		}
		else
		{
			//wait for start again
			_finisher.lock();
			_finisher.unlock();
		}
	}
}

ThreadPool::ThreadPool(int threadCount):_counter()
{
	_threads.reserve(threadCount + 1);

	for (int i = 0; i < threadCount; i++)
	{
		_threads.push_back(thread(waitForData));
	}

}

std::vector<Value> ThreadPool::run(Call call, const std::vector<Data> &data)
{
	_data = data;
	_call = call;
	_start.notify_all();




};
