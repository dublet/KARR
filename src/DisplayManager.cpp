#include "DisplayManager.h"

#include <assert.h>

using namespace KARR;

DisplayManager DisplayManager::_instance = DisplayManager();

DisplayManager::DisplayManager() {
    _current = 0;
}

DisplayManager &DisplayManager::instance() {
    return _instance;
}

void DisplayManager::registerDisplay(const Display *d) {
    _displays.push_back(d);
}

const Display *DisplayManager::getNext() {
    if (_current++ > (_displays.size() - 1))
	_current = 0;
    return _displays[_current];
}

const Display *DisplayManager::getCurrent() const {
    assert(!_displays.empty());
    return _displays[_current];
}
