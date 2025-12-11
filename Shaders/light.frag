#version 330 core
out vec4 FragColor;
uniform vec3 lightColor;
uniform sampler2D texSampler1;
in vec2 TexCoord;
void main()
{
	vec4 tex = texture(texSampler1,TexCoord);
	FragColor = vec4(lightColor*tex.rgb,tex.a);
	
}