#include "../header/IndexBuffer.h"
#include "../header/Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
{
    this->count = count;
    
    glGenBuffers(1, &render_id); //(버퍼 갯수, 버퍼 변수)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_id); //버퍼
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW);

    //GLuint == unsigned int 단, 다를 수도 있으니 GLuint로 하는 습관을 키우자
}
IndexBuffer::~IndexBuffer()
{
    GLCHECK(glDeleteBuffers(1, &render_id));
}

void IndexBuffer::bind() const
{ 
    GLCHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_id));
}

void IndexBuffer::unBind() const
{
    GLCHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0););
    
}
