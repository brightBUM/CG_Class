#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
//layout (location = 1) in vec3 aCol;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

//out vec3 Color;
out vec2 TexCoord;
vec3 Normal;
vec3 FragPos;

struct Material
{
	vec3 objectColor;
	float ambient;
	float specular;
	float shininess;
};
uniform Material material;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;
out vec3 result;

void main()
{
//    Color = aCol;
    TexCoord = aTexCoord;
    Normal = aNormal;
    FragPos = vec3(model * vec4(aPos, 1.0));

    //ambient lighting
   vec3 ambient = material.ambient*lightColor;

   //diffuse lighting
   vec3 A = normalize(Normal);
   vec3 B = normalize(lightPos-FragPos);
   float dotValue = dot(A,B);

   float diffValue = max(dotValue,0.0f);
   vec3 diffuse = diffValue*lightColor;

   //specular lighting
   vec3 ref = reflect(-B,Normal);
   vec3 viewDir = normalize(camPos-FragPos);
   float dotValue2 = dot(ref,viewDir);
   float specValue = pow(max(dotValue2,0.0f),material.shininess);
   vec3 specular = specValue*material.specular*lightColor;

   result = (diffuse+ambient+specular);

    gl_Position = proj*view*model*vec4(aPos.x, aPos.y, aPos.z, 1.0);
}