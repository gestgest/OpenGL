#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "src/header/Renderer.h"
#include "src/header/DebugRenderer.h"
#include "src/header/Shader.h"
#include "src/header/Texture.h"

#include "src/header/IndexBuffer.h"
#include "src/header/VertexBuffer.h"
#include "src/header/VertexArray.h"
#include "src/header/VertexBufferLayout.h"


void error_callback(int error, const char* description)
{
    std::cerr << "Error: " << description << std::endl;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main()
{
    GLFWwindow* window;

    // GLFW 라이브러리 초기화
    if (!glfwInit())
        exit(EXIT_FAILURE);

    //3.3.0 버전
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 윈도우 생성
    window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // 윈도우 이벤트 콜백 등록
    glfwSetErrorCallback(error_callback);
    glfwSetKeyCallback(window, key_callback);

    // OpenGL 컨텍스트 생성
    glfwMakeContextCurrent(window);

    //수직 동기화 => 주사율에 맞는 화면전환
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        std::cout << "error" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << glGetString(GL_VERSION) << std::endl; // 버전확인

    //수학 그래프 기준 x,y축
    /*
    float pos[] = {
        -0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 1.0f, 0.0f
    };
    */

    float pos[] = {
        -0.5f, 0.5f,
        -0.5f, -0.5f,
        0.5f, 0.5f,
        0.5f, -0.5f
    };

    unsigned int index_pos[6] = {
        0, 1, 2,
        1, 2, 3
    };

    VertexArray va;
    VertexBuffer vb(pos, sizeof(pos) * sizeof(float));
    IndexBuffer ib(index_pos, sizeof(index_pos));

    //VertexBufferLayout 라인
    VertexBufferLayout layout;
    layout.push<float>(2);
    //layout.push<float>(2);
    va.addBuffer(vb, layout);

    //버퍼 메모리 복원
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    //쉐이더 생성
    Shader sha("./res/basic.shader");
    sha.bind();

    Texture texture("./res/texture/thunder.png");
    texture.bind();
    //sha.setUniform1i("u_texture", 0);

    //바인드된 버퍼를 해제한다면?
    va.unBind();
    sha.unBind();
    GLCHECK(glBindBuffer(GL_ARRAY_BUFFER, 0)); //정점에 대한 데이터를 생성할 버퍼로 할당
    GLCHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); //버퍼

    Renderer renderer;

    float g = 0;
    float plus = 0.05f;

    // 렌더링 루프
    while (!glfwWindowShouldClose(window))
    {
        renderer.clear();

        sha.bind();
        sha.setUniform4f("u_color", 0.0f, g, 1.0f, 1.0f);
        /*
        sha.setUniform4f("u_color", 0.0f, g, 1.0f, 1.0f);

        va.bind();
        ib.bind();
        */
        renderer.draw(va, ib, sha);


        //glDrawArrays(GL_TRIANGLES, 0, 6);
        //GLCHECK(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        if (g < 0.0f) {
            plus = 0.05f;
        }
        else if (1.0f < g) {
            plus = -0.05f;
        }
        g += plus;

        /* 렌더링 작업
        glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(-0.5f, 0.5f);
        glVertex2f(0.5f, 0.5f);
        glEnd();
        */

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 종료
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
