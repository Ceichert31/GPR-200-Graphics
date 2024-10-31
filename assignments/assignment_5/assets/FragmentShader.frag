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
uniform vec3 viewPos;

//Lighting settings
float ambientStrength = 0.3;
float specularStrength = 0.5;
float shininess = 32.0;

vec3 lightColor = vec3(1.0, 1.0, 1.0);

void main()
{
	//FragColor = mix(texture(waterTexture, texCoord), texture(fishTexture, texCoord), 0.2);

	vec3 norm = normalize(normal);

	//Ambient lighting
	vec3 ambient = ambientStrength * lightColor;


	//Diffuse Lighting
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	//Specular Lighting
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);

	float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * texture(waterTexture, texCoord).rgb;

	FragColor = vec4(result, 1.0);
} 