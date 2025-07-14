/** \file timer.cpp */

#include "core/timer.hpp"

float Timer::reset() noexcept
{
	m_end = std::chrono::high_resolution_clock::now();
	m_durationSeconds = m_end - m_start;
	m_start = std::chrono::high_resolution_clock::now();
	return m_durationSeconds.count();
}

int64_t Timer::resetMs() noexcept
{
	m_end = std::chrono::high_resolution_clock::now();
	m_durationSeconds = m_end - m_start;
	m_durationMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(m_durationSeconds);
	m_start = std::chrono::high_resolution_clock::now();
	return m_durationMilliseconds.count();;
}
