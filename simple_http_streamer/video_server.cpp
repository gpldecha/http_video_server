#include "video_server.h"
#include <boost/bind.hpp>


VideoServer::VideoServer():
acceptor_(io_service, tcp::endpoint(tcp::v4(), 9801)){
    start_accept();
    bflag=false;
    //io_service.run();
    //thread = boost::thread(boost::bind(&VideoServer::run, this));

}

void VideoServer::run(){
    io_service.run_one();
}

void VideoServer::broadcast(std::string byte_string){
    if(bflag){
        new_connection->publish(byte_string);
        std::cout<< "bfag: " << bflag << std::endl;
    }
    /*if(new_connection!= NULL){
        std::cout<< byte_string << std::endl;
    }*/

  /*for(map_iterator=connections.begin();  map_iterator!=connections.end(); ++map_iterator){
        map_iterator->second->publish(byte_string);
  }*/
}

void VideoServer::start_accept(){
    new_connection = tcp_connection::create(acceptor_.get_io_service());
    acceptor_.async_accept(new_connection->socket(), boost::bind(&VideoServer::handle_accept, this, new_connection, boost::asio::placeholders::error));
    bflag=true;
    //std::string address = new_connection->socket().remote_endpoint().address().to_string();
    //std::cout<< "new client " << address << " connected" << std::endl;
    //connections.insert(std::pair<std::string, tcp_connection::pointer>(address, new_connection));
}

void VideoServer::handle_accept(tcp_connection::pointer new_connection, const boost::system::error_code& error){
    if (!error){
        new_connection->run();
    }
    start_accept();
}
