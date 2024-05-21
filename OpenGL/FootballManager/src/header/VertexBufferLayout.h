#pragma once
#include <vector>
#include "GL/glew.h"


struct VertexBufferElement {
	unsigned int type;
	unsigned int count;
	bool isNormalized;
};

class VertexBufferLayout {
private:
	std::vector<VertexBufferElement> elements;
	unsigned int stride; //몇 바이트
public:
	VertexBufferLayout();
	~VertexBufferLayout();

	template<typename T>
	void push(int count);
	
	template<typename T>
	void push<float>(unsigned int count);

	template<typename T>
	void push<unsigned int>(unsigned int count);

	template<typename T>
	void push<unsigned char>(unsigned int count);

	inline unsigned int getStride() const;
	inline const std::vector<VertexBufferElement>& getElements() const&;
};