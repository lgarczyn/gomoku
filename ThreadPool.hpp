//
// Created by Louis GARCZYNSKI on 4/30/16.
//


#pragma once

#include <thread>
#include <atomic>
#include <condition_variable>

template <typename Data, typename Value>
class ThreadPool
{
	typedef Value *Call(Data);

public:

	ThreadPool();
	~ThreadPool();

	std::vector<Value> run(Call call, const std::vector<Data> &data);

private:

	void waitForData();

	Call _call;
	std::mutex _mutex;
	std::condition_variable _start;
	std::condition_variable _finished;
	std::condition_variable _end;
	int _counter;
	std::vector<Data> _data;
	std::vector<Value> _values;
	std::vector<std::thread> _threads;
};


