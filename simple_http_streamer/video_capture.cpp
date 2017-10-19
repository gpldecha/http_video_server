#include "video_capture.h"

ThreadVideoCapture::ThreadVideoCapture(){
    video_thread = boost::thread(boost::bind(&ThreadVideoCapture::run, this));
}

std::string ThreadVideoCapture::get_frame(){
    std::vector<uchar> buf;
    if(frame.rows != 0){
        cv::imencode(".jpg", frame, buf, params);
        return std::string(&buf[0], &buf[0] + buf.size());
     }else{
        return std::string("");
    }
}

void ThreadVideoCapture::run(){
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
