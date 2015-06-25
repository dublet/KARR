
#include "CelicaDisplay.h"

#include <GL/glut.h>
#include <vg/openvg.h>

#include <sstream>
#include <iostream>

#include "CarDefinition.h"
#include "GLUtil.h"
#include "Status.h"

using namespace KARR;
using namespace std;

#include "Celica205Paths.C"

Celica205Display::Celica205Display() :
    _speedTicks(sizeof(speed_ticks_cmds) / sizeof(VGubyte), 
	    speed_ticks_cmds, speed_ticks_floats),
    _speedNeedle(sizeof(speed_needle2_path_cmds) / sizeof(VGubyte),
	    speed_needle2_path_cmds, speed_needle2_path_floats)
{
    _speedTicks.getMatrix().translate(1024 / 2 - 150 , 900);
    _speedTicks.setFillColour(0xffffffff);
    _speedNeedle.setFillColour(0xffffffff);
}

Celica205Display::~Celica205Display() {

}

void Celica205Display::draw() {
    Status &s = Status::instance();

    { /* Speedo ticks */
	_speedTicks.draw();
    }
    { /* Speedo needle */
	float speedNeedleAngle = s.getRpm() / float(CarDefinition::revs.max) * 360;
	VGPP::Matrix &m = const_cast<Celica205Display *>(this)->_speedNeedle.getMatrix();
	m.makeIdentity();
	m.translate(0, 600);
	m.rotate(speedNeedleAngle);
	_speedNeedle.draw();
#if 0
	vgLoadIdentity();
	vgSetParameterfv(needleFillColour, VG_PAINT_COLOR, 4, colour_white);

	float speedNeedleAngle = s.getRpm() / float(CarDefinition::revs.max) * 360;

	//vgScale(0.2, 0.2);
	//vgTranslate(640, 0);
	//vgSetf(VG_STROKE_LINE_WIDTH, 5);
	//vgDrawPath(speedNeedlePath, (VGint)speed_needle_style[9]);

	vgTranslate(0, 600);
	vgScale(.3, -0.3);
	vgRotate(speedNeedleAngle);

	vgDrawPath(speedNeedlePath, VG_FILL_PATH);
#endif
    }
#if 0

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
#endif
}

