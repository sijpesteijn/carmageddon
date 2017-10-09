//
// Created by Gijs Sijpesteijn on 06/10/2017.
//

#include "camera_resource.h"
#include <syslog.h>
#include "rest.h"
using namespace cv;
using namespace std;

#define CAMERA_STATUS "/camera/status"
#define CAMERA_SNAPSHOT "/camera/snapshot/{timestamp: .*}"
Camera *camera;

void get_camera_status_handler( const shared_ptr< Session > session)
{
    const string body = ::to_string(camera->status());
    session->close( OK, body, { { "Content-Length", ::to_string( body.size( ) ) } } );
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
    this->cameraGetStatusResource->set_path( CAMERA_STATUS );
    this->cameraGetStatusResource->set_method_handler( "GET", get_camera_status_handler );
    syslog(LOG_DEBUG, "Restbed: %s",  CAMERA_STATUS);

    this->cameraSnapshotResource->set_path( CAMERA_SNAPSHOT );
    this->cameraSnapshotResource->set_method_handler( "GET", get_camera_snapshot_handler );
    syslog(LOG_DEBUG, "Restbed: %s",  CAMERA_SNAPSHOT);
}

list<shared_ptr<Resource>> camera_resource::getResources() {
    list<shared_ptr<Resource>> l = { this->cameraSnapshotResource, this->cameraGetStatusResource };
    return l;
}
