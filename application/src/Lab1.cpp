#include "Lab1.hpp"
#include "scripts/include/camera.hpp"
#include "scripts/include/rotation.hpp"
#include <numeric> // For std::iota
#include <glm/gtc/matrix_transform.hpp>
#include "Grid.hpp"
#include <iostream>

Lab1::Lab1(GLFWWindowImpl& win) : Layer(win)
{
	//Camera
	Actor camera;

	//Character
	m_lab1Scene.reset(new Scene);
	ShaderDescription charShaderDesc;
	charShaderDesc.type = ShaderType::rasterization;
	charShaderDesc.vertexSrcPath = "./assets/shaders/lab1/charVertex.glsl";
	charShaderDesc.fragmentSrcPath = "./assets/shaders/lab1/charFrag.glsl";

	//Floor
	ShaderDescription floorShaderDesc;
	floorShaderDesc.type = ShaderType::rasterization;
	floorShaderDesc.vertexSrcPath = "./assets/shaders/lab1/floorVertex.glsl";
	floorShaderDesc.fragmentSrcPath = "./assets/shaders/lab1/floorFrag.glsl";

	//Skybox
	ShaderDescription skyboxShaderDesc;
	skyboxShaderDesc.type = ShaderType::rasterization;
	skyboxShaderDesc.vertexSrcPath = "./assets/shaders/lab1/skyboxVertex.glsl";
	skyboxShaderDesc.fragmentSrcPath = "./assets/shaders/lab1/skyboxFrag.glsl";

	//Floor
	std::shared_ptr<Shader> floorShader;
	floorShader = std::make_shared<Shader>(floorShaderDesc);
	VBOLayout floorLayout = {
		{GL_FLOAT, 3},	//pos
		{GL_FLOAT, 2}	//uv
	};

	std::shared_ptr<Grid> grid = std::make_shared<Grid>();
	std::vector<float> floor_vertices = grid->getVertices();
	std::vector<unsigned int> floor_indices = grid->getIndices();

	std::shared_ptr<VAO> floorVAO;
	floorVAO = std::make_shared<VAO>(floor_indices);
	floorVAO->addVertexBuffer(floor_vertices, floorLayout);
	
	std::shared_ptr<Texture> floorTexture;
	floorTexture = std::make_shared<Texture>("./assets/models/whiteCube/letterCube.png");

	std::shared_ptr<Material> floorMaterial;
	floorMaterial = std::make_shared<Material>(floorShader);
	floorMaterial->setValue("u_albedoMap", floorTexture);
	floorMaterial->setValue("u_albedo", m_floorColour);

	Actor floor;
	floor.geometry = floorVAO;
	floor.material = floorMaterial;
	floor.translation = glm::vec3(-50.0f, -5.0f, -50.0f);
	floor.recalc();

	//Character
	charShader = std::make_shared<Shader>(charShaderDesc);
	VBOLayout charLayout = {
		{GL_FLOAT, 3},	//pos
		{GL_FLOAT, 3},  //normal
		{GL_FLOAT, 2},	//uv
		{GL_FLOAT, 3}	//tan
	};

	uint32_t attributeTypes = Model::VertexFlags::positions |
		Model::VertexFlags::normals |
		Model::VertexFlags::uvs |
		Model::VertexFlags::tangents;

	Model charModel("./assets/models/Vampire/vampire.obj", attributeTypes);
	charVAO = std::make_shared<VAO>(charModel.m_meshes[0].indices);
	charVAO->addVertexBuffer(charModel.m_meshes[0].vertices, charLayout);

	//Find textures
	charTexture = std::make_shared<Texture>("./assets/models/Vampire/textures/diffuse.png");
	charNormalMap = std::make_shared<Texture>("./assets/models/Vampire/textures/normal.png");
	charSpecMap = std::make_shared<Texture>("./assets/models/Vampire/textures/specular.png");

	charMaterial = std::make_shared<Material>(charShader);
	charMaterial->setValue("u_albedoMap", charTexture);
	charMaterial->setValue("u_normalMap", charNormalMap);
	charMaterial->setValue("u_specularMap", charSpecMap);

	character.geometry = charVAO;
	character.material = charMaterial;
	character.translation = glm::vec3(0.0f, -5.0f, -11.0f);
	character.scale = glm::vec3(5.0f, 5.0f, 5.0f);
	character.recalc();

	//Skybox
	std::vector<uint32_t> skyboxIndices(skyboxVertices.size() / 3);
	std::iota(skyboxIndices.begin(), skyboxIndices.end(), 0);

	std::shared_ptr<VAO> skyboxVAO;
	skyboxVAO = std::make_shared<VAO>(skyboxIndices);
	skyboxVAO->addVertexBuffer(skyboxVertices, { {GL_FLOAT, 3} });

	std::shared_ptr<Shader> skyboxShader;
	skyboxShader = std::make_shared<Shader>(skyboxShaderDesc);

	std::shared_ptr<Material> skyboxMaterial;
	skyboxMaterial = std::make_shared<Material>(skyboxShader);

	std::array<const char*, 6> cubeMapPaths =
	{
		"./assets/textures/Skybox/right.png",
		"./assets/textures/Skybox/left.png",
		"./assets/textures/Skybox/top.png",
		"./assets/textures/Skybox/bottom.png",
		"./assets/textures/Skybox/back.png",
		"./assets/textures/Skybox/front.png"
	};
	std::shared_ptr<CubeMap> cubeMap;
	cubeMap = std::make_shared<CubeMap>(cubeMapPaths, false, false);

	Actor skybox;
	skybox.geometry = skyboxVAO;
	skybox.material = skyboxMaterial;
	skyboxMaterial->setValue("u_skyboxView", glm::inverse(skybox.transform));
	skyboxMaterial->setValue("u_skybox", cubeMap);

	//Add to actors vector
	cameraIdx = m_lab1Scene->m_actors.size();
	m_lab1Scene->m_actors.push_back(camera);

	floorIdx = m_lab1Scene->m_actors.size();
	m_lab1Scene->m_actors.push_back(floor);

	charIdx = m_lab1Scene->m_actors.size();
	m_lab1Scene->m_actors.push_back(character);

	skyIdx = m_lab1Scene->m_actors.size();
	m_lab1Scene->m_actors.push_back(skybox);

	//Lights
	//Directional
	DirectionalLight dl;
	dl.direction = glm::normalize(glm::vec3(-0.2, -1.0, -0.5));
	m_lab1Scene->m_directionalLights.push_back(dl);
	
	//Point
	PointLight pointLight;
	uint32_t numPointLights = 10;
	for (int i = 0; i < numPointLights; i++)
	{
		pointLight.colour = glm::vec3(Randomiser::uniformFloatBetween(0.0f, 1.0f), Randomiser::uniformFloatBetween(0.0f, 1.0f), Randomiser::uniformFloatBetween(0.0f, 1.0f));
		pointLight.position = glm::vec3(Randomiser::uniformFloatBetween(-30.0f, 30.0f), -1.0f, Randomiser::uniformFloatBetween(-30.0f, 30.0f));
		pointLight.constants = glm::vec3(1.0f, 0.09f, 0.032f);
		m_lab1Scene->m_pointLights.push_back(pointLight);
	}

	RenderPass mainPass;
	mainPass.scene = m_lab1Scene;
	mainPass.parseScene();
	mainPass.target = std::make_shared<FBO>(); ;
	mainPass.camera.projection = glm::perspective(45.f, m_winRef.getWidthf() / m_winRef.getHeightf(), 0.1f, 1000.f);
	mainPass.viewPort = { 0, 0, m_winRef.getWidth(), m_winRef.getHeight() };
	mainPass.camera.updateView(m_lab1Scene->m_actors.at(cameraIdx).transform);
	mainPass.UBOmanager.setCachedValue("b_camera", "u_view", mainPass.camera.view);
	mainPass.UBOmanager.setCachedValue("b_camera", "u_projection", mainPass.camera.projection);
	mainPass.UBOmanager.setCachedValue("b_camera", "u_viewPos", m_lab1Scene->m_actors.at(cameraIdx).translation);
	mainPass.UBOmanager.setCachedValue("b_lights", "dLight.colour", m_lab1Scene->m_directionalLights.at(0).colour);
	mainPass.UBOmanager.setCachedValue("b_lights", "dLight.direction", m_lab1Scene->m_directionalLights.at(0).direction);
	for (int i = 0; i < numPointLights; i++)
	{
		mainPass.UBOmanager.setCachedValue("b_lights", "pLights[" + std::to_string(i) + "].colour", m_lab1Scene->m_pointLights.at(i).colour);
		mainPass.UBOmanager.setCachedValue("b_lights", "pLights[" + std::to_string(i) + "].position", m_lab1Scene->m_pointLights.at(i).position);
		mainPass.UBOmanager.setCachedValue("b_lights", "pLights[" + std::to_string(i) + "].constants", m_lab1Scene->m_pointLights.at(i).constants);
	}

	m_lab1Scene->m_actors.at(cameraIdx).attachScript<CameraScript>(mainPass.scene->m_actors.at(cameraIdx), m_winRef, glm::vec3(1.6f, 0.6f, 2.f), 0.5f);

	m_lab1Renderer.addRenderPass(mainPass);
}

