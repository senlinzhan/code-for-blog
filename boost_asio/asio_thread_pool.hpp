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
        : work_(new boost::asio::io_service::work(service_))
    {
        for (int i = 0; i < threadNum; ++i)
        {
            threads_.emplace_back([this] () { service_.run(); });
        }
    }

    AsioThreadPool(const AsioThreadPool &) = delete;
    AsioThreadPool &operator=(const AsioThreadPool &) = delete;

    boost::asio::io_service &getIOService()
    {
        return service_;
    }

    void stop()
    {
        work_.reset();
        for (auto &t: threads_)
        {
            t.join();            
        }        
    }
private:
    boost::asio::io_service service_;
    std::unique_ptr<boost::asio::io_service::work> work_;
    std::vector<std::thread> threads_;
};

#endif /* THREAD_POOL_H */
