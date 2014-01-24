#include "SVGReader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include <assert.h>
#include <string.h>

#include <vg/openvg.h>

using namespace KARR;
using namespace VGPP;
using namespace std;

enum PathCommand {
    PCMD_ABS_MOVE,
    PCMD_REL_MOVE,
    PCMD_CLOSE,
    PCMD_ABS_LINE,
    PCMD_REL_LINE,
    PCMD_ABS_CURVETO,
    PCMD_REL_CURVETO,
    PCMD_ABS_SMOOTHCURVETO,
    PCMD_REL_SMOOTHCURVETO,
    PCMD_FINISHED,
    PCMD_NOT_CMD
};

enum PathCommand getPathCommand(const char c) {
    switch (c) {
	case 'm': return PCMD_REL_MOVE;
	case 'M': return PCMD_ABS_MOVE;
	case 'l': return PCMD_REL_LINE;
	case 'L': return PCMD_ABS_LINE;
	case 'c': return PCMD_REL_CURVETO;
	case 'C': return PCMD_ABS_CURVETO;
	case 'z': /* FALLTHROUGH */
	case 'Z': return PCMD_CLOSE;
	case '"': return PCMD_FINISHED;
	default:
		  break;
    }
    return PCMD_NOT_CMD;
}

bool isCmdNext(istringstream &iss) {
    iss.get(); // Ignore space;
    char next = iss.peek();
    return (getPathCommand(next) != PCMD_NOT_CMD);
}

/*
 * Draw a line from the current point to the given (x,y) coordinate which 
 * becomes the new current point. L (uppercase) indicates that absolute 
 * coordinates will follow; l (lowercase) indicates that relative coordinates 
 * will follow. A number of coordinates pairs may be specified to draw a 
 * polyline. At the end of the command, the new current point is set to the 
 * final set of coordinates provided.
 */
void readLineTo(istringstream &iss, std::vector<VGubyte> &commands, std::vector<VGfloat> &coords, bool isAbs) {
    do {
	VGfloat x, y;
	iss >> x; iss.get(); // Ignore comma
	iss >> y;
	cerr << "line to " << x << ", " << y<< endl;
	commands.push_back(VG_LINE_TO | isAbs ? VG_ABSOLUTE : VG_RELATIVE);
	coords.push_back(x); 
	coords.push_back(y);
    } while (!isCmdNext(iss));
}

/* 
 * Start a new sub-path at the given (x,y) coordinate. M (uppercase) indicates 
 * that absolute coordinates will follow; m (lowercase) indicates that 
 * relative coordinates will follow. If a moveto is followed by multiple pairs 
 * of coordinates, the subsequent pairs are treated as implicit lineto 
 * commands. Hence, implicit lineto commands will be relative if the moveto is 
 * relative, and absolute if the moveto is absolute. If a relative moveto (m) 
 * appears as the first element of the path, then it is treated as a pair of 
 * absolute coordinates. In this case, subsequent pairs of coordinates are 
 * treated as relative even though the initial moveto is interpreted as an 
 * absolute moveto. 
 */
void readMoveTo(istringstream &iss, std::vector<VGubyte> &commands, std::vector<VGfloat> &coords, bool isAbs) {
    VGfloat x, y;
    iss >> x; iss.get(); // Ignore comma
    iss >> y;
    cerr << "moving to " << x << ", " << y<< endl;
    commands.push_back(VG_MOVE_TO | isAbs ? VG_ABSOLUTE : VG_RELATIVE);
    coords.push_back(x); 
    coords.push_back(y);

    if (!isCmdNext(iss)) 
	readLineTo(iss, commands, coords, isAbs);
}

/*
 * Draws a cubic Bézier curve from the current point to (x,y) using (x1,y1) as 
 * the control point at the beginning of the curve and (x2,y2) as the control 
 * point at the end of the curve. C (uppercase) indicates that absolute 
 * coordinates will follow; c (lowercase) indicates that relative coordinates 
 * will follow. Multiple sets of coordinates may be specified to draw a 
 * polybézier. At the end of the command, the new current point becomes the 
 * final (x,y) coordinate pair used in the polybézier.
 */
