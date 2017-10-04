#include "asio_thread_pool.hpp"

#include <memory>
#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>

using boost::asio::ip::tcp;

class TCPConnection : public std::enable_shared_from_this<TCPConnection> 
{
public:
    TCPConnection(boost::asio::io_service &io_service)
        : socket_(io_service),
          strand_(io_service)
    {        
    }
    
    tcp::socket &socket()
    {
        return socket_;
    }

    void start()
    {
        auto self = shared_from_this();
        boost::asio::spawn(strand_,
                           [this, self](boost::asio::yield_context yield)
                           {                               
                               std::array<char, 8192> data;
                               while (true)
                               {
                                   boost::system::error_code ec;
                                   std::size_t n = socket_.async_read_some(boost::asio::buffer(data, data.size()),
                                                                           yield[ec]);
                                   if (ec)
                                   {
                                       break;
                                   }
                                   boost::asio::async_write(socket_, boost::asio::buffer(data, n), yield[ec]);                                       
                                   if (ec)
                                   {
                                       break;
                                   }
                               }
                           });
    }
    
private:
    tcp::socket socket_;
    boost::asio::io_service::strand strand_;
};

class EchoServer
{
public:
    EchoServer(boost::asio::io_service &io_service, unsigned short port)
        : io_service_(io_service),
          acceptor_(io_service, tcp::endpoint(tcp::v4(), port))
    {
        boost::asio::spawn(io_service_,
                           [this] (boost::asio::yield_context yield)
                           {
                               while (true)
                               {
                                   auto conn = std::make_shared<TCPConnection>(io_service_);    
                                   
                                   boost::system::error_code ec;
                                   acceptor_.async_accept(conn->socket(), yield[ec]);

                                   if (!ec)
                                   {
                                       conn->start();
                                   }
                               }                               
                           });
    }
    
private: 
    boost::asio::io_service &io_service_;
    tcp::acceptor acceptor_;
};

int main(int argc, char *argv[])
{
    AsioThreadPool pool;
    unsigned short port = 5800;
    
    EchoServer server(pool.getIOService(), port);
    pool.run();
    
    return 0;
}
