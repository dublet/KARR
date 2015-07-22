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
#ifndef HAVE_KARR_INSTRUMENT_H
#define HAVE_KARR_INSTRUMENT_H

#include <vector>
#include <boost/property_tree/ptree.hpp>

class Instrument {
    public:
	typedef enum {
	    IID_UNKNOWN = -1,
	    IID_FUEL_LEVEL,
	    IID_SPEED,
	    IID_REVS,
	    IID_BOOST_LEVEL,
	    IDD_ENGINE_TEMP,
	} InstrumentId;
	static InstrumentId GetInstrumentIdForString(const std::string &);
    protected:
	InstrumentId mId;
	int mX, mY, mW, mH;
	
    public:
	Instrument();
	virtual ~Instrument();

	InstrumentId getId();
	int getDisplayX();
	int getDisplayY();
	int getDisplayWidth();
	int getDisplayHeight();

	virtual void draw() = 0;

	static bool ParseInstruments(boost::property_tree::ptree &, std::vector<Instrument *> &);
	virtual bool parseFromTree(boost::property_tree::ptree &) = 0;

};
#endif
