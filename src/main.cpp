/**
 * Project
 * #    #     #     ######   ######   
 * #   #     # #    #     #  #     #  
 * #  #     #   #   #     #  #     #  
 * ###     #     #  ######   ######   
 * #  #    #######  #   #    #   #    
 * #   #   #     #  #    #   #    #   
 * #    #  #     #  #     #  #     #  
 *
 * Copyright (c) 2014, Project KARR
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, 
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, 
 * this list of conditions and the following disclaimer in the documentation 
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include <stdio.h>
#include <stdlib.h>

#include <GL/glut.h>
#include <vg/openvg.h>

#include <string>
#include <sstream>

#include "ArduinoDataPacket.h"
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
static SerialConnection sArduinoConnection;

void handleKeyboard(unsigned char key, int x, int y) {
    if (key == 27)
	exit(0);
}

void handleDisplay() {
    int now;
    static bool timeinit = false;
    static int fpsdraw = -1;
    static int fps = 0;
    static int lastfps = 0;
    static ArduinoDataPacket dataPacket;

    { /* Read data from Arduino */
	if (sArduinoConnection.isOpened()) {
	    memset(&dataPacket, '\0', sizeof(ArduinoDataPacket));
	    if (sArduinoConnection.read((void *)&dataPacket, sizeof(ArduinoDataPacket))) {

		// Update data
	    }
	}
    }

    /* Get interval from last redraw */
    now = glutGet(GLUT_ELAPSED_TIME);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    vgClear(0,0,w,h);

    /* Draw scene */
    const Display *currentDisplay = DisplayManager::instance().getCurrent();
    if (currentDisplay)
	const_cast<Display *>(currentDisplay)->draw(/*interval*/);
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
    return 0;
}


void registerDisplays() {
    DisplayManager &dm = DisplayManager::instance();
    dm.registerDisplay(new SimpleTextDisplay());
    dm.registerDisplay(new Celica205Display());
}

void initCommunication() {
    arduinoConnection.open("/dev/ttyAMA0");
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);

    initScreen();

    initCommunication();

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
