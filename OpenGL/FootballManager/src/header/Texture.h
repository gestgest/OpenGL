#pragma once
#include "Renderer.h"

class Texture {
private:
	unsigned int id;
	std::string filePath;
	unsigned char* localBuffer;
	int width, height, bpp;
public:
	Texture(const std::string& path);
	~Texture();

	void bind() const;
	void unBind();

	inline int getWidth() const;
	inline int getHeight() const;
};