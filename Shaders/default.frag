#version 330 core
out vec4 FragColor;

//in vec3 Color;
in vec2 TexCoord;
uniform sampler2D texSampler1;
uniform sampler2D texSampler2;
uniform float mixValue;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;
in vec3 Normal;
in vec3 FragPos;

void main()
{
   FragColor = mix(texture(texSampler1, TexCoord), texture(texSampler2, TexCoord), mixValue);

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
    float specValue = pow(max(dot(ref,camDir),0.0f),16.0f);
    vec3 specular = specValue*lightColor;

    vec3 objectColor = vec3(1.0);
    vec3 result = (ambient+diffuse+specular) * objectColor.rgb;
	FragColor = vec4(result,1.0f);
}