#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 eyePos;
uniform samplerCube skybox;

void main()
{    
    //월드공간
    vec3 I = normalize(Position - eyePos); //포지션에서 눈을 빼야함. 무조건 방향 벡터로 해야한다
    vec3 R = reflect(I, normalize(Normal)); //순서 바뀌면 큰일남 => 저 Normal은 fragment의 벡터임. 크기가 1이 아닐 수 있다.
    //FragColor = vec4(texture(skybox, R).rgb, 1.0);
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}