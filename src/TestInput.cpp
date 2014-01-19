#include "TestInput.h"

#include <thread>

#include <time.h>

#include "Status.h"
#include "Display.h"
#include "DisplayManager.h"
#include "CarDefinition.h"

using namespace KARR;

void generateTestData() {
    Status &s = Status::instance();

    struct timespec sleepTime;
    sleepTime.tv_sec = 0;
    sleepTime.tv_nsec = 40 * 1000; 

    int rpmDirection = 1;
    int speedDirection = 1;
    for (;;) {
	const Display *d = DisplayManager::instance().getCurrent();
	if (s.getRpm() >= CarDefinition::revs.max)
	    rpmDirection = -1;
	if (s.getRpm() == CarDefinition::revs.min)
	    rpmDirection = 1;

	if (s.getSpeed() >= CarDefinition::speed.max)
	    speedDirection = -1;
	if (s.getSpeed() == CarDefinition::speed.min)
	    speedDirection = 1;

	s.setRpm(s.getRpm() + rpmDirection);
	s.setSpeed(s.getSpeed() + speedDirection);

	nanosleep(&sleepTime, NULL);
    }


}

void TestInput::run() {
    std::thread tt(generateTestData);
    tt.detach();
}
