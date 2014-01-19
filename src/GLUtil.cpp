#include "GLUtil.h"
#include <GL/glut.h>

using namespace std;

void KARR::GLUtil::drawString(float x, float y, const string &s, bool alignRight)
{
    float k = 0.15;

    x+=0.5f;
    y+=0.5f;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glDisable(GL_MULTISAMPLE);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(x,y,0);
    glScalef(k,k,k);
    glLineWidth(1.0f);

    const char *text = s.c_str();
    int len = s.size();

    if (alignRight) {
	glTranslatef(-glutStrokeLength(GLUT_STROKE_ROMAN, 
		    (const unsigned char *)text), 0, 0);
    }

    for (int i = 0; i < len; ++i) {
	if (text[i] == '\n') {
	    y -= 20;
	    glLoadIdentity();
	    glTranslatef(x, y, 0);
	    glScalef(k,k,k);
	    continue; 
	}
	glutStrokeCharacter(GLUT_STROKE_ROMAN, text[i]);
    }

    glPopMatrix();
    glDisable(GL_LINE_SMOOTH);
}
