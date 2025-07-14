#include "Lab3.hpp"
#include "scripts/include/camera.hpp"
#include "scripts/include/rotation.hpp"
#include <numeric> // For std::iota
#include <glm/gtc/matrix_transform.hpp>
#include "Grid.hpp"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/constants.hpp>

Lab3::Lab3(GLFWWindowImpl& win) : Layer(win)
{
	//Reset scenes
	m_mainScene.reset(new Scene);

	//Camera
	Actor camera;
	cameraIdx = AddActorToRender(camera);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																																																		   //
	//																						Character																										   //
	//																																																		   //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//Shader description
	ShaderDescription charShaderDesc = CreateShaderDesc(ShaderType::rasterization, "./assets/shaders/Lab1/charVertex.glsl", "./assets/shaders/lab1/charFrag.glsl");

	//Shader
	std::shared_ptr<Shader> charShader = CreateShader(charShaderDesc);

		//Depth shader
	ShaderDescription charDepthShaderDesc;
	charDepthShaderDesc.type = ShaderType::rasterization;
	charDepthShaderDesc.vertexSrcPath = "./assets/shaders/lab3/depthVert.glsl";
	charDepthShaderDesc.fragmentSrcPath = "./assets/shaders/lab3/depthFrag.glsl";

	//Create depth shader
	std::shared_ptr<Shader> charDepthShader = CreateShader(charDepthShaderDesc);

	//Depth material
	std::shared_ptr<Material> charDepthMaterial;
	charDepthMaterial = std::make_shared<Material>(charDepthShader);

	//VBO Layout
	VBOLayout charLayout = {
		{GL_FLOAT, 3},	//pos
		{GL_FLOAT, 3},  //normal
		{GL_FLOAT, 2},	//uv
		{GL_FLOAT, 3}	//tan
	};

	//Depth vao
	VBOLayout charDepthLayout = { { GL_FLOAT, 3} };

	//Model positions
	uint32_t attributeTypes = Model::VertexFlags::positions |
		Model::VertexFlags::normals |
		Model::VertexFlags::uvs |
		Model::VertexFlags::tangents;

	//Model
	Model charModel("./assets/models/Vampire/vampire.obj", attributeTypes);

	//VAO
	std::shared_ptr<VAO> charVAO = CreateVAO(charModel.m_meshes[0].indices, charModel.m_meshes[0].vertices, charLayout);

	//Depth vao
	std::shared_ptr<VAO> charDepthVao;
	charDepthVao = std::make_shared<VAO>(charModel.m_meshes[0].indices);
	charDepthVao->addVertexBuffer(charModel.m_meshes[0].positions, charDepthLayout);

	//Find textures
	std::shared_ptr<Texture> charTexture = std::make_shared<Texture>("./assets/models/Vampire/textures/diffuse.png");
	std::shared_ptr<Texture> charNormalMap = std::make_shared<Texture>("./assets/models/Vampire/textures/normal.png");
	std::shared_ptr<Texture> charSpecMap = std::make_shared<Texture>("./assets/models/Vampire/textures/specular.png");

	//Material
	std::shared_ptr<Material> charMaterial;
	charMaterial = std::make_shared<Material>(charShader);
	charMaterial->setValue("u_albedoMap", charTexture);
	charMaterial->setValue("u_normalMap", charNormalMap);
	charMaterial->setValue("u_specularMap", charSpecMap);

	//Actor
	Actor character;
	character.geometry = charVAO;
	character.material = charMaterial;
	character.translation = glm::vec3(0.0f, -5.0f, -11.0f);
	character.scale = glm::vec3(5.0f, 5.0f, 5.0f);

	//Depth
	character.depthGeometry = charDepthVao;
	character.depthMaterial = charDepthMaterial;

	//Recalculate
	character.recalc();
	
	charIdx = m_mainScene->m_actors.size();
	m_mainScene->m_actors.push_back(character);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																																																		   //
	//																						Floor																											   //
	//																																																		   //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Shader description
	ShaderDescription floorShaderDesc = CreateShaderDesc(ShaderType::rasterization, "./assets/shaders/Lab1/floorVertex.glsl", "./assets/shaders/lab1/floorFrag.glsl");

	//Shader
	std::shared_ptr<Shader> floorShader = CreateShader(floorShaderDesc);
	VBOLayout floorLayout = {
		{GL_FLOAT, 3},	//pos
		{GL_FLOAT, 2}	//uv
	};

	//Depth VBO
	VBOLayout depthLayout = { {GL_FLOAT, 3} };

	//Depth shader description
	ShaderDescription floorDepthShaderDesc;
	floorDepthShaderDesc.type = ShaderType::rasterization;
	floorDepthShaderDesc.vertexSrcPath = "./assets/shaders/lab3/depthVert.glsl";
	floorDepthShaderDesc.fragmentSrcPath = "./assets/shaders/lab3/depthFrag.glsl";

	//Depth shader
	std::shared_ptr<Shader> floorDepthShader = CreateShader(floorDepthShaderDesc);

	//Depth material
	std::shared_ptr<Material> floorDepthMaterial;
	floorDepthMaterial = std::make_shared<Material>(floorDepthShader);

	//Create floor grid
	std::shared_ptr<Grid> grid = std::make_shared<Grid>();
	std::vector<float> floor_vertices = grid->getVertices();
	std::vector<unsigned int> floor_indices = grid->getIndices();

	//VAO
	std::shared_ptr<VAO> floorVAO = CreateVAO(floor_indices, floor_vertices, floorLayout);

	//Depth vao
	std::vector<float> floor_positions = grid->getVertexPositions();
	std::shared_ptr<VAO> floorDepthVAO = CreateVAO(floor_indices, floor_positions, depthLayout);

	//Texture
	std::shared_ptr<Texture> floorTexture;
	floorTexture = std::make_shared<Texture>("./assets/models/whiteCube/letterCube.png");

	//Material
	std::shared_ptr<Material> floorMaterial;
	floorMaterial = std::make_shared<Material>(floorShader);
	floorMaterial->setValue("u_albedoMap", floorTexture);
	floorMaterial->setValue("u_albedo", m_floorColour);

	//Create actor
	Actor floor;
	floor.geometry = floorVAO;
	floor.material = floorMaterial;
	floor.translation = glm::vec3(-50.0f, -5.0f, -50.0f);

	//Depth
	floor.depthGeometry = floorDepthVAO;
	floor.depthMaterial = floorDepthMaterial;

	//Recalculate values
	floor.recalc();

	//Add actor to render
	floorIdx = AddActorToRender(floor);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																																																		   //
	//																						Skybox																											   //
	//																																																		   //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Skybox
	//Shader description
	ShaderDescription skyboxShaderDesc = CreateShaderDesc(ShaderType::rasterization, "./assets/shaders/Lab1/skyboxVertex.glsl", "./assets/shaders/lab1/skyboxFrag.glsl");

	//Indices
	std::vector<uint32_t> skyboxIndices(skyboxVertices.size() / 3);

	//iota
	std::iota(skyboxIndices.begin(), skyboxIndices.end(), 0);

	//VAO
	std::shared_ptr<VAO> skyboxVAO = CreateVAO(skyboxIndices, skyboxVertices, { {GL_FLOAT, 3} });

	//Shader
	std::shared_ptr<Shader> skyboxShader = CreateShader(skyboxShaderDesc);

	//Material
	std::shared_ptr<Material> skyboxMaterial;
	skyboxMaterial = std::make_shared<Material>(skyboxShader);

	//Cubemap
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

	//Recalculate values
	skybox.recalc();

	//Add to render
	skyIdx = AddActorToRender(skybox);

	//Adjust uniform values
	skyboxMaterial->setValue("u_skyboxView", glm::inverse(skybox.transform));
	skyboxMaterial->setValue("u_skybox", cubeMap);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																																																		   //
	//																						Lights																											   //
	//																																																		   //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//Directional
	DirectionalLight dl;
	dl.direction = glm::normalize(glm::vec3(-0.2, -1.0, -0.5));
	m_mainScene->m_directionalLights.push_back(dl);

	//Point
	PointLight pointLight;
	for (int i = 0; i < numPointLights; i++)
	{
		pointLight.colour = glm::vec3(1.0f, 0.0f, 0.0f);
		pointLight.position = glm::vec3(Randomiser::uniformFloatBetween(-30.0f, 30.0f), -4.5f, Randomiser::uniformFloatBetween(-30.0f, 30.0f));
		pointLight.constants = m_pLightConstants;
		m_mainScene->m_pointLights.push_back(pointLight);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																																																		   //
	//																						Shadow Pass																										   //
	//																																																		   //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Setup FBO
	FBOLayout shadowLayout =
	{
		{AttachmentType::Depth, true}
	};

	//Find shadow view
	glm::vec3 lightPosition = (m_shadowMapVars.centre - m_mainScene->m_directionalLights.at(0).direction) * m_shadowMapVars.distanceAlongLightVector;
	glm::mat4 shadowView = glm::lookAt(lightPosition, m_shadowMapVars.centre, m_shadowMapVars.UP);

	DepthPass shadowPass;
	shadowPass.scene = m_mainScene;
	shadowPass.parseScene();
	shadowPass.target = std::make_shared<FBO>(glm::ivec2(4096, 4096), shadowLayout);
	shadowPass.viewPort = { 0, 0, 4096, 4096 };
	shadowPass.camera.view = shadowView;
	shadowPass.camera.projection = glm::ortho(-m_shadowMapVars.orthoSize,
											  m_shadowMapVars.orthoSize,
											  - m_shadowMapVars.orthoSize,
											  m_shadowMapVars.orthoSize,
											  -m_shadowMapVars.orthoSize / 2,
											  m_shadowMapVars.orthoSize * 2);
	shadowPass.UBOmanager.setCachedValue("b_LightCamera", "u_view", shadowPass.camera.view);
	shadowPass.UBOmanager.setCachedValue("b_LightCamera", "u_projection", shadowPass.camera.projection);

	//Update pass index
	m_mainRenderer.addDepthPass(shadowPass);
	shadowPassIdx = m_mainRenderer.getDepthPassCount();

	//Set shadow map values
	floorMaterial->setValue("u_shadowMap", shadowPass.target->getTarget(0));
	charMaterial->setValue("u_shadowMap", shadowPass.target->getTarget(0));

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																																																		   //
	//																						Main Pass																										   //
	//																																																		   //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Frame buffer layout
	FBOLayout typicalLayout =
	{
		{AttachmentType::ColourHDR, true},
		{AttachmentType::Depth, true}
	};

	//Main render pass
	RenderPass mainPass;
	mainPass.scene = m_mainScene;
	mainPass.parseScene();
	mainPass.target = std::make_shared<FBO>(m_winRef.getSize(), typicalLayout);
	mainPass.camera.projection = glm::perspective(45.f, m_winRef.getWidthf() / m_winRef.getHeightf(), 0.1f, 1000.f);
	mainPass.viewPort = { 0, 0, m_winRef.getWidth(), m_winRef.getHeight() };
	mainPass.camera.updateView(m_mainScene->m_actors.at(cameraIdx).transform);
	mainPass.UBOmanager.setCachedValue("b_camera", "u_view", mainPass.camera.view);
	mainPass.UBOmanager.setCachedValue("b_camera", "u_projection", mainPass.camera.projection);
	mainPass.UBOmanager.setCachedValue("b_camera", "u_viewPos", m_mainScene->m_actors.at(cameraIdx).translation);
	mainPass.UBOmanager.setCachedValue("b_lights", "dLight.colour", m_mainScene->m_directionalLights.at(0).colour);
	mainPass.UBOmanager.setCachedValue("b_lights", "dLight.direction", m_mainScene->m_directionalLights.at(0).direction);
	for (int i = 0; i < numPointLights; i++)
	{
		mainPass.UBOmanager.setCachedValue("b_lights", "pLights[" + std::to_string(i) + "].colour", m_mainScene->m_pointLights.at(i).colour);
		mainPass.UBOmanager.setCachedValue("b_lights", "pLights[" + std::to_string(i) + "].position", m_mainScene->m_pointLights.at(i).position);
		mainPass.UBOmanager.setCachedValue("b_lights", "pLights[" + std::to_string(i) + "].constants", m_mainScene->m_pointLights.at(i).constants);
	}

	float width = m_winRef.getWidth();
	float height = m_winRef.getHeight();

	//Update index and add render pass
	m_mainRenderer.addRenderPass(mainPass);
	mainPassIdx = m_mainRenderer.getRenderPassCount();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																																																		   //
	//																						Create Screen Quad (with blur shader)																			   //
	//																																																		   //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Reset scene
	m_screenScene.reset(new Scene);

	//Create screen quad actor
	//Screen vertices
	const std::vector<float> screenVertices =
	{		//Pos			//UV
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f,		//Bottom left
		width, 0.0f, 0.0f, 1.0f, 1.0f,		//Bottom right
		width, height, 0.0f, 1.0f, 0.0f,	//Top right
		0.0f, height, 0.0f, 0.0f, 0.0f		//Top left
	};

	//Screen indices
	const std::vector<uint32_t> screenIndices = { 0, 1, 2, 2, 3, 0 };

	//Layout
	VBOLayout screenQuadLayout =
	{
		{GL_FLOAT, 3},	//pos
		{GL_FLOAT, 2}	//uv
	};

	//VAO
	std::shared_ptr<VAO> screenQuadVAO = CreateVAO(screenIndices, screenVertices, screenQuadLayout);

	//Shader description for fog
	ShaderDescription blurShaderDesc = CreateShaderDesc(ShaderType::rasterization, "./assets/shaders/lab2/screenVert.glsl", "./assets/shaders/lab2/blurFrag.glsl");

	//Shader
	std::shared_ptr<Shader> blurShader = CreateShader(blurShaderDesc);

	//Material
	std::shared_ptr<Material> blurMaterial;
	blurMaterial = std::make_shared<Material>(blurShader);
	blurMaterial->setValue("u_inputTexture", mainPass.target->getTarget(0));

	//Actor
	Actor screenQuad;
	screenQuad.geometry = screenQuadVAO;
	screenQuad.material = blurMaterial;
	screenQuad.recalc();

	//Push quad to scene
	m_screenScene->m_actors.push_back(screenQuad);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																																																		   //
	//																						Blur Pass																										   //
	//																																																		   //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Create pass
	RenderPass blurPass;
	blurPass.scene = m_screenScene;
	blurPass.parseScene();
	blurPass.target = std::make_shared<FBO>(m_winRef.getSize(), typicalLayout);
	blurPass.viewPort = { 0, 0, m_winRef.getWidth(), m_winRef.getHeight() };
	blurPass.camera.projection = glm::ortho(0.0f, width, height, 0.0f);
	blurPass.UBOmanager.setCachedValue("b_camera2D", "u_view", blurPass.camera.view);
	blurPass.UBOmanager.setCachedValue("b_camera2D", "u_projection", blurPass.camera.projection);

	//Update index and add to render
	m_mainRenderer.addRenderPass(blurPass);
	blurPassIdx = m_mainRenderer.getRenderPassCount();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																																																		   //
	//																						DoF Pass																										   //
	//																																																		   //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Reset scene
	m_screenScene.reset(new Scene);

	//Shader description
	ShaderDescription dofShaderDesc = CreateShaderDesc(ShaderType::rasterization, "./assets/shaders/lab2/screenVert.glsl", "./assets/shaders/lab3/dofFrag.glsl");

	//Shader
	std::shared_ptr<Shader> dofShader = CreateShader(dofShaderDesc);

	//Material
	std::shared_ptr<Material> dofMaterial;
	dofMaterial = std::make_shared<Material>(dofShader);
	dofMaterial->setValue("u_inFocus", mainPass.target->getTarget(0));
	dofMaterial->setValue("u_outOfFocus", blurPass.target->getTarget(0));
	dofMaterial->setValue("u_depthTexture", mainPass.target->getTarget(1));

	//Update screen quad material
	screenQuad.material = dofMaterial;

	//Push to scene
	m_screenScene->m_actors.push_back(screenQuad);

	//Create pass
	RenderPass dofPass;
	dofPass.scene = m_screenScene;
	dofPass.parseScene();
	dofPass.target = std::make_shared<FBO>(m_winRef.getSize(), typicalLayout);
	dofPass.viewPort = { 0, 0, m_winRef.getWidth(), m_winRef.getHeight() };
	dofPass.camera.projection = glm::ortho(0.0f, width, height, 0.0f);
	dofPass.UBOmanager.setCachedValue("b_camera2D", "u_view", dofPass.camera.view);
	dofPass.UBOmanager.setCachedValue("b_camera2D", "u_projection", dofPass.camera.projection);

	//Update indexes and add to render
	m_mainRenderer.addRenderPass(dofPass);
	dofPassIdx = m_mainRenderer.getRenderPassCount();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																																																		   //
	//																						Fog Pass																										   //
	//																																																		   //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Reset scene
	m_screenScene.reset(new Scene);

	//Shader description
	ShaderDescription fogShaderDesc = CreateShaderDesc(ShaderType::rasterization, "./assets/shaders/lab2/screenVert.glsl", "./assets/shaders/lab3/fogFrag.glsl");

	//Shader
	std::shared_ptr<Shader> fogShader = CreateShader(fogShaderDesc);

	//Material
	std::shared_ptr<Material> fogMaterial;
	fogMaterial = std::make_shared<Material>(fogShader);
	fogMaterial->setValue("u_inputTexture", dofPass.target->getTarget(0));

	//Update screen quad material
	screenQuad.material = fogMaterial;

	//Push to scene
	m_screenScene->m_actors.push_back(screenQuad);

	//Create pass
	RenderPass fogPass;
	fogPass.scene = m_screenScene;
	fogPass.parseScene();
	fogPass.target = std::make_shared<FBO>(m_winRef.getSize(), typicalLayout);
	fogPass.viewPort = { 0, 0, m_winRef.getWidth(), m_winRef.getHeight() };
	fogPass.camera.projection = glm::ortho(0.0f, width, height, 0.0f);
	fogPass.UBOmanager.setCachedValue("b_camera2D", "u_view", fogPass.camera.view);
	fogPass.UBOmanager.setCachedValue("b_camera2D", "u_projection", fogPass.camera.projection);

	//Update index and add to render
	m_mainRenderer.addRenderPass(fogPass);
	fogPassIdx = m_mainRenderer.getRenderPassCount();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																																																		   //
	//																						Visualize shadow Pass																							   //
	//																																																		   //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//Reset the scene
	m_screenScene.reset(new Scene);

	//Shader description
	ShaderDescription visualizeShadowDesc = CreateShaderDesc(ShaderType::rasterization, "./assets/shaders/lab2/screenVert.glsl", "./assets/shaders/lab3/visualizeShadowFrag.glsl");

	//Shader
	std::shared_ptr<Shader> visualizeShadowShader = CreateShader(visualizeShadowDesc);

	//Material
	std::shared_ptr<Material> visualizeShadowMaterial;
	visualizeShadowMaterial = std::make_shared<Material>(visualizeShadowShader);
	visualizeShadowMaterial->setValue("u_inputTexture", shadowPass.target->getTarget(0));

	//Update screen quad
	screenQuad.material = visualizeShadowMaterial;

	//push to scene
	m_screenScene->m_actors.push_back(screenQuad);

	//Create render pass
	RenderPass visualizeShadowPass;
	visualizeShadowPass.scene = m_screenScene;
	visualizeShadowPass.parseScene();
	visualizeShadowPass.target = std::make_shared<FBO>(m_winRef.getSize(), typicalLayout);
	visualizeShadowPass.viewPort = { 0, 0, m_winRef.getWidth(), m_winRef.getHeight() };
	visualizeShadowPass.camera.projection = glm::ortho(0.0f, width, height, 0.0f);
	visualizeShadowPass.UBOmanager.setCachedValue("b_camera2D", "u_view", visualizeShadowPass.camera.view);
	visualizeShadowPass.UBOmanager.setCachedValue("b_camera2D", "u_projection", visualizeShadowPass.camera.projection);

	//add to render and update index
	m_mainRenderer.addRenderPass(visualizeShadowPass);
	visualizeShadowIdx = m_mainRenderer.getRenderPassCount();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																																																		   //
	//																						Screen Pass																										   //
	//																																																		   //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Reset scene
	m_screenScene.reset(new Scene);

	//Shader description
	ShaderDescription screenShaderDesc = CreateShaderDesc(ShaderType::rasterization, "./assets/shaders/lab2/screenVert.glsl", "./assets/shaders/lab2/screenFrag.glsl");

	//Shader
	std::shared_ptr<Shader> screenShader = CreateShader(screenShaderDesc);

	//Material
	std::shared_ptr<Material> screenMaterial;
	screenMaterial = std::make_shared<Material>(screenShader);
	screenMaterial->setValue("u_inputTexture", fogPass.target->getTarget(0));

	//Update screen quad material
	screenQuad.material = screenMaterial;

	//Push to scene
	m_screenScene->m_actors.push_back(screenQuad);

	//Screen pass with tonemapping
	RenderPass screenPass;
	screenPass.scene = m_screenScene;
	screenPass.parseScene();
	screenPass.target = std::make_shared<FBO>();
	screenPass.viewPort = { 0, 0, m_winRef.getWidth(), m_winRef.getHeight() };
	screenPass.camera.projection = glm::ortho(0.0f, width, height, 0.0f);
	screenPass.UBOmanager.setCachedValue("b_camera2D", "u_view", screenPass.camera.view);
	screenPass.UBOmanager.setCachedValue("b_camera2D", "u_projection", screenPass.camera.projection);

	//Update index and add render pass
	m_mainRenderer.addRenderPass(screenPass);
	screenPassIdx = m_mainRenderer.getRenderPassCount();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																																																		   //
	//																						Scripts																											   //
	//																																																		   //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Attach camera script for WASD movement
	m_mainScene->m_actors.at(cameraIdx).attachScript<CameraScript>(mainPass.scene->m_actors.at(cameraIdx), m_winRef, glm::vec3(1.6f, 0.6f, 2.f), 0.5f);
}



void Lab3::onRender() const
{
	m_mainRenderer.render();
}

void Lab3::onUpdate(float timestep)
{
	//Get shadow pass
	auto& shadowPass = m_mainRenderer.getDepthPass(shadowPassIdx);

	//Change floor colour and update lighting 
	auto floorMat = m_mainScene->m_actors.at(floorIdx).material;
	floorMat->setValue("u_albedo", m_floorColour);
	floorMat->setValue("u_pLightsAmbient", m_pointLightsAmbient);
	floorMat->setValue("u_pLightsShine", m_pointLightsShine);
	floorMat->setValue("u_lightSpaceTransform", shadowPass.camera.projection * shadowPass.camera.view);

	//Update character lighting
	auto& charMat = m_mainScene->m_actors.at(charIdx).material;
	charMat->setValue("u_pLightsAmbient", m_pointLightsAmbient);
	charMat->setValue("u_pLightsShine", m_pointLightsShine);
	charMat->setValue("u_lightSpaceTransform", shadowPass.camera.projection * shadowPass.camera.view);

	//Run all update functions
	for (auto it = m_mainScene->m_actors.begin(); it != m_mainScene->m_actors.end(); ++it)
	{
		it->onUpdate(timestep);
	}

	//Wireframe
	if (m_wireFrame)
	{
		m_mainRenderer.getRenderPass(mainPassIdx).drawInWireFrame = true;
	}
	else
	{
		m_mainRenderer.getRenderPass(mainPassIdx).drawInWireFrame = false;
	}

	//Get render pass
	auto& mainPass = m_mainRenderer.getRenderPass(mainPassIdx);

	//Skybox
	auto& skybox = m_mainScene->m_actors.at(skyIdx);
	skybox.material->setValue("u_skyboxView", glm::mat3(glm::mat3(mainPass.camera.view)));

	//Update tonemapping mode
	auto& screenQuadMat = m_mainRenderer.getRenderPass(screenPassIdx).scene->m_actors.at(0).material;
	screenQuadMat->setValue("u_tonemappingMode", m_tonemappingMode);

	//Point light constants
	for (int i = 0; i < numPointLights; i++)
	{
		auto& pointLight = m_mainScene->m_pointLights.at(i);
		pointLight.constants = m_pLightConstants;
		mainPass.UBOmanager.setCachedValue("b_lights", "pLights[" + std::to_string(i) + "].constants", m_mainScene->m_pointLights.at(i).constants);
	}

	//Update directional light colour
	auto& directionalLight = m_mainScene->m_directionalLights.at(0);
	directionalLight.direction = m_dlDirection;
	mainPass.UBOmanager.setCachedValue("b_lights", "dLight.colour", m_dlColour);
	mainPass.UBOmanager.setCachedValue("b_lights", "dLight.direction", m_dlDirection);

	//Blur
	//auto& blurMat = m_mainRenderer.getRenderPass(blurPassIdx).scene->m_actors.at(0).material;
	//if (m_blur > 0)
	//{
	//	blurMat->setValue("u_active", 1.0f);
	//	blurMat->setValue("u_blurRadius", m_blurRadius);
	//}
	//else
	//{
	//	blurMat->setValue("u_active", 0.0f);
	//}

	////Vignette
	//auto& vignetteMat = m_mainRenderer.getRenderPass(vignettePassIdx).scene->m_actors.at(0).material;
	//if (m_vignette > 0)
	//{
	//	vignetteMat->setValue("u_active", 1.0f);
	//	vignetteMat->setValue("u_innerRadius", m_vignetteControls.x);
	//	vignetteMat->setValue("u_outerRadius", m_vignetteControls.y);
	//}
	//else
	//{
	//	vignetteMat->setValue("u_active", 0.0f);
	//}

	////Sepia
	//auto& sepiaMat = m_mainRenderer.getRenderPass(sepiaPassIdx).scene->m_actors.at(0).material;
	//if (m_sepia > 0)
	//{
	//	sepiaMat->setValue("u_active", 1.0f);
	//}
	//else
	//{
	//	sepiaMat->setValue("u_active", 0.0f);
	//}

	////Edge detection
	//auto& edgeDetectionMat = m_mainRenderer.getRenderPass(edgeDetectionPassIdx).scene->m_actors.at(0).material;
	//if (m_edgeDetection > 0)
	//{
	//	edgeDetectionMat->setValue("u_active", 1.0f);
	//}
	//else
	//{
	//	edgeDetectionMat->setValue("u_active", 0.0f);
	//}

	////Fog
	auto& fogMat = m_mainRenderer.getRenderPass(fogPassIdx).scene->m_actors.at(0).material;
	if (m_fog > 0)
	{
		fogMat->setValue("u_active", 1.0f);
		fogMat->setValue("u_fogColour", m_fogColour);
		fogMat->setValue("u_fogDensity", m_fogDensity);
	}
	else
	{
		fogMat->setValue("u_active", 0.0f);
	}

	//DOF
	auto& dofMat = m_mainRenderer.getRenderPass(dofPassIdx).scene->m_actors.at(0).material;
	auto& blurMat = m_mainRenderer.getRenderPass(blurPassIdx).scene->m_actors.at(0).material;
	if (m_dof)
	{
		blurMat->setValue("u_active", 1.0f);
		blurMat->setValue("u_blurRadius", m_blurRadius);

		dofMat->setValue("u_active", 1.0f);
		dofMat->setValue("u_focalDistance", m_focalDistance);
		dofMat->setValue("u_focalRange", m_focalRange);
	}
	else
	{
		dofMat->setValue("u_active", 0.0f);
		blurMat->setValue("u_active", 0.0f);
	}

	//Adjust camera view and position
	auto& camera = m_mainScene->m_actors.at(cameraIdx);
	mainPass.camera.updateView(camera.transform);
	mainPass.UBOmanager.setCachedValue("b_camera", "u_view", mainPass.camera.view);
	mainPass.UBOmanager.setCachedValue("b_camera", "u_viewPos", camera.translation);

	//Update shadow map
	glm::vec3 lightPosition = (m_shadowMapVars.centre - glm::normalize(m_mainScene->m_directionalLights.at(0).direction)) * m_shadowMapVars.distanceAlongLightVector;
	glm::mat4 shadowView = glm::lookAt(lightPosition, m_shadowMapVars.centre, m_shadowMapVars.UP);
	shadowPass.camera.view = shadowView;
	shadowPass.UBOmanager.setCachedValue("b_LightCamera", "u_view", shadowPass.camera.view);
}

void Lab3::onKeyPressed(KeyPressedEvent& e)
{
	for (auto it = m_mainScene->m_actors.begin(); it != m_mainScene->m_actors.end(); ++it)
	{
		it->onKeyPress(e);
	}
}

void Lab3::onImGUIRender()
{
	//Milliseconds
	float ms = 1000.0f / ImGui::GetIO().Framerate;

	//Create new frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//Name
	ImGui::Begin("Lab 3");

	//Display fps
	ImGui::Text("FPS %.3 ms/frame (%.1f FPS)", ms, ImGui::GetIO().Framerate);

	//Edit floor colour
	ImGui::ColorEdit3("Floor Colour", (float*)&m_floorColour);

	//Directional light colour
	ImGui::ColorEdit3("Directional Light Colour", (float*)&m_dlColour);

	//Directional light direction
	ImGui::SliderFloat3("Directional Light Direction", glm::value_ptr(m_dlDirection), -1.0f, 1.0f);

	//Point lights ambient
	ImGui::SliderFloat("Point Lights Ambient Strength", &m_pointLightsAmbient, 0.0, 0.05);

	//Point lights shine
	ImGui::SliderFloat("Point Lights shine", &m_pointLightsShine, 0.0, 1024);

	//Point light constants
	ImGui::SliderFloat3("Point Light Constants", glm::value_ptr(m_pLightConstants), 0, 2.0f);

	//Wireframe checkbox
	ImGui::Checkbox("Wireframe ", &m_wireFrame);

	//Vignette checkbox
	ImGui::Checkbox("Vignette", (bool*)&m_vignette);

	//Vignette controls
	ImGui::SliderFloat2("Vignette Inner and Outer Radius", glm::value_ptr(m_vignetteControls), 0.0f, 2.0f);

	//Sepia checkbox
	ImGui::Checkbox("Sepia", (bool*)&m_sepia);

	//Edge detection checkbox
	ImGui::Checkbox("Edge Detection", (bool*)&m_edgeDetection);

	//Fog checkbox
	ImGui::Checkbox("Fog", (bool*)&m_fog);

	//Fog colour
	ImGui::SliderFloat3("Fog Colour", glm::value_ptr(m_fogColour), 0.0f, 1.0f);

	//Fog density
	ImGui::SliderFloat("Fog Density", &m_fogDensity, 0.0f, 0.02f);

	//DOF checkbox
	ImGui::Checkbox("Depth of Field", (bool*)&m_dof);

	//Blur radius slider
	ImGui::SliderFloat("Blur Radius", &m_blurRadius, 0.0, 10);

	//Focal distance
	ImGui::SliderFloat("Focal Distance", &m_focalDistance, 0.1f, 100.0f);

	//Focal range
	ImGui::SliderFloat("Focal Range", &m_focalRange, 0.1f, 20.0f);

	//Choose tonemapping method
	static int tonemappingMode = 6;
	const char* tonemappingModes[9] = { "ACES", "UNCHARTED2", "REINHARD", "FILMIC", "LOTTES", "NEUTRAL", "REINHART2", "UCHIMURA", "UNREAL" };
	if (ImGui::Combo("Tonemapping Mode", &tonemappingMode, tonemappingModes, IM_ARRAYSIZE(tonemappingModes)))
	{
		m_tonemappingMode = tonemappingMode;
	}

	//Display image
	GLuint textureID = m_mainRenderer.getRenderPass(visualizeShadowIdx).target->getTarget(0)->getID();
	ImVec2 imageSize = ImVec2(256, 256);
	ImVec2 uv0 = ImVec2(0.0f, 1.0f);
	ImVec2 uv1 = ImVec2(1.0f, 0.0f);
	ImGui::Image((void*)(intptr_t)textureID, imageSize, uv0, uv1);

	//End/Render
	ImGui::End();
	ImGui::Render();
}

ShaderDescription Lab3::CreateShaderDesc(ShaderType type, std::string vertexPath, std::string fragPath)
{
	ShaderDescription shaderDesc;
	shaderDesc.type = type;
	shaderDesc.vertexSrcPath = vertexPath;
	shaderDesc.fragmentSrcPath = fragPath;
	return shaderDesc;
}

std::shared_ptr<Shader> Lab3::CreateShader(ShaderDescription shaderDesc)
{
	std::shared_ptr<Shader> newShader;
	newShader = std::make_shared<Shader>(shaderDesc);
	return newShader;
}

std::shared_ptr<VAO> Lab3::CreateVAO(std::vector<unsigned int> indicies, std::vector<float> vertices, VBOLayout layout)
{
	std::shared_ptr<VAO> newVAO;
	newVAO = std::make_shared<VAO>(indicies);
	newVAO->addVertexBuffer(vertices, layout);
	return newVAO;
}

Actor Lab3::CreateActor(std::shared_ptr<VAO> Vao, std::shared_ptr<VAO> depthVao, std::shared_ptr<Material> material, 
	std::shared_ptr<Material> depthMat ,glm::vec3 translation, glm::vec3 scale)
{
	Actor actor;
	actor.geometry = Vao;
	actor.material = material;
	actor.translation = translation;
	actor.scale = scale;

	//Depth
	if (depthVao != nullptr)
	{
		actor.depthGeometry = depthVao;
	}
	if (depthMat != nullptr)
	{
		actor.depthMaterial = depthMat;
	}

	actor.recalc();
	return actor;
}

int Lab3::AddActorToRender(Actor actor)
{
	int index = m_mainScene->m_actors.size();
	m_mainScene->m_actors.push_back(actor);
	return index;
}
