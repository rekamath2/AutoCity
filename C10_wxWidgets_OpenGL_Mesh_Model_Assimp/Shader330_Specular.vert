#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec3 fragPosition;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 Mmodel;
uniform mat4 Mmvp;

void main()
{
    gl_Position = Mmvp * vec4(position, 1.0f);
    fragPosition = vec3(Mmodel * vec4(position, 1.0f));
    Normal = mat3(transpose(inverse(Mmodel))) * normal;
    TexCoords = vec2(texCoords.x, 1.0f - texCoords.y);
}

