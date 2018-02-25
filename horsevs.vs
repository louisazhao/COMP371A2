#version 330 core
layout(location=0) in vec3 aPos;
layout (location = 1) in vec3 normals;
layout (location=2) in vec2 aTexCoord;

out vec3 Normals;
out vec3 FragPos;
out vec3 LightPos;
out vec2 texCoordinate;

uniform vec3 lightPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position=projection*view*model*vec4(aPos,1.0);
    FragPos=vec3(view*model * vec4(aPos, 1.0));
    Normals=mat3(transpose(inverse(view*model)))*normals;
    LightPos=vec3(view*vec4(lightPos,1.0f));
    texCoordinate=vec2(aTexCoord.x,aTexCoord.y);
}

