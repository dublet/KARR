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


#include <string>
#include <sstream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "ArduinoDataPacket.h"
#include "Instrument.h"
#include "Screen.h"
#include "SerialConnection.h"
#include "TestInput.h"

/* Must be included after the OpenGL includes in Screen. */
#include <nanovg.h>
#include <nanovg_gl.h>

using namespace KARR;
using namespace std;

/* Globals */
NVGcontext *gNanoVGContext = nullptr;

/* Statics */
static const int w = 1024;
static const int h = 600;

static KARR::Screen *sScreen = nullptr;

static SerialConnection sArduinoConnection;
static std::vector<Instrument *> sInstruments;
static ArduinoDataPacket sDataPacket;

void handleKeyboard(unsigned char key, int x, int y) {
    if (key == 27)
	exit(0);
}

void KARRArduinoProtocol(const ArduinoDataPacket &packet) {
    if (packet.version == 7) {
	sInstruments[Instrument::IID_REVS]->update(packet.analog[0]);
	sInstruments[Instrument::IID_SPEED]->update(packet.analog[1]);
	sInstruments[Instrument::IID_FUEL_LEVEL]->update(packet.analog[2]);
	sInstruments[Instrument::IID_BOOST_LEVEL]->update(packet.analog[3]);
    }
}

void handleCommunication() {
    { /* Read data from Arduino */
	if (sArduinoConnection.isOpened()) {
	    memset(&sDataPacket, '\0', sizeof(ArduinoDataPacket));
	    if (sArduinoConnection.read((void *)&sDataPacket, sizeof(ArduinoDataPacket))) {
		// Update data
		KARRArduinoProtocol(sDataPacket);
	    }
	}
    }
}

void handleDisplay() {
    assert(sScreen);
    sScreen->preDraw();
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    nvgBeginFrame(gNanoVGContext, w, h, 1.0f);

    nvgSave(gNanoVGContext);

    for (Instrument *instrument : sInstruments) {
	if (instrument)
	    instrument->draw();
    }

    nvgRestore(gNanoVGContext);

    nvgEndFrame(gNanoVGContext);

    sScreen->postDraw();

int initScreen() {
    sScreen = new KARR::Screen(w, h);
    gNanoVGContext = nvgCreateGLES2(NVG_DEBUG);
    assert(gNanoVGContext);
    return 0;
}

void deinitScreen() {
    if (gNanoVGContext)
	nvgDeleteGLES2(gNanoVGContext);

    delete sScreen;
    sScreen = nullptr;
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
    if (maxId > -1 && maxId < 1024) {
	sInstruments.resize(maxId + 1, nullptr);
	for (Instrument *instr : instruments) {
	    sInstruments[instr->getId()] = instr;
	}
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

    initScreen();

    initInstruments();

    initCommunication();

    TestInput::run();

    assert(sScreen);
    do {
	assert(sScreen);
	if (!sScreen->processEvents())
	    break;
	handleCommunication();
	handleDisplay();
    } while (1);

    return 0;
}
