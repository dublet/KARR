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
#include "SerialConnection.h"

#include <errno.h>
#include <fcntl.h>
#include <termios.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

SerialConnection::SerialConnection(const boost::filesystem::path) : mFd(-1) {
}

SerialConnection::~SerialConnection() {
    close();
}

bool SerialConnection::isOpened() const {
    return mFd > -1;
}

void SerialConnection::open() {
    mFd = open(mPath.string().c_str(), O_RDWR | O_NONBLOCK | O_NDELAY);
    if (mFd < 0)
	abort();

    {
	struct termios tty;
	memset(&tty, '\0', sizeof(tty));

	cfsetospeed(&tty, B9600);

	tty.c_cc[VMIN] = 1;
	tty.c_cc[VTIME] = 0;
	cfmakeraw(&tty);

	if (tcsetattr(mFd, TCSAFLUSH, &tty) != 0) {
	    close();
	    abort();
	}
    }

}

void SerialConnection::close() {
    if (mFd > -1) 
	close(mFd);
    mFd = -1;
}

bool SerialConnection::read(void *buf, size_t size) {
    ssize_t readSize = ::read(mFd, buf, size);
    if (readSize != size)
	return false;
    return true;
}

void SerialConnection::write() {
}
