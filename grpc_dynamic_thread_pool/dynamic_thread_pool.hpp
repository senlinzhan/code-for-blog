#ifndef DYNAMIC_THREAD_POOL_H
#define DYNAMIC_THREAD_POOL_H

#include <condition_variable>
#include <mutex>
#include <thread>
#include <queue>
#include <list>

class DynamicThreadPool
{
public:
    explicit DynamicThreadPool(int reserve_threads);
    ~DynamicThreadPool();

    void Add(const std::function<void()> &callback);

private:
    class DynamicThread
    {
    public:
        DynamicThread(DynamicThreadPool* pool);
        ~DynamicThread();

    private:
        DynamicThreadPool* pool_;
        std::unique_ptr<std::thread> thd_;
        void ThreadFunc();
    };
    
    std::mutex mu_;
    std::condition_variable cv_;
    std::condition_variable shutdown_cv_;
    bool shutdown_;
    std::queue<std::function<void()>> callbacks_;
    int reserve_threads_;
    int nthreads_;
    int threads_waiting_;
    std::list<DynamicThread*> dead_threads_;

    void ThreadFunc();
    static void ReapThreads(std::list<DynamicThread*>* tlist);
};

#endif /* DYNAMIC_THREAD_POOL_H */
