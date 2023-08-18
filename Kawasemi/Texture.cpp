#include "Texture.h"

Texture::Texture(std::string path)
    : mPath(path)
{
	unsigned char header[54] = { 0 };
	unsigned int dataPos;
	unsigned int width, height;
	unsigned int imageSize;
	unsigned char* data;

	std::fstream ifs(path, std::ios::in | std::ios::binary);
	if (!ifs) { std::cout << "Could not open image file" << std::endl; }

	ifs.read((char*)header, 54);

	if (header[0] != 'B' || header[1] != 'M')
	{
		std::cout << "this is not a bmp file" << std::endl;
	}

	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);
	if (imageSize == 0) imageSize = width * height * 3;
	if (dataPos == 0) dataPos = 54;

	data = new unsigned char[imageSize];
	ifs.read((char*)data, imageSize);
	ifs.close();

	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	delete[] data;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);
}