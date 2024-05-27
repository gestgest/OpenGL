#pragma once
#include <vector>
#include "GL/glew.h"
#include "DebugRenderer.h"


struct VertexBufferElement {
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int getSizeOfType(unsigned int type)
	{
		switch (type) {
		case GL_FLOAT: return 4;
		case GL_UNSIGNED_INT: return 4;
		case GL_UNSIGNED_BYTE: return 1;
		}
		ASSERT(0);
		return 0;
	}
};

class VertexBufferLayout {

private:
	std::vector<VertexBufferElement> elements;
	unsigned int stride; //몇 바이트
public:
	VertexBufferLayout();


	template<typename T>
	void push(unsigned int count);
	
	template<>
	void push<float>(unsigned int count);

	template<>
	void push<unsigned int>(unsigned int count);

	template<>
	void push<unsigned char>(unsigned int count);

	unsigned int getStride(); //원래는 인라인 함수여야 함

	std::vector<VertexBufferElement>& getElements(); //원래는 인라인 함수여야 함
};