//
// Created by Gijs Sijpesteijn on 06/10/2017.
//

#include "observer_resource.h"
#include <syslog.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <iostream>
#include <jansson.h>

using namespace std;

#define OBSERVER_SETTINGS_POST "/observer"
#define OBSERVER_SETTINGS_GET  "/observer/{type: .*}"
static observer_resource *resource;

void post_observer_settings_handler(const shared_ptr<Session> session) {
    const auto request = session->get_request();
    int length = request->get_header( "Content-Length", 0 );

    session->fetch( length, [ ]( const shared_ptr< Session > session, const Bytes& )
    {
        const auto request = session->get_request( );
        const auto body = request->get_body( );

        fprintf( stdout, "Complete body content: %.*s\n", static_cast< int >( body.size( ) ), body.data( ) );
        string data = string(body.begin(), body.end());
        json_t* root;
        json_error_t error;

        root = json_loadb(data.c_str(), data.size(), 0, &error);
        if (!root) {
            fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
        }
        if (!json_is_object(root)) {
            fprintf(stderr, "error: commit data is not an object\n");
            json_decref(root);
        }

        json_t *value = json_object_get(root, "type");
        string type = "";
        type += json_string_value(value);
        observer* obs = resource->carmageddon->getObserver(type);
        if (obs == NULL) {
            resource->sendError(session, "Could not find " + type + " observer.");
        } else {
            obs->updateWithJson(root);
            const string body = data;
            session->close(OK, body, {
                    { "Content-Type", "application/json" },
                    { "Content-Length", ::to_string(body.size()) }
            });
        }
    });
}

void get_observer_settings_handler(const shared_ptr<Session> session) {
    const auto request = session->get_request();
    const string type = request->get_path_parameter("type").c_str();
    observer* obs = resource->carmageddon->getObserver(type);
    if (obs == NULL) {
        resource->sendError(session, "Could not find " + type + " observer.");
    } else {
        string body = obs->getJson();
        session->close(OK, body, {
                { "Content-Type", "application/json" },
                { "Content-Length", ::to_string(body.size()) }
        });
    }
}

observer_resource::observer_resource(cpu *carmageddon) {
    this->carmageddon = carmageddon;
    this->observerPostResource->set_path(OBSERVER_SETTINGS_POST);
    this->observerPostResource->set_method_handler("POST", post_observer_settings_handler);
    syslog(LOG_DEBUG, "Restbed: %s",  OBSERVER_SETTINGS_POST);

    this->observerGetResource->set_path(OBSERVER_SETTINGS_GET);
    this->observerGetResource->set_method_handler("GET", get_observer_settings_handler);
    syslog(LOG_DEBUG, "Restbed: %s",  OBSERVER_SETTINGS_GET);
    resource = this;
}

list<shared_ptr<Resource>> observer_resource::getResources() {
    list<shared_ptr<Resource>> l = {
            this->observerPostResource,
            this->observerGetResource
    };
    return l;
}
