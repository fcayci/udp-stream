#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgcodecs.hpp>

#include "socket.h"
#include "types.h"

#define PACK_SIZE 4096
#define PORT 14000
#define BUFSIZE 640*480

int main(void) {


    // fps calculator
    int frameCounter = 0;
    time_t timeBegin = time(0);
    int tick = 0;

    cv::Mat dst;

    UDPSocket client;
    std::string s = "0.0.0.0";
    int port = 14000;
    client.setDestination(s, port);

    char key = ' ';

    char buf[BUFSIZE];

    cv::VideoCapture cap(0);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

    cv::Mat frame;

    std::vector<uchar> encoded;

    while (key != 'q') {
        cap.read(frame);
        /* convert to greyscale if needed */
        if (frame.channels() == 3) {
            cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
        }

        std::vector <int> compression_params;
        compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
        compression_params.push_back(60);
        cv::imencode(".jpg", frame, encoded, compression_params);
        char *buf = reinterpret_cast<char*>(encoded.data());
        client.sendFrame(buf, encoded.size());

        cv::imshow("Local", frame);

        key = cv::waitKey(1);

        // Fps Calculator
        frameCounter++;
        time_t timeNow = time(0) - timeBegin;
        if (timeNow - tick >= 1) {
            tick++;
            std::cout << "FPS: " << frameCounter << ".\n";
            frameCounter = 0;
        }
    }

    return EXIT_SUCCESS;
}
