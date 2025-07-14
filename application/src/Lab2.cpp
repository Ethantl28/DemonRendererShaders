#include "Lab2.hpp"
#include "scripts/include/camera.hpp"
#include "scripts/include/rotation.hpp"
#include <numeric> // For std::iota
#include <glm/gtc/matrix_transform.hpp>
#include "Grid.hpp"
#include <iostream>

Lab2::Lab2(GLFWWindowImpl& win) : Layer(win)
{
	//Reset scenes
	m_lab2Scene.reset(new Scene);

	//Camera
	Actor camera;

	//Character
	//Shader description
	ShaderDescription charShaderDesc = CreateShaderDesc(ShaderType::rasterization, "./assets/shaders/Lab1/charVertex.glsl", "./assets/shaders/lab1/charFrag.glsl");

	//Create the character
	Actor character = CreateCharacter(charShaderDesc);

	//Floor
	//Shader description
	ShaderDescription floorShaderDesc = CreateShaderDesc(ShaderType::rasterization, "./assets/shaders/Lab1/floorVertex.glsl", "./assets/shaders/lab1/floorFrag.glsl");

	//Cretae the floor
	Actor floor = CreateFloor(floorShaderDesc);

	//Skybox
	//Shader description
	ShaderDescription skyboxShaderDesc = CreateShaderDesc(ShaderType::rasterization, "./assets/shaders/Lab1/skyboxVertex.glsl", "./assets/shaders/lab1/skyboxFrag.glsl");

	//Create the skybox
	Actor skybox = CreateSkybox(skyboxShaderDesc);

	//Add to actors vector
	cameraIdx = AddActorToRender(camera);
	floorIdx = AddActorToRender(floor);
	charIdx = AddActorToRender(character);
	skyIdx = AddActorToRender(skybox);

	//Lights
	//Directional
	DirectionalLight dl;
	dl.direction = glm::normalize(glm::vec3(-0.2, -1.0, -0.5));
	m_lab2Scene->m_directionalLights.push_back(dl);

	//Point
	PointLight pointLight;
	for (int i = 0; i < numPointLights; i++)
	{
		pointLight.colour = glm::vec3(Randomiser::uniformFloatBetween(0.0f, 1.0f), Randomiser::uniformFloatBetween(0.0f, 1.0f), Randomiser::uniformFloatBetween(0.0f, 1.0f));
		pointLight.position = glm::vec3(Randomiser::uniformFloatBetween(-30.0f, 30.0f), -4.5f, Randomiser::uniformFloatBetween(-30.0f, 30.0f));
		pointLight.constants = m_pLightConstants;
		m_lab2Scene->m_pointLights.push_back(pointLight);
	}

	//Frame buffer layout
	FBOLayout typicalLayout =
	{
		{AttachmentType::ColourHDR, true},
		{AttachmentType::Depth, false}
	};

	//Main render pass
	RenderPass mainPass;
	mainPass.scene = m_lab2Scene;
	mainPass.parseScene();
	//mainPass.target = std::make_shared<FBO>();
	mainPass.target = std::make_shared<FBO>(m_winRef.getSize(), typicalLayout);
	mainPass.camera.projection = glm::perspective(45.f, m_winRef.getWidthf() / m_winRef.getHeightf(), 0.1f, 1000.f);
	mainPass.viewPort = { 0, 0, m_winRef.getWidth(), m_winRef.getHeight() };
	mainPass.camera.updateView(m_lab2Scene->m_actors.at(cameraIdx).transform);
	mainPass.UBOmanager.setCachedValue("b_camera", "u_view", mainPass.camera.view);
	mainPass.UBOmanager.setCachedValue("b_camera", "u_projection", mainPass.camera.projection);
	mainPass.UBOmanager.setCachedValue("b_camera", "u_viewPos", m_lab2Scene->m_actors.at(cameraIdx).translation);
	mainPass.UBOmanager.setCachedValue("b_lights", "dLight.colour", m_lab2Scene->m_directionalLights.at(0).colour);
	mainPass.UBOmanager.setCachedValue("b_lights", "dLight.direction", m_lab2Scene->m_directionalLights.at(0).direction);
	for (int i = 0; i < numPointLights; i++)
	{
		mainPass.UBOmanager.setCachedValue("b_lights", "pLights[" + std::to_string(i) + "].colour", m_lab2Scene->m_pointLights.at(i).colour);
		mainPass.UBOmanager.setCachedValue("b_lights", "pLights[" + std::to_string(i) + "].position", m_lab2Scene->m_pointLights.at(i).position);
		mainPass.UBOmanager.setCachedValue("b_lights", "pLights[" + std::to_string(i) + "].constants", m_lab2Scene->m_pointLights.at(i).constants);
	}

	//Set previous pass
	previousPass = mainPass;

	//Get screen dimensions
	float width = m_winRef.getWidthf();
	float height = m_winRef.getHeightf();

	//Reset post process scene
	m_screenScene.reset(new Scene);

	//Shader description for invert colour shader
	ShaderDescription invertColourDesc = CreateShaderDesc(ShaderType::rasterization, "./assets/shaders/lab2/screenVert.glsl", "./assets/shaders/lab2/invertFrag.glsl");

	//Invert quad setup
	Actor screenQuad = CreateScreenQuad(invertColourDesc, width, height, previousPass);

	//Pushback to m_actors
	m_screenScene->m_actors.push_back(screenQuad);

	//Create invert colour render pass
	RenderPass invertPass;
	invertPass.scene = m_screenScene;
	invertPass.parseScene();
	invertPass.target = std::make_shared<FBO>(m_winRef.getSize(), typicalLayout);
	invertPass.viewPort = { 0, 0, m_winRef.getWidth(), m_winRef.getHeight() };
	invertPass.camera.projection = glm::ortho(0.0f, width, height, 0.0f);
	invertPass.UBOmanager.setCachedValue("b_camera2D", "u_view", invertPass.camera.view);
	invertPass.UBOmanager.setCachedValue("b_camera2D", "u_projection", invertPass.camera.projection);

	//Update previous pass
	previousPass = invertPass;

	//Reset scene
	m_screenScene.reset(new Scene);

	//Shader description for relative luminance shader
	ShaderDescription relativeLuminanceDesc = CreateShaderDesc(ShaderType::rasterization, "./assets/shaders/lab2/screenVert.glsl", "./assets/shaders/lab2/relativeLuminanceFrag.glsl");

	//Create and change material for screenQuad
	std::shared_ptr<Material> rlMaterial = CreateScreenQuadMaterial(relativeLuminanceDesc);
	screenQuad.material = rlMaterial;

	//Push quad to scene
	m_screenScene->m_actors.push_back(screenQuad);

	//Relative luminance pass
	RenderPass relativeLuminancePass;
	relativeLuminancePass.scene = m_screenScene;
	relativeLuminancePass.parseScene();
	relativeLuminancePass.target = std::make_shared<FBO>(m_winRef.getSize(), typicalLayout);
	relativeLuminancePass.viewPort = { 0, 0, m_winRef.getWidth(), m_winRef.getHeight() };
	relativeLuminancePass.camera.projection = glm::ortho(0.0f, width, height, 0.0f);
	relativeLuminancePass.UBOmanager.setCachedValue("b_camera2D", "u_view", relativeLuminancePass.camera.view);
	relativeLuminancePass.UBOmanager.setCachedValue("b_camera2D", "u_projection", relativeLuminancePass.camera.projection);

	//Update previous pass
	previousPass = relativeLuminancePass;

	//Reset scene
	m_screenScene.reset(new Scene);

	//Shader description for matrix shader
	ShaderDescription matrixShaderDesc = CreateShaderDesc(ShaderType::rasterization, "./assets/shaders/lab2/screenVert.glsl", "./assets/shaders/lab2/matrixFrag.glsl");

	//Create and change material for screenQuad
	std::shared_ptr<Material> matrixMaterial = CreateScreenQuadMaterial(matrixShaderDesc);
	screenQuad.material = matrixMaterial;

	//Push quad to scene
	m_screenScene->m_actors.push_back(screenQuad);

	//Relative luminance pass
	RenderPass matrixPass;
	matrixPass.scene = m_screenScene;
	matrixPass.parseScene();
	matrixPass.target = std::make_shared<FBO>(m_winRef.getSize(), typicalLayout);
	matrixPass.viewPort = { 0, 0, m_winRef.getWidth(), m_winRef.getHeight() };
	matrixPass.camera.projection = glm::ortho(0.0f, width, height, 0.0f);
	matrixPass.UBOmanager.setCachedValue("b_camera2D", "u_view", matrixPass.camera.view);
	matrixPass.UBOmanager.setCachedValue("b_camera2D", "u_projection", matrixPass.camera.projection);

	//Update previous pass
	previousPass = matrixPass;

	//Reset scene
	m_screenScene.reset(new Scene);

	//Shader description for blur shader
	ShaderDescription blurShaderDesc = CreateShaderDesc(ShaderType::rasterization, "./assets/shaders/lab2/screenVert.glsl", "./assets/shaders/lab2/blurFrag.glsl");

	//Create and change material for screenQuad
	std::shared_ptr<Material> blurMaterial = CreateScreenQuadMaterial(blurShaderDesc);
	blurMaterial->setValue("u_screenSize", glm::vec2(width, height));
	screenQuad.material = blurMaterial;

	//Push quad to scene
	m_screenScene->m_actors.push_back(screenQuad);

	//Relative luminance pass
	RenderPass blurPass;
	blurPass.scene = m_screenScene;
	blurPass.parseScene();
	blurPass.target = std::make_shared<FBO>(m_winRef.getSize(), typicalLayout);
	blurPass.viewPort = { 0, 0, m_winRef.getWidth(), m_winRef.getHeight() };
	blurPass.camera.projection = glm::ortho(0.0f, width, height, 0.0f);
	blurPass.UBOmanager.setCachedValue("b_camera2D", "u_view", blurPass.camera.view);
	blurPass.UBOmanager.setCachedValue("b_camera2D", "u_projection", blurPass.camera.projection);

	//Update previous pass
	previousPass = blurPass;

	//Reset scene
	m_screenScene.reset(new Scene);

	//Shader description for vignette shader
	ShaderDescription sepiaShaderDesc = CreateShaderDesc(ShaderType::rasterization, "./assets/shaders/lab2/screenVert.glsl", "./assets/shaders/lab2/sepiaFrag.glsl");

	//Create and change material for screenQuad
	std::shared_ptr<Material> sepiaMaterial = CreateScreenQuadMaterial(sepiaShaderDesc);
	screenQuad.material = sepiaMaterial;

	//Push quad to scene
	m_screenScene->m_actors.push_back(screenQuad);

	//Relative luminance pass
	RenderPass sepiaPass;
	sepiaPass.scene = m_screenScene;
	sepiaPass.parseScene();
	sepiaPass.target = std::make_shared<FBO>(m_winRef.getSize(), typicalLayout);
	sepiaPass.viewPort = { 0, 0, m_winRef.getWidth(), m_winRef.getHeight() };
	sepiaPass.camera.projection = glm::ortho(0.0f, width, height, 0.0f);
	sepiaPass.UBOmanager.setCachedValue("b_camera2D", "u_view", sepiaPass.camera.view);
	sepiaPass.UBOmanager.setCachedValue("b_camera2D", "u_projection", sepiaPass.camera.projection);

	//Update previous pass
	previousPass = sepiaPass;

	//Reset scene
	m_screenScene.reset(new Scene);

	//Shader description for vignette shader
	ShaderDescription edgeDetectionShaderDesc = CreateShaderDesc(ShaderType::rasterization, "./assets/shaders/lab2/screenVert.glsl", "./assets/shaders/lab2/edgeDetectionFrag.glsl");

	//Create and change material for screenQuad
	std::shared_ptr<Material> edgeDetectionMaterial = CreateScreenQuadMaterial(edgeDetectionShaderDesc);
	edgeDetectionMaterial->setValue("u_screenSize", glm::vec2(width, height));
	screenQuad.material = edgeDetectionMaterial;

	//Push quad to scene
	m_screenScene->m_actors.push_back(screenQuad);

	//Relative luminance pass
	RenderPass edgeDetectionPass;
	edgeDetectionPass.scene = m_screenScene;
	edgeDetectionPass.parseScene();
	edgeDetectionPass.target = std::make_shared<FBO>(m_winRef.getSize(), typicalLayout);
	edgeDetectionPass.viewPort = { 0, 0, m_winRef.getWidth(), m_winRef.getHeight() };
	edgeDetectionPass.camera.projection = glm::ortho(0.0f, width, height, 0.0f);
	edgeDetectionPass.UBOmanager.setCachedValue("b_camera2D", "u_view", edgeDetectionPass.camera.view);
	edgeDetectionPass.UBOmanager.setCachedValue("b_camera2D", "u_projection", edgeDetectionPass.camera.projection);

	//Update previous pass
	previousPass = edgeDetectionPass;

	//Reset scene
	m_screenScene.reset(new Scene);

	//Shader description for vignette shader
	ShaderDescription vignetteShaderDesc = CreateShaderDesc(ShaderType::rasterization, "./assets/shaders/lab2/screenVert.glsl", "./assets/shaders/lab2/vignetteFrag.glsl");

	//Create and change material for screenQuad
	std::shared_ptr<Material> vignetteMaterial = CreateScreenQuadMaterial(vignetteShaderDesc);
	vignetteMaterial->setValue("u_screenSize", glm::vec2(width, height));
	screenQuad.material = vignetteMaterial;

	//Push quad to scene
	m_screenScene->m_actors.push_back(screenQuad);

	//Relative luminance pass
	RenderPass vignettePass;
	vignettePass.scene = m_screenScene;
	vignettePass.parseScene();
	vignettePass.target = std::make_shared<FBO>(m_winRef.getSize(), typicalLayout);
	vignettePass.viewPort = { 0, 0, m_winRef.getWidth(), m_winRef.getHeight() };
	vignettePass.camera.projection = glm::ortho(0.0f, width, height, 0.0f);
	vignettePass.UBOmanager.setCachedValue("b_camera2D", "u_view", vignettePass.camera.view);
	vignettePass.UBOmanager.setCachedValue("b_camera2D", "u_projection", vignettePass.camera.projection);

	//Update previous pass
	previousPass = vignettePass;

	//Reset scene
	m_screenScene.reset(new Scene);

	//Shader description for default screen shader 
	ShaderDescription screenShaderDesc = CreateShaderDesc(ShaderType::rasterization, "./assets/shaders/lab2/screenVert.glsl", "./assets/shaders/lab2/screenFrag.glsl");

	//Create and set new material for default screenPass
	std::shared_ptr<Material> defaultMaterial = CreateScreenQuadMaterial(screenShaderDesc);
	screenQuad.material = defaultMaterial;

	//Push quad to scene
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

	//Update previous pass
	previousPass = screenPass;

	//Add all render passes
	m_lab2Renderer.addRenderPass(mainPass);
	m_lab2Renderer.addRenderPass(invertPass);
	m_lab2Renderer.addRenderPass(relativeLuminancePass);
	m_lab2Renderer.addRenderPass(matrixPass);
	m_lab2Renderer.addRenderPass(blurPass);
	m_lab2Renderer.addRenderPass(sepiaPass);
	m_lab2Renderer.addRenderPass(edgeDetectionPass);
	m_lab2Renderer.addRenderPass(vignettePass);
	m_lab2Renderer.addRenderPass(screenPass);

	//Update pass indexes (hardcoded for now)
	mainPassIdx = 0;
	invertPassIdx = 1;
	luminancePassIdx = 2;
	matrixPassIdx = 3;
	blurPassIdx = 4;
	sepiaPassIdx = 5;
	edgeDetectionPassIdx = 6;
	vignettePassIdx = 7;
	screenPassIdx = 8;

	//Attach camera script for WASD movement
	m_lab2Scene->m_actors.at(cameraIdx).attachScript<CameraScript>(mainPass.scene->m_actors.at(cameraIdx), m_winRef, glm::vec3(1.6f, 0.6f, 2.f), 0.5f);
}



