#version 330 core

layout (location = 0) in vec3 aVertexPosition;
layout (location = 1) in vec2 aTexturePosition;
layout (location = 2) in vec3 aNormalAttribute;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec3 lightVector;

out vec2 textureCoords;
out vec3 normalVector;
out vec3 fragmentPosition;
out vec3 lightPosition;

void main()
{
    textureCoords = aTexturePosition;
    normalVector = aNormalAttribute;

    //Change to view space before sending to fragment shader
    fragmentPosition = vec3(view * model * vec4(aVertexPosition, 1.0));
    lightPosition = vec3(view * vec4(lightVector, 1.0));

    gl_Position = projection * view * model * vec4(aVertexPosition, 1.0f);
}