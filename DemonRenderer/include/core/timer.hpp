/** \file timer.hpp */

#pragma once
#include <chrono>

/** \class Timer
 *  \brief Provides a basic timer which give a time delta (elapsed time) when reset is called.  
 *  The time delta can be given in seconds as a float(32 bit) or milliseconds as an int (64 bit, signed from the std::chrono library).
 */

class Timer
{
public:
	[[nodiscard]] float reset() noexcept; //!< Resets timer and gives back time elapsed in seconds
	[[nodiscard]] int64_t resetMs() noexcept; //!< Resets the time and gives back time elapsed in milliseconds
private:
	std::chrono::high_resolution_clock::time_point m_start{ std::chrono::high_resolution_clock::now() }; //!< Start time point
	std::chrono::high_resolution_clock::time_point m_end{ std::chrono::high_resolution_clock::now() }; //!< End time point
	std::chrono::duration<float> m_durationSeconds{0.f}; //!< Float duration
	std::chrono::milliseconds m_durationMilliseconds{0}; //!< Uint32_t duration
};