void Lab2::onRender() const
{
	m_lab2Renderer.render();
}

void Lab2::onUpdate(float timestep)
{
	//Change floor colour and point lights
	auto floorMat = m_lab2Scene->m_actors.at(floorIdx).material;
	floorMat->setValue("u_albedo", m_floorColour);
	floorMat->setValue("u_pLightsAmbient", m_pointLightsAmbient);
	floorMat->setValue("u_pLightsShine", m_pointLightsShine);

	//Change character point lights
	auto characterMat = m_lab2Scene->m_actors.at(charIdx).material;
	characterMat->setValue("u_pLightsAmbient", m_pointLightsAmbient);
	characterMat->setValue("u_pLightsShine", m_pointLightsShine);

	for (auto it = m_lab2Scene->m_actors.begin(); it != m_lab2Scene->m_actors.end(); ++it)
	{
		it->onUpdate(timestep);
	}

	if (m_wireFrame)
	{
		m_lab2Renderer.getRenderPass(mainPassIdx).drawInWireFrame = true;
	}
	else
	{
		m_lab2Renderer.getRenderPass(mainPassIdx).drawInWireFrame = false;
	}

	//Get render pass
	auto& mainPass = m_lab2Renderer.getRenderPass(mainPassIdx);

	//Skybox
	auto& skybox = m_lab2Scene->m_actors.at(skyIdx);
	skybox.material->setValue("u_skyboxView", glm::mat3(glm::mat3(mainPass.camera.view)));

	//Update tonemapping mode
	auto& screenQuadMat = m_lab2Renderer.getRenderPass(screenPassIdx).scene->m_actors.at(0).material;
	screenQuadMat->setValue("u_tonemappingMode", m_tonemappingMode);

	//Point light constants
	for (int i = 0; i < numPointLights; i++)
	{
		auto& pointLight = m_lab2Scene->m_pointLights.at(i);
		pointLight.constants = m_pLightConstants;
		mainPass.UBOmanager.setCachedValue("b_lights", "pLights[" + std::to_string(i) + "].constants", m_lab2Scene->m_pointLights.at(i).constants);
	}

	//Update directional light colour
	auto& directionalLight = m_lab2Scene->m_directionalLights.at(0);
	mainPass.UBOmanager.setCachedValue("b_lights", "dLight.colour", m_dlColour);
	mainPass.UBOmanager.setCachedValue("b_lights", "dLight.direction", m_dlDirection);	

	//Invert colors
	auto& invertMat = m_lab2Renderer.getRenderPass(invertPassIdx).scene->m_actors.at(0).material;
	if (m_invertColours > 0)
	{
		invertMat->setValue("u_active", 1.0f);
	}
	else
	{
		invertMat->setValue("u_active", 0.0f);
	}

	//Relative luminance
	auto& luminanceMat = m_lab2Renderer.getRenderPass(luminancePassIdx).scene->m_actors.at(0).material;
	if (m_relativeLuminance > 0)
	{
		luminanceMat->setValue("u_active", 1.0f);
	}
	else
	{
		luminanceMat->setValue("u_active", 0.0f);
	}

	//Matrix colours
	auto& matrixMat = m_lab2Renderer.getRenderPass(matrixPassIdx).scene->m_actors.at(0).material;
	if (m_matrixColours > 0)
	{
		matrixMat->setValue("u_active", 1.0f);
	}
	else
	{
		matrixMat->setValue("u_active", 0.0f);
	}

	//Blur
	auto& blurMat = m_lab2Renderer.getRenderPass(blurPassIdx).scene->m_actors.at(0).material;
	if (m_blur > 0)
	{
		blurMat->setValue("u_active", 1.0f);
		blurMat->setValue("u_blurRadius", m_blurRadius);
	}
	else
	{
		blurMat->setValue("u_active", 0.0f);
	}

	//Vignette
	auto& vignetteMat = m_lab2Renderer.getRenderPass(vignettePassIdx).scene->m_actors.at(0).material;
	if (m_vignette > 0)
	{
		vignetteMat->setValue("u_active", 1.0f);
		vignetteMat->setValue("u_innerRadius", m_vignetteControls.x);
		vignetteMat->setValue("u_outerRadius", m_vignetteControls.y);
	}
	else
	{
		vignetteMat->setValue("u_active", 0.0f);
	}

	//Sepia
	auto& sepiaMat = m_lab2Renderer.getRenderPass(sepiaPassIdx).scene->m_actors.at(0).material;
	if (m_sepia > 0)
	{
		sepiaMat->setValue("u_active", 1.0f);
	}
	else
	{
		sepiaMat->setValue("u_active", 0.0f);
	}

	//Edge detection
	auto& edgeDetectionMat = m_lab2Renderer.getRenderPass(edgeDetectionPassIdx).scene->m_actors.at(0).material;
	if (m_edgeDetection > 0)
	{
		edgeDetectionMat->setValue("u_active", 1.0f);
	}
	else
	{
		edgeDetectionMat->setValue("u_active", 0.0f);
	}

	//Adjust camera view and position
	auto& camera = m_lab2Scene->m_actors.at(cameraIdx);
	mainPass.camera.updateView(camera.transform);
	mainPass.UBOmanager.setCachedValue("b_camera", "u_view", mainPass.camera.view);
	mainPass.UBOmanager.setCachedValue("b_camera", "u_viewPos", camera.translation);
}

