#ifndef HAVE_KARR_DISPLAY_H
#define HAVE_KARR_DISPLAY_H

namespace KARR {
    class Display {
	public:
	    typedef enum {
		UNIT_METRIC,
		UNIT_IMPERIAL_US,
		UNIT_IMPERIAL_UK
	    } UnitMode_t;
	protected:
	    UnitMode_t _unitMode;

	    Display(const Display &rhs); // Disallow
	    Display & operator=(const Display &rhs); // Disallow
	public:
	    Display();
	    virtual ~Display();
	    virtual void draw() const = 0;

	    UnitMode_t getUnitMode() { return _unitMode; }
	    void setUnitMode(UnitMode_t unitMode) { _unitMode = unitMode; }

	    const char *getSpeedUnit() const;
	    const char *getTemperatureUnit() const;
	    const char *getVolume() const;
    };
}

#endif
