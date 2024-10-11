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


mat4 Scale(float x, float y, float z)
{
return mat4(
//Row 0
	x, 0.0, 0.0, 0.0, //Column 0 
	0.0, y, 0.0, 0.0,
	0.0, 0.0, z, 0.0,
    0.0, 0.0, 0.0, 1.0
);
}

//Rotate a radians around z axis
mat4 RotateZ(float a)
{
    mat4 m = mat4(1); //Identity
    m[0][0] = cos(a); //Col 0, Row 0
    m[1][0] = -sin(a);
    m[0][1] = sin(a);
    m[1][1] = cos(a);
    return m;
}

mat4 Translate(float x, float y, float z)
{
    mat4 m = mat4(1);
    m[3][0] = x; 
    m[3][1] = y;
    m[3][2] = z;
    return m;
}
//Scale rotate translate
void main()
{
    vec4 pos = vec4(aPos, 1.0);

    //pos = Scale(0.5, 0.5, 1.0) * pos;

    //pos = Translate(0.5, 0.5, 0.0) * pos;

    //pos.x += sin(_Time * _Speed + pos.x) * _Amplitude;

    //pos.y += sin(_Time * _Speed + pos.y) * _Amplitude;

    //pos.z += sin(_Time * _Speed + pos.z) * _Amplitude;

    gl_Position = _Projection * _View * _Model * pos; // see how we directly give a vec3 to vec4's constructor

    texCoord = aTexCoord;
}