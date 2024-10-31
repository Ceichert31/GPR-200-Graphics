#version 330 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 vertColor; // specify a color output to the fragment shader
out vec2 texCoord;
out vec3 normal;
out vec3 fragPos;
out vec3 worldPos;


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

    gl_Position = _Projection * _View * _Model * pos;

    texCoord = aTexCoord;

    fragPos = vec3(_Model * vec4(aPos, 1.0));

    //For future reference calculate on cpu and send in uniform
    normal = mat3(transpose(inverse(_Model))) * aNormal;
}