void Lab2::onKeyPressed(KeyPressedEvent& e)
{
	for (auto it = m_lab2Scene->m_actors.begin(); it != m_lab2Scene->m_actors.end(); ++it)
	{
		it->onKeyPress(e);
	}
}

void Lab2::onImGUIRender()
{
	//Milliseconds
	float ms = 1000.0f / ImGui::GetIO().Framerate;

	//Create new frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//Name
	ImGui::Begin("Lab 2");

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
	ImGui::SliderFloat3("Point Light Constants", glm::value_ptr(m_pLightConstants),0, 2.0f);

	//Wireframe checkbox
	ImGui::Checkbox("Wireframe ", &m_wireFrame);

	//Invert colours
	ImGui::Checkbox("Invert Colours", (bool*)&m_invertColours);

	//Relative luminance
	ImGui::Checkbox("Relative Luminance", (bool*)&m_relativeLuminance);

	//Matrix Colour power of checkbox
	ImGui::Checkbox("Matrix Colours", (bool*)&m_matrixColours);

	//Blur shader checkbox
	ImGui::Checkbox("Blur", (bool*)&m_blur);

	//Blur radius slider
	ImGui::SliderFloat("Blur Radius", &m_blurRadius, 0.0, 10);

	//Vignette checkbox
	ImGui::Checkbox("Vignette", (bool*)&m_vignette);

	//Vignette controls
	ImGui::SliderFloat2("Vignette Inner and Outer Radius", glm::value_ptr(m_vignetteControls), 0.0f, 2.0f);

	//Sepia checkbox
	ImGui::Checkbox("Sepia", (bool*)&m_sepia);

	//Edge detection checkbox
	ImGui::Checkbox("Edge Detection", (bool*)&m_edgeDetection);

	//Choose tonemapping method
	static int tonemappingMode = 0;
	const char* tonemappingModes[9] = { "ACES", "UNCHARTED2", "REINHARD", "FILMIC", "LOTTES", "NEUTRAL", "REINHART2", "UCHIMURA", "UNREAL"};
	if (ImGui::Combo("Tonemapping Mode", &tonemappingMode, tonemappingModes, IM_ARRAYSIZE(tonemappingModes)))
	{
		m_tonemappingMode = tonemappingMode;
	}

	//Display image
	GLuint textureID = m_lab2Renderer.getRenderPass(0).target->getTarget(0)->getID();
	ImVec2 imageSize = ImVec2(256, 256);
	ImVec2 uv0 = ImVec2(0.0f, 1.0f);
	ImVec2 uv1 = ImVec2(1.0f, 0.0f);
	ImGui::Image((void*)(intptr_t)textureID, imageSize, uv0, uv1);

	//End/Render
	ImGui::End();
	ImGui::Render();
}

