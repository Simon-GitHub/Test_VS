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
	boost::mutex mu; // �������������������ʹ��  
	boost::condition_variable_any cond_put; // д����������  
	boost::condition_variable_any cond_get; // ��ȡ��������  
 
	stack<int> stk; // ����������  
	int un_read, capacity;  
	bool is_full() // ���������ж�  
    {  
        return un_read == capacity;  
    }  
    bool is_empty()  // ���������ж�  
    {  
        return un_read == 0;  
    }  
  
public:  
    buffer(size_t n) : un_read(0), capacity(n){}  // ���캯��  
    void put(int x)  // д������  
    {  
        { // ��ʼһ���ֲ���  
            boost::mutex::scoped_lock lock(mu); //����������  
            while ( is_full() ) // ��黺�����Ƿ���  
            {  
                { // �ֲ�������cout���һ����Ϣ  
                    boost::mutex::scoped_lock lock(io_mu);  
                    cout << "full waiting..." << endl;  
                }  
                cond_put.wait(mu); // ���������ȴ�  
            } // �����������㣬ֹͣ�ȴ�  
            stk.push(x); // ѹջ��д������  
            ++un_read;  
        } // ����������������������֪ͨ����Ҫ����������  
        cond_get.notify_one(); // ֪ͨ���Զ�ȡ����  
    }  
  
    void get(int *x) // ��ȡ����  
    {  
        { // �ֲ���ʼ  
            boost::mutex::scoped_lock lock(mu); // ����������  
            while (is_empty()) // ��黺�����Ƿ��  
            {  
                {  
                    boost::mutex::scoped_lock lock(io_mu);  
                    cout << "empty waiting..." << endl;  
                }  
                cond_get.wait(mu); // ���������ȴ�  
            }  
            --un_read;  
            *x = stk.top(); // ��ȡ����  
            stk.pop(); // ��ջ  
        }  
       cond_put.notify_one(); // ֪ͨ����д������  
    }  
};  
  
buffer buf(5); // һ������������  
void producter(int n) // ������  
{  
    for (int i = 0; i < n; ++i)  
    {  
        {  
            boost::mutex::scoped_lock lock(io_mu);  
            cout << "put " << i << endl;  
        }  
        buf.put(i); // д������  
    }  
}  
 
void consumer(int n) // ������  
{  
    int x;  
    for (int i = 0; i < n; ++i)  
    {  
        buf.get(&x); // ��ȡ����  
        boost::mutex::scoped_lock lock(io_mu);  
       cout << "get " << x << endl;  
   }  
}  
  
int boost_condition_variable_any()  
{  
    boost::thread t1(producter, 20); // һ���������߳�  
    boost::thread t2(consumer, 10); // �����������߳�  
    boost::thread t3(consumer, 10);  
  
    t1.join(); 
    t2.join();  
    t3.join();  
  
    return 0;  
}  