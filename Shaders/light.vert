#version 330 core
layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec2 aTexCoord;
//layout (location = 1) in vec3 aCol;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

//out vec3 Color;
//out vec2 TexCoord;
void main()
{
//    Color = aCol;
//    TexCoord = aTexCoord;
    gl_Position = proj*view*model*vec4(aPos.x, aPos.y, aPos.z, 1.0);
}