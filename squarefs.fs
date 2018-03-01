#version 330 core
in vec2 texCoordinate;
//in vec4 FragPosLightSpace;
in vec3 Normals;
in vec3 FragPos;
in vec3 LightPos;

out vec4 FragColor;

uniform bool texOn;
uniform sampler2D grassTex;
uniform sampler2D shadowMap;
uniform vec3 lightColor;

void main()
{
    //ambient
    float ambientDegree=0.5;
    vec3 ambient=ambientDegree*lightColor;
    
    //diffuse
    vec3 norm=normalize(Normals);
    vec3 lightDir=normalize(LightPos-FragPos);
    float diff=max(dot(norm,lightDir),0.0f);
    vec3 diffuse=diff*lightColor;
    
    //specular
    float specularStrength=0.5f;
    vec3 viewDir=normalize(-FragPos);
    vec3 reflectDir=reflect(-lightDir,norm);
    float spec=pow(max(dot(viewDir,reflectDir),0.0),32);
    vec3 specular=specularStrength*spec*lightColor;
    
    if(texOn==false)
    {
        vec4 color=vec4(0.5f,0.5f,0.5f,1.0f);
        vec4 result=vec4((ambient+diffuse+specular),1.0f)*color;
        FragColor = result;
    }
    else
    {
        vec3 lightResult=(ambient+diffuse+specular);
        FragColor=vec4(lightResult,1.0)*texture(grassTex,texCoordinate);
    }
}

