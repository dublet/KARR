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

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "ArduinoDataPacket.h"
#include "Instrument.h"
#include "SerialConnection.h"
#include "TestInput.h"
#include "Knight_Industries.C"

using namespace KARR;
using namespace std;

static const int w = 1024;
static const int h = 600;
static SDL_Window *sSdlWindow = nullptr;
static SerialConnection sArduinoConnection;
static std::vector<Instrument *> sInstruments;

void handleKeyboard(unsigned char key, int x, int y) {
    if (key == 27)
	exit(0);
}

void handleDisplay() {
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

    for (Instrument *instrument : sInstruments) {
	if (instrument)
	    instrument->draw();
    }
    // Advance to next frame. Rendering thread will be kicked to
    // process submitted rendering primitives.
    bgfx::frame();
}


int initScreen() {
    SDL_InitSubSystem(SDL_INIT_VIDEO);

    sSdlWindow = SDL_CreateWindow("karr",
	    SDL_WINDOWPOS_UNDEFINED , SDL_WINDOWPOS_UNDEFINED,
	    w , h , SDL_WINDOW_SHOWN);

    bgfx::sdlSetWindow(sSdlWindow);

    bgfx::init();
    bgfx::reset(w, h, BGFX_RESET_VSYNC);

    // Set view 0 clear state.
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 
	    0x303030ff, 1.0f, 0);

    return 0;
}

void deinitScreen() {
    // Shutdown bgfx.
    bgfx::shutdown();

    SDL_DestroyWindow(sSdlWindow);
    SDL_Quit();
}

void initInstruments() {
    boost::filesystem::path instrumentsXMLPath(""); // TODO get path
    boost::property_tree::ptree instrumentsTree;
    boost::property_tree::read_xml(instrumentsXMLPath.string(), instrumentsTree, boost::property_tree::xml_parser::no_comments);

    // Parse into a temp vector.
    std::vector<Instrument *> instruments;
    Instrument::ParseInstruments(instrumentsTree, instruments);

    // Determine max id for size.
    Instrument::InstrumentId maxId = Instrument::IID_UNKNOWN;
    for (Instrument *instr : instruments) {
	if (instr->getId() > maxId)
	    maxId = instr->getId();
    }

    // Populate the instruments with the parsed. May result in sparse vector.
    sInstruments.resize(maxId, nullptr);
    for (Instrument *instr : instruments) {
	sInstruments[instr->getId()] = instr;
    }
}

void deinitInstruments() {
    for (Instrument *instr : sInstruments) {
	delete instr;
    }
}

void initCommunication() {
    sArduinoConnection.open("/dev/ttyAMA0");
}

void deinitCommunication() {
    sArduinoConnection.close();
}

void cleanup() {
    deinitCommunication();

    deinitInstruments();

    deinitScreen();
}

int main(int argc, char** argv) {
    atexit(cleanup);

    SDL_Init(0);

    initScreen();

    initInstruments();
    initCommunication();

    TestInput::run();

    do {

	SDL_Event event;
	if (SDL_PollEvent(&event)) {
	    // Do something with event
	}
	handleDisplay();
    } while (1);

    return 0;
}
