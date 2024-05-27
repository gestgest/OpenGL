#include "../header/DebugRenderer.h"


void glClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool glCheckError(const char* function_name, const char* file_name, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "Error Code [" << error << "] \nfilename : " << function_name
            << "\nfile_name : " << file_name << "\nline : " << line << "\n";
        return false;
    }
    return true;
}
