#version 330 core
out vec4 FragColor;
  
in vec3 vertColor; // the input variable from the vertex shader (same name and same type)  
in vec2 texCoord;

uniform float _Time;
uniform vec3 _FragmentColors;

uniform sampler2D fishTexture;
uniform sampler2D flounderTexture;

void main()
{
   FragColor = texture(fishTexture, texCoord);
} 