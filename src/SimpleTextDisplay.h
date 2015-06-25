#ifndef HAVE_KARR_SIMPLE_TEXT_DISPLAY_H
#define HAVE_KARR_SIMPLE_TEXT_DISPLAY_H

#include "Display.h"

namespace KARR {
    class SimpleTextDisplay : public Display {
	public:
	    SimpleTextDisplay();
	    virtual ~SimpleTextDisplay();
	    void draw();

    };
}

#endif
