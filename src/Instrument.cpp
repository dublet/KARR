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
#include "Instrument.h"
#include "AngularInstrument.h"

using namespace std;

Instrument::Instrument() : mId(IID_UNKNOWN), mX(0), mY(0), mW(0), mH(0) {
}

Instrument::~Instrument() {
}

Instrument::InstrumentId Instrument::getId() {
    return mId;
}

int Instrument::getDisplayX() {
    return mX;
}

int Instrument::getDisplayY() {
    return mY;
}

int Instrument::getDisplayWidth() {
    return mW;
}

int Instrument::getDisplayHeight() {
    return mH;
}

void Instrument::draw() {
}

Instrument:: InstrumentId Instrument::GetInstrumentIdForString(const std::string &str) {
    if (str == "fuel_level") return IID_FUEL_LEVEL;
    if (str == "speed") return IID_SPEED;
    if (str == "revs") return IID_REVS;
    if (str == "boost_level") return IID_BOOST_LEVEL;
    if (str == "engine_temp") return IDD_ENGINE_TEMP;
    return IID_UNKNOWN;
}

bool Instrument::ParseInstruments(boost::property_tree::ptree &inputTree, vector<Instrument *> &result) {

    auto root = inputTree.get_child("instruments");
    for (auto &child : root) {
	if (child.first == "instrument") {
	    for (auto &tags : child.second) {
		Instrument *currInstrument = nullptr;
		if (tags.first == "type") {
		    string type = tags.second.get_value<string>();
		    if (type == "angular") {
			currInstrument = new AngularInstrument();
		    } else {
			continue;
		    }
		}
		if (currInstrument) {
		    if (currInstrument->parseFromTree(child.second))
			result.push_back(currInstrument);
		    else {
			// Log error
		    }
		}
	    }
	}
    }
    return false;
}
