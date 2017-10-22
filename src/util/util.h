//
// Created by Gijs Sijpesteijn on 06/10/2017.
//

#ifndef CARMAGEDDON_UTIL_H
#define CARMAGEDDON_UTIL_H

#include <restbed>
#include <map>
#include <iostream>
#include <chrono>
#include <string>
#include <cstring>
#include <memory>
#include <utility>
#include <cstdlib>
#include <restbed>
#include <system_error>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>

using namespace restbed;
using namespace std;

multimap< string, string > build_websocket_handshake_response_headers( const shared_ptr< const Request >& request, string secret_key );
void close_handler( const shared_ptr< WebSocket > socket );


#endif //CARMAGEDDON_UTIL_H
