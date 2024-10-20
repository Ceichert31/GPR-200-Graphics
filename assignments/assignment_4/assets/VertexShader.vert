#version 330 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
layout (location = 1) in vec2 aTexCoord;

out vec3 vertColor; // specify a color output to the fragment shader
out vec2 texCoord;


uniform mat4 _ScalarMatrix;
uniform vec3 _Color;

uniform float _Time;
uniform float _Speed = 2;
uniform float _Amplitude = 0.3;

uniform mat4 _Model;
uniform mat4 _View;
uniform mat4 _Projection;

//Scale rotate translate
void main()
{
    vec4 pos = vec4(aPos, 1.0);

    gl_Position = _Projection * _View * _Model * pos; // see how we directly give a vec3 to vec4's constructor

    texCoord = aTexCoord;
}