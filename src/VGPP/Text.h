#ifndef HAVE_VGPP_TEXT_H
#define HAVE_VGPP_TEXT_H

#include <vg/openvg.h>
#include <string>

#include "Font.h"

namespace VGPP {
    class Text {
	private:
	    Font _font;
	public:
	    Text();
	    ~Text();
	    void render(int x, int y, std::string &text);
	    void render(int x, int y, const char *text, std::size_t len);
    };
}

#endif
