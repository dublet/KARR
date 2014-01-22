#include "Font.h"

using namespace VGPP;

Font::Font() {
    _font = vgCreateFont();
}

Font::~Font() {
    vgDestroyFont(_font);
}
