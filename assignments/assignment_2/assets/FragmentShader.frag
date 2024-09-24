#version 330 core
out vec4 FragColor;
  
in vec3 vertColor; // the input variable from the vertex shader (same name and same type)  

uniform float _Time;
uniform vec3 _FragmentColors;

void main()
{
    vec3 finalColor = vertColor + _FragmentColors;

    FragColor = vec4(finalColor, 1.0);
} 