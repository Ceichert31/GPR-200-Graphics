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
uniform float ambientStrength = 0.3;
uniform float specularStrength = 0.5;
uniform float diffuseStrength = 0.0;
uniform float shininess = 32.0;
uniform bool blinnPhong;

uniform vec3 lightColor;

void main()
{
	//FragColor = mix(texture(waterTexture, texCoord), texture(fishTexture, texCoord), 0.2);

	vec3 norm = normalize(normal);

	//Ambient lighting
	vec3 ambient = ambientStrength * lightColor;


	//Diffuse Lighting
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(norm, lightDir), diffuseStrength);
	vec3 diffuse = diff * lightColor;

	//Specular Lighting
	vec3 viewDir = normalize(viewPos - fragPos);

	float spec = 0.0;

	//Blinn-Phong specular lighting
	if (blinnPhong)
	{
		vec3 halfwayDir = normalize(lightDir + viewDir);
		spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
	}
	else
	{
		vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	}

	vec3 specular = spec * lightColor;
	vec3 result = (ambient + diffuse + specular) * texture(waterTexture, texCoord).rgb;

	FragColor = vec4(result, 1.0);
} 