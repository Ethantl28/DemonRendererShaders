/* \file window.hpp */
#pragma once
#include <memory>
#include <gsl/gsl>
#include "events/eventHandler.hpp"

/**	\struct WindowProperties
 *	\brief Holds core properties of a window
 */
struct WindowProperties
{
	const char* title{ "" }; //!< Title shown on menu bar
	float aspectRatio{ 0.f }; //!< Aspect ratio
	uint32_t width{ 0 }; //!< Width in pixels
	uint32_t height{ 0 }; //!< Height in pixels
	bool isFullScreen{ false }; //!< Should window be in full screen mode
	bool isResizable{ true }; //!< Can the window be resized?
	bool isVSync{ false }; //!< Is vsync on
	bool isHostingImGui{ false }; //!< Does this window host IMGUI

	WindowProperties() {} //!< Default constructor

	WindowProperties(const char* newTitle, uint32_t w = 800, uint32_t h = 800, bool fullScreen = false, bool resizable = true) //!< Typically used constructor
		: title{ newTitle }
		, width{ w }
		, height{ h }
		, aspectRatio{ static_cast<float>(w) / static_cast<float>(h) }
		, isFullScreen(fullScreen)
		, isResizable(resizable)
		, isVSync(true)
	{}


};

/**	\class IWindow
 *	\brief Interface class for a window implementation. Used Non-Virtual Iterface (NVI) idiom.  Template pattern is all used.
 */
template<typename Native, typename nativeDStor, typename GraphicsCtxt>
class IWindow
{
public:
	IWindow() = default; //!< Default contructor
	virtual ~IWindow() {}; //!< Virtual destructor
	void open(const WindowProperties& properties); //!< Open a window with properites
	void onUpdate(float timestep) noexcept; //!< Update window. Should be run every frame with the frametime. 
	void setVSync(bool VSync) noexcept { Expects(m_nativeWindow); m_isVSync = VSync; doSetVSync(VSync); }; //!< Set the Vsync of the window.
	[[nodiscard]] inline glm::ivec2 getSize() const noexcept { Expects(m_nativeWindow); return doGetSize(); } //!< Returns the windows width
	[[nodiscard]] inline glm::vec2 getSizef() const noexcept { auto s = getSize(); return glm::vec2(static_cast<float>(s.x), static_cast<float>(s.y)); } //!< Returns the windows width
	[[nodiscard]] inline uint32_t getWidth() const noexcept { return getSize().x; } //!< Returns the windows width
	[[nodiscard]] inline uint32_t  getHeight() const noexcept { return getSize().y; } //!< Returns the windows height
	[[nodiscard]] inline float getWidthf() const noexcept { return static_cast<float>(getWidth()); } //!< Returns the windows width as a float
	[[nodiscard]] inline float  getHeightf() const noexcept { return static_cast<float>(getHeight()); } //!< Returns the windows height as a float
	[[nodiscard]] inline bool isFullScreenMode() const noexcept { return m_isFullScreen; }  //!< Returns if the windows is in fullscreen mode or not
	[[nodiscard]] inline bool isVSync() const noexcept { return m_isVSync; } //!< Returns whether the window is Vsync or not
	[[nodiscard]] inline bool isHostingImGui() const noexcept { return m_isHostingImGui; } //!< Returns whether the window is capable of hosting any ImGui windows
	[[nodiscard]] bool isKeyPressed(int32_t keyCode) const { Expects(m_nativeWindow); return doIsKeyPressed(keyCode); } //!< is the key with library specfic keycode currently pressed?
	[[nodiscard]] bool isMouseButtonPressed(int32_t mouseButton) const { Expects(m_nativeWindow); return doIsMouseButtonPressed(mouseButton); } // is the mouse button with library specfic code currently pressed?
	[[nodiscard]] glm::vec2 getMousePosition() const { Expects(m_nativeWindow); return doGetMousePosition(); } //!< Returns the current mouse position
	EventHandler handler; //!< Event handler
protected:
	GraphicsCtxt m_graphicsContext; //!< Graphics context
	std::unique_ptr<Native, nativeDStor> m_nativeWindow{ nullptr }; //!< Native window
	bool m_isFullScreen{ false }; //!< Is this window in full screen mode?
	bool m_isVSync{ false };  //!< Is VSync enabled?
	bool m_isHostingImGui{ false };  //!< Is this window hosting an ImGui context
private:
	virtual void doOpen(const WindowProperties& properties) = 0; //!< Virtual function acts to open
	virtual void doOnUpdate(float timestep) = 0; //!< Virtual function acts on update
	virtual void doSetVSync(bool VSync) = 0; //!< Virtual function acts on set vsync
	[[nodiscard]] virtual bool doIsKeyPressed(int32_t keyCode) const = 0; //!< Virtual function acts on  is key pressed
	[[nodiscard]] virtual bool doIsMouseButtonPressed(int32_t mouseButton) const = 0; //!< Virtual function acts on is mouse button pressed
	[[nodiscard]] virtual glm::vec2 doGetMousePosition() const = 0; //!< Virtual function acts on get mouse position
	[[nodiscard]] virtual glm::ivec2 doGetSize() const = 0; //!< Virtual function acts on get the window size
};

template<typename Native, typename nativeDStor, typename GraphicsCtxt>
void IWindow<Native, nativeDStor, GraphicsCtxt>::open(const WindowProperties& properties) { 
	Expects(m_nativeWindow == nullptr); 
	m_isFullScreen = properties.isFullScreen; 
	m_isVSync = properties.isVSync;
	m_isHostingImGui = properties.isHostingImGui;
	doOpen(properties); 
}; //!< Open a window with properites

template<typename Native, typename nativeDStor, typename GraphicsCtxt>
void IWindow<Native, nativeDStor, GraphicsCtxt>::onUpdate(float timestep) noexcept {
	Expects(m_nativeWindow);
	if (isHostingImGui()) {
		ImGui::Render();
	}
	doOnUpdate(timestep);
}