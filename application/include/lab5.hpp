#pragma once
#include <DemonRenderer.hpp>
#include <vector>

class Lab5 : public Layer
{
public:
	Lab5(GLFWWindowImpl& win);
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
	Renderer m_initialRenderer;

	//Actor positions for ease of use and avoiding magic numbers
	size_t cameraIdx = 0, floorIdx = 0, charIdx = 0, treeIdx = 0, particlesIdx = 0;
	std::vector<size_t> characterIndexes;

	//Render pass indexes
	size_t mainPassIdx = 0,
		skyboxPassIdx = 0,
		lightPassIdx = 0,
		fogPassIdx = 0,
		visualizeShadowIdx = 0,
		ultraPPPassIdx =  0,
		dofPassIdx = 0,
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
	//Lights
	glm::vec3 m_floorColour = glm::vec3(0.0f, 0.12f, 0.12f);
	glm::vec3 m_dlColour{ 1.0 };
	glm::vec3 m_dlDirection{ -0.2f, -0.75f, -0.5f };
	glm::vec3 m_pLightConstants = { 20.0f, 0.0f, 0.044f };

	//Geometry
	bool m_wireFrame = false;
	float m_heightMapScale = 0.0f;

	//Post processing
	float m_tonemappingMode = 6;
	bool m_edgeDetection = false;
	bool m_blur = false;
	float m_blurRadius = 5.0f;
	bool m_vignette = false;
	glm::vec2 m_vignetteControls = { 0.0f, 0.7f };
	float m_vignetteIntensity = 0.99f;
	bool m_sepia = false;
	bool m_depthOfField = false;
	float m_dofBlurAmmount = 5.0f;
	float m_dofFocusDistance = 50.0f;
	float m_dofFocalRange = 10.0f;
	bool m_fog = false;
	glm::vec3 m_fogColour = glm::vec3(1.0);
	float m_fogDensity = 0.004f;

	//Compute shader uniforms
	float m_frequency = 10.0f;
	float m_amplitude = 1.5f;
	float m_octaves = 8.0f;
	float m_lacunarity = 2.0f;
	float m_persitance = 0.5f;
	bool m_snowActive = true;
	glm::vec3 m_particleColour{ 1.0 };

	//Particles
	struct Vertex
	{
		glm::vec4 Position{ 0.0f };
		glm::vec4 Normal{ 0.0f };
	};

	struct Particle
	{
		glm::vec4 Position{ 0.0f };
		glm::vec4 Velocity{ 0.0f };
	};


	//Compute global variables
	const int MAX_PARTICLES = 1280000;
	std::vector<glm::vec4> m_particleData;
	float m_windForce = 1.5f;
	float m_windInterval = 2.0f;
	float m_windTimer = 0.0f;

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