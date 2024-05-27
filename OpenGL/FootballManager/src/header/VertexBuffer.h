#pragma once
#include "DebugRenderer.h"

class VertexBuffer {
private:
	unsigned int render_id; //일종의 랜더러 아이디가 필요하다
public:
	VertexBuffer(const void *data, unsigned int size);
	~VertexBuffer();

	void bind() const;
	void unBind() const;

};