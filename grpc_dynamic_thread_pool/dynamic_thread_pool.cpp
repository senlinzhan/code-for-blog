#include "dynamic_thread_pool.hpp"

DynamicThreadPool::DynamicThreadPool(int reserve_threads)
    : shutdown_(false),
      reserve_threads_(reserve_threads),
      nthreads_(0),
      threads_waiting_(0)
{
    for (int i = 0; i < reserve_threads_; i++)
    {
        std::lock_guard<std::mutex> lock(mu_);
        nthreads_++;
        new DynamicThread(this);
    }
}

DynamicThreadPool::~DynamicThreadPool()
{
    std::unique_lock<std::mutex> lock_(mu_);
    shutdown_ = true;
    cv_.notify_all();

    while (nthreads_ != 0)
    {
        shutdown_cv_.wait(lock_);        
    }

    ReapThreads(&dead_threads_);    
}

void DynamicThreadPool::Add(const std::function<void ()> &callback)
{
    std::lock_guard<std::mutex> lock(mu_);

    // Add works to the callbacks list
    callbacks_.push(callback);

    // Increase pool size or notify as needed
    if (threads_waiting_ == 0)
    {
        // Kick off a new thread
        nthreads_++;
        new DynamicThread(this);
    }
    else
    {
        cv_.notify_one();
    }

    // Also use this chance to harvest dead threads
    if (!dead_threads_.empty())
    {
        ReapThreads(&dead_threads_);
    }
}

void DynamicThreadPool::ReapThreads(std::list<DynamicThread*> *tlist)
{
    for (auto t = tlist->begin(); t != tlist->end(); t = tlist->erase(t))
    {
        delete *t;
    }
}

void DynamicThreadPool::ThreadFunc()
{
    for (;;)
    {
        std::unique_lock<std::mutex> lock(mu_);

        // Wait until work is available or we are shutting down.
        if (!shutdown_ && callbacks_.empty())
        {
            // If there are too many threads waiting, then quit this thread
            if (threads_waiting_ >= reserve_threads_)
            {
                break;
            }

            threads_waiting_++;
            cv_.wait(lock);
            threads_waiting_--;
        }
        
        // Drain callbacks before considering shutdown to ensure all work gets completed.
        if (!callbacks_.empty())
        {
            auto cb = callbacks_.front();
            callbacks_.pop();
            lock.unlock();
            cb();            
        }
        else if (shutdown_)
        {
            break;            
        }
    }
}

DynamicThreadPool::DynamicThread::DynamicThread(DynamicThreadPool *pool)
    : pool_(pool),
      thd_(new std::thread(&DynamicThreadPool::DynamicThread::ThreadFunc, this))
{
}

DynamicThreadPool::DynamicThread::~DynamicThread()
{
    thd_->join();
    thd_.reset();    
}

void DynamicThreadPool::DynamicThread::ThreadFunc()
{
    pool_->ThreadFunc();

    // Now that we have killed ourselves, we should reduce the thread count
    std::unique_lock<std::mutex> lock(pool_->mu_);
    pool_->nthreads_--;

    // Move ourselves to dead list
    pool_->dead_threads_.push_back(this);

    if ((pool_->shutdown_) && (pool_->nthreads_ == 0))
    {
        pool_->shutdown_cv_.notify_one();
    }
}
