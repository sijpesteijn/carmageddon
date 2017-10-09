//
// Created by Gijs Sijpesteijn on 06/10/2017.
//

#include "traffic_light.h"
#include <iostream>
#include <string>
#include <syslog.h>
#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace std;

pthread_mutex_t snapshot_lock = PTHREAD_MUTEX_INITIALIZER;

traffic_light::traffic_light(Size size) {
    this->type = "traffic_light";
    this->roi = Rect(0, 0, size.width, size.height);
}

string traffic_light::getJson(void) {
    json_t *root = json_object();
    json_object_set_new( root, "type", json_string( this->type ) );
    json_object_set_new( root, "order", json_integer( this->order ) );
    json_object_set_new( root, "condition_achieved", json_integer( this->condition_achieved ) );
    json_object_set_new( root, "active", json_integer( this->active ) );
    json_t* roi = json_object();
    json_object_set_new( roi, "x", json_integer( this->roi.x ) );
    json_object_set_new( roi, "y", json_integer( this->roi.y ) );
    json_object_set_new( roi, "height", json_integer( this->roi.height ) );
    json_object_set_new( roi, "width", json_integer( this->roi.width ) );
    json_object_set_new( root, "roi", roi);
    json_object_set_new( root, "pixel_difference", json_integer( this->pixel_difference ) );
    json_object_set_new( root, "current_pixel_difference", json_integer( this->current_pixel_difference ) );

    string dump = json_dumps(root, 0);
    json_decref(root);
    return dump;
}

int traffic_light::updateWithJson(json_t* root) {
    this->condition_achieved = json_integer_value(json_object_get(root, "condition_achieved"));
    this->pixel_difference = json_integer_value(json_object_get(root, "pixel_difference"));
    this->current_pixel_difference = json_integer_value(json_object_get(root, "current_pixel_difference"));
    this->active = json_integer_value(json_object_get(root, "active"));
    this->order = json_integer_value(json_object_get(root, "order"));
    json_t* roi = json_object_get(root, "roi");
    this->roi.x = json_integer_value(json_object_get(roi, "x"));
    this->roi.y = json_integer_value(json_object_get(roi, "y"));
    this->roi.width = json_integer_value(json_object_get(roi, "width"));
    this->roi.height = json_integer_value(json_object_get(roi, "height"));
    return 0;
}

observer* traffic_light::processSnapshot(Mat snapshot) {
    if (this->condition_achieved == 0 && this->last_snapshot != NULL) {
        if (pthread_mutex_lock(&snapshot_lock) != 0) {
            syslog(LOG_ERR, "%s", "Sockethandler: Could not get a lock on the queue");
        }
        this->roi = this->verifyRoi(snapshot);
        Mat latest = imread("wit.png"); // Mat(this->last_snapshot->rows, this->last_snapshot->cols, this->last_snapshot->type(), this->last_snapshot->data);
        Mat roi_latest = Mat(latest, this->roi);
//		Mat roi = snapshot(this->roi);
        Mat img2 = imread("wit.png");
        if (this->count > 10 && this->count < 20)
            img2 = imread("dot.png");
        this->count++;
        Mat roi = img2(this->roi);
        Mat result;
        absdiff(roi, roi_latest, result);
        cvtColor(result, result, CV_BGR2GRAY);
        float threshold = 30;
        Mat mask = result < threshold;
        imwrite("moe.jpg", mask);
        int nonZero = countNonZero(mask);
        int percentage = 0;
        if (nonZero > 0 ) {
            percentage = 100 - (int) (nonZero*100/mask.total());
        }
        this->current_pixel_difference = percentage;
        if (percentage > this->pixel_difference) {
            this->condition_achieved = 1;
        }
        cout << "Difference: " << to_string(percentage) << "%" << endl;
        if (pthread_mutex_unlock(&snapshot_lock) != 0) {
            syslog(LOG_ERR, "%s", "Sockethandler: Could not get a lock on the queue");
        }
    }
    this->last_snapshot = &snapshot;
    return this;
}

Rect traffic_light::verifyRoi(Mat snapshot) {
    if (this->roi.x + this->roi.width > snapshot.cols) {
        if (this->roi.x > snapshot.cols) {
            this->roi.x = 0;
            this->roi.width = snapshot.cols;
        } else {
            this->roi.width = snapshot.cols - this->roi.x;
        }
    }
    if (this->roi.y + this->roi.height > snapshot.rows) {
        if (this->roi.y > snapshot.rows) {
            this->roi.y = 0;
            this->roi.height = snapshot.rows;
        } else {
            this->roi.height = snapshot.rows - this->roi.y;
        }
    }
    return this->roi;
}
