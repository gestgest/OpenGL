#include "../header/Texture.h"

Texture::Texture(const std::string& path) {
	this->filePath = path;
	localBuffer = nullptr;
	this->width = 0;
	this->height = 0;
	this->pixel_bit = 0;

	//GL 형식상 png의 텍스처를 거꾸로 만들어야 함 [상하반전]
	//GL은 왼쪽 하단에서 시작하기 때문, png는 왼쪽 상단
	stbi_set_flip_vertically_on_load(1); //상하반전 함수
	localBuffer = stbi_load(path.c_str(), &width, &height, &pixel_bit, 4); //4 -> rgba, 채널 4개
	
	GLCHECK(glGenTextures(1, &id));
	GLCHECK(glBindTexture(GL_TEXTURE_2D, id));

	//텍스처 파라미터 넣는 함수. 필수로 4개의 파라미터를 지정해야 함 = > 안하면 검은색 질감으로 보임
	GLCHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE)); //x
	GLCHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE)); //y

	//타겟, 레벨(다중 레벨이면 1이상), 내부포멧, width, height, border, 외부포멧, 타입, 픽셀버퍼(nullptr)
	GLCHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer));
	GLCHECK(glBindTexture(GL_TEXTURE_2D, 0)); //바인딩 해제

	if (localBuffer)
		stbi_image_free(localBuffer);

}

Texture::~Texture() 
{
	GLCHECK(glDeleteTextures(1, &id));
}

void Texture::bind(unsigned int slot) const
{
	GLCHECK(glActiveTexture(GL_TEXTURE0 + slot)); //0슬롯 활성화 (GL_TEXTURE0 + 1)
	GLCHECK(glBindTexture(GL_TEXTURE_2D, id));
}

void Texture::unBind() {
	GLCHECK(glBindTexture(GL_TEXTURE_2D, 0)); //바인딩 해제
}

inline int Texture::getWidth() const {
	return this->width;
}

inline int Texture::getHeight() const {
	return this->height;
}
unsigned int Texture::getId() const {
	return this->id;
}