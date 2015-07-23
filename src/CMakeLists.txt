
cmake_minimum_required(VERSION 2.8)
set(PROJECT_NAME "karr")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")


PROJECT(${PROJECT_NAME})

find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)
find_package(ShivaVG REQUIRED)
find_package(Boost COMPONENTS filesystem system REQUIRED)
find_package(nanosvg REQUIRED)
find_package(nanovg REQUIRED)
find_package(bgfx REQUIRED)
INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)

ADD_DEFINITIONS(
    -std=c++11 
    -Wall -Werror
)

add_executable(${PROJECT_NAME}
    AngularInstrument.cpp
    CarDefinition.cpp 
    GLUtil.cpp
    Input.cpp
    Instrument.cpp
    main.cpp
    SerialConnection.cpp
    Status.cpp
    SVGImage.cpp
    TestInput.cpp )

include_directories( ${OPENGL_INCLUDE_DIRS}  
	${GLUT_INCLUDE_DIRS} ${SHIVAVG_INCLUDE_DIRS} ${BOOST_INCLUDEDIR} 
	${nanosvg_INCLUDE_DIR} ${bgfx_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS}
	${nanovg_INCLUDE_DIR} )

target_link_libraries(${PROJECT_NAME} 
	${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${SHIVAVG_LIBRARY} 
	${Boost_LIBRARIES} ${bgfx_LIBRARIES} ${SDL2_LIBRARIES})
