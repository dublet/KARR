#include "SVGImage.h"

#define NANOSVG_IMPLEMENTATION 
#include <nanosvg.h>
#include <nanovg.h>
// XXX This is bad, but easy.
#define NANOVG_GL_IMPLEMENTATION
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>
#include <nanovg_gl.h>
#include <nanovg.c>


SVGImage::SVGImage() : _nanoSvgImage(nullptr) {
}

SVGImage::SVGImage(const boost::filesystem::path &path) : SVGImage() {
    open(path);
}

SVGImage::~SVGImage() {
    if (_nanoSvgImage) {
	::nsvgDelete(_nanoSvgImage);
    }
    _nanoSvgImage = nullptr;
}

bool SVGImage::open(const boost::filesystem::path &path) {
    _nanoSvgImage = ::nsvgParseFromFile(path.string().c_str(), "", 72.f);
    return (_nanoSvgImage->width > 0.0f && _nanoSvgImage->height > 0.0 && _nanoSvgImage->shapes);
}

void SVGImage::draw() {
}
