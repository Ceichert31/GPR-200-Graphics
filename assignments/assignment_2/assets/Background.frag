#version 330 core
out vec4 FragColor;
  
in vec3 vertColor; // the input variable from the vertex shader (same name and same type)  
in vec2 texCoord;

uniform float _Time;
uniform vec3 _FragmentColors;

uniform sampler2D ourTexture;

void main()
{
    //vec3 finalColor = vertColor + _FragmentColors;

    //FragColor = vec4(finalColor, 1.0);

    FragColor = texture(ourTexture, texCoord);
} 