#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 textCoords;


out vec2 texture_coordinates;

uniform mat4 transform;

void main()
{
    texture_coordinates = textCoords;
    gl_Position = transform * vec4(aPos, 1.0);
}
