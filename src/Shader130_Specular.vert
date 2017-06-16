#version 130	// If using nVidia gfx card, change 130 to 150
in vec3 position;
in vec3 normal;
in vec2 texCoords;

out vec3 fragPosition;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 Mmodel;
uniform mat4 Mmvp;
uniform mat4 Mtranspose_inverse_model;

void main()
{
    gl_Position = Mmvp * vec4(position, 1.0f);
    fragPosition = vec3(Mmodel * vec4(position, 1.0f));
    //Normal = mat3(transpose(inverse(Mmodel))) * normal;
    Normal = mat3(Mtranspose_inverse_model) * normal;
    TexCoords = vec2(texCoords.x, 1.0f - texCoords.y);
}

