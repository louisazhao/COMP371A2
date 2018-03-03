#version 330 core
in vec2 texCoordinate;
in vec4 FragPosLightSpace;
in vec3 Normals;
in vec3 FragPos;
//in vec3 LightPos;

out vec4 FragColor;

uniform bool texOn;
uniform bool shadowOn;
uniform sampler2D grassTex;
uniform sampler2D shadowMap;
uniform vec3 lightColor;
uniform vec3 LightPos;
uniform vec3 viewPos;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xz).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.y;
    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.3;
    return shadow;
}

void main()
{
    float shadow=ShadowCalculation(FragPosLightSpace);
    vec3 color=vec3(0.5f,0.5f,0.5f);
    if (texOn) {
        color=texture(grassTex,texCoordinate).rgb;
    }
    //ambient
    float ambientDegree=0.3;
    vec3 ambient=ambientDegree*lightColor;
    
    //diffuse
    vec3 norm=normalize(Normals);
    vec3 lightDir=normalize(FragPos-LightPos);
    //vec3 lightDir=normalize(LightPos-FragPos);
    //vec3 lightDir=normalize(-LightPos);
    //vec3 lightDir=normalize(vec3(0,3,0));
    float diff=max(dot(norm,lightDir),0.0);
    vec3 diffuse=diff*lightColor;
    
    //specular
    float specularStrength=0.5f;
    vec3 viewDir=normalize(viewPos-FragPos);
    vec3 reflectDir=reflect(-lightDir,norm);
    //float spec=0.0;
    //vec3 halfwayDir=normalize(lightDir+viewDir);
    float spec=pow(max(dot(viewDir,reflectDir),0.0),32);
    vec3 specular=specularStrength*spec*lightColor;
    
    
    vec3 lightResult=(ambient+diffuse+specular)*color;
    if(shadowOn)
    {
        lightResult=(ambient+(1.0-shadow)*(diffuse+specular))*color;
    }

    FragColor = vec4(lightResult, 1.0);
}

