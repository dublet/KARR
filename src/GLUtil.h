#ifndef HAVE_KARR_GLUTIL_H
#define HAVE_KARR_GLUTIL_H

#include <string>

namespace KARR {
    namespace GLUtil {
	void drawString(float x, float y, const std::string &s, bool alignRight = false);
    }
}

#endif
