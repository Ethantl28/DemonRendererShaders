#pragma once
#include <DemonRenderer.hpp>
class StartPoint : public Layer
{
public:
	StartPoint(GLFWWindowImpl& win);
private:
	void onRender() const override;
	void onUpdate(float timestep) override;
	void onKeyPressed(KeyPressedEvent& e) override;
	void onImGUIRender() override;
private:
	std::shared_ptr<Scene> m_mainScene;

	Renderer m_mainRenderer;
	// Actor positions for ease of use and to avoid additonal magic numbers
	size_t modelIdx;
	size_t cameraIdx;

	//Gui
	glm::vec3 m_colour{ 1.0 };
};