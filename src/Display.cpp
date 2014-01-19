#include "Display.h"

using namespace KARR;

Display::Display() {
}

Display::~Display() {
}

const char *Display::getSpeedUnit() const {
    switch (_unitMode) {
	case UNIT_METRIC:
	    return "km/h";
	case UNIT_IMPERIAL_US: /* FALLTHROUGH */
	case UNIT_IMPERIAL_UK:
	    return "mph";
	default:
	    break;
    }
    return "";
}

const char *Display::getTemperatureUnit() const {
    switch (_unitMode) {
	case UNIT_METRIC:
	case UNIT_IMPERIAL_UK: /* FALLTHROUGH */
	    return "C";
	case UNIT_IMPERIAL_US: 
	    return "F";
	default:
	    break;
    }
    return "";
}

const char *Display::getVolume() const {
    switch (_unitMode) {
	case UNIT_METRIC:
	    return "l";
	case UNIT_IMPERIAL_US: 
	    return "gal (US)";
	case UNIT_IMPERIAL_UK:
	    return "gal (UK)";
	default:
	    break;
    }
    return "";
}

