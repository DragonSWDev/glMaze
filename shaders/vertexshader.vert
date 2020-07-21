#version 330 core

layout (location = 0) in vec3 objectPosition;
layout (location = 1) in vec2 texturePosition;

out vec2 textureCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(objectPosition, 1.0f);
    textureCoords = texturePosition;
}