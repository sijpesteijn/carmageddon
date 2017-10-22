cmake_minimum_required(VERSION 3.5)
project(carmageddon)

set(CMAKE_CXX_STANDARD 11)
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories( /usr/local/include/restbed )
include_directories( /usr/local/include/openssl )
include_directories( /usr/local/include/jansson )

set(SOURCE_FILES src/main.cpp src/domain/camera.h src/domain/camera.cpp src/domain/pwm.h src/domain/pwmmock.cpp src/domain/steer.h src/domain/steer.cpp src/domain/engine.h src/domain/engine.cpp src/domain/car.h src/domain/car.cpp src/rest/carmageddon_resource.h src/rest/camera_resource.h src/rest/camera_resource.cpp src/rest/car_resource.cpp src/rest/car_resource.h src/rest/rest.cpp src/rest/rest.h src/rest/carstatus_handler.cpp src/rest/carstatus_handler.h src/util/util.cpp src/util/util.h src/rest/lifeline_handler.cpp src/rest/lifeline_handler.h src/rest/observer_resource.cpp src/rest/observer_resource.h src/cpu.cpp src/cpu.h src/domain/observer.h src/observers/traffic_light.cpp src/observers/traffic_light.h src/observers/lane_detection.cpp src/observers/lane_detection.h)

add_executable(carmageddon ${SOURCE_FILES})
target_link_libraries( carmageddon ${OpenCV_LIBS} restbed crypto ssl jansson pthread)
