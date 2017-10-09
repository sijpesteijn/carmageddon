//
// Created by Gijs Sijpesteijn on 06/10/2017.
//

#include "car_resource.h"
#include <syslog.h>
using namespace std;

#define CAR_MODE "/car/mode"
#define CAR_ANGLE_POST "/car/steer/{angle: .*}"
#define CAR_THROTTLE_POST "/car/engine/{throttle: .*}"
#define CAR_MODE_POST "/car/mode/{mode: .*}"

static Car *car;
static car_resource *resource;

void post_angle_handler(const shared_ptr<Session> session) {
    if (car->getMode() == car_mode::stopped) {
        const string body = "Car stopped";
        session->close(405, body, {{ "Content-Length", ::to_string(body.size()) }});
    } else {
        const auto request = session->get_request();
        const int angle = atoi(request->get_path_parameter("angle").c_str());

        const string body = car->setAngle(angle) == 0 ? "true" : "false";
        session->close(OK, body, {
                { "Content-Type", "application/json" },
                { "Content-Length", ::to_string(body.size()) }
        });
    }
}

void post_throttle_handler(const shared_ptr<Session> session) {
    if (car->getMode() == car_mode::stopped) {
        const string body = "Car stopped";
        session->close(405, body, {{ "Content-Length", ::to_string(body.size()) }});
    } else {
        const auto request = session->get_request();
        const int throttle = atoi(
                request->get_path_parameter("throttle").c_str());

        const string body = car->setThrottle(throttle) == 0 ? "true" : "false";
        session->close(OK, body, {
                { "Content-Type", "application/json" },
                { "Content-Length", ::to_string(body.size()) }
        });
    }
}

void get_car_mode_handler(const shared_ptr<Session> session) {
    const string body = to_string(static_cast<std::underlying_type<car_mode>::type>(car->getMode()));
    session->close(OK, body, {
            { "Content-Type", "application/json" },
            { "Content-Length", ::to_string(body.size()) }
    });
}

void post_car_mode_handler(const shared_ptr<Session> session) {
    const auto& request = session->get_request();
    if (car->getEnabled()) {
        const string mode_str = request->get_path_parameter("mode");
        if (isdigit(mode_str[0])) {
            car_mode mode = static_cast<car_mode>(atoi(mode_str.c_str()));
            int modeInt = (int)mode;
            if (modeInt >= 0 && modeInt <= (int)car_mode::num_values) {
                syslog(LOG_DEBUG, "Car mode set to: %d", static_cast<std::underlying_type<car_mode>::type>(mode));
                car->setMode(mode);
                const string body = "{\"mode\": " + to_string(static_cast<std::underlying_type<car_mode>::type>(car->getMode())) + "}";
                session->close(OK, body, {
                        { "Content-Type", "application/json" },
                        { "Content-Length", ::to_string(body.size()) }
                });
            } else {
                resource->sendError(session, "unknown mode " + to_string(modeInt));
            }
        } else {
            resource->sendError(session, "unknown mode " + mode_str);
        }
    } else {
        resource->sendError(session, "car not connected");
    }
}

car_resource::car_resource(Car *c) {
    car = c;

    this->carGetModeResource->set_path(CAR_MODE);
    this->carGetModeResource->set_method_handler("GET", get_car_mode_handler);
    syslog(LOG_DEBUG, "Restbed: %s",  CAR_MODE);

    this->carPostModeResource->set_path(CAR_MODE_POST);
    this->carPostModeResource->set_method_handler("POST", post_car_mode_handler);
    syslog(LOG_DEBUG, "Restbed: %s",  CAR_MODE_POST);

    this->steerResource->set_path(CAR_ANGLE_POST);
    this->steerResource->set_method_handler("POST", post_angle_handler);
    syslog(LOG_DEBUG, "Restbed: %s",  CAR_ANGLE_POST);

    this->engineResource->set_path(CAR_THROTTLE_POST);
    this->engineResource->set_method_handler("POST", post_throttle_handler);
    syslog(LOG_DEBUG, "Restbed: %s",  CAR_THROTTLE_POST);
    resource = this;
}

list<shared_ptr<Resource>> car_resource::getResources() {
    list<shared_ptr<Resource>> l = { this->carGetModeResource,
                                     this->carPostModeResource, this->steerResource, this->engineResource };
    return l;
}
