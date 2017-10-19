#include "tcp_connection.h"
#include <iostream>

tcp_connection::tcp_connection(boost::asio::io_service& io_service):
    socket_(io_service){
    first_call=true;
}

void tcp_connection::run(){
    while(true){
        /*std::string frame =thread_video_capture.get_frame();
        if(frame.size() != 0){
          publish(frame);
        }*/

        if(first_call){
            std::string msg =
                    std::string("HTTP/1.0 200 OK\r\n") +
                    std::string("Server: AdxVideoServer \r\n") +
                    std::string("Connection: close\r\n") +
                    std::string("Max-Age: 0\r\n") +
                    std::string("Expires: 0\r\n") +
                    std::string("Cache-Control: no-store, no-cache, must-revalidate, pre-check=0, post-check=0, max-age=0\r\n") +
                    std::string("Pragma: no-cache\r\n") +
                    std::string("Content-Type: multipart/x-mixed-replace; boundary=--BoundaryString\r\n\r\n") +
                    std::string("\r\n--BoundaryString\r\n");
            socket_.send(boost::asio::buffer(msg));
            first_call=false;
        }else{
            if(frame_buffer.size() != 0){
                std::string frame = frame_buffer.front();
                frame_buffer.pop_front();
                std::string msg = std::string("Content-Type: image/jpeg\r\n\r\n") + frame + std::string("\r\n\r\n' + b'\r\n--BoundaryString\r\n");
                socket_.send(boost::asio::buffer(msg));
            }
        }
    }
}

void tcp_connection::publish(std::string bytestring){
    if(frame_buffer.size() <= max_size){
      frame_buffer.push_back(bytestring);
    }
}

tcp::socket& tcp_connection::socket(){
    return socket_;
}
