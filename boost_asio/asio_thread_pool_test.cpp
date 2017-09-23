#include "asio_thread_pool.hpp"

#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>
#include <chrono>
#include <iostream>
#include <mutex>

int main()
{
    AsioThreadPool pool(4);    // 开启 4 个线程
    
    boost::asio::steady_timer timer1{pool.getIOService(), std::chrono::seconds{2}};
    boost::asio::steady_timer timer2{pool.getIOService(), std::chrono::seconds{2}};    

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
    pool.stop();
    
    return 0;
}
