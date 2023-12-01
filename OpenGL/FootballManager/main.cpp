#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

static int compileShader(unsigned char type, const std::string& source)
{
    //?
    unsigned char id = glCreateShader(type);
}

//정적으로 선언 [정점 셰이더, 프래그먼트 셰이더]
static int createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    //프로그램을 만든다.
    unsigned char program = glCreateProgram();
    unsigned char vs = compileShader(GL_VERTEX_SHADER, vertexShader);
}

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

    if (glewInit() != GLEW_OK) {
        std::cout << "error" << std::endl;
        exit(EXIT_FAILURE);
    }
    //std::cout << glGetString(GL_VERSION) << std::endl; 버전확인

    float pos[6] = {
        -0.5f, -0.5f,
        -0.5f, 0.5f,
        0.5f, 0.5f
    };

    unsigned int buffer; //GLuint
    
    glGenBuffers(1, &buffer); //(버퍼 갯수, 버퍼 변수)
    glBindBuffer(GL_ARRAY_BUFFER, buffer); //정점에 대한 데이터를 생성할 버퍼로 할당
    glBufferData(GL_ARRAY_BUFFER, sizeof(pos) * sizeof(float), pos, GL_STATIC_DRAW);

    //정점 활성화
    glEnableVertexAttribArray(0);

    //버퍼안에 있는 정보의 속성을 알려주는 함수
    //몇번째 index 속성, 단위 size (x,y면 2개), type, 정규화여부, 정점 크기, 시작 위치
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    //버퍼 메모리 복원
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // 렌더링 루프
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

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
