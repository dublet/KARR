#ifndef HAVE_KARR_CONVERTOR_H
#define HAVE_KARR_CONVERTOR_H

namespace KARR {
    namespace Convertor {
	inline float kmphToMph(float kmph) { return kmph / 1.609344f; }
	inline float celciusToFahrenheit(float c) { return c * 9f / 5f + 32f; }
    };
}
#endif
