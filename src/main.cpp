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

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

#include <bgfx.h>
#include <bgfxplatform.h>

#include <string>
#include <sstream>

#include "ArduinoDataPacket.h"
#include "Display.h"
#include "DisplayManager.h"
#include "GLUtil.h"
#include "CelicaDisplay.h"
#include "Instrument.h"
#include "SimpleTextDisplay.h"
#include "SerialConnection.h"
#include "TestInput.h"
#include "Knight_Industries.C"

using namespace KARR;
using namespace std;

static const int w = 1024;
static const int h = 600;
static SerialConnection sArduinoConnection;
static std::vector<Instrument> sInstruments;

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

    // Set view 0 default viewport.
    bgfx::setViewRect(0, 0, 0, w, h);
    // This dummy draw call is here to make sure that view 0 is cleared
    // if no other draw calls are submitted to view 0.
    bgfx::submit(0);
    // Use debug font to print information about this example.
    bgfx::dbgTextClear();
    bgfx::dbgTextPrintf(0, 1, 0x4f, "bgfx/examples/00-helloworld");
    bgfx::dbgTextPrintf(0, 2, 0x6f, "Description: Initialization and debug text.");

    for (Instrument &instrument : sInstruments) {
	instrument.draw();
    }
    // Advance to next frame. Rendering thread will be kicked to
    // process submitted rendering primitives.
	bgfx::frame();
}

void cleanup() {
    // Shutdown bgfx.
    bgfx::shutdown();
}

int initScreen() {
    SDL_InitSubSystem(SDL_INIT_VIDEO);

    auto sdlWindow = SDL_CreateWindow("karr"
	    , SDL_WINDOWPOS_UNDEFINED
	    , SDL_WINDOWPOS_UNDEFINED
	    , w
	    , h
	    , SDL_WINDOW_SHOWN
	    );

    bgfx::sdlSetWindow(sdlWindow);
#if 0
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_STENCIL | GLUT_MULTISAMPLE);
    glutInitWindowPosition(0,0);
    glutInitWindowSize(w,h);
    glutCreateWindow("KARR");

    glutDisplayFunc(handleDisplay);
    glutIdleFunc(glutPostRedisplay );
    glutKeyboardFunc(handleKeyboard);
#endif
    atexit(cleanup);

    bgfx::init();
    bgfx::reset(w, h, BGFX_RESET_VSYNC);

    // Set view 0 clear state.
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 
	    0x303030ff, 1.0f, 0);
    return 0;
}


void initCommunication() {
    sArduinoConnection.open("/dev/ttyAMA0");
}

int main(int argc, char** argv) {
    SDL_Init(0);

    initScreen();

    initCommunication();

    TestInput::run();

    do {

	SDL_Event event;
	if (SDL_PollEvent(&event)) {
	    // Do something with event
	}
	handleDisplay();
    } while (1);
    glutMainLoop();
    return 0;
}
