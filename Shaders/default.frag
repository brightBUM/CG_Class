#version 330 core
out vec4 FragColor;

//in vec3 Color;
in vec2 TexCoord;
in vec3 Normal;

uniform sampler2D texSampler1;
uniform sampler2D texSampler2;
uniform float mixValue;
uniform vec3 lightColor;

void main()
{
   vec4 texMix = mix(texture(texSampler1, TexCoord), texture(texSampler2, TexCoord), mixValue);

	FragColor = vec4(texMix.rgb*lightColor,texMix.a);
}