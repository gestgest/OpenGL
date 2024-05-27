#pragma once
#include "DebugRenderer.h"

class IndexBuffer {
private:
	unsigned int render_id; //일종의 랜더러 아이디가 필요하다
	unsigned int count;

public:
	//IndexBuffer(const void* data, unsigned int size);
	IndexBuffer(const unsigned int *data, unsigned int count); // 변수 타입이 정해졌으므로 사이즈 변수는 필요없다.
	~IndexBuffer();

	void bind() const;
	void unBind() const;

	inline unsigned int getCount() const { return count; }
};