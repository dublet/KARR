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
#include "Screen.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace KARR;

KARR::Screen::Screen(int w, int h) 
    : mW(w), mH(h)
    , mGLESDisplay(0), mGLESContext(0), mGLESSurface(0) {
    { // Create native Window
#ifdef KARR_RASPBERRY_PI
#else
	/*
	 * X11 native display initialization
	 */
	Window x11Window;

	mXDisplay = XOpenDisplay(nullptr);
	if (mXDisplay == nullptr)
	    abort();

	{
	    XSetWindowAttributes swAttr;
	    swAttr.event_mask = ExposureMask | PointerMotionMask | KeyPressMask;
	    Window x11RootWindow = DefaultRootWindow(mXDisplay);
	    x11Window = XCreateWindow(mXDisplay, x11RootWindow, 0, 0, mW, mH, 0,
		    CopyFromParent, InputOutput, CopyFromParent, 
		    CWEventMask, &swAttr);
	}

	{
	    XSetWindowAttributes xattr;
	    xattr.override_redirect = 0;
	    XChangeWindowAttributes(mXDisplay, x11Window, 
		    CWOverrideRedirect, &xattr);
	}

	{
	    XWMHints hints;
	    hints.input = 1;
	    hints.flags = InputHint;
	    XSetWMHints(mXDisplay, x11Window, &hints);
	}

	// make the window visible on the screen
	XMapWindow(mXDisplay, x11Window);
	XStoreName(mXDisplay, x11Window, "KARR");

	{
	    // get identifiers for the provided atom name strings
	    Atom wm_state = XInternAtom(mXDisplay, "_NET_WM_STATE", 0);
	    XEvent xev;
	    memset(&xev, 0, sizeof(xev));
	    xev.type = ClientMessage;
	    xev.xclient.window = x11Window;
	    xev.xclient.message_type = wm_state;
	    xev.xclient.format = 32;
	    xev.xclient.data.l[0] = 1;
	    xev.xclient.data.l[1] = 0;
	    XSendEvent(mXDisplay, DefaultRootWindow(mXDisplay),
		    0, SubstructureNotifyMask, &xev);
	}

	mNativeWindow = (NativeWindowType)x11Window;
#endif
    }
    { // OpenGL ES

	if (eglBindAPI(EGL_OPENGL_ES_API) == EGL_FALSE)
	    abort();

	mGLESDisplay = eglGetDisplay((EGLNativeDisplayType)mXDisplay);
	if (mGLESDisplay == EGL_NO_DISPLAY)
	    abort();

	{
	    EGLint majorVersion;
	    EGLint minorVersion;
	    if (!eglInitialize(mGLESDisplay, &majorVersion, &minorVersion))
		abort();
	}
	EGLConfig config;
	{
	    EGLint attribList[] = {
		EGL_CONFORMANT, EGL_OPENGL_ES2_BIT,
		EGL_RED_SIZE, 5,
		EGL_GREEN_SIZE, 6,
		EGL_BLUE_SIZE, 5,
		EGL_ALPHA_SIZE, EGL_DONT_CARE,
		EGL_DEPTH_SIZE, EGL_DONT_CARE,
		EGL_STENCIL_SIZE, EGL_DONT_CARE,
		EGL_SAMPLE_BUFFERS, 0,
		EGL_NONE
	    };
	    EGLint numConfigs;
	    if (!eglGetConfigs(mGLESDisplay, nullptr, 0, &numConfigs))
		abort();
	    if (!eglChooseConfig(mGLESDisplay, attribList, &config, 1, &numConfigs))
		abort();
	}

	mGLESSurface = eglCreateWindowSurface(mGLESDisplay, config, 
		mNativeWindow, nullptr);
	if (mGLESSurface == EGL_NO_SURFACE)
	    abort();

	{
	    EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, 
		EGL_NONE };
	    mGLESContext = eglCreateContext(mGLESDisplay, config, EGL_NO_CONTEXT, 
		    contextAttribs);
	    if (mGLESContext == EGL_NO_CONTEXT) 
		abort();
	}

	if (!eglMakeCurrent(mGLESDisplay, mGLESSurface, mGLESSurface, mGLESContext))
	    abort();
    }
}

KARR::Screen::~Screen() {
}

bool KARR::Screen::processEvents() {
#ifndef KARR_RASPBERRY_PI
    while (XPending(mXDisplay)) {
	XEvent xE;
	XNextEvent(mXDisplay, &xE);
	if (xE.type == DestroyNotify)
	    return false;
    }

    return true;
#else
#endif
}

void KARR::Screen::preDraw() {
    glViewport(0, 0, mW, mH);
}

void KARR::Screen::postDraw() {
    eglSwapBuffers(mGLESDisplay, mGLESSurface);
}

