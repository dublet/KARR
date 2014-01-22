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
    PCMD_MOVE,
    PCMD_CLOSE,
    PCMD_ABS_LINE,
    PCMD_REL_LINE,
    PCMD_ABS_CURVETO,
    PCMD_REL_CURVETO,
    PCMD_ABS_SMOOTHCURVETO,
    PCMD_REL_SMOOTHCURVETO,
    PCMD_NOT_CMD
};

enum PathCommand getPathCommand(const char c) {
    switch (c) {
	case 'm': /* FALLTHROUGH */
	case 'M': return PCMD_MOVE;
	case 'l': return PCMD_REL_LINE;
	case 'L': return PCMD_ABS_LINE;
	case 'c': return PCMD_REL_CURVETO;
	case 'C': return PCMD_ABS_CURVETO;
	case 'z': /* FALLTHROUGH */
	case 'Z': return PCMD_CLOSE;
	default:
		  break;
    }
    return PCMD_NOT_CMD;
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
	    pathDataStr += 3;

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
			commands.push_back(VG_CLOSE_PATH);
			break;
		    case PCMD_MOVE: 
			{
			    VGfloat x, y;
			    iss >> x;
			    iss.get(); // Ignore comma
			    iss >> y;
			    cerr << "moving to " << x << ", " << y<< endl;
			    commands.push_back(VG_MOVE_TO);
			    coords.push_back(x);
			    coords.push_back(y);
			} break;
		    case PCMD_REL_LINE: /* FALLTHROUGH */
		    case PCMD_ABS_LINE:
			{
			    VGfloat x, y;
			    iss >> x;
			    iss.get(); // Ignore comma
			    iss >> y;
			    cerr << "line to " << x << ", " << y<< endl;
			    commands.push_back(VG_LINE_TO | (cmd == PCMD_ABS_LINE) ? VG_ABSOLUTE : VG_RELATIVE);
			    coords.push_back(x);
			    coords.push_back(y);
			} break;
		    case PCMD_ABS_CURVETO:
		    case PCMD_REL_CURVETO:
			{
			    do {
				VGfloat x0, x1, y0, y1, x, y;
				iss >> x0;
				iss.get(); // Ignore comma
				iss >> y1;
				iss.get(); // Ignore space
				iss >> x1;
				iss.get(); // Ignore comma
				iss >> y1;
				iss.get(); // Ignore space
				iss >> x;
				iss.get(); // Ignore comma
				iss >> y;
				cerr << "cubic to " << x0 << ", " << y0 << " " << x1 << ", " << y1 << " " << x <<", " << y<< endl;
				commands.push_back(VG_CUBIC_TO | (cmd == PCMD_ABS_CURVETO) ? VG_ABSOLUTE : VG_RELATIVE);
				coords.push_back(x0);
				coords.push_back(y0);
				coords.push_back(x1);
				coords.push_back(y1);
				coords.push_back(x);
				coords.push_back(y);

				iss.get(); // Ignore space;

				char next = iss.peek();
				if (getPathCommand(next) != PCMD_NOT_CMD) {
				    break;
				}
			    } while (1);
			} break;
		    case PCMD_ABS_SMOOTHCURVETO:
			// TODO
			break;
		    case PCMD_REL_SMOOTHCURVETO:
			// TODO
			break;
		    case PCMD_NOT_CMD: /* FALLTHROUGH */
		    default:
			done = true;
			// TODO
			break;
		}
	    } while (iss.good() && !done);
	    if (commands.size() > 1) {
		VGubyte *cmdData = new VGubyte[commands.size()];
		VGfloat *floatData = new VGfloat[coords.size()];
		memcpy(cmdData, &commands[0], commands.size() * sizeof(VGfloat));
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
