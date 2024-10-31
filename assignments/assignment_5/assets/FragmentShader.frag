#version 330 core
out vec4 FragColor;

in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;
in vec3 worldPos;

uniform float _Time;

uniform sampler2D fishTexture;
uniform sampler2D waterTexture;

uniform vec3 lightPos;

float ambientStrength = 0.1;	

vec3 lightColor = vec3(1.0, 1.0, 1.0);

void main()
{
   //FragColor = mix(texture(waterTexture, texCoord), texture(fishTexture, texCoord), 0.2);

   vec3 norm = normalize(normal);

   vec3 lightDir = normalize(lightPos - fragPos);

   float diff = max(dot(norm, lightDir), 0.0);

   vec3 diffuse = diff * lightColor;

   vec3 ambient = ambientStrength * lightColor;

   vec3 result = (ambient + diffuse) * texture(waterTexture, texCoord).rgb;

   FragColor = vec4(result, 1.0);
} 