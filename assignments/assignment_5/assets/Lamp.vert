#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 _Model;
uniform mat4 _View;
uniform mat4 _Projection;

void main()
{
	vec4 pos = vec4(aPos, 1.0);

    gl_Position = _Projection * _View * _Model * pos;
}