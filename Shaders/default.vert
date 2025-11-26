#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;

uniform mat4 model;
uniform mat4 view;
uniform float uTime;

out vec3 Color;
void main()
{
    Color = aCol;

	float frequency = 5.0;
    float amplitude = 0.5;
    float speed = 3.0;

    // Oscillate Y based on X and Time
    float yOffset = sin(uTime * speed + frequency) * amplitude;
    
    gl_Position = model*vec4(aPos.x, aPos.y+yOffset, aPos.z, 1.0);
}