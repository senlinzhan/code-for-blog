#include "asio_thread_pool.hpp"

#include <boost/asio.hpp>
#include <array>

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
        doRead();
    }
    
private:
    void doRead()
    {
        auto self = shared_from_this();
        socket_.async_read_some(boost::asio::buffer(buffer_, buffer_.size()),
                                strand_.wrap([this, self](boost::system::error_code ec,
                                                          std::size_t bytes_transferred)
                                             {
                                                 if (!ec)
                                                 {
                                                     doWrite(bytes_transferred);
                                                 }
                                             }));        
    }

    void doWrite(std::size_t length)
    {
        auto self = shared_from_this();        
        boost::asio::async_write(socket_, boost::asio::buffer(buffer_, length),
                                 strand_.wrap([this, self](boost::system::error_code ec,
                                                           std::size_t /* bytes_transferred */)
                                              {
                                                  if (!ec)
                                                  {
                                                      doRead();                                         
                                                  }
                                              }));
    }

private:
    tcp::socket socket_;
    boost::asio::io_service::strand strand_;
    std::array<char, 8192> buffer_;
};

class EchoServer
{
public:
    EchoServer(boost::asio::io_service &io_service, unsigned short port)
        : io_service_(io_service),
          acceptor_(io_service, tcp::endpoint(tcp::v4(), port))
    {
        doAccept();
    }

    void doAccept()
    {
        auto conn = std::make_shared<TCPConnection>(io_service_);
        acceptor_.async_accept(conn->socket(),
                               [this, conn](boost::system::error_code ec)
                               {
                                   if (!ec)
                                   {
                                       conn->start();
                                   }                                   
                                   this->doAccept();
                               });    
    }
    
private: 
    boost::asio::io_service &io_service_;
    tcp::acceptor acceptor_;
};

int main(int argc, char *argv[])
{
    AsioThreadPool pool(4);

    unsigned short port = 5800;
    EchoServer server(pool.getIOService(), port);

    pool.stop();
    
    return 0;
}
