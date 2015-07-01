#include "SimpleTextDisplay.h"

#include <GL/glut.h>
#include <vg/openvg.h>

#include <sstream>
#include <iostream>

#include "VGPP/Path.h"
#include "GLUtil.h"
#include "Status.h"

using namespace KARR;
using namespace std;

static const VGfloat test_floats[]={
    -100.0f ,-100.0f, 
    100.0f, -100.0f, 
    100.0f, 100.0f, 
    100.0f, -100.0f,
    0.0f, 0.0f
};

static const VGubyte test_cmds[]={
    VG_MOVE_TO | VG_ABSOLUTE, VG_LINE_TO | VG_ABSOLUTE, VG_LINE_TO | VG_ABSOLUTE, VG_LINE_TO | VG_ABSOLUTE, VG_CLOSE_PATH
};

SimpleTextDisplay::SimpleTextDisplay() {
}

SimpleTextDisplay::~SimpleTextDisplay() {

}

void SimpleTextDisplay::draw() {
    VGPP::Path p(4, test_cmds, test_floats);
    p.getMatrix().translate(300, 300);
    //VGPP::Path p(sizeof(test_cmds) / sizeof(VGubyte), test_cmds, test_floats);
    p.setFillColour(0xffffffff);
    p.setStrokeColour(0xffffffff);
    p.draw();
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

