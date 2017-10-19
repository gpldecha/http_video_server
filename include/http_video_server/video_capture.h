#ifndef VIDEO_CAPTURE_H_
#define VIDEO_CAPTURE_H_

#include <boost/thread.hpp>
#include <boost/thread/synchronized_value.hpp>

#include "opencv2/opencv.hpp"

class ThreadVideoCapture{

public:

    ThreadVideoCapture();

private:

    void run();

private:

    boost::thread video_thread;

};



#endif
