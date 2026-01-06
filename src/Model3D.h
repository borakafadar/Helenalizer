//
// Created by borak on 5.01.2026.
//

#ifndef HELENALIZER_MODEL3D_H
#define HELENALIZER_MODEL3D_H

#include <vector>
#include <string>
#include <SFML/OpenGL.hpp> // Gives us glBegin, glVertex3f, etc.

class Model3D {
public:
    // Loads the .glb file
    bool load(const std::string& path);

    // Draws the model at specific location
    // scale: 1.0 = original size
    void render(float x, float y, float z, float scale, float rotationAngle);

private:
    // We store positions as a flat list: {x1,y1,z1, x2,y2,z2, ...}
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texCoords;

    GLuint textureID = 0;
};

#endif
