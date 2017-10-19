#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/thread.hpp>


using boost::asio::ip::tcp;

#include "opencv2/opencv.hpp"

class ThreadVideoCapture{

public:

	ThreadVideoCapture(){
    video_thread = boost::thread(boost::bind(&ThreadVideoCapture::run, this));
	}

	cv::Mat get_frame(){
		return frame;
	}

private:

	void run(){

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

		 boost::thread video_thread;
		 cv::Mat frame;

};


int main(int argc, char** argv){

	ThreadVideoCapture thread_video_capture;


	try{
    boost::asio::io_service io_service;
    tcp::endpoint endpoint(tcp::v4(), 9800);
    tcp::acceptor acceptor(io_service, endpoint);

		std::stringstream ssOut;


    for (;;)
    {
      tcp::iostream stream;
      boost::system::error_code ec;
      acceptor.accept(*stream.rdbuf(), ec);
      if (!ec)
      {

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
