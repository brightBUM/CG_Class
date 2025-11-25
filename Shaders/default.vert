#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;
uniform mat4 model;

out vec3 Color;
void main()
{
	Color = aCol;
   gl_Position = model*vec4(aPos.x, aPos.y, aPos.z, 1.0);
}