#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>
#include <chrono>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

class AsioThreadPool
{
public:
    AsioThreadPool(boost::asio::io_service &io_service,
                   int threadNum = std::thread::hardware_concurrency())
        : service_(io_service),
          work_(new boost::asio::io_service::work(io_service))
    {
        for (int i = 0; i < threadNum; ++i)
        {
            threads_.emplace_back([this] () { service_.run(); });
        }
    }

    ~AsioThreadPool()
    {
        work_.reset();
        for (auto &t: threads_)
        {
            t.join();            
        }
    }
    
private:
    boost::asio::io_service &service_;
    std::unique_ptr<boost::asio::io_service::work> work_;
    std::vector<std::thread> threads_;
};

int main()
{
    boost::asio::io_service io_service;
    AsioThreadPool pool(io_service, 4);    // 开启 4 个线程

    boost::asio::steady_timer timer1{io_service, std::chrono::seconds{1}};
    boost::asio::steady_timer timer2{io_service, std::chrono::seconds{1}};    

    int value = 0;
    std::mutex mtx;    // 互斥锁，用来保护 std::cout 和 value 
    
    timer1.async_wait([&mtx, &value] (const boost::system::error_code &ec)
                      {
                          std::lock_guard<std::mutex> lock(mtx);
                          std::cout << "Hello, World! " << value++ << std::endl;
                      });

    timer2.async_wait([&mtx, &value] (const boost::system::error_code &ec)
                      {
                          std::lock_guard<std::mutex> lock(mtx);                          
                          std::cout << "Hello, World! " << value++ << std::endl;
                      });
    return 0;
}
