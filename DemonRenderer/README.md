# DemonRenderer

The DemonRenderer is a library containing everything you need to create a window with an OpenGL context and render objects in 3D using OpenGL. 

## Usage

DemonRenderer compiles to a static library. 
To use the DemonRenderer you will need to inherit from the Application class in your excutable application.
This will setup the window and ImGUI functionality should you wish to make use of it.

```
// main.hpp
#pragma once

// Entry point -  extern in DemonRenderer application.hpp
#include "core/entryPoint.hpp"

class App : public Application
{
public:
	App(const WindowProperties& winProps);
private:
};

```

To begin rendering items in you scene you first need to connect a layer to the application.
```
// app.cpp
#include "app.hpp"
#include "demo.hpp"

App::App(const WindowProperties& winProps) : Application(winProps)
{
	m_layer = std::unique_ptr<Layer>(new DemoLayer(m_window));
}

Application* startApplication()
{
	WindowProperties props("Demo", 800, 600);
	props.isHostingImGui = true;
	props.isResizable = false;
	return new App(props);
}
```
This layer has a scene to which actors and lights can be added. We begin be getting the geometry and shader for the thing we wish to render.  Usually this is done in the layer's constructor.  We will load a shader, a model, a texture and create a VAO (geometry) and a material with these.
```
ShaderDescription phongShaderDesc;
phongShaderDesc.type = ShaderType::rasterization;
phongShaderDesc.vertexSrcPath = "./assets/shaders/Phong/Vert.glsl";
phongShaderDesc.fragmentSrcPath = "./assets/shaders/Phong/Frag.glsl";

std::shared_ptr<Shader> phongShader;
phongShader = std::make_shared<Shader>(phongShaderDesc);

Model cubeModel("./assets/models/whitecube/whitecube.obj");

VBOLayout cubeLayout = {
	{GL_FLOAT, 3}, // Position
	{GL_FLOAT, 3}, // Normal
	{GL_FLOAT, 2}  // UV co-ords
};

std::shared_ptr<VAO> cubeVAO;
cubeVAO = std::make_shared<VAO>(cubeModel.m_meshes[0].indices);
cubeVAO->addVertexBuffer(cubeModel.m_meshes[0].vertices, cubeLayout);

std::shared_ptr<Texture> cubeTexture;
cubeTexture = std::make_shared<Texture>(cubeModel.m_meshes[0].texturePaths[aiTextureType_DIFFUSE].string().c_str());

std::shared_ptr<Material> cubeMaterial;
cubeMaterial = std::make_shared<Material>(phongShader, "u_model");
cubeMaterial->setValue("u_albedo", glm::vec3(1.0f));
cubeMaterial->setValue("u_albedoMap", cubeTexture);
```
We can now initialise the scene and add an actor for our geometry.
```
m_scene.reset(new Scene);

Actor cube;
cube.geometry = cubeVAO;
cube.material = cubeMaterial;

cube.translation = glm::vec3(0.f, -1.f, -6.f);
cube.rotation = glm::quat(glm::vec3(0.0f, 0.4f, 0.0f));
cube.recalc();
m_scene->m_actors.push_back(cube);
```
We are also going to need lights (a single directional light for this scene) and a camera.
```
	DirectionalLight dl;
	dl.direction = glm::normalize(glm::vec3(1.f, -2.5f, -2.f));
	m_scene->m_directionalLights.push_back(dl);

	Actor camera;
	m_cameraIdx = m_scene->m_actors.size();
	m_scene->m_actors.push_back(camera);
```
Finally we set up our renderpasses.  In this example we have a single renderpass.  We also need to set values in our UBOs at this point.
```
	RenderPass mainPass;

	mainPass.scene = m_scene;
	mainPass.parseScene();
	mainPass.target = std::make_shared<FBO>(); // Default framebuffer
	mainPass.camera.projection = glm::perspective(45.f, m_winRef.getWidthf() / m_winRef.getHeightf(), 0.1f, 100.f);
	mainPass.viewPort = { 0, 0, m_winRef.getWidth(), m_winRef.getHeight() };

	mainPass.camera.updateView(m_scene->m_actors.at(m_cameraIdx).transform);

	mainPass.UBOmanager.setCachedValue("b_camera", "u_view", mainPass.camera.view);
	mainPass.UBOmanager.setCachedValue("b_camera", "u_projection", mainPass.camera.projection);

	mainPass.UBOmanager.setCachedValue("b_lights", "u_viewPos", m_scene->m_actors.at(m_cameraIdx).translation);
	mainPass.UBOmanager.setCachedValue("b_lights", "dLight.colour", m_scene->m_directionalLights.at(0).colour);
	mainPass.UBOmanager.setCachedValue("b_lights", "dLight.direction", m_scene->m_directionalLights.at(0).direction);

	m_renderer.addRenderPass(mainPass);
```
The rendering functioning for this demo is very simple, just call the renderer.
```
void DemoLayer::onRender() const
{
	m_renderer.render();
}
```
Currently, nothing needs to happen on update.
```
void DemoLayer::onUpdate(float timestep)
{
}
```
## Adding Interactivity with Native Scripts