ShaderDescription Lab2::CreateShaderDesc(ShaderType type, std::string vertexPath, std::string fragPath)
{
	ShaderDescription shaderDesc;
	shaderDesc.type = type;
	shaderDesc.vertexSrcPath = vertexPath;
	shaderDesc.fragmentSrcPath = fragPath;
	return shaderDesc;
}

std::shared_ptr<Shader> Lab2::CreateShader(ShaderDescription shaderDesc)
{
	std::shared_ptr<Shader> newShader;
	newShader = std::make_shared<Shader>(shaderDesc);
	return newShader;
}

std::shared_ptr<VAO> Lab2::CreateVAO(std::vector<unsigned int> indicies, std::vector<float> vertices, VBOLayout layout)
{
	std::shared_ptr<VAO> newVAO;
	newVAO = std::make_shared<VAO>(indicies);
	newVAO->addVertexBuffer(vertices, layout);
	return newVAO;
}

Actor Lab2::CreateActor(std::shared_ptr<VAO> VAO, std::shared_ptr<Material> material, glm::vec3 translation, glm::vec3 scale)
{
	Actor actor;
	actor.geometry = VAO;
	actor.material = material;
	actor.translation = translation;
	actor.scale = scale;
	actor.recalc();
	return actor;
}

Actor Lab2::CreateFloor(ShaderDescription shaderDesc)
{
	//Shader
	std::shared_ptr<Shader> floorShader = CreateShader(shaderDesc);
	VBOLayout floorLayout = {
		{GL_FLOAT, 3},	//pos
		{GL_FLOAT, 2}	//uv
	};

	//Create floor grid
	std::shared_ptr<Grid> grid = std::make_shared<Grid>();
	std::vector<float> floor_vertices = grid->getVertices();
	std::vector<unsigned int> floor_indices = grid->getIndices();

	//VAO
	std::shared_ptr<VAO> floorVAO = CreateVAO(floor_indices, floor_vertices, floorLayout);

	//Texture
	std::shared_ptr<Texture> floorTexture;
	floorTexture = std::make_shared<Texture>("./assets/models/whiteCube/letterCube.png");

	//Material
	std::shared_ptr<Material> floorMaterial;
	floorMaterial = std::make_shared<Material>(floorShader);
	floorMaterial->setValue("u_albedoMap", floorTexture);
	floorMaterial->setValue("u_albedo", m_floorColour);

	//Actor
	Actor floor = CreateActor(floorVAO, floorMaterial, glm::vec3(-50.0f, -5.0f, -50.0f));

	return floor;
}

