
#include "CelicaDisplay.h"

#include <GL/glut.h>
#include <vg/openvg.h>

#include <sstream>
#include <iostream>

#include "CarDefinition.h"
#include "Status.h"
#include "SVGReader.h"

using namespace KARR;
using namespace std;

static const VGfloat test_floats[]={
    -100.0f ,-100.0f, 
    100.0f, -100.0f, 
    100.0f, 100.0f, 
    100.0f, -100.0f
};

static const VGubyte test_cmds[]={
    VG_MOVE_TO, VG_LINE_TO, VG_LINE_TO, VG_LINE_TO, VG_CLOSE_PATH
};

#include "Celica205Paths.C"

Celica205Display::Celica205Display() :
    _speedNeedle(sizeof(speed_needle2_path_cmds) / sizeof(VGubyte),
	    speed_needle2_path_cmds, speed_needle2_path_floats),
    _speedTicks(sizeof(speed_ticks_cmds) / sizeof(VGubyte), 
	    speed_ticks_cmds, speed_ticks_floats),
    //_parkingBrakeIcon(SVGReader::readPath("../svg/parking_brake.svg")),
    _mainBeamIcon(SVGReader::readPath("../svg/main_beam.svg"))
   , _testPath(sizeof(test_cmds), test_cmds, test_floats)
{
    _speedTicks.getMatrix().translate(1024 / 2 - 150 , 300);
    _speedTicks.setFillColour(0xffffffff);
    _speedNeedle.setFillColour(0xffffffff);
    _mainBeamIcon.setFillColour(0xffffffff);
    _testPath.setFillColour(0xffffffff);
    _testPath.getMatrix().translate(612, 300);
    _mainBeamIcon.getMatrix().translate( 312 , 300);
    //_parkingBrakeIcon.getMatrix().translate(-100, 30);
}

Celica205Display::~Celica205Display() {

}

void Celica205Display::draw() {
    Status &s = Status::instance();

    const DynamicCarDefinition &dcc = DynamicCarDefinition::GetCarDefinition();

#if 0
    { /* Speedo ticks */
	_speedTicks.draw();
    }
#endif
    { /* Speedo needle */
	float speedNeedleAngle = s.getSpeed() / float(dcc.speed.max) * 6 + 60;
	VGPP::Matrix &m = _speedNeedle.getMatrix();
	m.makeIdentity();
	m.translate((1024 - _speedNeedle.getWidth()) / 2, 600 - _speedNeedle.getHeight() + 50);
	m.rotate(speedNeedleAngle);
	m.translate(0, _speedNeedle.getHeight() - 50);
	_speedNeedle.draw();
    }
#if 0
    {
	const int onColour = 0xff0000ff;
	//const int offColour = 0x333333ff;
	_parkingBrakeIcon.getMatrix().makeIdentity();
	_parkingBrakeIcon.getMatrix().translate(100, 800);
	_parkingBrakeIcon.getMatrix().scale(-0.5, 0.5);
	_parkingBrakeIcon.setFillColour(onColour);
	_parkingBrakeIcon.draw();

    }
#endif
	_mainBeamIcon.draw();
    _testPath.draw();
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

