#pragma once

#include <string>
#include <memory>
#include <map>

#include <iostream>

struct TextureUnit
{
	unsigned int RendererID;

	std::string Filepath;
	std::string Name;

	unsigned char* LocalBuffer;
	int Width, Height, BPP;

	TextureUnit(std::string filepath = "");
	~TextureUnit();

private:
	static constexpr unsigned char m_White[4] = { 0xff, 0xff, 0xff, 0xff };
};

class Texture
{
private:
	std::shared_ptr<TextureUnit> m_TexUnit;

	static std::map<std::string, std::weak_ptr<TextureUnit>> m_Cache;

public:
	Texture(std::string filepath = "");
	~Texture();


	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return m_TexUnit->Width; }
	inline int GetHeight() const { return m_TexUnit->Height; }

	inline const int GetID() const { return m_TexUnit->RendererID; }
	inline const std::string& GetPath() const { return m_TexUnit->Filepath; }
	inline const std::string& GetName() const { return m_TexUnit->Name; }


	inline const unsigned char* GetBuffer() const { return m_TexUnit->LocalBuffer; }
	inline const int GetTexUseCount() const { return m_Cache.at(m_TexUnit->Filepath).use_count(); }
};