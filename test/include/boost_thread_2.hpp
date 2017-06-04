#pragma once
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>
#include <iostream>

void count(int id)
{ 
	for (int i=0; i<100; ++i)
	{ 
		boost::mutex::scoped_lock lock(io_mutex);
		std::cout << id << ": " <<	i << std::endl;
	}
} 

void count_2(int id)
{ 
	for (int i=0; i<100; ++i)
	{ 
		boost::mutex::scoped_lock lock(io_mutex);
		std::cout << id << ": " <<	i << std::endl;
	}
} 

int boost_thread_2()
{ 
	boost::thread thrd1(boost::bind(&count, 1));
	boost::thread thrd2(boost::bind(&count_2, 2));
	thrd1.join();
	thrd2.join();
	return 0;
}