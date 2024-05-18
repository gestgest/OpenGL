#include "../header/VertexBuffer.h"
#include "../header/Renderer.h"


VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    glGenBuffers(1, &render_id); //(버퍼 갯수, 버퍼 변수)
    glBindBuffer(GL_ARRAY_BUFFER, render_id); //정점에 대한 데이터를 생성할 버퍼로 할당
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW); //sizeof(pos) * sizeof(float)

}
VertexBuffer::~VertexBuffer()
{
    GLCHECK(glDeleteBuffers(1, &render_id));
}

void VertexBuffer::bind() const
{
    GLCHECK(glBindBuffer(GL_ARRAY_BUFFER, render_id));
}

void VertexBuffer::unBind() const
{
    GLCHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

