#pragma once
#include <vector>


struct VertexBufferElement {
	
};

class VertexBufferLayout {
private:
	std::vector<VertexBufferElement> elements;
public:
	VertexBufferLayout();
	~VertexBufferLayout();

	template<typename T>
	void Push(int count)
	{
		
	}
	void Push<float>(int count)
	{

	}

};