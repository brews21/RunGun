#ifndef __TextManager__
#define __TextManager__

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include <stdio.h>
#include <string>

class TextManager
{
public:
	TextManager();
	~TextManager();
	TextManager(std::string fontName, int fontSize, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha = 255);

	void renderText(std::string msg, int x, int y);

	// set colour modulation
	void setColour(Uint8 red, Uint8 green, Uint8 blue) { m_Red = red; m_Green = green; m_Blue = blue; }

	// set alpha
	void setAlpha(Uint8 alpha) { m_Alpha = alpha; }

private:
	SDL_Renderer* m_pRenderer;
	SDL_Texture* m_pTexture;
	SDL_Color m_pColour;

	TTF_Font* m_pFont;

	int m_Width, m_Height;

	Uint8 m_Red, m_Blue, m_Green, m_Alpha;
};

#endif // __TextManager__