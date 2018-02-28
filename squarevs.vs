#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 normals;
layout(location=2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;
uniform vec3 lightPos;


out vec2 texCoordinate;
out vec4 FragPosLightSpace;
out vec3 Normals;
out vec3 FragPos;
out vec3 LightPos;

void main()
{
    gl_Position=projection*view*model*vec4(aPos,1.0);
    FragPos=vec3(view*model * vec4(aPos, 1.0));
    Normals=mat3(transpose(inverse(view*model)))*normals;
    LightPos=vec3(view*vec4(lightPos,1.0f));
    texCoordinate=vec2(aTexCoord.x,aTexCoord.y);
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
}

