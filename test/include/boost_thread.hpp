#pragma once
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <iostream>
boost::mutex io_mutex;
#pragma comment(lib,"libboost_thread-vc100-mt-gd-1_59.lib")
struct _count
{ 
	_count(int id) 
		: id(id) 
	{
	}
	void operator()()
	{ 
		for (int i = 0; i < 10; ++i)
		{ 
			boost::mutex::scoped_lock lock(io_mutex);
			std::cout << id << ": "	<< i << std::endl;
		}
	} 
	int id;
};

int boost_thread()
{ 
	boost::thread thrd1(_count(1));
	boost::thread thrd2(_count(2));
	thrd1.join();
	thrd2.join();
	return 0;
}