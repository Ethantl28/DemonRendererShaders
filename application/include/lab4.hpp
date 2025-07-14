#pragma once
#include <DemonRenderer.hpp>
#include <vector>

class Lab4 : public Layer
{
public:
	Lab4(GLFWWindowImpl& win);
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
	Actor CreateActor(std::shared_ptr<VAO> Vao, std::shared_ptr<VAO> depthVao = nullptr, std::shared_ptr<Material> material = nullptr,
		std::shared_ptr<Material> depthMat = nullptr, glm::vec3 translation = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f));
	int AddActorToRender(Actor actor);
private:

	//Scenes
	std::shared_ptr<Scene> m_mainScene;
	std::shared_ptr<Scene> m_screenScene;
	std::shared_ptr<Scene> m_skyboxScene;

	//Main Renderer
	Renderer m_mainRenderer;

	//Actor positions for ease of use and avoiding magic numbers
	size_t cameraIdx = 0, floorIdx = 0, charIdx = 0;
	std::vector<size_t> characterIndexes;

	//Render pass indexes
	size_t mainPassIdx = 0,
		skyboxPassIdx = 0,
		lightPassIdx = 0,
		blurPassIdx = 0,
		dofBlurPassIdx = 0,
		dofPassIdx = 0,
		sepiaPassIdx = 0,
		edgeDetectionPassIdx = 0,
		vignettePassIdx = 0,
		fogPassIdx = 0,
		visualizeShadowIdx = 0,
		screenPassIdx = 0;

	//Depth pass indexes
	size_t shadowPassIdx = 0,
		prePassIdx = 0;

	//Shadowmap variables
	struct ShadowMapVars
	{
		glm::vec3 centre;
		float distanceAlongLightVector;
		float orthoSize;
		const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);

		ShadowMapVars(glm::vec3 c = glm::vec3(0.0f), float dist = 60.0f) : centre(c), distanceAlongLightVector(dist)
		{
			orthoSize = distanceAlongLightVector * 0.75f;
		};
	};
	ShadowMapVars m_shadowMapVars;

	//GUI
	glm::vec3 m_floorColour = glm::vec3(0.0f, 0.12f, 0.12f);
	glm::vec3 m_dlColour{ 1.0 };
	glm::vec3 m_dlDirection{ -0.2f, -0.75f, -0.5f };
	bool m_wireFrame = false;
	float m_tonemappingMode = 6;
	float m_pointLightsAmbient = 0;
	float m_pointLightsShine = 64;
	glm::vec3 m_pLightConstants = { 1.0f, 0.0f, 0.044f };
	float m_invertColours = 0;
	float m_relativeLuminance = 0;
	float m_matrixColours = 0;
	float m_blur = 0;
	float m_blurRadius = 1;
	float m_vignette = 0;
	glm::vec2 m_vignetteControls = glm::vec2(0.1, 0.7);
	float m_sepia = 0;
	float m_edgeDetection = 0;
	float m_fog = 0;
	glm::vec3 m_fogColour = glm::vec3(0.5, 0.0, 0.0);
	float m_fogDensity = 0.004f;
	float m_dof = 0;
	float m_focalDistance = 10.0f;
	float m_focalRange = 5.0f;

	//Point lights
	uint32_t numPointLights = 10;

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