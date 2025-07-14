#include "Lab5.hpp"
#include "scripts/include/camera.hpp"
#include "scripts/include/rotation.hpp"
#include <numeric> // For std::iota
#include <glm/gtc/matrix_transform.hpp>
#include "Grid.hpp"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/constants.hpp>

//Ethan Tudor-Lloyd
//P2656479

Lab5::Lab5(GLFWWindowImpl& win) : Layer(win)
{
	//Reset scenes
	m_mainScene.reset(new Scene);

	//Camera
	Actor camera;
	camera.translation.y = camera.translation.y + 15;
	camera.translation.z = camera.translation.z + 20;
	cameraIdx = AddActorToRender(camera);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																																																		   //
	//																						Character																										   //
	//																																																		   //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Shader description
	ShaderDescription charShaderDesc = CreateShaderDesc(ShaderType::rasterization, "./assets/shaders/Lab1/charVertex.glsl", "./assets/shaders/lab4/gBufferCharFrag.glsl");

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
	ShaderDescription floorShaderDesc;
	floorShaderDesc.type = ShaderType::tessellationAndGeometry;
	floorShaderDesc.vertexSrcPath = "./assets/shaders/Lab1/floorVertex.glsl";
	floorShaderDesc.controlSrcPath = "./assets/shaders/lab5/floorTCS.glsl";
	floorShaderDesc.evaluationSrcPath = "./assets/shaders/lab5/floorTES.glsl";
	floorShaderDesc.geometrySrcPath = "./assets/shaders/lab5/floorGeo.glsl";
	floorShaderDesc.fragmentSrcPath = "./assets/shaders/lab4/gBufferFloorFrag.glsl";

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

	//Base texture
	std::shared_ptr<Texture> floorTexture;
	floorTexture = std::make_shared<Texture>("./assets/models/whiteCube/letterCube.png");

	//Height map texture
	std::shared_ptr<Texture> floorHeightMap;
	floorHeightMap = std::make_shared<Texture>("./assets/textures/height/usa_valley_heightmap.png");

	//Material
	std::shared_ptr<Material> floorMaterial;
	floorMaterial = std::make_shared<Material>(floorShader);
	floorMaterial->setValue("u_heightMap", floorHeightMap);
	floorMaterial->setValue("u_heightMapScale", 5.0f);
	floorMaterial->setPrimitive(GL_PATCHES);

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
	//																						Trees																											   //
	//																																																		   //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Shader description
	ShaderDescription billboardShaderDesc;
	billboardShaderDesc.type = ShaderType::geometry;
	billboardShaderDesc.vertexSrcPath = "./assets/shaders/lab5/billboardVertex.glsl";
	billboardShaderDesc.geometrySrcPath = "./assets/shaders/lab5/billboardGeo.glsl";
	billboardShaderDesc.fragmentSrcPath = "./assets/shaders/lab5/billboardFrag.glsl";

	//Shader
	std::shared_ptr<Shader> treeShader = CreateShader(billboardShaderDesc);

	//Decide number of trees
	int numTrees = 10;

	//Declare billboard positions
	std::vector<float> treePositions;

	//Generate positions
	for (int i = 0; i < numTrees; i++)
	{
		treePositions.push_back(Randomiser::uniformFloatBetween(-50.0f, 50.0f)); //x
		treePositions.push_back(Randomiser::uniformFloatBetween(0.0f, 0.0f)); //y
		treePositions.push_back(Randomiser::uniformFloatBetween(-30.0f, -35.0f)); //z
	}

	//Indices
	std::vector<uint32_t> treeIndices(treePositions.size() / 3);

	//iota
	std::iota(treeIndices.begin(), treeIndices.end(), 0);

	//VAO
	std::shared_ptr<VAO> treeVAO;
	treeVAO = std::make_shared<VAO>(treeIndices);
	treeVAO->addVertexBuffer(treePositions, { {GL_FLOAT, 3} });

	//Texture
	std::shared_ptr<Texture> treeTexture;
	treeTexture = std::make_shared<Texture>("./assets/textures/trees/tree.png");

	//Material
	std::shared_ptr<Material> treeMaterial;
	treeMaterial = std::make_shared<Material>(treeShader);
	treeMaterial->setPrimitive(GL_POINTS);
	treeMaterial->setValue("u_billboardImage", treeTexture);

	//Actor
	Actor trees;
	trees.geometry = treeVAO;
	trees.material = treeMaterial;
	trees.recalc();

	//Add to scene and update index
	treeIdx = AddActorToRender(trees);

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

	//Dont add to render here, do it later on skybox render pass

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
		pointLight.colour = glm::vec3(1.0f, 1.0f, 1.0f);
		pointLight.position = glm::vec3(Randomiser::uniformFloatBetween(-30.0f, 30.0f), -4.5f, Randomiser::uniformFloatBetween(-30.0f, 30.0f));
		pointLight.constants = m_pLightConstants;
		m_mainScene->m_pointLights.push_back(pointLight);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																																																		   //
	//																						Compute Shaders																									   //
	//																																																		   //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																																																		   //
	//																						Height Map and normal generator																					   //
	//																																																		   //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//Texture desc
	TextureDescription textDesc;
	textDesc.width = 512;
	textDesc.height = 512;
	textDesc.channels = 4;
	textDesc.isHDR = false;

	//Empty texture
	std::shared_ptr<Texture> emptyTexture = std::make_shared<Texture>(textDesc);
	std::shared_ptr<Texture> emptyTexture1 = std::make_shared<Texture>(textDesc);

	//Shader desc
	ShaderDescription textureComputeDesc;
	textureComputeDesc.type = ShaderType::compute;
	textureComputeDesc.computeSrcPath = "./assets/shaders/lab5/compute_textureTest.glsl";

	//Shader
	std::shared_ptr<Shader> textureComputeShader = CreateShader(textureComputeDesc);

	//Material
	std::shared_ptr<Material> textureComputeMaterial;
	textureComputeMaterial = std::make_shared<Material>(textureComputeShader);
	textureComputeMaterial->setValue("u_heightMapScale", m_heightMapScale);

	//Pass
	ComputePass textureComputePass;
	textureComputePass.material = textureComputeMaterial;
	textureComputePass.workgroups = { 32, 32, 1 };
	textureComputePass.barrier = MemoryBarrier::ShaderImageAccess;

	//Noise map image
	Image noiseMapImg;
	noiseMapImg.mipLevel = 0;
	noiseMapImg.layered = false;
	noiseMapImg.texture = emptyTexture;
	noiseMapImg.imageUnit = textureComputePass.material->m_shader->m_imageBindingPoints["noiseMapImg"];
	noiseMapImg.access = TextureAccess::WriteOnly;

	//Create image to write per vertex normals to
	Image perVertexNormalImg;
	perVertexNormalImg.mipLevel = 0;
	perVertexNormalImg.layered = false;
	perVertexNormalImg.texture = emptyTexture1;
	perVertexNormalImg.imageUnit = textureComputePass.material->m_shader->m_imageBindingPoints["perVertexNormalImg"];
	perVertexNormalImg.access = TextureAccess::WriteOnly;

	//Push image to the array
	textureComputePass.images.push_back(noiseMapImg);
	textureComputePass.images.push_back(perVertexNormalImg);

	//Add to render
	m_mainRenderer.addComputePass(textureComputePass);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																																																		   //
	//																						Particle System Pass																							   //
	//																																																		   //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Create particle data
	std::vector<glm::vec4> particleData(MAX_PARTICLES * 2);
	for (size_t i = 0; i < MAX_PARTICLES; i++) {
		//Position
		particleData[i * 2] = glm::vec4{
			Randomiser::uniformFloatBetween(-15.0f, 15.0f),
			Randomiser::uniformFloatBetween(2.0f, 15.0f),
			Randomiser::uniformFloatBetween(-15.0f, 15.0f),
			1.0f
		};

		//Velocity
		particleData[i * 2 + 1] = glm::vec4{
			Randomiser::uniformFloatBetween(-0.5f, 0.5f),
			Randomiser::uniformFloatBetween(-1.5f, -0.5f),
			Randomiser::uniformFloatBetween(-0.5f, 0.5f),
			0.0f
		};
	}

	//Update random spawning positions
	std::vector<glm::vec4> randomPositions(MAX_PARTICLES);
	for (auto& pos : randomPositions)
	{
		pos = glm::vec4
		{
			Randomiser::uniformFloatBetween(-15.0f, 15.0f),
			Randomiser::uniformFloatBetween(2.0f, 15.0f),
			Randomiser::uniformFloatBetween(-15.0f, 15.0f),
			1.0f
		};
	}

	//combine particle data and random positions
	particleData.insert(particleData.end(), randomPositions.begin(), randomPositions.end());

	//Create and bind SSBO
	std::shared_ptr<SSBO> testParticleSSBO;
	testParticleSSBO = std::make_shared<SSBO>(particleData.size() * sizeof(glm::vec4), MAX_PARTICLES, particleData.data());
	testParticleSSBO->bind(0);

	//Compute shader description
	ShaderDescription particleComputeDesc;
	particleComputeDesc.type = ShaderType::compute;
	particleComputeDesc.computeSrcPath = "./assets/shaders/lab5/particleCompute.glsl";

	//Create shader and material
	std::shared_ptr<Shader> particleComputeShader = std::make_shared<Shader>(particleComputeDesc);
	std::shared_ptr<Material> particleComputeMaterial = std::make_shared<Material>(particleComputeShader);

	//Create compute pass
	ComputePass particleComputePass;
	particleComputePass.material = particleComputeMaterial;
	particleComputePass.workgroups = { MAX_PARTICLES / 256, 1, 1 };
	particleComputePass.barrier = MemoryBarrier::ShaderStorageAccess;
	particleComputePass.ssbo = testParticleSSBO;

	//Add to main renderer
	m_mainRenderer.addComputePass(particleComputePass);

	//Main particle shader description
	ShaderDescription particleRenderDesc;
	particleRenderDesc.type = ShaderType::geometry;
	particleRenderDesc.vertexSrcPath = "./assets/shaders/lab5/particleVertex.glsl";
	particleRenderDesc.fragmentSrcPath = "./assets/shaders/lab5/particleFrag.glsl";
	particleRenderDesc.geometrySrcPath = "./assets/shaders/lab5/particleGeo.glsl";

	//Create shader and material
	std::shared_ptr<Shader> particleRenderShader = CreateShader(particleRenderDesc);
	std::shared_ptr<Material> particleRenderMaterial = std::make_shared<Material>(particleRenderShader);
	particleRenderMaterial->setPrimitive(GL_POINTS);

	//Generate indices using iota
	std::vector<uint32_t> particleIndices(MAX_PARTICLES);
	std::iota(particleIndices.begin(), particleIndices.end(), 0);

	//Create VAO using geenerated indices
	std::shared_ptr<VAO> particleVAO = std::make_shared<VAO>(particleIndices);

	//Create actor
	Actor particleActor;
	particleActor.geometry = particleVAO;
	particleActor.material = particleRenderMaterial;
	particleActor.recalc();

	//Set global particles vector to the same local vector
	m_particleData = particleData;

	//Push the particles to the main scene
	particlesIdx = m_mainScene->m_actors.size();
	m_mainScene->m_actors.push_back(particleActor);

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
		-m_shadowMapVars.orthoSize,
		m_shadowMapVars.orthoSize,
		-m_shadowMapVars.orthoSize / 2,
		m_shadowMapVars.orthoSize * 2.5f);
	shadowPass.UBOmanager.setCachedValue("b_LightCamera", "u_view", shadowPass.camera.view);
	shadowPass.UBOmanager.setCachedValue("b_LightCamera", "u_projection", shadowPass.camera.projection);

	//Update pass index
	m_mainRenderer.addDepthPass(shadowPass);
	shadowPassIdx = m_mainRenderer.getDepthPassCount() + m_mainRenderer.getRenderPassCount() + m_mainRenderer.getComputePassCount();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																																																		   //
	//																						Main Pass																										   //
	//																																																		   //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Create GBuffer layout
	FBOLayout GBuffer =
	{
		{AttachmentType::ColourHDR, true},
		{AttachmentType::ColourHDR, true},
		{AttachmentType::ColourHDR, true},
		{AttachmentType::ColourHDR, true},
		{AttachmentType::Depth, true}
	};

	//Main render pass
	RenderPass mainPass;
	mainPass.scene = m_mainScene;
	mainPass.parseScene();
	mainPass.target = std::make_shared<FBO>(m_winRef.getSize(), GBuffer);
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
	mainPassIdx = m_mainRenderer.getRenderPassCount() + m_mainRenderer.getComputePassCount();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																																																		   //
	//																						Skybox Pass																										   //
	//																																																		   //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Create skybox buffer
	FBOLayout skyboxLayout =
	{
		{AttachmentType::ColourHDR, true}
	};

	//Reset skybox scene
	m_skyboxScene.reset(new Scene);

	//Add skybox to scene
	m_skyboxScene->m_actors.push_back(skybox);

	//Create skybox pass
	RenderPass skyboxPass;
	skyboxPass.scene = m_skyboxScene;
	skyboxPass.parseScene();
	skyboxPass.target = std::make_shared<FBO>(m_winRef.getSize(), skyboxLayout);
	skyboxPass.camera.projection = glm::perspective(45.f, m_winRef.getWidthf() / m_winRef.getHeightf(), 0.1f, 1000.f);
	skyboxPass.viewPort = { 0, 0, m_winRef.getWidth(), m_winRef.getHeight() };
	skyboxPass.camera.updateView(m_mainScene->m_actors.at(cameraIdx).transform);
	skyboxPass.UBOmanager.setCachedValue("b_camera", "u_view", skyboxPass.camera.view);
	skyboxPass.UBOmanager.setCachedValue("b_camera", "u_projection", skyboxPass.camera.projection);
	skyboxPass.UBOmanager.setCachedValue("b_camera", "u_viewPos", m_mainScene->m_actors.at(cameraIdx).translation);

	//Update index and add to render
	m_mainRenderer.addRenderPass(skyboxPass);
	skyboxPassIdx = m_mainRenderer.getRenderPassCount() + m_mainRenderer.getComputePassCount();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																																																		   //
	//																						Create Screen Quad (with light pass shader)																		   //
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

	//Shader description
	ShaderDescription lightPassShaderDesc = CreateShaderDesc(ShaderType::rasterization, "./assets/shaders/lab2/screenVert.glsl", "./assets/shaders/lab4/lightPassFrag.glsl");

	//Shader
	std::shared_ptr<Shader> lightPassShader = CreateShader(lightPassShaderDesc);

	//Material
	std::shared_ptr<Material> lightPassMaterial;
	lightPassMaterial = std::make_shared<Material>(lightPassShader);
	lightPassMaterial->setValue("u_posInWs", mainPass.target->getTarget(0));
	lightPassMaterial->setValue("u_normal", mainPass.target->getTarget(1));
	lightPassMaterial->setValue("u_diffSpec", mainPass.target->getTarget(2));
	lightPassMaterial->setValue("u_lightSpaceTransform", shadowPass.camera.projection * shadowPass.camera.view);
	lightPassMaterial->setValue("u_shadowMap", shadowPass.target->getTarget(0));
	lightPassMaterial->setValue("u_depth", mainPass.target->getTarget(4));
	lightPassMaterial->setValue("u_skyboxColour", skyboxPass.target->getTarget(0));
	lightPassMaterial->setValue("u_metallicRoughness", mainPass.target->getTarget(3));

	//Actor
	Actor screenQuad;
	screenQuad.geometry = screenQuadVAO;
	screenQuad.material = lightPassMaterial;
	screenQuad.recalc();

	//Push quad to scene
	m_screenScene->m_actors.push_back(screenQuad);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																																																		   //
	//																						Light Pass																										   //
	//																																																		   //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Frame buffer layout
	FBOLayout typicalLayout =
	{
		{AttachmentType::ColourHDR, true},
		{AttachmentType::Depth, true}
	};

	//Light pass
	RenderPass lightPass;
	lightPass.scene = m_screenScene;
	lightPass.parseScene();
	lightPass.target = std::make_shared<FBO>(m_winRef.getSize(), typicalLayout);
	lightPass.viewPort = { 0, 0, m_winRef.getWidth(), m_winRef.getHeight() };
	lightPass.camera.projection = glm::ortho(0.0f, width, height, 0.0f);
	lightPass.UBOmanager.setCachedValue("b_camera2D", "u_view2D", lightPass.camera.view);
	lightPass.UBOmanager.setCachedValue("b_camera2D", "u_projection2D", lightPass.camera.projection);
	lightPass.UBOmanager.setCachedValue("b_lights", "dLight.colour", m_mainScene->m_directionalLights.at(0).colour);
	lightPass.UBOmanager.setCachedValue("b_lights", "dLight.direction", m_mainScene->m_directionalLights.at(0).direction);
	for (int i = 0; i < numPointLights; i++)
	{
		lightPass.UBOmanager.setCachedValue("b_lights", "pLights[" + std::to_string(i) + "].colour", m_mainScene->m_pointLights.at(i).colour);
		lightPass.UBOmanager.setCachedValue("b_lights", "pLights[" + std::to_string(i) + "].position", m_mainScene->m_pointLights.at(i).position);
		lightPass.UBOmanager.setCachedValue("b_lights", "pLights[" + std::to_string(i) + "].constants", m_mainScene->m_pointLights.at(i).constants);
	}

	//Add to render and update index
	m_mainRenderer.addRenderPass(lightPass);
	lightPassIdx = m_mainRenderer.getRenderPassCount() + m_mainRenderer.getComputePassCount();


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
	visualizeShadowPass.UBOmanager.setCachedValue("b_camera2D", "u_view2D", visualizeShadowPass.camera.view);
	visualizeShadowPass.UBOmanager.setCachedValue("b_camera2D", "u_projection2D", visualizeShadowPass.camera.projection);

	//add to render and update index
	m_mainRenderer.addRenderPass(visualizeShadowPass);
	visualizeShadowIdx = m_mainRenderer.getRenderPassCount() + m_mainRenderer.getComputePassCount();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																																																		   //
	//																						Fog Pass																										   //
	//																																																		   //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Reset scene
	m_screenScene.reset(new Scene);

	//Shader desc
	ShaderDescription fogShaderDesc = CreateShaderDesc(ShaderType::rasterization, "./assets/shaders/lab2/screenVert.glsl", "./assets/shaders/lab3/fogFrag.glsl");

	//Shader
	std::shared_ptr<Shader> fogShader = CreateShader(fogShaderDesc);

	//Material
	std::shared_ptr<Material> fogMaterial;
	fogMaterial = std::make_shared<Material>(fogShader);
	fogMaterial->setValue("u_inputTexture", lightPass.target->getTarget(0));
	fogMaterial->setValue("u_depthTexture", mainPass.target->getTarget(4));

	//Apply material
	screenQuad.material = fogMaterial;

	//Add to scene
	m_screenScene->m_actors.push_back(screenQuad);

	//Pass
	RenderPass fogPass;
	fogPass.scene = m_screenScene;
	fogPass.parseScene();
	fogPass.target = std::make_shared<FBO>(m_winRef.getSize(), typicalLayout);
	fogPass.viewPort = { 0, 0, m_winRef.getWidth(), m_winRef.getHeight() };
	fogPass.camera.projection = glm::ortho(0.0f, width, height, 0.0f);
	fogPass.UBOmanager.setCachedValue("b_camera2D", "u_view2D", fogPass.camera.view);
	fogPass.UBOmanager.setCachedValue("b_camera2D", "u_projection2D", fogPass.camera.projection);

	//Add to render and update index
	m_mainRenderer.addRenderPass(fogPass);
	fogPassIdx = m_mainRenderer.getRenderPassCount() + m_mainRenderer.getComputePassCount();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																																																		   //
	//																						Ultra Post Process Pass																							   //
	//																																																		   //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//New fbo
	FBOLayout postProcessLayout =
	{
		{AttachmentType::ColourHDR, true},	//Normal output
		{AttachmentType::ColourHDR, true},	//In focus no effects
		{AttachmentType::ColourHDR, true}	//Out of focus with gaussian blur
	};

	//Reset scene
	m_screenScene.reset(new Scene);

	//Shader description
	ShaderDescription ultraPPShaderDesc = CreateShaderDesc(ShaderType::rasterization, "./assets/shaders/lab2/screenVert.glsl", "./assets/shaders/lab5/ultraPPFrag.glsl");

	//Shader
	std::shared_ptr<Shader> ultraPPShader = CreateShader(ultraPPShaderDesc);

	//Material
	std::shared_ptr<Material> ultraPPMaterial;
	ultraPPMaterial = std::make_shared<Material>(ultraPPShader);
	ultraPPMaterial->setValue("u_inputTexture", fogPass.target->getTarget(0));
	ultraPPMaterial->setValue("u_screenSize", glm::vec2(width, height));

	//Apply material
	screenQuad.material = ultraPPMaterial;

	//Push to scene
	m_screenScene->m_actors.push_back(screenQuad);

	//Create render pass
	RenderPass ultraPPPass;
	ultraPPPass.scene = m_screenScene;
	ultraPPPass.parseScene();
	ultraPPPass.target = std::make_shared<FBO>(m_winRef.getSize(), postProcessLayout);
	ultraPPPass.viewPort = { 0, 0, m_winRef.getWidth(), m_winRef.getHeight() };
	ultraPPPass.camera.projection = glm::ortho(0.0f, width, height, 0.0f);
	ultraPPPass.UBOmanager.setCachedValue("b_camera2D", "u_view2D", ultraPPPass.camera.view);
	ultraPPPass.UBOmanager.setCachedValue("b_camera2D", "u_projection2D", ultraPPPass.camera.projection);

	//Add to render and update index
	m_mainRenderer.addRenderPass(ultraPPPass);
	ultraPPPassIdx = m_mainRenderer.getRenderPassCount() + m_mainRenderer.getComputePassCount();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																																																		   //
	//																						Depth of Field Pass 																							   //
	//																																																		   //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Reset scene
	m_screenScene.reset(new Scene);

	//Shader desc
	ShaderDescription dofShaderDesc = CreateShaderDesc(ShaderType::rasterization, "./assets/shaders/lab2/screenVert.glsl", "./assets/shaders/lab3/dofFrag.glsl");

	//Shader
	std::shared_ptr<Shader> dofShader = CreateShader(dofShaderDesc);

	//Material
	std::shared_ptr<Material> dofMaterial;
	dofMaterial = std::make_shared<Material>(dofShader);
	dofMaterial->setValue("u_inFocus", ultraPPPass.target->getTarget(0));
	dofMaterial->setValue("u_outOfFocus", ultraPPPass.target->getTarget(2));
	dofMaterial->setValue("u_depthTexture", mainPass.target->getTarget(4));

	//Apply material
	screenQuad.material = dofMaterial;

	//push to scene
	m_screenScene->m_actors.push_back(screenQuad);

	//Create pass
	RenderPass dofPass;
	dofPass.scene = m_screenScene;
	dofPass.parseScene();
	dofPass.target = std::make_shared<FBO>(m_winRef.getSize(), typicalLayout);
	dofPass.viewPort = { 0, 0, m_winRef.getWidth(), m_winRef.getHeight() };
	dofPass.camera.projection = glm::ortho(0.0f, width, height, 0.0f);
	dofPass.UBOmanager.setCachedValue("b_camera2D", "u_view2D", dofPass.camera.view);
	dofPass.UBOmanager.setCachedValue("b_camera2D", "u_projection2D", dofPass.camera.projection);

	//Add to render and update index
	m_mainRenderer.addRenderPass(dofPass);
	dofPassIdx = m_mainRenderer.getRenderPassCount() + m_mainRenderer.getComputePassCount();

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
	screenMaterial->setValue("u_inputTexture", dofPass.target->getTarget(0));

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
	screenPass.UBOmanager.setCachedValue("b_camera2D", "u_view2D", screenPass.camera.view);
	screenPass.UBOmanager.setCachedValue("b_camera2D", "u_projection2D", screenPass.camera.projection);

	//Update index and add render pass
	m_mainRenderer.addRenderPass(screenPass);
	screenPassIdx = m_mainRenderer.getRenderPassCount() + m_mainRenderer.getComputePassCount();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																																																		   //
	//																						Scripts																											   //
	//																																																		   //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Attach camera script for WASD movement
	m_mainScene->m_actors.at(cameraIdx).attachScript<CameraScript>(mainPass.scene->m_actors.at(cameraIdx), m_winRef, glm::vec3(1.6f, 0.6f, 2.f), 0.5f);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																																																		   //
	//																						Render Any Initial Renderers																					   //
	//																																																		   //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}



