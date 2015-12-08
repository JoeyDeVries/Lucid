#ifndef FONT_H
#define FONT_H
#include "../Renderer/texture2D.h"
#include <glm/glm.hpp>
#include <map>
#include <memory>

struct CharInfo {
	glm::vec2 Position;
	glm::vec2 Scale;
	glm::vec2 Offset;
	float XAdvance;	
};

struct CharRenderInfo {
	glm::vec4 Vertices[6];
    //float XAdvance;
};

class Font
{
private:
	std::map<char, CharInfo> m_CharInfo;
	std::map<char, CharRenderInfo> m_CharRenderInfo;
	std::shared_ptr<Texture2D> m_FontAtlas;

	int m_Width, m_Height;
    int m_CharSize;
public:
	Font();

	void Load(std::shared_ptr<Texture2D> fontAtlas, std::map<char, CharInfo> charInfo, int charSize, int width, int height);

	const std::map<char, CharInfo>&       GetCharInfo();
	const std::map<char, CharRenderInfo>& GetCharRenderInfo();
    std::shared_ptr<Texture2D>            GetFontAtlas();

    int GetWidth();
    int GetHeight();
    int GetCharSize();
};

#endif