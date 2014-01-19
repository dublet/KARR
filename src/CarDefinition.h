#ifndef HAVE_KARR_CAR_DEFINITION_H
#define HAVE_KARR_CAR_DEFINITION_H

namespace KARR {

    template<typename T>
    struct MinMaxPair {
	T min, max;
	MinMaxPair(T min, T max) : min(min), max(max) { }
    };
    
    struct CarDefinition {
	static MinMaxPair<int> revs;
	static MinMaxPair<short> speed;
	static MinMaxPair<short> engineTemp;
	static MinMaxPair<int> fuelLevel;
    };
}
#endif