void Lab5::onRender() const
{
	m_mainRenderer.render();
}

void Lab5::onUpdate(float timestep)
{
	//Get all passes
	//Shadow pass
	auto& shadowPass = m_mainRenderer.getDepthPass(shadowPassIdx);

	//Skybox pass
	auto& skyboxPass = m_mainRenderer.getRenderPass(skyboxPassIdx);
	
	//Main pass
	auto& mainPass = m_mainRenderer.getRenderPass(mainPassIdx);

	//Height compute pass
	auto& heightComputePass = m_mainRenderer.getComputePass(0);

	//Particle compute pass
	auto& particleComputePass = m_mainRenderer.getComputePass(1);

	//Light pass
	auto& lightPass = m_mainRenderer.getRenderPass(lightPassIdx);

	//Fog pass
	auto& fogPass = m_mainRenderer.getRenderPass(fogPassIdx);

	//Post process pass
	auto& ultraPPPass = m_mainRenderer.getRenderPass(ultraPPPassIdx);

	//Dof pass
	auto& dofPass = m_mainRenderer.getRenderPass(dofPassIdx);
	
	//Screen pass
	auto& screenPass = m_mainRenderer.getRenderPass(screenPassIdx);
	
	//Get camera
	auto& camera = m_mainScene->m_actors.at(cameraIdx);
	
	//Update height compute pass
	auto& heightComputeMaterial = heightComputePass.material;
	heightComputeMaterial->setValue("u_heightMapScale", m_heightMapScale);
	heightComputeMaterial->setValue("u_frequency", m_frequency);
	heightComputeMaterial->setValue("u_amplitude", m_amplitude);
	heightComputeMaterial->setValue("u_octaves", abs(m_octaves));
	heightComputeMaterial->setValue("u_lacunarity", m_lacunarity);
	heightComputeMaterial->setValue("u_persitance", m_persitance);

	//Update particle compute pass
	auto& particleMaterial = particleComputePass.material;
	particleMaterial->setValue("deltaTime", timestep);

	//Update particle render material uniforms
	auto& particleRenderMaterial = m_mainScene->m_actors.at(particlesIdx).material;
	float snowActiveValue = m_snowActive ? 1.0 : 0.0;
	particleRenderMaterial->setValue("u_active", snowActiveValue);
	particleRenderMaterial->setValue("u_particleColour", m_particleColour);

	//Apply random wind to particles
	if (m_windTimer >= m_windInterval)
	{
		//Need to lerp between old velocity and new velocity to simulate wind change
		m_windTimer = 0.0f;
	}
	else
	{
		m_windTimer += timestep;
	}

	//Update floor values
	auto& floorMat = m_mainRenderer.getRenderPass(mainPassIdx).scene->m_actors.at(floorIdx).material;
	floorMat->setValue("u_viewPosTCS", m_mainScene->m_actors.at(cameraIdx).translation);
	floorMat->setValue("u_heightMapScale", m_heightMapScale);
	floorMat->setValue("u_perVertexNormals", heightComputePass.images[1].texture);
	floorMat->setValue("u_heightMap", heightComputePass.images[0].texture);

	//Update light space pass uniforms
	auto& lightPassMat = lightPass.scene->m_actors.at(0).material;
	lightPassMat->setValue("u_lightSpaceTransform", shadowPass.camera.projection * shadowPass.camera.view);
	lightPassMat->setValue("u_viewPos", mainPass.scene->m_actors.at(cameraIdx).translation);

	//Run all update functions
	for (auto it = m_mainScene->m_actors.begin(); it != m_mainScene->m_actors.end(); ++it)
	{
		it->onUpdate(timestep);
	}





	//Post processing update 
	//Post process material
	auto& ultraPPPassMat = ultraPPPass.scene->m_actors.at(0).material;

	//Wireframe
	mainPass.drawInWireFrame = m_wireFrame;

	//Edge detection
	float edgeDetectionValue = m_edgeDetection ? 1.0f : 0.0f;
	ultraPPPassMat->setValue("u_edgeDetectionActive", edgeDetectionValue);

	//Blur
	float blurValue = m_blur ? 1.0f : 0.0f;
	ultraPPPassMat->setValue("u_blurActive", blurValue);
	ultraPPPassMat->setValue("u_blurRadius", m_blurRadius);

	//Vignette
	float vignetteValue = m_vignette ? 1.0f : 0.0f;
	ultraPPPassMat->setValue("u_vignetteActive", vignetteValue);
	ultraPPPassMat->setValue("u_vigInnerRadius", m_vignetteControls.x);
	ultraPPPassMat->setValue("u_vigOuterRadius", m_vignetteControls.y);
	ultraPPPassMat->setValue("u_vigIntensity", m_vignetteIntensity);

	//Sepia
	float sepiaValue = m_sepia ? 1.0f : 0.0f;
	ultraPPPassMat->setValue("u_sepiaActive", sepiaValue);

	//Depth of field
	auto& dofMat = dofPass.scene->m_actors.at(0).material;
	float dofValue = m_depthOfField ? 1.0f : 0.0f;
	dofMat->setValue("u_active", dofValue);
	dofMat->setValue("u_focalRange", m_dofFocalRange);
	dofMat->setValue("u_focalDistance", m_dofFocusDistance);
	ultraPPPassMat->setValue("u_dofBlurRadius", m_dofBlurAmmount);

	//Fog
	auto& fogMaterial = fogPass.scene->m_actors.at(0).material;
	float fogValue = m_fog ? 1.0f : 0.0f;
	fogMaterial->setValue("u_active", fogValue);
	fogMaterial->setValue("u_fogColour", m_fogColour);
	fogMaterial->setValue("u_fogDensity", m_fogDensity);




	//Skybox
	auto& skybox = m_skyboxScene->m_actors.at(0);
	skybox.material->setValue("u_skyboxView", glm::mat3(glm::mat3(mainPass.camera.view)));

	////Update tonemapping mode
	auto& screenQuadMat = screenPass.scene->m_actors.at(0).material;
	screenQuadMat->setValue("u_tonemappingMode", m_tonemappingMode);

	//Point light constants
	for (int i = 0; i < numPointLights; i++)
	{
		auto& pointLight = m_mainScene->m_pointLights.at(i);
		pointLight.constants = m_pLightConstants;
		lightPass.UBOmanager.setCachedValue("b_lights", "pLights[" + std::to_string(i) + "].constants", m_mainScene->m_pointLights.at(i).constants);
	}

	//Update directional light colour
	auto& directionalLight = m_mainScene->m_directionalLights.at(0);
	directionalLight.direction = m_dlDirection;
	lightPass.UBOmanager.setCachedValue("b_lights", "dLight.colour", m_dlColour);
	lightPass.UBOmanager.setCachedValue("b_lights", "dLight.direction", m_dlDirection);

	//Adjust camera view and position
	mainPass.camera.updateView(camera.transform);
	mainPass.UBOmanager.setCachedValue("b_camera", "u_view", mainPass.camera.view);
	mainPass.UBOmanager.setCachedValue("b_camera", "u_viewPos", camera.translation);

	//Adjust camera view for light pass
	//lightPass.camera.updateView(camera.transform);
	lightPass.UBOmanager.setCachedValue("b_camera", "u_view", mainPass.camera.view);
	lightPass.UBOmanager.setCachedValue("b_camera", "u_viewPos", camera.translation);

	//Update skybox
	skyboxPass.UBOmanager.setCachedValue("b_camera", "u_view", mainPass.camera.view);
	skyboxPass.UBOmanager.setCachedValue("b_camera", "u_viewPos", camera.translation);

	//Update shadow map
	glm::vec3 lightPosition = (m_shadowMapVars.centre - glm::normalize(m_mainScene->m_directionalLights.at(0).direction)) * m_shadowMapVars.distanceAlongLightVector;
	glm::mat4 shadowView = glm::lookAt(lightPosition, m_shadowMapVars.centre, m_shadowMapVars.UP);
	shadowPass.camera.view = shadowView;
	shadowPass.UBOmanager.setCachedValue("b_LightCamera", "u_view", shadowPass.camera.view);
}

