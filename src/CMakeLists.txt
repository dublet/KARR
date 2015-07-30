
cmake_minimum_required(VERSION 2.8)
set(PROJECT_NAME "karr")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")


PROJECT(${PROJECT_NAME})

find_package(Boost COMPONENTS filesystem system REQUIRED)
find_package(nanosvg REQUIRED)
find_package(nanovg REQUIRED)
find_package(GLES2 REQUIRED)

INCLUDE(FindPkgConfig)

ADD_DEFINITIONS(
    -std=c++11 
    -Wall -Werror
    -DNANOVG_GLES2
)

add_executable(${PROJECT_NAME}
    AngularInstrument.cpp
    CarDefinition.cpp 
    Instrument.cpp
    main.cpp
    Screen.cpp
    SerialConnection.cpp
    Status.cpp
    SVGImage.cpp
    TestInput.cpp )

include_directories( 
	${BOOST_INCLUDEDIR} ${nanosvg_INCLUDE_DIR} 
	${nanovg_INCLUDE_DIR} ${GLES2_INCLUDE_DIRS} )

target_link_libraries(${PROJECT_NAME} 
	${Boost_LIBRARIES} 
	${GLES2_LIBRARIES})
