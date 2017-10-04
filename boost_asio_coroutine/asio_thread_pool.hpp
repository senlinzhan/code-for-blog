#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <boost/asio.hpp>
#include <memory>
#include <thread>
#include <vector>

class AsioThreadPool
{
public:
    AsioThreadPool(int threadNum = std::thread::hardware_concurrency())
        : threadNum_(threadNum)
    { }

    AsioThreadPool(const AsioThreadPool &) = delete;
    AsioThreadPool &operator=(const AsioThreadPool &) = delete;

    boost::asio::io_service &getIOService()
    {
        return service_;
    }

    void run()
    {
        for (int i = 0; i < threadNum_; ++i)
        {
            threads_.emplace_back([this] ()
                                  {
                                      service_.run();
                                  });
        }

        for (auto &t : threads_)
        {
            t.join();
        }
    }
    
private:
    std::size_t                 threadNum_;
    boost::asio::io_service     service_;
    std::vector<std::thread>    threads_;
};

#endif /* THREAD_POOL_H */
