#ifndef HAVE_KARR_SVGSHAPE_H
#define HAVE_KARR_SVGSHAPE_H

#include <boost/filesystem/path.hpp>

struct NSVGimage;

class SVGImage {
	NSVGimage *_nanoSvgImage;
    public:
	SVGImage();
	SVGImage(const boost::filesystem::path &);
	~SVGImage();
	void open(const boost::filesystem::path &);

};

#endif
