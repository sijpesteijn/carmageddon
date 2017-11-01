//
// Created by Gijs Sijpesteijn on 06/10/2017.
//

#ifndef CARMAGEDDON_CAMERA_H
#define CARMAGEDDON_CAMERA_H

#include <opencv2/opencv.hpp>
#include <pthread.h>
#include <vector>

using namespace cv;
using namespace std;

class Camera {
public:
    Camera();
    Mat takeSnapshot();
    Size getDimensions();
    void setDimension(int width, int height);
    VideoCapture cap;
    Mat frame;
    pthread_cond_t frame_not_empty = PTHREAD_COND_INITIALIZER;

    int capture_delay = 200;
    int preview_delay = 6 * 1000;

private:
    vector<Vec2f> detectLines(Mat src);
};


#endif //CARMAGEDDON_CAMERA_H
