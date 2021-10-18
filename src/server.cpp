#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

#include "socket.h"
#include "types.h"

#define BUFSIZE 640*480
#define PORT 14000

int main(void) {
    // fps calculator
    int frameCounter = 0;
    time_t timeBegin = time(0);
    int tick = 0;

    cv::namedWindow("Transferred", 1);
    cv::Mat dst;

    UDPSocket server;
    server.setLocalPort(PORT);

    char key = ' ';
    char buf[BUFSIZE];

    while (key != 'q') {
        server.receiveFrame((char *)buf);

        cv::Mat rawData = cv::Mat(1, sizeof(buf), CV_8UC1, buf);
        dst = cv::imdecode(rawData, cv::IMREAD_COLOR);

        cv::imshow("Transferred", dst);

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
