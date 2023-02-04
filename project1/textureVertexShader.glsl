#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aText;

uniform vec4 offset_vec;

out vec2 textCoords;


void main()
{
    textCoords = aText;
    gl_Position = vec4(aPos.x+offset_vec.x, aPos.y+offset_vec.y, aPos.z, 1.0);
}
