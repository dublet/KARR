#include "CarDefinition.h"

using namespace KARR;

MinMaxPair<int> CarDefinition::revs = MinMaxPair<int>(0, 7200);
MinMaxPair<short> CarDefinition::speed = MinMaxPair<short>(0, 240);
MinMaxPair<short> CarDefinition::engineTemp = MinMaxPair<short>(0, 120);
MinMaxPair<int> CarDefinition::fuelLevel = MinMaxPair<int>(0,  55);
