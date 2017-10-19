#include "http_video_server/video_capture.h"
#include <iostream>


ThreadVideoCapture::ThreadVideoCapture(){

    video_thread = boost::thread(boost::bind(&ThreadVideoCapture::run, this));
}


void ThreadVideoCapture::run(){


    cv::VideoCapture cap(0); // open the default camera
    if(!cap.isOpened()){  // check if we succeeded
        std::cout<< "cv::VideoCapture failed to open!" << std::endl;
        return;
    }

    std::cout<< "starting video capture..." << std::endl;
    cv::Mat edges;
    cv::namedWindow("edges",1);
    for(;;){
           cv::Mat frame;
           cap >> frame; // get a new frame from camera
           cv::cvtColor(frame, edges, CV_BGR2GRAY);
           cv::GaussianBlur(edges, edges, cv::Size(7,7), 1.5, 1.5);
           cv::Canny(edges, edges, 0, 30, 3);
           cv::imshow("edges", edges);
           if(cv::waitKey(30) >= 0) break;
       }
       // the camera will be deinitialized automatically in VideoCapture destructor
}
