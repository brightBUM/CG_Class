#version 330 core
out vec4 FragColor;

//in vec3 Color;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texSampler1;
uniform sampler2D texSampler2;
uniform float mixValue;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main()
{
   vec4 texMix = mix(texture(texSampler1, TexCoord), texture(texSampler2, TexCoord), mixValue);

   //diffuse lighting
   vec3 A = normalize(Normal);
   vec3 B = normalize(lightPos-FragPos);
   float dot = dot(A,B);

   float diffuse = max(dot,0.0f);
   vec4 result = vec4(diffuse*texMix.rgb,texMix.a);
FragColor = result;
}