#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <vector>
#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include "video_server.h"

using boost::asio::ip::tcp;

#include "opencv2/opencv.hpp"

class ThreadVideoCapture{

public:

	ThreadVideoCapture(){
        video_thread = boost::thread(boost::bind(&ThreadVideoCapture::run, this));
	}

    std::string get_frame(){
        std::vector<uchar> buf;
        if(frame.rows != 0){
            cv::imencode(".jpg", frame, buf, params);
            return std::string(&buf[0], &buf[0] + buf.size());
         }else{
            return std::string("");
        }
	}

private:

    void run(){

        std::cout<< "started run of ThreadVideoCapture" << std::endl;

        cv::VideoCapture cap(0); // open the default camera
        if(!cap.isOpened()){  // check if we succeeded
            std::cout<< "cv::VideoCapture failed to open!" << std::endl;
            return;
        }

        std::cout<< "starting video capture..." << std::endl;
        cv::Mat edges;
        cv::namedWindow("edges",1);
        for(;;){
            cap >> frame; // get a new frame from camera
            cv::cvtColor(frame, edges, CV_BGR2GRAY);
            cv::GaussianBlur(edges, edges, cv::Size(7,7), 1.5, 1.5);
            cv::Canny(edges, edges, 0, 30, 3);
            cv::imshow("edges", edges);
            if(cv::waitKey(30) >= 0) break;
        }
    }

	 private:

        std::vector<int> params = {cv::IMWRITE_JPEG_QUALITY, 90};
        boost::thread video_thread;
        cv::Mat frame;

};


class tcp_connection: public boost::enable_shared_from_this<tcp_connection>{

public:

  typedef boost::shared_ptr<tcp_connection> pointer;

  static pointer create(boost::asio::io_service& io_service){
    return pointer(new tcp_connection(io_service));
  }

  tcp::socket& socket(){
    return socket_;
  }

  void publish(std::string frame){
      if(frame_buffer.size() <= max_size){
        frame_buffer.push_back(frame);
      }
  }



  void run(){

      ThreadVideoCapture thread_video_capture;

      while(true){
          std::string frame =thread_video_capture.get_frame();
          if(frame.size() != 0){
            publish(frame);
          }

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

private:

  tcp_connection(boost::asio::io_service& io_service):
      socket_(io_service){
      first_call=true;
  }

  void handle_write(const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/){
  }

  boost::thread video_thread;
  tcp::socket socket_;
  std::string message_;
  std::deque<std::string> frame_buffer;
  bool first_call;
  int max_size = 10;

};

class tcp_server{

public:

tcp_server(boost::asio::io_service& io_service): acceptor_(io_service, tcp::endpoint(tcp::v4(), 9800)){
    start_accept();
}


void broadcast(std::vector<uchar>& image){

}

private:

void start_accept(){
    new_connection = tcp_connection::create(acceptor_.get_io_service());
    acceptor_.async_accept(new_connection->socket(), boost::bind(&tcp_server::handle_accept, this, new_connection, boost::asio::placeholders::error));

}

void handle_accept(tcp_connection::pointer new_connection, const boost::system::error_code& error){
    if (!error){
        new_connection->run();
    }
    start_accept();
}

tcp_connection::pointer new_connection ;
tcp::acceptor acceptor_;

};



int main(int argc, char** argv){

    /*ThreadVideoCapture threadvideocapture;

    while(true)
    {
        sleep(1);
    }*/

    try
    {
      boost::asio::io_service io_service;
      tcp_server server(io_service);
      io_service.run();
    }
    catch (std::exception& e)
    {
      std::cerr << e.what() << std::endl;
    }

    return 0;
}

/*
int main(int argc, char** argv){

    ThreadVideoCapture thread_video_capture;


	try{
    boost::asio::io_service io_service;
    tcp::endpoint endpoint(tcp::v4(), 9800);
    tcp::acceptor acceptor(io_service, endpoint);
    std::vector<uchar> buf;
    std::vector<int> params = {cv::IMWRITE_JPEG_QUALITY, 90};


    for (;;)
    {
      tcp::iostream stream;
      boost::system::error_code ec;
      acceptor.accept(*stream.rdbuf(), ec);
      if (!ec){
        stream << "HTTP/1.0 200 OK\r\n";
        stream << "Server: AdxVideoServer \r\n";
        stream << "Connection: close\r\n";
        stream << "Max-Age: 0\r\n";
        stream << "Expires: 0\r\n";
        stream << "Cache-Control: no-store, no-cache, must-revalidate, pre-check=0, post-check=0, max-age=0\r\n";
        stream << "Pragma: no-cache\r\n";
        stream << "Content-Type: multipart/x-mixed-replace; boundary=--BoundaryString\r\n\r\n";
        stream << "Content-Type: image/jpeg\r\n\r\n";

        cv::Mat frame = thread_video_capture.get_frame();
        cv::imencode(".jpg", frame, buf,params);

        // stream << is.rdbuf() << "\r\n\r\n" << "\r\n--" << "BoundaryString" << "\r\n";
        stream << std::endl;
      }
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

	return 0;
}
*/
