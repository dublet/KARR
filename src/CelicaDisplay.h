#ifndef HAVE_KARR_CELICA_205_DISPLAY_H
#define HAVE_KARR_CELICA_205_DISPLAY_H

#include "Display.h"
#include "VGPP/Path.h"

namespace KARR {
    class Celica205Display : public Display {
	private:
	    VGPP::Path _speedNeedle, _speedTicks;
	    VGPP::Path _parkingBrakeIcon, _mainBeamIcon;
	    VGPP::Path _testPath;
	public:
	    Celica205Display();
	    virtual ~Celica205Display();
	    void draw();

    };
}

#endif
