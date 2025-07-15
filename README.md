# Demon Renderer - Enhanced OpenGL Shader Project

This project began as a basic OpenGL renderer provided by my university, titled Demon Renderer. I extensively rebuilt and expanded the renderer, implementing modern rendering techniques, a powerful GPU-driven particle system, and a full suite of visual effects.

#
# 🔥 Project Highlights
## 🚀 Advanced Rendering Pipeline
* **Deferred Renderer**
    * Implemented a complete G-buffer based deferred rendering pipeline to decouple geometry and lighting passes for efficient rendering of complex scenes.
* **Shadows**
    * Real-time shadow mapping in a seperate pass for directional lights.
* **PBR Materials**
    * Physically-Based Rendering for all scene materials using metallic/roughness workflow and environment reflections.

#
# 🧠 GPU Features and Optimizations
* **Level of Detail (LOD) System
    * Custom LOD system using tessellation shaders and geometry shaders, dynamically adjusting m esh complexity based on camera distance.
* **Compute Shader Particle System**
    * Real-time simulation of ~3 million snow particles using compute shaders and **Shader Storage Buffer Objects(SSBOs)** for efficient GPU communication.
* **Procedural Terrain via Noise**
    * Heightmap generation using noise functions to deform the ground plane procedurally.

#
# 🌲 Scene Enhancements
* **Billboarded Trees**
    * Background foliage rendered as screen-facing billboards for performance.
* **Skybox**
    * Rendered seperetly from the deferred pipeline for proper environment mapping.

#
# 🎨 Post-Processing Effects
A wide range of full screen image effects were implemented, including:
* A range of tonemapping options
* Edge Detection
* Gaussian blur
* Vignette
* Sepia
* Depth of Field (with gaussian blur)
* Fog

#
# 🧱 Build Instructions
## ✅ Prerequisites
* Visual Studio 22
* C++17
* CMake 3.10+
* OpenGL 4.5 capable GPU

## 🪜 Steps
* 1 - Clone the repository:
```
git clone https://github.com/Ethantl28/DemonRendererShaders.git
cd DemonRendererShaders
```
* 2 - Generate build files with CMake:
```
runCmake.bat
```
* 3 - You can find the sln file inside the ```build``` folder. 

#
# 🎮 Controls
|        Control       |       Action           |  
| ---------------------|------------------------|    
| W / A / S / D        |Move camera             |
| UP / DOWN arrow keys |Move camera up and down |


#
# 📚 Educational Context
This project is based on a stripped-down OpenGL renderer titled "Demon Renderer", provided by De Montfort University. It served as the foundation for an advanced graphics project exploring modern GPU programming techniques.

The goal was to demonstrate:
* Real-time deferred rendering
* Advanced shader usage across the graphics pipeline
* Efficient particle simulation with compute shaders
* Shader-based procedural content (noise, LOD)
* Post-processing chains and screen-space effects

#
# 📸 Gallery
