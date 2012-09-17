//
//
//   File Name : ITHREAD
//   Date :      2012/09/17 14:14:09
//   Author :    lijun(lijun@zlg.cn)
//   Describle:  thread pool. detail see threadbase.h/cpp
//
#ifndef _ITHREAD_H
#define _ITHREAD_H
// example
//class Test : public IDelegate
//{
//public:
//    Test(int id)
//        :id_(id)
//    {
//
//    }
//    void Run()
//    {
//        printf("test : %d\r\n", id_);
//    }
//
//    int id_;
//};
//
//void TestThreadPool()
//{
//    IThreadPool pool(10);
//
//    Test* test[10] = {NULL};
//    for (int i=0; i<10; ++i)
//    {
//        test[i] = new Test(i+1);
//    }
//
//    pool.Start();
//    for (int i=0; i<10; ++i)
//    {
//        pool.AddWork(test[i]);
//    }
//
//    pool.JoinAll();
//    for (int i=0; i<10; ++i)
//    {
//        delete test[i];
//    }
//
//}

#include <string>
#include <queue>
#include <vector>
#include "utils.h"
#include "NonCopyable.h"
using namespace std;

typedef queue<IDelegate*>       Queue;
typedef vector<IThreadBase2*>   ThreadBaseArray;

class IThreadPool : public IDelegate
{
    DECLARE_NON_COPY_ABLE(IThreadPool)
public:
    IThreadPool(size_t ThreadCnt = 10)
        :num_threads_(ThreadCnt), event_(TRUE, FALSE)
    {
    }
    
    bool Start()
    {
        for (size_t i=0; i<num_threads_; ++i)
        {
            IThreadBase2* pBase = new IThreadBase2(this);
            pBase->Start();
            threads_.push_back(pBase);
        }
        return true;
    }
    void JoinAll()
    {
        // Tell all our threads to quit their worker loop.
        AddWork(NULL, num_threads_);

        // Join and destroy all the worker threads.
        for (size_t i = 0; i < num_threads_; ++i) {
            threads_[i]->Join();
            delete threads_[i];
        }
        threads_.clear();
    }
    void AddWork(IDelegate* delegate, int repeat_count)
    {
        CSimpleAutoLock autoLock(&queueLock);
        for (int i=0; i<repeat_count; ++i)
            workerQueue_.push(delegate);

        //if (!workerQueue_.empty())
        if (!event_.IsSignaled())
            event_.Signal();
    }
    void AddWork(IDelegate* delegate)
    {
        AddWork(delegate, 1);
    }
    void Run()
    {
        IDelegate* work = NULL;

        while (true) {
            event_.Wait();
            {
                CSimpleAutoLock locked(&queueLock);
                if (!event_.IsSignaled())
                    continue;

                work = workerQueue_.front();
                workerQueue_.pop();

                // Signal to any other threads that we're currently out of work.
                if (workerQueue_.empty())
                    event_.Reset();
            }

            // A NULL delegate pointer signals us to quit.
            if (!work)
                break;

            work->Run();
        }
    }

private:
    size_t          num_threads_;
    Queue           workerQueue_;
    CSimpleLock     queueLock;
    CWaitableEvent  event_;
    ThreadBaseArray threads_;

};

#endif // _ITHREAD_H
