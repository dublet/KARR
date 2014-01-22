#ifndef HAVE_KARR_SVG_READER_H
#define HAVE_KARR_SVG_READER_H

#include "VGPP/Path.h"

#include <string>

namespace KARR {
    class SVGReader {
	public:
	static VGPP::Path readPath(const std::string &);

    };

}

#endif

