#ifndef HAVE_KARR_DISPLAY_MANAGER_H
#define HAVE_KARR_DISPLAY_MANAGER_H

#include <vector>

namespace KARR {
    class Display; 

    class DisplayManager {
	private:
	    std::vector<const Display *> _displays;

	    int _current;

	    static DisplayManager _instance;

	    DisplayManager();
	    DisplayManager(const DisplayManager &rhs); // disallow
	public:
	    static DisplayManager &instance();

	    void registerDisplay(const Display *);

	    /**
	     * Get the next display, if there are any others. 
	     */
	    const Display *getNext();
	    /**
	     * Get the current display.
	     */
	    const Display *getCurrent() const;

    };
}
#endif
