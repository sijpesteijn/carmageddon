//
// Created by Gijs Sijpesteijn on 06/10/2017.
//

#include "camera_resource.h"
#include <syslog.h>
#include <jansson.h>
using namespace cv;
using namespace std;

#define CAMERA_SETTINGS "/camera/settings"
#define CAMERA_DIMENSIONS "/camera/dimensions"
#define CAMERA_SNAPSHOT "/camera/snapshot/{timestamp: .*}"
Camera *camera;

void get_camera_settings_handler(const shared_ptr<Session> session)
{
    const string body = "{\"dimension\":{\"name\": \"" + to_string(camera->getDimensions().width) + "x" + to_string(camera->getDimensions().height) + "\","
                        + "\"width\":" + to_string(camera->getDimensions().width) + ","
                        + "\"height\": " + to_string(camera->getDimensions().height) + "},"
                        + "\"captureDelay\": " + to_string(camera->capture_delay) + ","
                        + "\"previewDelay\": " + to_string(camera->preview_delay) + "}";
    session->close( OK, body, { { "Content-Length", ::to_string( body.size( ) ) } } );
}

void post_camera_settings_handler( const shared_ptr< Session > session )
{
    const auto request = session->get_request( );

    int content_length = request->get_header( "Content-Length", 0 );

    session->fetch( content_length, [ ]( const shared_ptr< Session > session, const Bytes & body )
    {
        fprintf( stdout, "%.*s\n", ( int ) body.size( ), body.data( ) );
        json_t* root;
        json_error_t error;

        root = json_loadb(reinterpret_cast<const char *>(body.data()), body.size(), 0, &error);
        if (!root) {
            fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
        }
        if (!json_is_object(root)) {
            fprintf(stderr, "error: commit data is not an object\n");
            json_decref(root);
        }
        json_t *resolutionJson = json_object_get(root,"dimension");
        int width = json_number_value(json_object_get(resolutionJson, "width"));
        int height = json_number_value(json_object_get(resolutionJson, "height"));
        camera->setDimension(width, height);
        camera->capture_delay = json_number_value(json_object_get(root, "captureDelay"));
        camera->preview_delay = json_number_value(json_object_get(root, "previewDelay"));

        session->close( OK, body, { { "Content-Length", ::to_string( body.size( ) ) } } );
    } );
}

void get_camera_dimensions_handler(const shared_ptr<Session> session)
{
    const string body = "[{\"name\": \"352x288\", \"width\": 352, \"height\": 288},{\"name\": \"640x480\", \"width\": 640, \"height\": 480}]";
    session->close( OK, body, { { "Content-Length", ::to_string( body.size( ) ) } } );
}

string getSettingsJson() {
    const string body = "{\"dimension\":{\"name\": \"" + to_string(camera->getDimensions().width) + "x" + to_string(camera->getDimensions().height) + "\","
                        + "\"width\":" + to_string(camera->getDimensions().width) + ","
                        + "\"height\": " + to_string(camera->getDimensions().height) + "},"
                        + "\"captureDelay\": " + to_string(camera->capture_delay) + ","
                        + "\"previewDelay\": " + to_string(camera->preview_delay) + "}";
    return body;
}

void get_camera_snapshot_handler( const shared_ptr< Session > session)
{
    Mat snapshot = camera->takeSnapshot();
    // TODO iets beters dan dit. Mat moet een ifstream worden. Maar hoe?
    imwrite("bla.jpg", snapshot);
//	Mat snapshot = imread("bla.png");
//    Canny(snapshot, snapshot, 50, 200, 3);
//    cvtColor(snapshot, snapshot, CV_GRAY2BGR);
//    vector<Vec4i> lines;
//      HoughLinesP(snapshot, lines, 1, CV_PI/180, 50, 50, 10 );
//      for( size_t i = 0; i < lines.size(); i++ )
//      {
//        Vec4i l = lines[i];
//        line( snapshot, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
//      }
//      imwrite("output.png", snapshot);
    ifstream stream("bla.jpg", ios::in | ios::binary);
    const string body = string( istreambuf_iterator< char >( stream ), istreambuf_iterator< char >( ) );
    const multimap< string, string > headers
            {
                    { "Content-Type", "image/jpeg" },
                    { "Content-Length", ::to_string( body.length() ) }
            };

    session->close( OK, body, headers );
}

camera_resource::camera_resource(Camera *cam) {
    camera = cam;
    this->cameraGetDimensionsResource->set_path( CAMERA_DIMENSIONS );
    this->cameraGetDimensionsResource->set_method_handler("GET", get_camera_dimensions_handler);
    syslog(LOG_DEBUG, "Restbed: %s",  CAMERA_DIMENSIONS);

    this->cameraSettingsResource->set_path( CAMERA_SETTINGS );
    this->cameraSettingsResource->set_method_handler("GET", get_camera_settings_handler);
    this->cameraSettingsResource->set_method_handler("POST", post_camera_settings_handler);
    syslog(LOG_DEBUG, "Restbed: %s",  CAMERA_SETTINGS);

    this->cameraSnapshotResource->set_path( CAMERA_SNAPSHOT );
    this->cameraSnapshotResource->set_method_handler( "GET", get_camera_snapshot_handler );
    syslog(LOG_DEBUG, "Restbed: %s",  CAMERA_SNAPSHOT);
}

list<shared_ptr<Resource>> camera_resource::getResources() {
    list<shared_ptr<Resource>> l = {
            this->cameraSnapshotResource,
            this->cameraSettingsResource,
            this->cameraGetDimensionsResource
    };
    return l;
}
