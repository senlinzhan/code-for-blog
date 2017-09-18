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
    
    AsioThreadPool pool(io_service, 4);

    boost::asio::steady_timer timer1{io_service, std::chrono::seconds{1}};
    boost::asio::steady_timer timer2{io_service, std::chrono::seconds{1}};    

    int value = 0;
    boost::asio::io_service::strand strand(io_service);
    
    timer1.async_wait(strand.wrap([&value] (const boost::system::error_code &ec)
                                  {
                                      std::cout << "Hello, World! " << value++ << std::endl;
                                  }));

    timer2.async_wait(strand.wrap([&value] (const boost::system::error_code &ec)
                                  {
                                      std::cout << "Hello, World! " << value++ << std::endl;
                                  }));
     
    return 0;
}
