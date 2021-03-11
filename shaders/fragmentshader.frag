#version 330 core

in vec2 textureCoords;
in vec3 normalVector;
in vec3 fragmentPosition;
in vec3 lightPosition;

out vec4 FragColor;

uniform vec3 lightColor;
uniform sampler2D texture1;

void main()
{
    //Phong shading

    //Ambient
    float ambientStrenght = 0.1;
    vec3 ambient = ambientStrenght * lightColor;

    //Diffuse
    vec3 normal = normalize(normalVector);
    vec3 lightDirection = normalize(lightPosition - fragmentPosition);
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = diff * lightColor;

    //Specular
    float specularStrength = 0.5;
    vec3 viewDirection = normalize(-fragmentPosition);
    vec3 reflectDirection = reflect(-lightDirection, normalVector);
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 64);
    vec3 specular = specularStrength * spec * lightColor;

    //Point light attentuation (from Ogre3D)
    float distance = length(lightPosition - fragmentPosition);
    float attenuation = 1.0 / (1.0 + 0.35 * distance + 0.44 * (distance * distance));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 lightResult = (ambient + diffuse + specular) * texture(texture1, textureCoords).rgb;

    FragColor = vec4(lightResult, 1.0);
}