/** \file mesh.hpp */
#pragma once

#include <filesystem>
#include <vector>
#include <unordered_map>
#include <string>
#include <glm/glm.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

/** \struct Mesh
*	\brief Holds mesh data in RAM on the host.
*/
struct Mesh
{
	std::vector<float> vertices; //!< Vertices which make up the mesh 
	std::vector<float> positions; //!< Positions which make up the mesh 
	std::vector<uint32_t> indices; //!< Indices which make up the faces
	std::unordered_map<aiTextureType, std::filesystem::path> texturePaths; //!< Textures to be mapped
	glm::mat4 relativeTransform{ glm::mat4(1.f) }; //!< Transform relative to the models root
};

/** \class Model
*	\brief A model held in RAM on the host as a collection of meshes and attributes.
*/
class Model
{
public:
	Model(std::filesystem::path path, uint32_t options = positions | normals | uvs); //!< Constructor with filepath and options
	std::vector<Mesh> m_meshes; //!< A collection of meshes
	const uint32_t m_options; //!< Options used to load the model
	const std::filesystem::path m_rootPath; //!< Root directory path of the model
	/** \enum VertexFlags
	*	\brief Options for which vertices to obtain on loading */
	enum VertexFlags {
		positions = 1 << 0,
		normals = 1 << 1,
		uvs = 1 << 2,
		tangents = 1 << 3,
		biTangents = 1 << 4
	};
private:
	void processNode(aiNode* node, const aiScene* scene); //!< Process a node within a heirachical model.
	void processMesh(aiMesh* mesh, const aiScene* scene, const glm::mat4& relativeTransform); //!< Process a mesh from a node within a heirachical model.
	/** Types of textues which could loaded. */
	const inline static std::vector<aiTextureType> m_textureTypes = {
				aiTextureType_NONE,
				aiTextureType_DIFFUSE,
				aiTextureType_SPECULAR,
				aiTextureType_AMBIENT,
				aiTextureType_EMISSIVE,
				aiTextureType_HEIGHT,
				aiTextureType_NORMALS,
				aiTextureType_SHININESS,
				aiTextureType_OPACITY,
				aiTextureType_DISPLACEMENT,
				aiTextureType_LIGHTMAP,
				aiTextureType_REFLECTION,
				aiTextureType_BASE_COLOR,
				aiTextureType_NORMAL_CAMERA,
				aiTextureType_EMISSION_COLOR,
				aiTextureType_METALNESS,
				aiTextureType_DIFFUSE_ROUGHNESS,
				aiTextureType_AMBIENT_OCCLUSION,
				aiTextureType_UNKNOWN
	};
};