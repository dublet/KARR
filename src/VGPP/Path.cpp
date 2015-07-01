#include "Path.h"

#include <iostream>

using namespace VGPP;

Path::Path() {
    init();
}

Path::Path(int numSegments, const VGubyte *segments, const VGfloat *pathData) {
    init();
    setData(numSegments, segments, pathData);
}

Path::~Path() {
    vgDestroyPaint(_fillColour);
    vgDestroyPaint(_strokeColour);
    vgDestroyPath(_path);
}

void Path::init() {
    _path = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F,
	    1,0,0,0, VG_PATH_CAPABILITY_ALL);

    _strokeColour = vgCreatePaint();
    _fillColour = vgCreatePaint();
}

void Path::draw(VGbitfield paintMode) const {
    vgSeti(VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE);
    vgLoadMatrix(_matrix.getRawMatrix());
    if ((paintMode & VG_FILL_PATH) != 0) {
	vgSetPaint(_fillColour, paintMode);
    } 
    if ((paintMode & VG_STROKE_PATH) != 0) {
	vgSetPaint(_strokeColour, paintMode);
    }

    vgDrawPath(_path, paintMode);
}

void Path::setData(int numSegments, const VGubyte *segments, const VGfloat *pathData) {
    vgAppendPathData(_path, numSegments, segments, pathData);
    VGfloat min_x, min_y, width, height;
    vgPathBounds(_path, &min_x, &min_y, &width, &height);
    fprintf(stderr, "x, y= (%f, %f) w, h = (%f, %f)\n", min_x, min_y, width, height);
}

void Path::setStrokeColour(const VGuint newColour) {
#ifdef vgSetColor
    vgSetColor(_strokeColour, newColour);
#else
    VGfloat rgba[4];
    rgba[0] = ((newColour >> 24) & 0xff) / 255.0f;
    rgba[1] = ((newColour >> 16) & 0xff) / 255.0f;
    rgba[2] = ((newColour >>  8) & 0xff) / 255.0f;
    rgba[3] = (newColour & 0xff) / 255.0f;
    vgSetParameterfv(_strokeColour, VG_PAINT_COLOR, 4, rgba);
#endif
}

void Path::setFillColour(const VGuint newColour) {
#ifdef vgSetColor
    vgSetColor(_fillColour, newColour);
#else
    VGfloat rgba[4];
    rgba[0] = ((newColour >> 24) & 0xff) / 255.0f;
    rgba[1] = ((newColour >> 16) & 0xff) / 255.0f;
    rgba[2] = ((newColour >>  8) & 0xff) / 255.0f;
    rgba[3] = (newColour & 0xff) / 255.0f;
    vgSetParameterfv(_fillColour, VG_PAINT_COLOR, 4, rgba);
#endif
}

Matrix &Path::getMatrix() {
    return _matrix;
}

const Matrix &Path::getMatrix() const {
    return _matrix;
}

const VGfloat Path::getX() const {
    VGfloat min_x, min_y, width, height;
    vgPathBounds(_path, &min_x, &min_y, &width, &height);
    return min_x;
}

const VGfloat Path::getY() const {
    VGfloat min_x, min_y, width, height;
    vgPathBounds(_path, &min_x, &min_y, &width, &height);
    return min_y;
}

const VGfloat Path::getWidth() const {
    VGfloat min_x, min_y, width, height;
    vgPathBounds(_path, &min_x, &min_y, &width, &height);
    return width;
}

const VGfloat Path::getHeight() const {
    VGfloat min_x, min_y, width, height;
    vgPathBounds(_path, &min_x, &min_y, &width, &height);
    return height;
}
