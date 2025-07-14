#pragma once
#include <DemonRenderer.hpp>
#include <vector>
class Lab2 : public Layer
{
public:
	Lab2(GLFWWindowImpl& win);
private:
	//Rendering
	void onRender() const override;

	//Update
	void onUpdate(float timestep) override;

	//Input
	void onKeyPressed(KeyPressedEvent& e) override;

	//GUI
	void onImGUIRender() override;

	//Setup Actor
	ShaderDescription CreateShaderDesc(ShaderType type, std::string vertexPath, std::string fragPath);
	std::shared_ptr<Shader> CreateShader(ShaderDescription shaderDesc);
	std::shared_ptr<VAO> CreateVAO(std::vector<unsigned int> indicies, std::vector<float> vertices, VBOLayout layout);
	Actor CreateActor(std::shared_ptr<VAO> VAO, std::shared_ptr<Material> material, glm::vec3 translation = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f));
	Actor CreateFloor(ShaderDescription shaderDesc);
	Actor CreateCharacter(ShaderDescription shaderDesc);
	Actor CreateSkybox(ShaderDescription shaderDesc);
	Actor CreateScreenQuad(ShaderDescription shaderDesc, float screenWidth, float screenHeight, RenderPass mainPass);
	int AddActorToRender(Actor actor);
	std::shared_ptr<Material> CreateScreenQuadMaterial(ShaderDescription shaderDesc);
private:

	//Scenes
	std::shared_ptr<Scene> m_lab2Scene;
	std::shared_ptr<Scene> m_screenScene;

	//Main Renderer
	Renderer m_lab2Renderer;

	//Actor positions for ease of use and avoiding magic numbers
	size_t cameraIdx = 0, floorIdx = 0, charIdx = 0, skyIdx = 0;

	//Render pass indexes
	size_t mainPassIdx = 0, invertPassIdx = 0, luminancePassIdx = 0, matrixPassIdx = 0, blurPassIdx = 0, sepiaPassIdx = 0, edgeDetectionPassIdx = 0, vignettePassIdx = 0, screenPassIdx = 0;

	//GUI
	glm::vec3 m_floorColour{ 1.0 };
	glm::vec3 m_dlColour{ 1.0 };
	glm::vec3 m_dlDirection{ 1.0 };
	bool m_wireFrame = false;
	float m_tonemappingMode = 0;
	float m_pointLightsAmbient = 0;
	float m_pointLightsShine = 64;
	glm::vec3 m_pLightConstants = { 1.0f, 0.0f, 0.044f };
	float m_invertColours = 0;
	float m_relativeLuminance = 0;
	float m_matrixColours = 0;
	float m_blur = 0;
	float m_blurRadius = 1;
	float m_vignette = 0;
	glm::vec2 m_vignetteControls = glm::vec2(0.0, 0.0);
	float m_sepia = 0;
	float m_edgeDetection = 0;

	//Point lights
	uint32_t numPointLights = 50;

	//Handle previous pass
	RenderPass previousPass;

	//Skybox
	std::vector<float> skyboxVertices = {
		// positions          
		-100.f,  100.f, -100.f,
		-100.f, -100.f, -100.f,
		 100.f, -100.f, -100.f,
		 100.f, -100.f, -100.f,
		 100.f,  100.f, -100.f,
		-100.f,  100.f, -100.f,

		-100.f, -100.f,  100.f,
		-100.f, -100.f, -100.f,
		-100.f,  100.f, -100.f,
		-100.f,  100.f, -100.f,
		-100.f,  100.f,  100.f,
		-100.f, -100.f,  100.f,

		 100.f, -100.f, -100.f,
		 100.f, -100.f,  100.f,
		 100.f,  100.f,  100.f,
		 100.f,  100.f,  100.f,
		 100.f,  100.f, -100.f,
		 100.f, -100.f, -100.f,

		-100.f, -100.f,  100.f,
		-100.f,  100.f,  100.f,
		 100.f,  100.f,  100.f,
		 100.f,  100.f,  100.f,
		 100.f, -100.f,  100.f,
		-100.f, -100.f,  100.f,

		-100.f,  100.f, -100.f,
		 100.f,  100.f, -100.f,
		 100.f,  100.f,  100.f,
		 100.f,  100.f,  100.f,
		-100.f,  100.f,  100.f,
		-100.f,  100.f, -100.f,

		-100.f, -100.f, -100.f,
		-100.f, -100.f,  100.f,
		 100.f, -100.f, -100.f,
		 100.f, -100.f, -100.f,
		-100.f, -100.f,  100.f,
		 100.f, -100.f,  100.f
	};
};