#include "asio_thread_pool.hpp"

#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>
#include <chrono>
#include <iostream>

int main()
{
    AsioThreadPool pool(4);

    boost::asio::steady_timer timer1{pool.getIOService(), std::chrono::seconds{1}};
    boost::asio::steady_timer timer2{pool.getIOService(), std::chrono::seconds{1}};
    
    int value = 0;
    boost::asio::io_service::strand strand{pool.getIOService()};
    
    timer1.async_wait(strand.wrap([&value] (const boost::system::error_code &ec)
                                  {
                                      std::cout << "Hello, World! " << value++ << std::endl;
                                  }));

    timer2.async_wait(strand.wrap([&value] (const boost::system::error_code &ec)
                                  {
                                      std::cout << "Hello, World! " << value++ << std::endl;
                                  }));

    pool.stop();
    
    return 0;
}
