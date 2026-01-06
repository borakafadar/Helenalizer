#include "Model3D.h"
#include <iostream>
#include <vector>
#include <cstdio> // Use C-style file I/O (fopen, fread)
#include "tiny_gltf.h"

// Helper to get buffer data
const unsigned char* getBufferPtr(const tinygltf::Model& model, const tinygltf::Accessor& accessor) {
    const auto& bufferView = model.bufferViews[accessor.bufferView];
    const auto& buffer = model.buffers[bufferView.buffer];
    return &buffer.data[bufferView.byteOffset + accessor.byteOffset];
}

bool Model3D::load(const std::string& path) {
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err, warn;

    // --- NEW: C-STYLE FILE READING (Safest method) ---
    // Bypass std::ifstream entirely to avoid DLL/ABI crashes
    FILE* file = fopen(path.c_str(), "rb");
    if (!file) {
        std::cerr << "Error: Failed to open file (C-Style): " << path << std::endl;
        return false;
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (size <= 0) {
        std::cerr << "Error: File is empty or invalid: " << path << std::endl;
        fclose(file);
        return false;
    }

    // Allocate buffer and read
    std::vector<unsigned char> buffer(size);
    size_t readSize = fread(buffer.data(), 1, size, file);
    fclose(file); // Close immediately

    if (readSize != static_cast<size_t>(size)) {
        std::cerr << "Error: Failed to read all bytes from: " << path << std::endl;
        return false;
    }

    // Load from memory buffer
    bool ret = loader.LoadBinaryFromMemory(&model, &err, &warn, buffer.data(), size, "");

    // --- REST OF THE CODE IS UNCHANGED ---
    if (!warn.empty()) std::cerr << "GLTF Warn: " << warn << std::endl;
    if (!err.empty()) std::cerr << "GLTF Err: " << err << std::endl;
    if (!ret) return false;

    vertices.clear();
    normals.clear();
    texCoords.clear();

    // 1. TEXTURE LOADING
    if (!model.images.empty()) {
        const tinygltf::Image& image = model.images[0];

        if (textureID == 0) glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        GLenum format = GL_RGBA;
        if (image.component == 3) format = GL_RGB;

        glTexImage2D(GL_TEXTURE_2D, 0, format, image.width, image.height,
                     0, format, GL_UNSIGNED_BYTE, &image.image[0]);
    }

    // Temp buffers
    std::vector<float> tempPos;
    std::vector<float> tempNorm;
    std::vector<float> tempUV;

    for (const auto& mesh : model.meshes) {
        for (const auto& primitive : mesh.primitives) {

            // 2. EXTRACT RAW DATA
            if (primitive.attributes.find("POSITION") != primitive.attributes.end()) {
                const auto& accessor = model.accessors[primitive.attributes.at("POSITION")];
                const unsigned char* data = getBufferPtr(model, accessor);
                const auto& bufferView = model.bufferViews[accessor.bufferView];
                int stride = accessor.ByteStride(bufferView) ? accessor.ByteStride(bufferView) : sizeof(float) * 3;

                for (size_t i = 0; i < accessor.count; ++i) {
                    const float* p = reinterpret_cast<const float*>(data + i * stride);
                    tempPos.push_back(p[0]); tempPos.push_back(p[1]); tempPos.push_back(p[2]);
                }
            }

            if (primitive.attributes.find("NORMAL") != primitive.attributes.end()) {
                const auto& accessor = model.accessors[primitive.attributes.at("NORMAL")];
                const unsigned char* data = getBufferPtr(model, accessor);
                const auto& bufferView = model.bufferViews[accessor.bufferView];
                int stride = accessor.ByteStride(bufferView) ? accessor.ByteStride(bufferView) : sizeof(float) * 3;

                for (size_t i = 0; i < accessor.count; ++i) {
                    const float* n = reinterpret_cast<const float*>(data + i * stride);
                    tempNorm.push_back(n[0]); tempNorm.push_back(n[1]); tempNorm.push_back(n[2]);
                }
            }

            if (primitive.attributes.find("TEXCOORD_0") != primitive.attributes.end()) {
                const auto& accessor = model.accessors[primitive.attributes.at("TEXCOORD_0")];
                const unsigned char* data = getBufferPtr(model, accessor);
                const auto& bufferView = model.bufferViews[accessor.bufferView];
                int stride = accessor.ByteStride(bufferView) ? accessor.ByteStride(bufferView) : sizeof(float) * 2;

                for (size_t i = 0; i < accessor.count; ++i) {
                    const float* uv = reinterpret_cast<const float*>(data + i * stride);
                    tempUV.push_back(uv[0]); tempUV.push_back(uv[1]);
                }
            }

            // 3. RE-ASSEMBLE
            if (primitive.indices >= 0) {
                const auto& accessor = model.accessors[primitive.indices];
                const unsigned char* data = getBufferPtr(model, accessor);

                for (size_t i = 0; i < accessor.count; ++i) {
                    int index = 0;
                    if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT)
                        index = reinterpret_cast<const unsigned short*>(data)[i];
                    else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT)
                        index = reinterpret_cast<const unsigned int*>(data)[i];
                    else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE)
                        index = reinterpret_cast<const unsigned char*>(data)[i];

                    if (index * 3 + 2 < tempPos.size()) {
                        vertices.push_back(tempPos[index * 3 + 0]);
                        vertices.push_back(tempPos[index * 3 + 1]);
                        vertices.push_back(tempPos[index * 3 + 2]);
                    }
                    if (index * 3 + 2 < tempNorm.size()) {
                        normals.push_back(tempNorm[index * 3 + 0]);
                        normals.push_back(tempNorm[index * 3 + 1]);
                        normals.push_back(tempNorm[index * 3 + 2]);
                    }
                    if (index * 2 + 1 < tempUV.size()) {
                        texCoords.push_back(tempUV[index * 2 + 0]);
                        texCoords.push_back(tempUV[index * 2 + 1]);
                    }
                }
            } else {
                vertices = tempPos;
                normals = tempNorm;
                texCoords = tempUV;
            }
            tempPos.clear(); tempNorm.clear(); tempUV.clear();
        }
    }

    std::cout << "Loaded Model Manually: " << vertices.size() / 3 << " triangles." << std::endl;
    return true;
}

// ... render() function remains exactly the same ...

void Model3D::render(float x, float y, float z, float scale, float rotationAngle) {
    if (vertices.empty()) return;

    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f);
    glScalef(scale, scale, scale);

    // ENABLE TEXTURING
    if (textureID != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);
        // Set color to WHITE so the texture shows its true colors
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    } else {
        // Fallback if no texture found
        glColor3f(1.0f, 0.0f, 1.0f);
    }

    glBegin(GL_TRIANGLES);
    for (size_t i = 0, v = 0, t = 0; i < vertices.size(); i += 3, v += 3, t += 2) {

        // Pass UV Coordinate (Critical for texture mapping)
        if (t < texCoords.size()) {
            glTexCoord2f(texCoords[t], texCoords[t+1]);
        }

        // Pass Normal (Critical for lighting)
        if (v < normals.size()) {
            glNormal3f(normals[v], normals[v+1], normals[v+2]);
        }

        // Pass Vertex
        glVertex3f(vertices[i], vertices[i+1], vertices[i+2]);
    }
    glEnd();

    // DISABLE TEXTURING (Cleanup)
    if (textureID != 0) {
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
    }

    glPopMatrix();
}