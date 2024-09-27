#version 330 core
out vec4 FragColor;
  
in vec3 vertColor; // the input variable from the vertex shader (same name and same type)  
in vec2 texCoord;

uniform float _Time;
uniform vec3 _FragmentColors;

uniform sampler2D backgroundTexture;
uniform sampler2D bubbleTexture;

void main()
{
    FragColor = mix(texture(backgroundTexture, texCoord), texture(bubbleTexture, texCoord), 0.8);
} 