Actor Lab2::CreateCharacter(ShaderDescription shaderDesc)
{
	//Shader
	std::shared_ptr<Shader> charShader = CreateShader(shaderDesc);

	//VBO Layout
	VBOLayout charLayout = {
		{GL_FLOAT, 3},	//pos
		{GL_FLOAT, 3},  //normal
		{GL_FLOAT, 2},	//uv
		{GL_FLOAT, 3}	//tan
	};

	//Model positions
	uint32_t attributeTypes = Model::VertexFlags::positions |
		Model::VertexFlags::normals |
		Model::VertexFlags::uvs |
		Model::VertexFlags::tangents;

	//Model
	Model charModel("./assets/models/Vampire/vampire.obj", attributeTypes);

	//VAO
	std::shared_ptr<VAO> charVAO = CreateVAO(charModel.m_meshes[0].indices, charModel.m_meshes[0].vertices, charLayout);

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
	Actor character = CreateActor(charVAO, charMaterial, glm::vec3(0.0f, -5.0f, -11.0f), glm::vec3(5.0f, 5.0f, 5.0f));

	return character;
}

Actor Lab2::CreateSkybox(ShaderDescription shaderDesc)
{
	//Indices
	std::vector<uint32_t> skyboxIndices(skyboxVertices.size() / 3);

	//iota
	std::iota(skyboxIndices.begin(), skyboxIndices.end(), 0);

	//VAO
	std::shared_ptr<VAO> skyboxVAO = CreateVAO(skyboxIndices, skyboxVertices, { {GL_FLOAT, 3} });

	//Shader
	std::shared_ptr<Shader> skyboxShader = CreateShader(shaderDesc);

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

	//Actor
	Actor skybox = CreateActor(skyboxVAO, skyboxMaterial);

	//Adjust uniform values
	skyboxMaterial->setValue("u_skyboxView", glm::inverse(skybox.transform));
	skyboxMaterial->setValue("u_skybox", cubeMap);

	return skybox;
}

