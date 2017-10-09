//
// Created by Gijs Sijpesteijn on 06/10/2017.
//

#ifndef CARMAGEDDON_LANE_DETECTION_H
#define CARMAGEDDON_LANE_DETECTION_H

#include "../domain/observer.h"
#include <jansson.h>

class lane_detection:public observer {
public:
    lane_detection(Size size);
    string getJson(void);
    int updateWithJson(json_t* root);
    observer* processSnapshot(Mat snapshot);
    Rect verifyRoi(Mat snaphot);
private:
};

#endif //CARMAGEDDON_LANE_DETECTION_H
