#version 330 core

in vec2 textCoords;

out vec4 FragColor;

uniform vec4 set_color;
uniform bool use_set_color;

uniform sampler2D ourTexture;

void main()
{
    if (use_set_color) {
        FragColor = set_color;
        return;
    }
    FragColor = texture(ourTexture,textCoords);
    
}