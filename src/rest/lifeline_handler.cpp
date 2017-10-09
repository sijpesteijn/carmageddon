//
// Created by Gijs Sijpesteijn on 06/10/2017.
//

#include "lifeline_handler.h"
#include <iostream>
#include <syslog.h>
#include "../util/util.h"

using namespace std;
using namespace restbed;
using namespace std::chrono;

#define LIFELINE "/lifeline"

static Car *car;
static pthread_mutex_t checker_lock = PTHREAD_MUTEX_INITIALIZER;
static shared_ptr< Service > service = nullptr;
static map< string, shared_ptr< WebSocket > > sockets = { };

void lifeline_close_handler( const shared_ptr< WebSocket > socket )
{
    syslog(LOG_DEBUG, "Lifeline close handler");
    if ( socket->is_open( ) )
    {
        auto response = make_shared< WebSocketMessage >( WebSocketMessage::CONNECTION_CLOSE_FRAME, Bytes( { 10, 00 } ) );
        socket->send( response );
    }

    const auto key = socket->get_key( );
    sockets.erase( key );
    syslog(LOG_DEBUG, "%lu", sockets.size());
    if (car->getEnabled() != 0 && sockets.size() == 0) {
        syslog(LOG_ERR, "No connections car stopped");
        car->setEnabled(0);
    }

    syslog(LOG_DEBUG, "Closed connection to %s.\n", key.data( ));
}

void lifeline_error_handler( const shared_ptr< WebSocket > socket, const error_code error )
{
    const auto key = socket->get_key( );
    syslog(LOG_ERR, "WebSocket Errored '%s' for %s.\n", error.message( ).data( ), key.data( ) );
}

void lifeline_message_handler( const shared_ptr< WebSocket > source, const shared_ptr< WebSocketMessage > message )
{
    const auto opcode = message->get_opcode( );

    if ( opcode == WebSocketMessage::PING_FRAME )
    {
        auto response = make_shared< WebSocketMessage >( WebSocketMessage::PONG_FRAME, message->get_data( ) );
        source->send( response );
    }
    else if ( opcode == WebSocketMessage::PONG_FRAME )
    {
        cout << "PONG" << endl;
        //Ignore PONG_FRAME.
        //
        //Every time the ping_handler is scheduled to run, it fires off a PING_FRAME to each
        //WebSocket. The client, if behaving correctly, will respond with a PONG_FRAME.
        //
        //On each occasion the underlying TCP socket sees any packet data transfer, whether
        //a PING, PONG, TEXT, or BINARY... frame. It will automatically reset the timeout counter
        //leaving the connection active; see also Settings::set_connection_timeout.
        return;
    }
    else if ( opcode == WebSocketMessage::CONNECTION_CLOSE_FRAME )
    {
        source->close( );
    }
    else if ( opcode == WebSocketMessage::BINARY_FRAME )
    {
        //We don't support binary data.
        auto response = make_shared< WebSocketMessage >( WebSocketMessage::CONNECTION_CLOSE_FRAME, Bytes( { 10, 03 } ) );
        source->send( response );
    }
    else if ( opcode == WebSocketMessage::TEXT_FRAME )
    {
        const string body = "{\"mode\": " + to_string(static_cast<std::underlying_type<car_mode>::type>(car->getMode())) + ","
                            + "\"angle\": " + to_string(car->getAngle()) + ","
                            + "\"enabled\": " + to_string(car->getEnabled()) + ","
                            + "\"throttle\": " + to_string(car->getThrottle()) + "}";
        auto response = make_shared< WebSocketMessage >(WebSocketMessage::TEXT_FRAME, body );
        source->send(response);
//        response->set_mask( 0 );
//        for ( auto socket : sockets )
//        {
//            auto destination = socket.second;
//            destination->send( response );
//        }
//
//        const auto key = source->get_key( );
//        const auto data = String::format( "Received message '%.*s' from %s\n", message->get_data( ).size( ), message->get_data( ).data( ), key.data( ) );
//        fprintf( stderr, "%s", data.data( ) );
    }
}

void get_lifeline_method_handler( const shared_ptr< Session > session )
{
    const auto request = session->get_request( );
    const auto connection_header = request->get_header( "connection", String::lowercase );

    if ( connection_header.find( "upgrade" ) not_eq string::npos )
    {
        if ( request->get_header( "upgrade", String::lowercase ) == "websocket" )
        {
            const auto headers = build_websocket_handshake_response_headers( request, "258EAFA5-E914-47DA-95CA-C5AB0DC85B11" );

            session->upgrade( SWITCHING_PROTOCOLS, headers, [ ]( const shared_ptr< WebSocket > socket )
            {
                if ( socket->is_open( ) )
                {
                    if (pthread_mutex_lock(&checker_lock) != 0) {
                        syslog(LOG_ERR, "Sockethandler: Could not get a lock on the queue");
                    }
                    socket->set_close_handler( lifeline_close_handler );
                    socket->set_error_handler( lifeline_error_handler );
                    socket->set_message_handler( lifeline_message_handler );

                    const string body = "{\"mode\": " + to_string(static_cast<std::underlying_type<car_mode>::type>(car->getMode())) + ","
                                        + "\"angle\": " + to_string(car->getAngle()) + ","
                                        + "\"throttle\": " + to_string(car->getThrottle()) + "}";
                    socket->send( body, [ ]( const shared_ptr< WebSocket > socket )
                    {
                        const auto key = socket->get_key( );
                        sockets.insert( make_pair( key, socket ) );
                        syslog(LOG_DEBUG, "%lu", sockets.size());
                        car->setEnabled(1);

                        fprintf( stderr, "Sent welcome message to %s.\n", key.data( ) );
                    } );
                    if (pthread_mutex_unlock(&checker_lock) != 0) {
                        syslog(LOG_ERR, "%s", "Sockethandler: Could not unlock the queue");
                    }
                }
                else
                {
                    fprintf( stderr, "WebSocket Negotiation Failed: Client closed connection.\n" );
                }
            } );

            return;
        }
    }

    session->close( BAD_REQUEST );
}

//void* connectionChecker(void* params) {
//	Car *car = (Car*) params;
//	while(1) {
//		if (pthread_mutex_lock(&checker_lock) != 0) {
//			syslog(LOG_ERR, "Sockethandler: Could not get a lock on the queue");
//		}
//		if (car->getEnabled() != 0 && sockets.size() == 0) {
//			syslog(LOG_ERR, "No connections car stopped");
//			car->setEnabled(0);
//		}
//		if (car->getEnabled() == 0 && sockets.size() > 0) {
//			syslog(LOG_ERR, "Enabling car since we have websocket connections.");
//			car->setEnabled(1);
//		}
//		if (pthread_mutex_unlock(&checker_lock) != 0) {
//			syslog(LOG_ERR, "Sockethandler: Could not unlock the queue");
//		}
//	}
//	return NULL;
//}

lifeline_handler::lifeline_handler(Car *carP) {
    car = carP;
    this->resource = make_shared< Resource >( );
    this->resource->set_path( LIFELINE );
    this->resource->set_method_handler( "GET", get_lifeline_method_handler );
    syslog(LOG_DEBUG, "Restbed websocket: %s", LIFELINE );
//	pthread_t checker;
//	pthread_create(&checker, NULL, connectionChecker, carP);
}

list<shared_ptr<Resource>> lifeline_handler::getResources() {
    return { this->resource };
}

void lifeline_handler::notifyClients() {
//    for(map<string, shared_ptr< WebSocket>>::iterator it = sockets.begin(); it != sockets.end(); ++it) {
//        cout << "MOE" << endl;
//    }
}