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

#include "AngularInstrument.h"

#include <string>

using namespace std;

AngularInstrument::AngularInstrument() : Instrument() {
}

AngularInstrument::~AngularInstrument() {
}

void AngularInstrument::draw() {
}

void AngularInstrument::update(float newVal) {
}

bool AngularInstrument::parseFromTree(boost::property_tree::ptree &instrumentTree) {

    for (auto &tags : instrumentTree) {
	if (tags.first == "id") {
	    mId = GetInstrumentIdForString(tags.second.get_value<string>());
	    if (mId == IID_UNKNOWN)
		return false;
	}
	else if (tags.first == "area") {
	    auto attrTree = tags.second.get_child("<xmlattr>");
	    mX = attrTree.get_child("x").get_value<int>();
	    mY = attrTree.get_child("y").get_value<int>();
	    mW = attrTree.get_child("w").get_value<int>();
	    mH = attrTree.get_child("h").get_value<int>();
	}
	else if (tags.first == "background") {
	    mBackground.open(tags.second.get_value<string>());
	}
	else if (tags.first == "needle") {
	    mNeedle.open(tags.second.get_value<string>());
	    auto attrTree = tags.second.get_child("<xmlattr>");
	    mNeedleOriginX = attrTree.get_child("x").get_value<int>();
	    mNeedleOriginY = attrTree.get_child("y").get_value<int>();
	}
    }
    return true;
}
