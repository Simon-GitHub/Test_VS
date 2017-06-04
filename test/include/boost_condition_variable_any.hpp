#pragma once
#include <boost/thread.hpp>  
#include <boost/thread/mutex.hpp>  
  
#include <iostream>  
#include <stack>  
  
using namespace std;  
 
boost::mutex io_mu;  
 
class buffer  
{  
private:  
	boost::mutex mu; // 互斥量，配合条件变量使用  
	boost::condition_variable_any cond_put; // 写入条件变量  
	boost::condition_variable_any cond_get; // 读取条件变量  
 
	stack<int> stk; // 缓冲区对象  
	int un_read, capacity;  
	bool is_full() // 缓冲区满判断  
    {  
        return un_read == capacity;  
    }  
    bool is_empty()  // 缓冲区空判断  
    {  
        return un_read == 0;  
    }  
  
public:  
    buffer(size_t n) : un_read(0), capacity(n){}  // 构造函数  
    void put(int x)  // 写入数据  
    {  
        { // 开始一个局部域  
            boost::mutex::scoped_lock lock(mu); //锁定互斥量  
            while ( is_full() ) // 检查缓冲区是否满  
            {  
                { // 局部域，锁定cout输出一条信息  
                    boost::mutex::scoped_lock lock(io_mu);  
                    cout << "full waiting..." << endl;  
                }  
                cond_put.wait(mu); // 条件变量等待  
            } // 条件变脸满足，停止等待  
            stk.push(x); // 压栈，写入数据  
            ++un_read;  
        } // 解锁互斥量，条件变量的通知不需要互斥量锁定  
        cond_get.notify_one(); // 通知可以读取数据  
    }  
  
    void get(int *x) // 读取数据  
    {  
        { // 局部域开始  
            boost::mutex::scoped_lock lock(mu); // 锁定互斥量  
            while (is_empty()) // 检查缓冲区是否空  
            {  
                {  
                    boost::mutex::scoped_lock lock(io_mu);  
                    cout << "empty waiting..." << endl;  
                }  
                cond_get.wait(mu); // 条件变量等待  
            }  
            --un_read;  
            *x = stk.top(); // 读取数据  
            stk.pop(); // 弹栈  
        }  
       cond_put.notify_one(); // 通知可以写入数据  
    }  
};  
  
buffer buf(5); // 一个缓冲区对象  
void producter(int n) // 生产者  
{  
    for (int i = 0; i < n; ++i)  
    {  
        {  
            boost::mutex::scoped_lock lock(io_mu);  
            cout << "put " << i << endl;  
        }  
        buf.put(i); // 写入数据  
    }  
}  
 
void consumer(int n) // 消费者  
{  
    int x;  
    for (int i = 0; i < n; ++i)  
    {  
        buf.get(&x); // 读取数据  
        boost::mutex::scoped_lock lock(io_mu);  
       cout << "get " << x << endl;  
   }  
}  
  
int boost_condition_variable_any()  
{  
    boost::thread t1(producter, 20); // 一个生产者线程  
    boost::thread t2(consumer, 10); // 两个消费者线程  
    boost::thread t3(consumer, 10);  
  
    t1.join(); 
    t2.join();  
    t3.join();  
  
    return 0;  
}  