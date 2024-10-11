#version 330 core
out vec4 FragColor;
  
in vec2 texCoord;

uniform float _Time;
uniform vec3 _FragmentColors;

uniform sampler2D backgroundTexture;
uniform sampler2D bubbleTexture;

void main()
{
    FragColor = mix(texture(backgroundTexture, texCoord), texture(bubbleTexture, texCoord), 0.2);
} 