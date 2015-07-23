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

AngularInstrument::AngularInstrument() : Instrument(),
    mNeedleOriginX(0), mNeedleOriginY(0),
    mNeedleMinAngle(0.0f), mNeedleMaxAngle(0.0f),
    mMinValue(0.0f), mMaxValue(0.0f), mValue(0.0f)
    {
}

AngularInstrument::~AngularInstrument() {
}

void AngularInstrument::draw() {
}

void AngularInstrument::update(float newVal) {
    mValue = newVal;
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
	    if (!mBackground.open(tags.second.get_value<string>()))
		return false;
	}
	else if (tags.first == "needle") {
	    if (!mNeedle.open(tags.second.get_value<string>()))
		return false;
	    auto attrTree = tags.second.get_child("<xmlattr>");
	    mNeedleOriginX = attrTree.get_child("x").get_value<int>();
	    mNeedleOriginY = attrTree.get_child("y").get_value<int>();
	    mNeedleMinAngle = attrTree.get_child("minAngle").get_value<float>();
	    mNeedleMaxAngle = attrTree.get_child("maxAngle").get_value<float>();
	    mMinValue = attrTree.get_child("minValue").get_value<float>();
	    mMaxValue = attrTree.get_child("maxValue").get_value<float>();
	    mValue = mMinValue;
	}
    }
    return true;
}
