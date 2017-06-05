#version 330 core

in vec3 position;

uniform mat4 Mmvp;

void main()
{
    gl_Position = Mmvp * vec4(position, 1.0f);
}
