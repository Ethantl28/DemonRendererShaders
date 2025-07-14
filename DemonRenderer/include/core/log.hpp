/** \file log.hpp */
#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
// Includes for ostream custom type overload declarations
#include "events/event.hpp"
#include <glm/glm.hpp>

/** \class LogSystem
 *  \brief Initialises spdlog in a pretty way.
 */

class LogSystem 
{
public:
	LogSystem() noexcept;
};

/** \fn operator<<(std::ostream& os, const EventType& ev)
 *  \brief Classic ostream overloading for EventType
 *  \param os the output stream used by the logger.  This will be passed by spdlog.
 *  \param ev an event type which will be turned into streamed output.
 *  \return the processed ostream operator
 */
std::ostream& operator<<(std::ostream& os, const EventType& ev);

/** \fn operator<<(std::ostream& os, const glm::ivec2& iv2)
 *  \brief Classic ostream overloading for ivec2
 *  \param os the output stream used by the logger.  This will be passed by spdlog.
 *  \param iv2 an ivec2 which will be turned into streamed output.
 *  \return the processed ostream operator
 */
std::ostream& operator<<(std::ostream& os, const glm::ivec2& iv2);