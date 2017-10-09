/*
 * carmageddon_resource.h
 *
 *  Created on: 15 Jul 2017
 *      Author: gijs
 */

#ifndef SRC_REST_CARMAGEDDON_RESOURCE_H_
#define SRC_REST_CARMAGEDDON_RESOURCE_H_

#include <restbed>
#include <string>
#include <list>
#include <memory>
//#include "../domain/observer.h"

using namespace std;
using namespace restbed;

class carmageddon_resource {
public:
	virtual ~carmageddon_resource() {}
	void sendError(const shared_ptr<Session> session, string msg) {
		const string body = "{\"error\": \"" + msg + "\"}";
		session->close(500, body, {
				{ "Content-Type", "application/json" },
				{ "Content-Length", ::to_string(body.size()) }
		});
	}
	virtual list<shared_ptr<Resource>> getResources() = 0;
};

#endif /* SRC_REST_CARMAGEDDON_RESOURCE_H_ */
