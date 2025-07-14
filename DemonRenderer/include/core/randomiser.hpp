/** \file randomiser.h */
#pragma once

#include <random>
#include <chrono>

class Randomiser
{
public:
	static int32_t uniformIntBetween(int32_t lower, int32_t upper); //!< Get an integer i such that lower <= i <= upper
	static float uniformFloatBetween(float lower, float upper); //!< Get a float i such that lower <= i <= upper
	static int32_t normalInt(float c, float sigma); //!< Get a int from the Gaussian distribution described by C and Sigma
	static float normalFloat(float c, float sigma); //!< Get a float from the Gaussian distribution described by C and Sigma
private:
	/** Random number engine */
	inline static std::mt19937 s_generator { std::mt19937(std::chrono::high_resolution_clock::now().time_since_epoch().count()) };
	/** Uniform interger distribution */
	inline static std::uniform_int_distribution<int32_t> s_uniformInt { std::uniform_int_distribution<int32_t>(std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max()) }; 
	/** Uniform float distribution */
	inline static std::uniform_real_distribution<float> s_uniformFloat { std::uniform_real_distribution<float>(std::numeric_limits<float>::min(), std::numeric_limits<float>::max()) };
	/** Range of int32_t */
	inline static float s_intRange { static_cast<float>((std::numeric_limits<int32_t>::max() - (float)std::numeric_limits<int32_t>::min())) };
	/** Range of 32bit float */
	inline static float s_floatRange { std::numeric_limits<float>::max() - std::numeric_limits<float>::min() };
};
