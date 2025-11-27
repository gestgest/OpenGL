#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords; //원래는 vec2였지만 vec3로 바꿈


uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos; //방향벡터
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww; //항상 z는 w / w를 해서 1값이 나옴 => depth값이 1(맨 뒤)
}  