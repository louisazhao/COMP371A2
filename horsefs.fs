#version 330 core

in vec3 Normals;
in vec3 FragPos;
in vec3 LightPos;
in vec2 texCoordinate;

out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 partColor;
uniform sampler2D horseTex;
uniform bool texOn;

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
        vec3 result=(ambient+diffuse+specular)*partColor;
        FragColor = vec4(result, 1.0);
    }
    else
    {
        vec3 lightResult=(ambient+diffuse+specular);
        FragColor = vec4(lightResult,1.0)*texture(horseTex,texCoordinate);
        
    }
}


