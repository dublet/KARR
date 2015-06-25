#include "SimpleTextDisplay.h"

#include <GL/glut.h>

#include <sstream>
#include <iostream>

#include "GLUtil.h"
#include "Status.h"

using namespace KARR;
using namespace std;

SimpleTextDisplay::SimpleTextDisplay() {
}

SimpleTextDisplay::~SimpleTextDisplay() {

}

void SimpleTextDisplay::draw() {
    GLfloat overcolor[4] = {1, 1, 1, 1};
    glColor4fv(overcolor);
    Status &s = Status::instance();
    {
	stringstream ss;

	ss << s.getRpm() << "rpm";
	GLUtil::drawString(100, 100, ss.str(), true);
    }
    {
	stringstream ss;

	ss << s.getSpeed() << getSpeedUnit();
	GLUtil::drawString(200, 100, ss.str(), true);
    }
}

