#ifndef VIDEO_CAPTURE_H_
#define VIDOE_CAPTURE_H_

#include "opencv2/opencv.hpp"
#include <boost/thread.hpp>

class ThreadVideoCapture{

public:

    ThreadVideoCapture();

    std::string get_frame();

private:

    void run();

private:

    std::vector<int> params = {cv::IMWRITE_JPEG_QUALITY, 90};
    boost::thread video_thread;
    cv::Mat frame;

};

#endif
