
cmake_minimum_required(VERSION 2.8)
set(PROJECT_NAME "karr")

PROJECT(${PROJECT_NAME})

find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)
find_package(ShivaVG REQUIRED)
find_package(Boost COMPONENTS filesystem system REQUIRED)

ADD_DEFINITIONS(
    -std=c++11 
)

add_executable(${PROJECT_NAME}
    VGPP/Path.cpp 
    VGPP/Matrix.cpp 
    CelicaDisplay.cpp 
    CarDefinition.cpp 
    Display.cpp
    DisplayManager.cpp
    GLUtil.cpp
    Input.cpp
    main.cpp
    SimpleTextDisplay.cpp
    Status.cpp
    TestInput.cpp )

include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} ${SHIVAVG_INCLUDE_DIRS} ${BOOST_INCLUDEDIR} )

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${SHIVAVG_LIBRARY} ${Boost_LIBRARIES} )
