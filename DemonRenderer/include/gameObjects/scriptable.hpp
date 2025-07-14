/** \file Actor.hpp*/
#pragma once

#include "events/keyEvents.hpp"
#include <memory>

class Actor; // Forward declaration

/**	\class Script
*	\brief A class which exhibits runtime behavoiur.
*	Script class must be attached to Actoirs via a Scriptable component
*/

class Script
{
public:
	explicit Script(Actor& actor) : m_actor(actor) {} //!< Constructor which takes a Actor which the script is attached to
	virtual void onUpdate(float timestep) = 0; //!< Runs on update
	virtual void onImGuiRender() {}; //!< Runs on ImGuiRender
	virtual void onKeyPress(KeyPressedEvent& e) = 0; //!< Runs on key press
	virtual void onKeyRelease(KeyReleasedEvent& e) = 0; //!< Runs on key release
protected:
	Actor& m_actor; //!< Actor which the script is attached to
};

/** \class Scriptable
*	\brief Class which attaches a script to an actor
*/

class Scriptable
{
public:
	Scriptable() = default; //!< Default constructor
	/** Copy constructor which require that the script has no been attached*/
	Scriptable(const Scriptable& other)
	{
		if (other.m_script) spdlog::error("Cannot copy a Scriptable thing once a script has been attached");
	}
	/** Runs on update*/
	void onUpdate(float timestep) { if (m_script) m_script->onUpdate(timestep); }
	/** Runs on ImGuiRender */
	void onImGuiRender() { if (m_script) m_script->onImGuiRender(); }
	/** Runs on key press*/
	void onKeyPress(KeyPressedEvent& e) { if (m_script) m_script->onKeyPress(e); }
	/** Runs on key release*/
	void onKeyRelease(KeyReleasedEvent& e) { if (m_script) m_script->onKeyRelease(e); }
	/** Method which attaches the Script subclass to the the actor */
	template <typename T, typename ...Args> void attachScript(Args&& ...args)
	{
		T* ptr = new T(std::forward<Args>(args)...);
		m_script.reset(static_cast<Script*>(ptr));
	}
private:
	std::unique_ptr<Script> m_script{ nullptr }; //!< Script subclass
};