void readCubicTo(istringstream &iss, std::vector<VGubyte> &commands, std::vector<VGfloat> &coords, bool isAbs) {
    do {
	VGfloat x1, y1, x2, y2, x, y;
	iss >> x1; iss.get(); // Ignore comma
	iss >> y1; iss.get(); // Ignore space
	iss >> x2; iss.get(); // Ignore comma
	iss >> y2; iss.get(); // Ignore space
	iss >> x;  iss.get(); // Ignore comma
	iss >> y;
	cerr << "cubic from " << x << ", " << y << " to " << x1 << ", " << y1 << " " << x2 <<", " << y2<< endl;
	commands.push_back(VG_CUBIC_TO | isAbs ? VG_ABSOLUTE : VG_RELATIVE);
	coords.push_back(x1); coords.push_back(y1);
	coords.push_back(x2); coords.push_back(y2);
	coords.push_back(x);  coords.push_back(y);
    } while (!isCmdNext(iss));
}

Path SVGReader::readPath(const string &filename) {
    ifstream input(filename, ios::in | ios::ate);
    assert(input.is_open());

    streampos size = input.tellg();
    char *fileContents = new char[size + streampos(1)];
    fileContents[size] = '\0'; // Ensure null termination.
    input.seekg(0, ios::beg);
    input.read(fileContents, size);
    input.close();

    Path returnPath;
    char *pathStr;
    if ((pathStr = strstr(fileContents, "<path")) != NULL) {
	cerr << "Found path" << endl;
	char *pathDataStr;
	if ((pathDataStr = strstr(pathStr, "d=")) != NULL) {
	    cerr << "Found path data" << endl;
	    pathDataStr += 3; // Ignore the d="

	    istringstream iss(pathDataStr);
	    std::vector<VGubyte> commands;
	    std::vector<VGfloat> coords;
	    char cmdchar;
	    bool done = false;
	    do {
		iss >> cmdchar;
		enum PathCommand cmd = getPathCommand(cmdchar);
		switch (cmd) {
		    case PCMD_CLOSE:
			cerr << "close" << endl;
			commands.push_back(VG_MOVE_TO);
			coords.push_back(0);
			coords.push_back(0);
			break;
		    case PCMD_REL_MOVE: readMoveTo(iss, commands, coords, false); break;
		    case PCMD_ABS_MOVE: readMoveTo(iss, commands, coords, true); break;
		    case PCMD_REL_LINE: /* FALLTHROUGH */
		    case PCMD_ABS_LINE:
			readLineTo(iss, commands, coords, (cmd == PCMD_ABS_LINE));
			break;
		    case PCMD_ABS_CURVETO:
		    case PCMD_REL_CURVETO:
			readLineTo(iss, commands, coords, (cmd == PCMD_ABS_CURVETO));
			break;
		    case PCMD_NOT_CMD: /* FALLTHROUGH */
			cerr << "Aborting, unknown path command " << cmdchar << endl;
		    case PCMD_FINISHED:
		    default:
			done = true;
			break;
		}
	    } while (iss.good() && !done);
	    if (commands.size() > 1) {
		commands.pop_back();
		coords.pop_back();
		coords.pop_back();
		commands.push_back(VG_CLOSE_PATH);
		VGubyte *cmdData = new VGubyte[commands.size()];
		VGfloat *floatData = new VGfloat[coords.size()];
		memcpy(cmdData, &commands[0], commands.size() * sizeof(VGubyte));
		memcpy(floatData, &coords[0], coords.size() * sizeof(VGfloat));

		// TODO make path take ownership.
		returnPath.setData(commands.size(), cmdData, floatData);
	    }
	} else {
	    cerr << "Could not find path data" << endl;
	}
    } else {
	cerr << "Could not find path" << endl;
    }


    delete[] fileContents;
    return returnPath;
}
