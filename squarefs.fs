#version 330 core
in vec2 texCoordinate;

out vec4 FragColor;

uniform bool texOn;
uniform sampler2D grassTex;

void main()
{
    if(texOn==false)
    {
        FragColor=vec4(0.5f,0.5f,0.5f,1.0f);
    }
    else
    {
        FragColor=texture(grassTex,texCoordinate);
    }
}

