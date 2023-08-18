#include "Material.h"
#include "Texture.h"

Material::Material(std::string name, class Texture* texture, glm::vec3 color, bool useTexture)
	: mName(name)
	, mTexture(texture)
	, mColor(color)
    , mUseTexture(useTexture)
{
}

GLuint Material::GetTextureID()
{
	return mTexture->GetTextureID();
}