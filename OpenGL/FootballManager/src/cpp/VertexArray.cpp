#include "../header/VertexArray.h"

VertexArray::VertexArray() {
    GLCHECK(glGenVertexArrays(1, &renderer_id));
    //GLCHECK(glBindVertexArray(renderer_id)); -> 바인딩할 이유가 없음
}

//메모리 제거
VertexArray::~VertexArray() {
    GLCHECK(glDeleteVertexArrays(1, &renderer_id));
}

void VertexArray::addBuffer(const VertexBuffer& vb, VertexBufferLayout& layout)
{
    bind();
    vb.bind();

    std::vector<VertexBufferElement> v = layout.getElements();
    unsigned int offset = 0;

    for (int i = 0; i < v.size(); i++)
    {
        //정점 활성화
        glEnableVertexAttribArray(i);

        //버퍼안에 있는 정보의 속성을 알려주는 함수
        //몇번째 index 속성, 단위 size (x,y면 2개), type, 정규화여부, 정점 크기, 시작 위치
        glVertexAttribPointer(i, v[i].count, v[i].type, v[i].normalized, layout.getStride(), (const void*)offset);
        offset += v[i].count * VertexBufferElement::getSizeOfType(v[i].type);
    }
    
}

void VertexArray::bind() const{
    GLCHECK(glBindVertexArray(renderer_id));
}

void VertexArray::unBind() const {
    GLCHECK(glBindVertexArray(0));
}