#version 330 core
out vec4 FragColor;

in vec3 Color;
in vec2 TexCoord;
uniform sampler2D texSampler1;
uniform sampler2D texSampler2;
uniform float mixValue;
void main()
{
   FragColor = mix(texture(texSampler1, TexCoord), texture(texSampler2, TexCoord), mixValue);
}