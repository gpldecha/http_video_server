#ifndef VIDEO_SERVER_H_
#define VIDEO_SERVER_H_

#include "tcp_connection.h"

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <deque>
#include <map>
#include <boost/make_shared.hpp>


class VideoServer{

public:

    VideoServer();

    void broadcast(std::string byte_string);

    void run();


private:

    void start_accept();

    void handle_accept(tcp_connection::pointer new_connection, const boost::system::error_code& error);

private:
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::acceptor acceptor_;
    tcp_connection::pointer new_connection;
    std::map<std::string, tcp_connection::pointer> connections;
    std::map<std::string, tcp_connection::pointer>::iterator map_iterator;
    boost::thread thread;
    bool bflag;
};

class ThreadVideoServer{

public:

    ThreadVideoServer(){
        thread = boost::thread(boost::bind(&ThreadVideoServer::run, this));
    }

private:

    void run(){
        ptr_video_server = new VideoServer();

    }

public:

    VideoServer* ptr_video_server;

private:

    boost::thread thread;

};


#endif
