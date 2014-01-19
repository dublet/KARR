#include <stdio.h>
#include <stdlib.h>

#include <GL/glut.h>
#include <vg/openvg.h>

#include <string>
#include <sstream>

#include "Display.h"
#include "DisplayManager.h"
#include "GLUtil.h"
#include "CelicaDisplay.h"
#include "SimpleTextDisplay.h"
#include "TestInput.h"
#include "Knight_Industries.C"

using namespace KARR;
using namespace std;

static VGImage knightIndustries;
static const int w = 1024;
static const int h = 600;

void handleKeyboard(unsigned char key, int x, int y) {
    if (key == 27)
	exit(0);
}


void handleDisplay() {
    int now, msinterval;
    float interval;
    static bool timeinit = false;
    static int lastdraw = -1;
    static int fpsdraw = -1;
    static int fps = 0;
    static int lastfps = 0;

    /* Get interval from last redraw */
    now = glutGet(GLUT_ELAPSED_TIME);
    if (!timeinit) 
	lastdraw = now;
    msinterval = now - lastdraw;
    interval = (float)msinterval / 1000;
    lastdraw = now;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    vgClear(0,0,w,h);

    /* Draw scene */
    const Display *currentDisplay = DisplayManager::instance().getCurrent();
    if (currentDisplay)
	currentDisplay->draw(/*interval*/);
    else
    {
	vgSeti(VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE);
	vgLoadIdentity();
	vgScale(1, -1); // Flip the picture
	vgTranslate(w / 2 - knight_industries.width / 2, -433); // Move to to the center
	vgDrawImage(knightIndustries);
    }


    if (fpsdraw > -1) {
	/* Draw fps */
	GLfloat overcolor[4] = {1, 1, 1, 1};
	glColor4fv(overcolor);
	stringstream ss;
	ss << "FPS: " << fpsdraw;
	GLUtil::drawString(10, 10, ss.str());
    }

    /* Swap */
    glutSwapBuffers();

    /* Count frames per second */
    ++fps;

    if (timeinit) {
	if (now - lastfps > 1000) {
	    lastfps = now;
	    fpsdraw = fps;
	    fps = 0;
	}
    } else {
	lastfps = now;
	timeinit = true;
    }
}

void cleanup() {
    vgDestroyContextSH();
}

int initScreen() {
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_STENCIL | GLUT_MULTISAMPLE);

    glutInitWindowPosition(0,0);
    glutInitWindowSize(w,h);
    glutCreateWindow("KARR");

    glutDisplayFunc(handleDisplay);
    glutIdleFunc(glutPostRedisplay );
    glutKeyboardFunc(handleKeyboard);
    atexit(cleanup);

    vgCreateContextSH(w,h);
}

void registerDisplays() {
    DisplayManager &dm = DisplayManager::instance();
    dm.registerDisplay(new Celica205Display());
    dm.registerDisplay(new SimpleTextDisplay());
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);

    initScreen();

    knightIndustries = vgCreateImage(VG_sARGB_8888, 300, 267, 
	    VG_IMAGE_QUALITY_NONANTIALIASED);
    vgImageSubData(knightIndustries, knight_industries.pixel_data, 
	    knight_industries.width * knight_industries.bytes_per_pixel,
	    VG_sRGB_565,
	    0, 0, knight_industries.width, knight_industries.height);


    registerDisplays();

    TestInput::run();

    glutMainLoop();
    return 0;
}
