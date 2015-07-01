#include "Text.h"

using namespace VGPP;

const VGuint maxBufferSize = 1024;
VGuint buffer[maxBufferSize];

Text::Text() { 
}

Text::~Text() { 
}

void Text::render(int x, int y, std::string &text) {
    render(x, y, text.c_str(), text.size());
}

void Text::render(int x, int y, const char *text, size_t len) {
    VGuint /*glyphCount = len,*/ i;
    for (i = 0; i < len; ++i) {
	//buffer[i] = glyphIndexForChar(text[i]);
    }
    buffer[i] = '\0';

    //XXX not in ShivaVG: :( vgDrawGlyphs(_font, buffer, NULL, NULL, VG_FILL_PATH, VG_FALSE);
}
