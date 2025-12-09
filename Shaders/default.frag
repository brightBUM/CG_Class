#version 330 core
out vec4 FragColor;

//in vec3 Color;
in vec2 TexCoord;
uniform sampler2D texSampler1;
uniform sampler2D texSampler2;
uniform float mixValue;
uniform vec3 lightColor;
uniform vec3 lightPos;
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

    vec3 result = (ambient+diffuse) * FragColor.rgb;

	FragColor = vec4(result,FragColor.a);
}