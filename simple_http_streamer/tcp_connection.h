#ifndef TCP_CONNECTION_H_
#define TCP_CONNECTION_H

#include <queue>          // std::queue
#include <boost/lexical_cast.hpp>

#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>


using boost::asio::ip::tcp;

class tcp_connection: public boost::enable_shared_from_this<tcp_connection>{

public:

  typedef boost::shared_ptr<tcp_connection> pointer;

public:

    tcp_connection(boost::asio::io_service& io_service);

    void run();

    void handle_write(const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/){}

    void publish(std::string bytestring);

    tcp::socket& socket();

    static pointer create(boost::asio::io_service& io_service){
        return pointer(new tcp_connection(io_service));
    }

private:

  tcp::socket socket_;
  std::deque<std::string> frame_buffer;
  bool first_call;
  int max_size = 10;


};

#endif
