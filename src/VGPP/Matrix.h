#ifndef HAVE_VGPP_MATRIX_H
#define HAVE_VGPP_MATRIX_H

#include <vg/openvg.h>

namespace VGPP {
    class Matrix {
	private:
	    /*! 
	     * For matrix:
	     * [ sx  shx tx
	     *   shy sy  ty
	     *   w0  w1  w2 ]
	     * Order is: 
             *   0   1    2   3    4   5   6   7   8
	     * [ sx, shy, w0, shx, sy, w1, tx, ty, w2 ]
	     */
	    VGfloat _m[9];
	public:
	    // New Matrix, set to identity matrix.
	    Matrix();
	    Matrix(const VGfloat *);
	    ~Matrix();

	    void makeIdentity();

	    void transpose();

	    const VGfloat *getRawMatrix() const { return &_m[0]; }

	    inline VGfloat &sx() { return _m[0]; }
	    inline VGfloat &sy() { return _m[4]; }
	    inline VGfloat &shx() { return _m[3]; }
	    inline VGfloat &shy() { return _m[1]; }
	    inline VGfloat &tx() { return _m[6]; }
	    inline VGfloat &ty() { return _m[7]; }
	    inline VGfloat &w0() { return _m[2]; }
	    inline VGfloat &w1() { return _m[5]; }
	    inline VGfloat &w2() { return _m[8]; }

	    void rotate(VGfloat degrees);
	    void translate(VGfloat tx, VGfloat ty);
	    void scale(VGfloat sx, VGfloat sy);
	    void shear(VGfloat shx, VGfloat shy);
	    Matrix &operator*=(const VGfloat *rhs);
	    Matrix &operator*=(const Matrix &rhs);
    };
}

#endif
