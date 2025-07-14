/* \file windowsSystem.hpp */
#pragma once
#include <functional>

/** \class WindowsSystem
 * \brief Provides a templated constructor, destructor pair allowing a range of windows system to be implemented 
 */

template <typename Ctor, typename Dtor>
class WindowsSystem
{
public:
	WindowsSystem() { Ctor{}(); } //!< Default ctor which runs the function passed as the Ctor argument
	~WindowsSystem() { Dtor{}(); } //!< Default dtor which runs the function passed as the Dtor argument
	WindowsSystem(const WindowsSystem& other) = delete; //!< Copy constructor deleted
	WindowsSystem(WindowsSystem&& other) = delete;  //!< Move constructor deleted
	WindowsSystem& operator=(const WindowsSystem& other) = delete; //!< Copy assignment operator deleted
	WindowsSystem& operator=(WindowsSystem&& other) = delete; //!< Move assignment operator deleted
};

