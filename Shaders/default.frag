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
uniform vec3 camPos;

struct Material
{
	float ambient;
	float specular;
	float shininess;
};
uniform Material material;
void main()
{
   vec4 texMix = mix(texture(texSampler1, TexCoord), texture(texSampler2, TexCoord), mixValue);

   //ambient lighting
   vec3 ambient = material.ambient*lightColor;

   //diffuse lighting
   vec3 A = normalize(Normal);
   vec3 B = normalize(lightPos-FragPos);
   float dotValue = dot(A,B);

   vec3 objectColor = vec3(0.5f,0.8f,0.2f);
   float diffValue = max(dotValue,0.0f);
   vec3 diffuse = diffValue*lightColor;

   //specular lighting
   vec3 ref = reflect(-B,Normal);
   vec3 viewDir = normalize(camPos-FragPos);
   float dotValue2 = dot(ref,viewDir);
   float specValue = pow(max(dotValue2,0.0f),material.shininess);
   vec3 specular = specValue*material.specular*lightColor;

   vec4 result = vec4((diffuse+ambient+specular)*objectColor,1.0f);
	FragColor = result;
}