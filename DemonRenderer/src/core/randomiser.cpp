/** \file randomiser.cpp */
#pragma once

#include "core/randomiser.hpp"

int32_t Randomiser::uniformIntBetween(int32_t lower, int32_t upper)
{
	float t = (static_cast<float>(s_uniformInt(s_generator)) / s_intRange) + 0.5f;
	float range = upper - lower;
	return lower + static_cast<int32_t>(t * range);
}

float Randomiser::uniformFloatBetween(float lower, float upper)
{
	float t = (s_uniformFloat(s_generator) / s_floatRange);
	float range = upper - lower;
	return lower + (t * range);
}

int32_t Randomiser::normalInt(float c, float sigma)
{
	std::normal_distribution<float> distribution(c, sigma);
	return static_cast<int32_t>(distribution(s_generator));
}

float Randomiser::normalFloat(float c, float sigma)
{
	std::normal_distribution<float> distribution(c, sigma);
	return distribution(s_generator);
}
