#include <glad/gl.h>
#include "assets/cubeMap.hpp"
#include "assets/texture.hpp"

#include "stbImage/stb_image.h"
#include "core/log.hpp"

CubeMap::CubeMap(const std::array<const char*, 6>& filepaths, bool isHDR, bool flipUvs)
{
    if (!flipUvs)stbi_set_flip_vertically_on_load(false);

    int32_t width = 0, height = 0, channels = 0;
    unsigned char* data = nullptr;
    float* dataf = nullptr;
    GLenum format = 0;
    GLenum storageDataType = 0;
    glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_ID);

    glTextureParameteri(m_ID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (isHDR) {
        dataf = stbi_loadf(filepaths[0], &width, &height, &channels, 0);
        if (channels == 3) {
            glTextureStorage2D(m_ID, 1, GL_RGB16F, width, height);
            format = GL_RGB;
            storageDataType = GL_FLOAT;
        }
        else if (channels == 4) {
            glTextureStorage2D(m_ID, 1, GL_RGBA16F, width, height);
            format = GL_RGBA;
            storageDataType = GL_FLOAT;
        }
        else {
            spdlog::error("CubeMap load error: {} channels are not supported (HDR).", channels);
            stbi_image_free(dataf);
            return;
        }
        glTextureSubImage3D(m_ID, 0, 0, 0, 0, width, height, 1, format, storageDataType, dataf);
        stbi_image_free(dataf);
    }
    else {

        data = stbi_load(filepaths[0], &width, &height, &channels, 0);
        if (channels == 3) {
            glTextureStorage2D(m_ID, 1, GL_RGB8, width, height);
            format = GL_RGB;
            storageDataType = GL_UNSIGNED_BYTE;
        }
        else if (channels == 4) {
            glTextureStorage2D(m_ID, 1, GL_RGBA8, width, height);
            format = GL_RGBA;
            storageDataType = GL_UNSIGNED_BYTE;
        }
        else {
            spdlog::error("CubeMap load error: {} channels are not supported (non-HDR).", channels);
            stbi_image_free(dataf);
            return;
        }
        glTextureSubImage3D(m_ID, 0, 0, 0, 0, width, height, 1, format, storageDataType, data);
        stbi_image_free(data);
    }

    for (size_t i = 1; i < 6; i++) {
        if (isHDR) {
            dataf = stbi_loadf(filepaths[i], &width, &height, &channels, 0);
            glTextureSubImage3D(m_ID, 0, 0, 0, i, width, height, 1, format, storageDataType, dataf);
            stbi_image_free(dataf);
        }
        else {
            data = stbi_load(filepaths[i], &width, &height, &channels, 0);
            glTextureSubImage3D(m_ID, 0, 0, 0, i, width, height, 1, format, storageDataType, data);
            stbi_image_free(data);
            data = nullptr;
            width = 0;
        }
    }
}

CubeMap::~CubeMap()
{
    glDeleteTextures(1, &m_ID);
}