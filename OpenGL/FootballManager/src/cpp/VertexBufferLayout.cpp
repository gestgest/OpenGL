#include "../header/VertexBufferLayout.h"

VertexBufferLayout::VertexBufferLayout() {
	stride = 0;
}

VertexBufferLayout::~VertexBufferLayout() {

}

template<typename T>
void VertexBufferLayout::push(int count) {

}

template<typename T>
void VertexBufferLayout::push<float>(unsigned int count) {
	stride += sizeof(GLfloat);
	elements.push_back({ GL_FLOAT, count, false });
}

template<typename T>
void VertexBufferLayout::push<unsigned int>(unsigned int count) {
	stride += sizeof(GLuint);
	elements.push_back({ GL_UNSIGNED_INT, count, false });
}

template<typename T>
void VertexBufferLayout::push<unsigned char>(unsigned int count) {
	stride += sizeof(GLbyte);
	elements.push_back({ GL_UNSIGNED_BYTE, count, false });
}

inline unsigned int VertexBufferLayout::getStride() const {
	return this->stride;
}
inline const std::vector<VertexBufferElement>& VertexBufferLayout::getElements() const& {
	return this->elements;
}
