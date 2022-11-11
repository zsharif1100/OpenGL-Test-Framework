#include "Texture.h"

#include <GL/glew.h>
#include "GLErrorManager.h"

#include "stb_image/stb_image.h"

std::map<std::string, std::weak_ptr<TextureUnit>> Texture::m_Cache;

TextureUnit::TextureUnit(std::string filepath)
	:RendererID(0), Filepath(filepath), LocalBuffer(nullptr), Width(1), Height(1), BPP(0)
{
	if (!Filepath.empty())
	{
		auto lastSlashPos = Filepath.rfind("/");
		Name = Filepath.substr(lastSlashPos);
	}

	GLCall(glGenTextures(1, &RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, RendererID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));  //behaviour when scaled down
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)); //behaviour when scaled up
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE)); //horizontal wrap (don't extend area)
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE)); //vertical wrap (don't extend area)

	if (filepath.empty()) {
		Width = 1; Width = 1;
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &m_White));
	}
	else {
		stbi_set_flip_vertically_on_load(1);
		LocalBuffer = stbi_load(filepath.c_str(), &Width, &Height, &BPP, 4);
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, LocalBuffer));
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));
	}

	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

TextureUnit::~TextureUnit()
{
	if (LocalBuffer) {
		stbi_image_free(LocalBuffer);
		LocalBuffer = nullptr;
	}

	GLCall(glDeleteTextures(1, &RendererID));
}

Texture::Texture(std::string filepath)
{

	if (m_Cache.count(filepath)) {
		m_TexUnit = m_Cache.at(filepath).lock();
	}
	else {
		m_TexUnit = std::make_shared<TextureUnit>(filepath);
		m_Cache.insert({ filepath, std::weak_ptr<TextureUnit>(m_TexUnit) });
	}

	//printf("CREATE:  '%-30s'\tID %i : %i shared \tBuffer: %p\n", this->GetName().c_str(), this->GetID(), m_TexUnit.use_count(), this->GetBuffer());
}

Texture::~Texture()
{
	int count = m_Cache.count(m_TexUnit->Filepath) ? m_Cache.at(m_TexUnit->Filepath).use_count() : 0;

	//printf("DESTROY: '%-30s'\tID %i : %i shared \tBuffer: %p\n", m_TexUnit->Name.c_str(), m_TexUnit->RendererID, m_TexUnit.use_count(),this->GetBuffer());

	if (this->GetTexUseCount() == 1)
	{
		std::string filepath = m_TexUnit->Filepath;
		m_TexUnit.reset();
		m_Cache.erase(filepath);
	}
}

void Texture::Bind(unsigned int slot /*= 0*/) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_TexUnit->RendererID));
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}