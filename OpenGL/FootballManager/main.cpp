#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

static void InputShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType 
    {
        NONE = -1,
        VERTEX= 0,
        FRAGMENT = 1,
    };

    ShaderType type = ShaderType::NONE;
    std::string line;
    std::stringstream ss[2];
    
    
    while (getline(stream, line) )
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("#vertex") != std::string::npos)
            {

            }
            else if (line.find("#fragment") != std::string::npos)
            {

            }
        }
    }

}

static unsigned int compileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type); //셰이더
    const char* src = &source[0]; //source[]에서 source[0] 의 주소값
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    //std::cout << id << std::endl;

    //에러 핸들링
    int result; 
    glGetShaderiv(id, GL_COMPILE_STATUS, &result); //result가 반환함
    //std::cout << id << std::endl;
    if (result == GL_FALSE) //셰이더 컴파일 실패
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length); //오류 로그의 길이를 가져와라
        char* message = new char [length];  //힙에 저장

        //char* message = (char*)alloca(sizeof(char) * length); 스택에 저장, 이 if문이 끝나면 없어진다.
        //단, 스택오버플로우가 날 수 있다.
        //영상에서는 스택 초기화 때문에 alloca를 썼지만 visual studio에서는 되도록이면 쓰지말라고 했다.
        
        glGetShaderInfoLog(id, length, &length, message); //쉐이더 정보 로그를 가져와라 => 실패 로그

        //여담으로 한글쓰면 오류난다.
        std::cout << "Compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "Shader Error" << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(id);

        delete[] message;
        return 0;
    }

    return id;
}

//정적으로 선언 [정점 셰이더, 프래그먼트 셰이더]
static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    //프로그램을 만든다.
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader); //vertexShader를 만듬
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader); //fragmentShader를 만듬

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);

    int linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    if (linkStatus != GL_TRUE) {
        // 프로그램 링크 오류 처리 코드
        int length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        char* message = new char[length];
        glGetProgramInfoLog(program, length, &length, message);

        std::cout << length << std::endl;
        std::cout << "Linking Error" << std::endl;
        //std::cout << message << std::endl;

        delete[] message;
        return 0;
    }

    glValidateProgram(program);

    return program;
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
    //std::cout << glGetString(GL_VERSION) << std::endl; // 버전확인

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

    //쉐이더 생성
    const std::string vertexShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec3 position;"
        "\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
        "}\0";

    const std::string fragmentShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   color = vec4(0.5, 0.0, 0.0, 1.0);\n"
        "}\0";

    unsigned int shader = createShader(vertexShader, fragmentShader);
    glUseProgram(shader);

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

    //쉐이더 지우기
    glDeleteShader(shader);
    
    // 종료
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
