#ifndef HAVE_VGPP_PATH_H
#define HAVE_VGPP_PATH_H

#include <vg/openvg.h>

#include "Matrix.h"

namespace VGPP {
    class Path {
	private:
	    VGPaint _strokeColour, _fillColour;
	    VGPath _path;
	    Matrix _matrix;
	    void init();
	public:
	    Path();
	    Path(int numSegments, const VGubyte *, const VGfloat *);
	    ~Path();
	    void draw(VGbitfield paintMode = VG_FILL_PATH) const;
	    void setData(int numSegments, const VGubyte *, const VGfloat *);
	    void setStrokeColour(const VGuint newColour);
	    void setFillColour(const VGuint newColour);
	    Matrix &getMatrix();
	    const Matrix &getMatrix() const;
    };
}

#endif
