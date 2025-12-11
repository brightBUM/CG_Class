#pragma once
#include <vector>
#include <cmath>

struct SphereData {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
};

inline SphereData GenerateSphere(float radius, unsigned int sectors, unsigned int stacks)
{
    SphereData sphere;

    const float PI = 3.14159265359f;
    float sectorStep = 2 * PI / sectors;
    float stackStep = PI / stacks;

    // Generate vertices
    for (unsigned int i = 0; i <= stacks; ++i) {
        float stackAngle = PI / 2 - i * stackStep;  // from pi/2 to -pi/2
        float xy = radius * cosf(stackAngle);        // r * cos(u)
        float z = radius * sinf(stackAngle);         // r * sin(u)

        for (unsigned int j = 0; j <= sectors; ++j) {
            float sectorAngle = j * sectorStep;      // from 0 to 2pi

            // Position
            float x = xy * cosf(sectorAngle);        // r * cos(u) * cos(v)
            float y = xy * sinf(sectorAngle);        // r * cos(u) * sin(v)
            sphere.vertices.push_back(x);
            sphere.vertices.push_back(y);
            sphere.vertices.push_back(z);

            // Normal (normalized position for a sphere centered at origin)
            float nx = x / radius;
            float ny = y / radius;
            float nz = z / radius;
            sphere.vertices.push_back(nx);
            sphere.vertices.push_back(ny);
            sphere.vertices.push_back(nz);

            // UV coordinates
            float u = (float)j / sectors;
            float v = (float)i / stacks;
            sphere.vertices.push_back(u);
            sphere.vertices.push_back(v);
        }
    }

    // Generate indices
    for (unsigned int i = 0; i < stacks; ++i) {
        unsigned int k1 = i * (sectors + 1);
        unsigned int k2 = k1 + sectors + 1;

        for (unsigned int j = 0; j < sectors; ++j, ++k1, ++k2) {
            if (i != 0) {
                sphere.indices.push_back(k1);
                sphere.indices.push_back(k2);
                sphere.indices.push_back(k1 + 1);
            }

            if (i != (stacks - 1)) {
                sphere.indices.push_back(k1 + 1);
                sphere.indices.push_back(k2);
                sphere.indices.push_back(k2 + 1);
            }
        }
    }

    return sphere;
}