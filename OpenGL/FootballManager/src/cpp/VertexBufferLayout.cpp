#include "../header/VertexBufferLayout.h"

VertexBufferLayout::VertexBufferLayout() {
	stride = 0;
}

template<typename T>
void VertexBufferLayout::push(unsigned int count) {

}

template<>
void VertexBufferLayout::push<float>(unsigned int count) {
	unsigned int type = GL_FLOAT;
	elements.push_back({ type, count, GL_FALSE });
	stride += count * VertexBufferElement::getSizeOfType(type);
}

template<>
void VertexBufferLayout::push<unsigned int>(unsigned int count) {
	unsigned int type = GL_UNSIGNED_INT;
	elements.push_back({ type, count, GL_FALSE });
	stride += count * VertexBufferElement::getSizeOfType(type);
}

template<>
void VertexBufferLayout::push<unsigned char>(unsigned int count) {
	unsigned int type = GL_UNSIGNED_BYTE;
	elements.push_back({ type, count, GL_TRUE });
	stride += count * VertexBufferElement::getSizeOfType(type);
}

unsigned int VertexBufferLayout::getStride() {
	return this->stride;
}

std::vector<VertexBufferElement>& VertexBufferLayout::getElements() {
	return this->elements;
}