void Lab5::onKeyPressed(KeyPressedEvent& e)
{
	for (auto it = m_mainScene->m_actors.begin(); it != m_mainScene->m_actors.end(); ++it)
	{
		it->onKeyPress(e);
	}
}

void Lab5::onImGUIRender()
{
	//Milliseconds
	float ms = 1000.0f / ImGui::GetIO().Framerate;

	//Create new frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//Name
	ImGui::Begin("GAMR3521");

	//Display fps
	ImGui::Text("FPS %.3 ms/frame (%.1f FPS)", ms, ImGui::GetIO().Framerate);

	if (ImGui::BeginTabBar("Tabs"))
	{
		if (ImGui::BeginTabItem("Lights"))
		{
			//Directional light colour
			ImGui::ColorEdit3("Directional Light Colour", (float*)&m_dlColour);

			//Directional light direction
			ImGui::SliderFloat3("Directional Light Direction", glm::value_ptr(m_dlDirection), -1.0f, 1.0f);

			//Point light constants
			ImGui::SliderFloat("Point Light x Constant", &m_pLightConstants.x, 0, 30.0f);

			//End tab
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Geometry"))
		{
			//Wireframe checkbox
			ImGui::Checkbox("Wireframe ", &m_wireFrame);

			//Height map scale
			ImGui::SliderFloat("Height Map Scale", &m_heightMapScale, 0.0f, 100.0f);

			//End tab
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Visualizations"))
		{
			//Display shadow map
			GLuint textureID = m_mainRenderer.getRenderPass(visualizeShadowIdx).target->getTarget(0)->getID();
			ImVec2 imageSize = ImVec2(256, 256);
			ImVec2 uv0 = ImVec2(0.0f, 1.0f);
			ImVec2 uv1 = ImVec2(1.0f, 0.0f);
			ImGui::Image((void*)(intptr_t)textureID, imageSize, uv0, uv1);
			ImGui::SameLine();

			//Display gPosition
			textureID = m_mainRenderer.getRenderPass(mainPassIdx).target->getTarget(0)->getID();
			ImGui::Image((void*)(intptr_t)textureID, imageSize, uv0, uv1);
			ImGui::SameLine();

			//Display gNormal
			textureID = m_mainRenderer.getRenderPass(mainPassIdx).target->getTarget(1)->getID();
			ImGui::Image((void*)(intptr_t)textureID, imageSize, uv0, uv1);

			//Display gDiffSpec
			textureID = m_mainRenderer.getRenderPass(mainPassIdx).target->getTarget(2)->getID();
			ImGui::Image((void*)(intptr_t)textureID, imageSize, uv0, uv1);
			ImGui::SameLine();

			//End tab
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Post Processing"))
		{
			//Choose tonemapping method
			static int tonemappingMode = 5;
			const char* tonemappingModes[9] = { "ACES", "UNCHARTED2", "REINHARD", "FILMIC", "LOTTES", "NEUTRAL", "REINHART2", "UCHIMURA", "UNREAL" };
			if (ImGui::Combo("Tonemapping Mode", &tonemappingMode, tonemappingModes, IM_ARRAYSIZE(tonemappingModes)))
			{
				m_tonemappingMode = tonemappingMode;
			}

			//Edge detection
			ImGui::Checkbox("Edge Detection", &m_edgeDetection);

			//Blur
			ImGui::Checkbox("Blur", &m_blur);
			ImGui::SliderFloat("Blur Radius (BE CAREFUL!)", &m_blurRadius, 0.001f, 75.0f);

			//Vignette
			ImGui::Checkbox("Vignette", &m_vignette);
			ImGui::SliderFloat2("Vignette Inner/Outer", glm::value_ptr(m_vignetteControls), 0.0f, 1.0f);
			ImGui::SliderFloat("Vignette Intensity", &m_vignetteIntensity, 0.0f, 1.0f);

			//Sepia
			ImGui::Checkbox("Sepia Colours", &m_sepia);

			//Depth of field
			ImGui::Checkbox("Depth Of Field", &m_depthOfField);
			ImGui::SliderFloat("Blur Ammount", &m_dofBlurAmmount, 0.001f, 75.0f);
			ImGui::SliderFloat("Focus Distance", &m_dofFocusDistance, 0.0f, 500.0f);
			ImGui::SliderFloat("Focal Range", &m_dofFocalRange, 0.0f, 500.0f);

			//Fog
			ImGui::Checkbox("Fog", &m_fog);
			ImGui::SliderFloat3("Fog Colour", glm::value_ptr(m_fogColour), 0.0f, 1.0f);
			ImGui::SliderFloat("Fog Density", &m_fogDensity, 0.0f, 0.01f);

			//End tab
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Noise"))
		{
			//Frequency
			ImGui::SliderFloat("Frequency", &m_frequency, 0.0f, 100.0f);

			//Amplitude
			ImGui::SliderFloat("Amplitude", &m_amplitude, 0.0f, 20.0f);

			//Octaves
			ImGui::SliderFloat("Octaves", &m_octaves, 0.0f, 10.0f);

			//Lacunarity
			ImGui::SliderFloat("Lacunarity", &m_lacunarity, 0.0f, 5.0f);

			//Persitance
			ImGui::SliderFloat("Persitance", &m_persitance, 0.0f, 5.0f);

			GLuint textureID = m_mainRenderer.getComputePass(0).images[0].texture->getID();
			ImVec2 imageSize = ImVec2(512, 512);
			ImVec2 uv0 = ImVec2(0.0f, 1.0f);
			ImVec2 uv1 = ImVec2(1.0f, 0.0f);
			ImGui::Image((void*)(intptr_t)textureID, imageSize, uv0, uv1);
			ImGui::SameLine();

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Particles"))
		{
			//Snow checkbox
			ImGui::Checkbox("Snow", &m_snowActive);

			//Particle size
			ImGui::SliderFloat3("Particle Colour", glm::value_ptr(m_particleColour), 0.1f, 1.0f);

			ImGui::EndTabItem();
		}

		//End tab bar
		ImGui::EndTabBar();
	}

	//End/Render
	ImGui::End();
	ImGui::Render();
}

ShaderDescription Lab5::CreateShaderDesc(ShaderType type, std::string vertexPath, std::string fragPath)
{
	ShaderDescription shaderDesc;
	shaderDesc.type = type;
	shaderDesc.vertexSrcPath = vertexPath;
	shaderDesc.fragmentSrcPath = fragPath;
	return shaderDesc;
}

std::shared_ptr<Shader> Lab5::CreateShader(ShaderDescription shaderDesc)
{
	std::shared_ptr<Shader> newShader;
	newShader = std::make_shared<Shader>(shaderDesc);
	return newShader;
}

std::shared_ptr<VAO> Lab5::CreateVAO(std::vector<unsigned int> indicies, std::vector<float> vertices, VBOLayout layout)
{
	std::shared_ptr<VAO> newVAO;
	newVAO = std::make_shared<VAO>(indicies);
	newVAO->addVertexBuffer(vertices, layout);
	return newVAO;
}

Actor Lab5::CreateActor(std::shared_ptr<VAO> Vao, std::shared_ptr<VAO> depthVao, std::shared_ptr<Material> material,
	std::shared_ptr<Material> depthMat, glm::vec3 translation, glm::vec3 scale)
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

int Lab5::AddActorToRender(Actor actor)
{
	int index = m_mainScene->m_actors.size();
	m_mainScene->m_actors.push_back(actor);
	return index;
}
