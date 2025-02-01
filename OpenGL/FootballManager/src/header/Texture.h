#pragma once
#include "Renderer.h"
#include "DebugRenderer.h"
#include "stb/stb_image.h"

class Texture {
private:
	unsigned int id;
	std::string filePath;
	unsigned char* localBuffer;
	int width, height, pixel_bit; //bpp : 1 픽셀당 비트
public:
	Texture(const std::string& path);
	~Texture();

	void bind(unsigned int slot = 0) const;
	void unBind();

	inline int getWidth() const;
	inline int getHeight() const;
	unsigned int getId() const;
	//만약 inline으로 설정하면 오류가 생김. => 외부 참조를 하면 시스템상 컴파일러가 인식을 못함.

};