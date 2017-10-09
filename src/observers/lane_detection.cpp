//
// Created by Gijs Sijpesteijn on 06/10/2017.
//

#include "lane_detection.h"
#include <string>
#include <syslog.h>
#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace std;

lane_detection::lane_detection(Size size) {
    this->type = "lane_detection";
    this->roi = Rect(0, 0, size.width, size.height);
}

string lane_detection::getJson(void) {
    json_t *root = json_object();
    json_object_set_new( root, "type", json_string( this->type ) );
    json_object_set_new( root, "order", json_integer( this->order ) );
//	json_object_set_new( root, "condition_achieved", json_integer( this->condition_achieved ) );
    json_object_set_new( root, "active", json_integer( this->active ) );
    json_t* roi = json_object();
    json_object_set_new( roi, "x", json_integer( this->roi.x ) );
    json_object_set_new( roi, "y", json_integer( this->roi.y ) );
    json_object_set_new( roi, "height", json_integer( this->roi.height ) );
    json_object_set_new( roi, "width", json_integer( this->roi.width ) );
    json_object_set_new( root, "roi", roi);
//	json_object_set_new( root, "pixel_difference", json_integer( this->pixel_difference ) );
//	json_object_set_new( root, "current_pixel_difference", json_integer( this->current_pixel_difference ) );

    string dump = json_dumps(root, 0);
    json_decref(root);
    return dump;
}

int lane_detection::updateWithJson(json_t* root) {
    this->condition_achieved = json_integer_value(json_object_get(root, "condition_achieved"));
//	this->pixel_difference = json_integer_value(json_object_get(root, "pixel_difference"));
//	this->current_pixel_difference = json_integer_value(json_object_get(root, "current_pixel_difference"));
    this->active = json_integer_value(json_object_get(root, "active"));
    this->order = json_integer_value(json_object_get(root, "order"));
    json_t* roi = json_object_get(root, "roi");
    this->roi.x = json_integer_value(json_object_get(roi, "x"));
    this->roi.y = json_integer_value(json_object_get(roi, "y"));
    this->roi.width = json_integer_value(json_object_get(roi, "width"));
    this->roi.height = json_integer_value(json_object_get(roi, "height"));
    return 0;
}

Rect lane_detection::verifyRoi(Mat snapshot) {
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

observer* lane_detection::processSnapshot(Mat snapshot) {
    return this;
}