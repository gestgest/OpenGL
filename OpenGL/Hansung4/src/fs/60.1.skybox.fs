#version 330 core
out vec4 FragColor;

in vec3 TexCoords; //방향벡터

uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, TexCoords);  //텍스쳐 매핑
}