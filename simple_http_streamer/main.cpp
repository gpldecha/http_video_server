#include <string>
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>

#include "video_capture.h"
#include "video_server.h"

typedef std::chrono::duration<int, std::micro> microseconds_type;


int main(int argc, char** argv){


    ThreadVideoCapture thread_video_capture;
    ThreadVideoServer video_server;


    double dt = 0.1;
    microseconds_type loop_rate_micro_s = microseconds_type(std::size_t(dt * 1000000.0));
    auto start_time  = std::chrono::steady_clock::now();
    auto end_time   = start_time + loop_rate_micro_s;

    std::string byte_string;

    sleep(1.0);

    while(true){
        start_time = std::chrono::steady_clock::now();
        end_time   = start_time + loop_rate_micro_s;

        byte_string = thread_video_capture.get_frame();
        video_server.ptr_video_server->broadcast(byte_string);
        //video_server.broadcast(byte_string);

        std::this_thread::sleep_until(end_time);
    }


    return 0;
}
