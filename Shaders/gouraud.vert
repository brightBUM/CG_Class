#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
//out vec3 Color;
out vec2 TexCoord;
out vec3 result;
vec3 FragPos;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;
vec3 Normal;

void main()
{
//    Color = aCol;
    gl_Position = proj*view*model*vec4(aPos.x, aPos.y, aPos.z, 1.0);
    TexCoord = aTexCoord;
    Normal = mat3(transpose(inverse(model))) * aNormal;
    FragPos = vec3(model * vec4(aPos, 1.0));

    //ambient 
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;

    //diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos-FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;


    //specular
    vec3 ref = reflect(-lightDir,norm);
    vec3 camDir = normalize(camPos-FragPos);
    float specValue = pow(max(dot(ref,camDir),0.0f),32.0f);
    vec3 specular = specValue*lightColor;

    vec3 objectColor = vec3(1.0);
    result = (ambient+diffuse+specular) * objectColor.rgb;

}