#pragma once
#include <GL/glew.h>
#include <iostream>

#define ASSERT(x) if (!(x)) __debugbreak();

//초기화 -> x의 함수를 부르고 -> 체크
#define GLCHECK(x) glClearError();\
    x;\
    ASSERT(glCheckError(#x, __FILE__, __LINE__));

//그 전에 있던 에러 코드 배열? 초기화 함수
void glClearError();

//에러출력 함수
bool glCheckError(const char* function_name, const char* file_name, int line);