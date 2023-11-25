#include "Utility.h"

#include <cstdlib>

float RandValueInRange(float LowValue, float HighValue)
{
	return LowValue + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (HighValue - LowValue));
}

// Maps number from one range into the other
float MapNumber(float NumberToMap, float CurrentMinRange, float CurrentMaxRange, float TargetMinRange, float TargetMaxRange)
{
	return TargetMinRange + (TargetMaxRange - TargetMinRange) * ((NumberToMap - CurrentMinRange) / (CurrentMaxRange - CurrentMinRange));
}