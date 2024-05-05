#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define ASSERT(x) if (!(x)) __debugbreak();

//초기화 -> x의 함수를 부르고 -> 체크
#define GLCHECK(x) glClearError();\
    x;\
    ASSERT(glCheckError(#x, __FILE__, __LINE__));

//그 전에 있던 에러 코드 배열? 초기화 함수
static void glClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

//에러출력 함수
static bool glCheckError(const char* function_name, const char* file_name, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "Error Code [" << error << "] \nfilename : " << function_name 
            << "\nfile_name : " << file_name << "\nline : " << line << "\n";
        return false;
    }
    return true;
}

//참고로 한글 출력은 안됨
struct ShaderSource 
{
    std::string vertexSource;
    std::string fragmentSource;
};

static ShaderSource InputShader(const std::string& filepath)
{
    std::ifstream stream(filepath);
    //std::cout << filepath << '\n';

    enum class ShaderType 
    {
        NONE = -1,
        VERTEX= 0,
        FRAGMENT = 1,
    };

    ShaderType type = ShaderType::NONE;
    std::string line;
    std::stringstream ss[2];
    
    
    while (getline(stream, line, '\r'))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            if (type != ShaderType::NONE) {
                //스트림에다가 명령어 코드 넣는 거
                ss[(int)type] << line << '\n';
            }
        }
    }
    return { ss[0].str(), ss[1].str()};
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
    if (result == GL_FALSE) //셰이더 컴파일 실패
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length); //오류 로그의 길이를 가져와라
        char* message = new char [length];  //힙에 저장

        //char* message = (char*)alloca(sizeof(char) * length); 스택에 저장, 이 if문이 끝나면 없어진다.
        //단, 스택오버플로우가 날 수 있다.
        //영상에서는 스택 초기화 때문에 alloca를 썼지만 visual studio에서는 되도록이면 쓰지말라고 했다.
        
        glGetShaderInfoLog(id, length, &length, message); //쉐이더 정보 로그를 가져와라 => 실패 로그

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

    //수학 그래프 기준 x,y축
    float pos[8] = {
        -0.5f, 0.5f,
        -0.5f, -0.5f,
        0.5f, 0.5f,
        0.5f, -0.5f,
    };

    unsigned int index_pos[6] = {
        0, 1, 2,
        1, 2, 3
    };

    unsigned int buffer; //GLuint

    glGenBuffers(1, &buffer); //(버퍼 갯수, 버퍼 변수)
    glBindBuffer(GL_ARRAY_BUFFER, buffer); //정점에 대한 데이터를 생성할 버퍼로 할당
    glBufferData(GL_ARRAY_BUFFER, sizeof(pos) * sizeof(float), pos, GL_STATIC_DRAW);


    unsigned int ibo; //GLuint

    glGenBuffers(1, &ibo); //(버퍼 갯수, 버퍼 변수)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); //버퍼
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_pos) * sizeof(unsigned int), index_pos, GL_STATIC_DRAW);


    //정점 활성화
    glEnableVertexAttribArray(0);

    //버퍼안에 있는 정보의 속성을 알려주는 함수
    //몇번째 index 속성, 단위 size (x,y면 2개), type, 정규화여부, 정점 크기, 시작 위치
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    //버퍼 메모리 복원
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    //쉐이더 생성
    ShaderSource shader_source = InputShader("./basic.shader");

    unsigned int shader = createShader(shader_source.vertexSource, shader_source.fragmentSource);
    glUseProgram(shader);

    int location = glGetUniformLocation(shader, "u_color");
    ASSERT(location != -1)
    glUniform4f(location, 0.0f, 0.5f, 1.0f, 1.0f);

    // 렌더링 루프
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        //glDrawArrays(GL_TRIANGLES, 0, 6);
        GLCHECK(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

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
