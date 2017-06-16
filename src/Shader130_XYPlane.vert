#version 130

in vec3 position;
in vec3 color;

out vec3 ourColor;

uniform mat4 Mmvp;

void main()
{
    gl_Position = Mmvp * vec4(position, 1.0f);
    ourColor = color;
}
