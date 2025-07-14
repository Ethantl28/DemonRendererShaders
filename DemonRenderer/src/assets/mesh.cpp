#include "assets/mesh.hpp"
#include "core/log.hpp"

Model::Model(std::filesystem::path path, uint32_t options) : 
	m_options(options),
	m_rootPath(std::filesystem::path(path).remove_filename())
{
	auto scene = aiImportFile(path.string().c_str(), aiProcess_Triangulate | aiProcess_CalcTangentSpace );
	if(scene) processNode(scene->mRootNode, scene);
	else spdlog::error("No model loaded: scene is empty. Error msg: {}", aiGetErrorString());

	aiReleaseImport(scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	// Get nodes relative transform
	aiMatrix4x4& noteTrans = node->mTransformation;	
	glm::mat4 transform;

	transform[0][0] = noteTrans.a1; transform[1][0] = noteTrans.a2; transform[2][0] = noteTrans.a3; transform[3][0] = noteTrans.a4;
	transform[0][1] = noteTrans.b1; transform[1][1] = noteTrans.b2; transform[2][1] = noteTrans.b3; transform[3][1] = noteTrans.b4;
	transform[0][2] = noteTrans.c1; transform[1][2] = noteTrans.c2; transform[2][2] = noteTrans.c3; transform[3][2] = noteTrans.c4;
	transform[0][3] = noteTrans.d1; transform[1][3] = noteTrans.d2; transform[2][3] = noteTrans.d3; transform[3][3] = noteTrans.d4;

	// process all the node's meshes
	for (uint32_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(mesh, scene, transform);
	}

	//  Process child nodes
	for (uint32_t i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

void Model::processMesh(aiMesh* assimpMesh, const aiScene* scene, const glm::mat4& relativeTransform)
{
	uint32_t vertexComponents = 0;
	if (m_options && positions) vertexComponents += 3;
	if (m_options && normals) vertexComponents += 2;
	if (m_options && uvs) vertexComponents += 3;
	if (m_options && tangents) vertexComponents += 3;
	if (m_options && biTangents) vertexComponents += 3;

	Mesh mesh;
	mesh.relativeTransform = relativeTransform;
	mesh.vertices.resize(vertexComponents * assimpMesh->mNumVertices);
	mesh.positions.resize(3 * assimpMesh->mNumVertices);
	mesh.indices.resize(assimpMesh->mNumFaces * 3);

	// Vertices
	size_t j = 0, k = 0;
	for (size_t i = 0; i < assimpMesh->mNumVertices; i++)
	{
		mesh.positions[k] = assimpMesh->mVertices[i].x; k++;
		mesh.positions[k] = assimpMesh->mVertices[i].y; k++;
		mesh.positions[k] = assimpMesh->mVertices[i].z; k++;
		if (m_options & positions) {
			mesh.vertices[j] = assimpMesh->mVertices[i].x; j++;
			mesh.vertices[j] = assimpMesh->mVertices[i].y; j++;
			mesh.vertices[j] = assimpMesh->mVertices[i].z; j++;
		}

		if (m_options & normals) {
			mesh.vertices[j] = assimpMesh->mNormals[i].x; j++;
			mesh.vertices[j] = assimpMesh->mNormals[i].y; j++;
			mesh.vertices[j] = assimpMesh->mNormals[i].z; j++;
		}

		if (m_options & uvs) {
			mesh.vertices[j] = assimpMesh->mTextureCoords[0][i].x; j++;
			mesh.vertices[j] = assimpMesh->mTextureCoords[0][i].y; j++;
		}

		if (m_options & tangents) {
			mesh.vertices[j] = assimpMesh->mTangents[i].x; j++;
			mesh.vertices[j] = assimpMesh->mTangents[i].y; j++;
			mesh.vertices[j] = assimpMesh->mTangents[i].z; j++;
		}

		if (m_options & biTangents) {
			mesh.vertices[j] = assimpMesh->mBitangents[i].x; j++;
			mesh.vertices[j] = assimpMesh->mBitangents[i].y; j++;
			mesh.vertices[j] = assimpMesh->mBitangents[i].z; j++;
		}
	}

	// Indices
	j = 0;
	for (size_t i = 0; i < assimpMesh->mNumFaces; i++)
	{
		mesh.indices[j] = assimpMesh->mFaces[i].mIndices[0]; j++;
		mesh.indices[j] = assimpMesh->mFaces[i].mIndices[1]; j++;
		mesh.indices[j] = assimpMesh->mFaces[i].mIndices[2]; j++;
	}

	// Texture paths
	aiMaterial* material = scene->mMaterials[assimpMesh->mMaterialIndex];

	for (auto type : m_textureTypes)
	{
		std::filesystem::path textureFilePath = m_rootPath;
		for (uint32_t i = 0; i < material->GetTextureCount(type); i++)
		{
			aiString str;
			material->GetTexture(type, i, &str);
			std::filesystem::path fileName(str.C_Str());
			textureFilePath /= fileName;
			mesh.texturePaths[type] = textureFilePath;
		}
	}

	m_meshes.push_back(mesh);
}