To add a native script your class needs to inherit from script.  The header of a simple camera controller script is given below.
```
#pragma once

#include <glm/glm.hpp>

#include "gameObjects/actor.hpp"
#include "windows/GLFWWindowImpl.hpp"

class CameraScript : public Script
{
public:
	CameraScript(Actor& actor, GLFWWindowImpl& win, const glm::vec3& movementSpeed, float turnSpeed) :
		Script(actor),
		m_winRef(win),
		m_movementSpeed(movementSpeed),
		m_turnSpeed(turnSpeed)
	{}
	virtual void onUpdate(float timestep) override;
	virtual void onKeyPress(KeyPressedEvent& e) override {};
	virtual void onKeyRelease(KeyReleasedEvent& e) override {};
private:
	GLFWWindowImpl& m_winRef;
	glm::vec3 m_movementSpeed{ 1.f, 1.f, 1.f };
	float m_turnSpeed{ 0.5f };
};
```
The accompanying implementation is listed below:
```
#include "scripts/include/camera.hpp"

void CameraScript::onUpdate(float timestep)
{
	bool recalc = false;

	glm::vec3 right = { m_actor.transform[0][0], m_actor.transform[0][1], m_actor.transform[0][2] };
	glm::vec3 forward = { -m_actor.transform[2][0], -m_actor.transform[2][1], -m_actor.transform[2][2] };

	if (m_winRef.doIsKeyPressed(GLFW_KEY_W)) { m_actor.translation += forward * m_movementSpeed.z * timestep; recalc = true; }
	if (m_winRef.doIsKeyPressed(GLFW_KEY_A)) { m_actor.translation -= right * m_movementSpeed.x * timestep; recalc = true; }
	if (m_winRef.doIsKeyPressed(GLFW_KEY_S)) { m_actor.translation -= forward* m_movementSpeed.z * timestep; recalc = true; }
	if (m_winRef.doIsKeyPressed(GLFW_KEY_D)) { m_actor.translation += right * m_movementSpeed.x * timestep; recalc = true; }
	if (m_winRef.doIsKeyPressed(GLFW_KEY_UP)) { m_actor.translation.y += m_movementSpeed.y * timestep; recalc = true; }
	if (m_winRef.doIsKeyPressed(GLFW_KEY_DOWN)) { m_actor.translation.y -= m_movementSpeed.y * timestep; recalc = true; }


	if (m_winRef.doIsKeyPressed(GLFW_KEY_LEFT)) {
		glm::vec3 EulerAngles = glm::eulerAngles(m_actor.rotation);
		EulerAngles.y += m_turnSpeed * timestep;
		m_actor.rotation = glm::quat(EulerAngles);
		recalc = true;
	}

	if (m_winRef.doIsKeyPressed(GLFW_KEY_RIGHT)) {
		glm::vec3 EulerAngles = glm::eulerAngles(m_actor.rotation);
		EulerAngles.y -= m_turnSpeed * timestep;
		m_actor.rotation = glm::quat(EulerAngles);
		recalc = true;
	}

	if (recalc) m_actor.recalc();
}

```

We now need to add the camera script to the camera actor. **Scripts rely on pointers, and therefore the address where actors are located not moving. As such, if you are storing actors in a vector you can only add scripts to actor after the vectors size and been finalised.**  The code below show a our scsript being added to the camera actor.
```
m_scene->m_actors.at(m_cameraIdx).attachScript<CameraScript>(mainPass.scene->m_actors.at(m_cameraIdx), m_winRef, glm::vec3(1.6f, 0.6f, 2.f), 0.5f);
```
We now need to call the script and update the camera position in the UBO every.  The code to do this in onUpdate is given below.  Ofcourse, if you are using uniforms rather than UBOs then the values of the uniforms need updating in the relevant materials instead.
```
void DemoLayer::onUpdate(float timestep)
{
	// Update scripts
	for (auto& actor : m_scene->m_actors)
	{
		actor.onUpdate(timestep);
	}

	// Update camera  and its position in UBO
	auto& camera = m_scene->m_actors.at(m_cameraIdx);

	auto& pass = m_renderer.getRenderPass(0);

	pass.camera.updateView(camera.transform);
	pass.UBOmanager.setCachedValue("b_camera", "u_view", pass.camera.view);
	pass.UBOmanager.setCachedValue("b_camera", "u_viewPos", camera.translation);
}
```
This concludes the demo code.