Actor Lab2::CreateScreenQuad(ShaderDescription shaderDesc, float screenWidth, float screenHeight, RenderPass mainPass)
{
	//Screen vertices
	const std::vector<float> screenVertices =
	{		//Pos			//UV
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f,		//Bottom left
		screenWidth, 0.0f, 0.0f, 1.0f, 1.0f,		//Bottom right
		screenWidth, screenHeight, 0.0f, 1.0f, 0.0f,	//Top right
		0.0f, screenHeight, 0.0f, 0.0f, 0.0f		//Top left
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

	//Shader
	std::shared_ptr<Shader> screenQuadShader = CreateShader(shaderDesc);

	//Material
	std::shared_ptr<Material> screenQuadMaterial;
	screenQuadMaterial = std::make_shared<Material>(screenQuadShader);
	screenQuadMaterial->setValue("u_inputTexture", mainPass.target->getTarget(0));

	//Actor
	Actor quad = CreateActor(screenQuadVAO, screenQuadMaterial);

	return quad;
}

int Lab2::AddActorToRender(Actor actor)
{
	int index = m_lab2Scene->m_actors.size();
	m_lab2Scene->m_actors.push_back(actor);
	return index;
}

std::shared_ptr<Material> Lab2::CreateScreenQuadMaterial(ShaderDescription shaderDesc)
{
	//Shader
	std::shared_ptr<Shader> shader = CreateShader(shaderDesc);

	//Material
	std::shared_ptr<Material> material;
	material = std::make_shared<Material>(shader);
	material->setValue("u_inputTexture", previousPass.target->getTarget(0));
	
	return material;
}
