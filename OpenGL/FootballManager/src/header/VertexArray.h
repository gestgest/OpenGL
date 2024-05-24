#pragma once
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray{
private:
	unsigned int renderer_id;
public:
	VertexArray();
	~VertexArray();

	void addBuffer(const VertexBuffer & vb, VertexBufferLayout& layout);

	void bind() const;
	void unBind() const;

};