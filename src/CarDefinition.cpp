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
#include "CarDefinition.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/filesystem.hpp>

using namespace KARR;

std::vector<DynamicCarDefinition> DynamicCarDefinition::sDccList;
int DynamicCarDefinition::sCurrentDefinition = -1;

DynamicCarDefinition::DynamicCarDefinition() {
}

template<typename T>
bool parseRange(boost::property_tree::ptree &pt, MinMaxPair<T> p) {
    auto minChild = pt.get_child_optional("min");
    auto maxChild = pt.get_child_optional("max");
    if (minChild && maxChild) {
	p.min = minChild.get().get_value<T>();
	p.max = maxChild.get().get_value<T>();
	return true;
    }
    return false;
}

bool parseCarDefinition(boost::property_tree::ptree &pt, DynamicCarDefinition &dcc) {
    dcc.id = pt.get_child("id").get_value<std::string>();
    dcc.make = pt.get_child("make").get_value<std::string>();
    dcc.model = pt.get_child("model").get_value<std::string>();
#define PARSE_ATTRIBUTE(name, type) \
    { \
	MinMaxPair<type> name; \
	if (parseRange(pt.get_child(#name), name)) \
	    dcc.name = name; \
	else \
	    return false; \
    }
    PARSE_ATTRIBUTE(revs, unsigned int);
    PARSE_ATTRIBUTE(speed, unsigned short);
    PARSE_ATTRIBUTE(engineTemp, short);
    PARSE_ATTRIBUTE(fuelLevel, unsigned int);
    PARSE_ATTRIBUTE(boostLevel, long long int);
    
    return true;
}


int DynamicCarDefinition::ParseDynamicCarDefinitions() {
    std::vector<DynamicCarDefinition> parsedCarDefs;

    boost::filesystem::path searchDir = "cardefinitions";
    boost::filesystem::directory_iterator di_end = boost::filesystem::directory_iterator(); 
    for (boost::filesystem::directory_iterator di = boost::filesystem::directory_iterator(searchDir); di != di_end; ++di ) {
	const boost::filesystem::directory_entry &de = *di;
	if (boost::filesystem::is_regular_file(de.path()) && de.path().extension().string() == ".xml") {
	    boost::property_tree::ptree pt;
	    boost::property_tree::read_xml(de.path().string(), pt);
	    DynamicCarDefinition dcc;
	    if (parseCarDefinition(pt.get_child("cardefinition"), dcc)) {
		parsedCarDefs.push_back(dcc);
	    }
	}
    }

    return parsedCarDefs.size();
}

bool DynamicCarDefinition::SelectCarDefinition(const std::string &id) {
    int i = 0;
    bool matched = false;
    for (auto &def : sDccList) {
	if (def.id == id) {
	    sCurrentDefinition = i;
	    matched = true;
	    break;
	}
	++i;
    }
    if (!matched)
	abort();

    return matched;
}

const DynamicCarDefinition &DynamicCarDefinition::GetCarDefinition() {
    if (sCurrentDefinition == -1)
	abort();
    return sDccList[sCurrentDefinition];
}
