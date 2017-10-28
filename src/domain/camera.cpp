//
// Created by Gijs Sijpesteijn on 06/10/2017.
//

#include "camera.h"
#include <chrono>
#include <pthread.h>
#include <syslog.h>
#include <thread>
#include <chrono>

pthread_mutex_t frame_lock = PTHREAD_MUTEX_INITIALIZER;

void* frameGrabber(void* params) {
    Camera *camera = (Camera*) params;
    while(1) {
        if (pthread_mutex_lock(&frame_lock) != 0) {
            syslog(LOG_ERR, "%s", "Sockethandler: Could not get a lock on the queue");
        }
        camera->cap >> camera->frame;
        pthread_cond_signal(&camera->frame_not_empty);
        if (pthread_mutex_unlock(&frame_lock) != 0) {
            syslog(LOG_ERR, "%s", "Sockethandler: Could not unlock the queue");
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    return NULL;
}

Camera::Camera():cap(0) {
    this->cap.set(CV_CAP_PROP_FRAME_WIDTH,352);
    this->cap.set(CV_CAP_PROP_FRAME_HEIGHT,288);
//	this->cap.set(CV_CAP_PROP_BUFFERSIZE,3);
//	this->cap.set(CV_CAP_PROP_XI_BUFFERS_QUEUE_SIZE,3);

//	cout << "Buffer queue size: " << this->cap.get(CV_CAP_PROP_XI_BUFFERS_QUEUE_SIZE) << endl;
//	cout << "Buffer size: " << this->cap.get(CV_CAP_PROP_BUFFERSIZE) << endl;
//	cout << "Pos mSec: " << this->cap.get(CV_CAP_PROP_POS_MSEC) << endl;
//	cout << "Pos frames: " << this->cap.get(CV_CAP_PROP_POS_FRAMES) << endl;
//	cout << "Pos avi ratio: " << this->cap.get(CV_CAP_PROP_POS_AVI_RATIO) << endl;
//	cout << "Frame width: " << this->cap.get(CV_CAP_PROP_FRAME_WIDTH) << endl;
//	cout << "Frame height: " << this->cap.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;
//	cout << "Frames per seconde: " << this->cap.get(CV_CAP_PROP_FPS) << endl;
//	cout << "FourCC: " << this->cap.get(CV_CAP_PROP_FOURCC) << endl;
//	cout << "Frame count: " << this->cap.get(CV_CAP_PROP_FRAME_COUNT) << endl;
//	cout << "Format: " << this->cap.get(CV_CAP_PROP_FORMAT) << endl;
//	cout << "Mode: " << this->cap.get(CV_CAP_PROP_MODE) << endl;
//	cout << "Brightness: " << this->cap.get(CV_CAP_PROP_BRIGHTNESS) << endl;
//	cout << "Contrast: " << this->cap.get(CV_CAP_PROP_CONTRAST) << endl;
//	cout << "Saturation: " << this->cap.get(CV_CAP_PROP_SATURATION) << endl;
//	cout << "Hue: " << this->cap.get(CV_CAP_PROP_HUE) << endl;
//	cout << "Gain: " << this->cap.get(CV_CAP_PROP_GAIN) << endl;
//	cout << "Exposure: " << this->cap.get(CV_CAP_PROP_EXPOSURE) << endl;
//	cout << "Convert RGB: " << this->cap.get(CV_CAP_PROP_CONVERT_RGB) << endl;
//	cout << "Rectification: " << this->cap.get(CV_CAP_PROP_RECTIFICATION) << endl;
    pthread_t grabber;
    pthread_create(&grabber, NULL, frameGrabber, this);
}

int Camera::status() {
    return this->cap.isOpened();
}

Mat Camera::takeSnapshot() {
    if (pthread_mutex_lock(&frame_lock) != 0) {
        syslog(LOG_ERR, "%s", "Sockethandler: Could not get a lock on the queue");
    }
    auto then = std::chrono::system_clock::now();
//	this->cap >> this->frame;
//    pthread_cond_wait(&this->frame_not_empty, &this->frame_lock);
//        Rect roi = Rect(0, 0, 640, 240);
//        this->frame = Mat(this->frame, roi);
//    Canny(this->frame, this->frame, 50, 200, 3);
//    cvtColor(this->frame, this->frame, COLOR_GRAY2BGR);
//    vector<Vec2f> lines = this->detectLines(this->frame);
//    for( size_t i = 0; i < lines.size(); i++ )
//    {
//        float rho = lines[i][0], theta = lines[i][1];
//        Point pt1, pt2;
//        double a = cos(theta), b = sin(theta);
//        double x0 = a*rho, y0 = b*rho;
//        pt1.x = cvRound(x0 + 1000*(-b));
//        pt1.y = cvRound(y0 + 1000*(a));
//        pt2.x = cvRound(x0 - 1000*(-b));
//        pt2.y = cvRound(y0 - 1000*(a));
//        line( this->frame, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
//    }
    if (pthread_mutex_unlock(&frame_lock) != 0) {
        syslog(LOG_ERR, "%s", "Sockethandler: Could not unlock the queue");
    }
    auto now = std::chrono::system_clock::now();
    auto dur = now - then;
    typedef std::chrono::duration<float> microseconds;
    auto secs = std::chrono::duration_cast<microseconds>(dur);
    syslog(LOG_DEBUG, "Took %f usec",  secs.count());

    return this->frame;
}

vector<Vec2f> Camera::detectLines(Mat img) {
    vector<Vec2f> lines;
    HoughLinesP(img, lines, 1, CV_PI/180, 50, 50, 10 );
    return lines;
}

Size Camera::getDimensions() {
//    return Size(640, 360);
	return Size(this->cap.get(CV_CAP_PROP_FRAME_WIDTH),this->cap.get(CV_CAP_PROP_FRAME_HEIGHT));
}