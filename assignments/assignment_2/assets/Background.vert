#version 330 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
layout (location = 1) in vec3 aColor;  
layout (location = 2) in vec2 aTexCoord;

out vec3 vertColor; // specify a color output to the fragment shader
out vec2 texCoord;


uniform mat4 _ScalarMatrix;
uniform vec3 _Color;

uniform float _Time;
uniform float _Speed = 2;
uniform float _Amplitude = 0.3;

uniform mat4 _Rotation;

void main()
{
    vec3 pos = aPos;

    pos.x += sin(_Time * _Speed + pos.x) * _Amplitude;

    pos.y += sin(_Time * _Speed + pos.y) * _Amplitude;

    pos.z += sin(_Time * _Speed + pos.z) * _Amplitude;

    gl_Position = _Rotation * vec4(pos.x, pos.y, pos.z, 1.0); // see how we directly give a vec3 to vec4's constructor

    vertColor = aColor; 

    texCoord = aTexCoord;
}