void Lab1::onRender() const
{
	m_lab1Renderer.render();
}

void Lab1::onUpdate(float timestep)
{
	auto floorMat = m_lab1Scene->m_actors.at(floorIdx).material;
	floorMat->setValue("u_albedo", m_floorColour);

	for (auto it = m_lab1Scene->m_actors.begin(); it != m_lab1Scene->m_actors.end(); ++it)
	{
		it->onUpdate(timestep);
	}

	if (m_wireFrame)
	{
		m_lab1Renderer.getRenderPass(0).drawInWireFrame = true;
	}
	else
	{
		m_lab1Renderer.getRenderPass(0).drawInWireFrame = false;
	}

	//Get render pass
	auto& pass = m_lab1Renderer.getRenderPass(0);

	//Skybox
	auto& skybox = m_lab1Scene->m_actors.at(skyIdx);
	skybox.material->setValue("u_skyboxView", glm::mat3(glm::mat3(pass.camera.view)));

	//Adjust camera view and position
	auto& camera = m_lab1Scene->m_actors.at(cameraIdx);
	pass.camera.updateView(camera.transform);
	pass.UBOmanager.setCachedValue("b_camera", "u_view", pass.camera.view);
	pass.UBOmanager.setCachedValue("b_camera", "u_viewPos", camera.translation);
}

void Lab1::onKeyPressed(KeyPressedEvent& e)
{
	for (auto it = m_lab1Scene->m_actors.begin(); it != m_lab1Scene->m_actors.end(); ++it)
	{
		it->onKeyPress(e);
	}
}

void Lab1::onImGUIRender()
{
	float ms = 1000.0f / ImGui::GetIO().Framerate; //Get milliseconds
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("Lab 1");
	ImGui::Text("FPS %.3 ms/frame (%.1f FPS)", ms, ImGui::GetIO().Framerate);	//Display fps
	ImGui::ColorEdit3("Floor Colour", (float*)&m_floorColour);
	ImGui::Checkbox("Wireframe ", &m_wireFrame);
	ImGui::End();
	ImGui::Render();
}
