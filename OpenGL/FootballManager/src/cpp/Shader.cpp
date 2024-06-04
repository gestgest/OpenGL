#include "../header/Shader.h"

Shader::Shader(const std::string& filepath)
{
    this->filepath = filepath;

    ShaderProgramSource shader_source = parseShader();
    this->id = createShader(shader_source.vertexSource, shader_source.fragmentSource);
}

Shader::~Shader()
{
    GLCHECK(glDeleteShader(id));
}


ShaderProgramSource Shader::parseShader()
{
    std::ifstream stream(filepath);
    //std::cout << filepath << '\n';

    enum class ShaderType
    {
        NONE = -1,
        VERTEX = 0,
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
    return { ss[0].str(), ss[1].str() };
}





unsigned int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader)
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


unsigned int Shader::compileShader(unsigned int type, const std::string& source)
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
        char* message = new char[length];  //힙에 저장

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

/// <summary>
/// ///////////////////////////////////////////////////////////////////////////////////////////////
/// </summary>

void Shader::bind() const
{
    GLCHECK(glUseProgram(id));
}

void Shader::unBind() const {
    GLCHECK(glUseProgram(0));
}



void Shader::setUniform1f(const std::string& name, float value)
{
    GLCHECK(glUniform1f(getUniformLocation(name), value));
}
void Shader::setUniform4f(const std::string& name, float v1, float v2, float v3, float v4)
{
    GLCHECK(glUniform4f(getUniformLocation(name), v1, v2, v3, v4));
}

void Shader::setUniform1i(const std::string& name, int value)
{
    GLCHECK(glUniform1i(getUniformLocation(name), value));
}

void Shader::setUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    //transpose는 전치 행렬 할거냐 (T)
    //(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
    GLCHECK(glUniformMatrix4fv(getUniformLocation(name),1,GL_FALSE, &matrix[0][0]));
}

//private
int Shader::getUniformLocation(const std::string& name)
{
    if (cache_location.count(name) != 0) {
        return cache_location[name];
    }

    int location = glGetUniformLocation(id, name.c_str());
    //ASSERT(location != -1);
    if (location == -1) {
        std::cout << "uniform don't exist " << '\n';
    }
    cache_location[name] = location;

    return cache_location[name];
}

