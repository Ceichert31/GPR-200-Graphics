#version 330 core
out vec4 FragColor;

in vec2 texCoord;

uniform float _Time;
uniform vec3 _FragmentColors;

uniform sampler2D fishTexture;
uniform sampler2D flounderTexture;

void main()
{
   FragColor = texture(fishTexture, texCoord);
} 