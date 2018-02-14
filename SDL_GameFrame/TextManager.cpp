#include "TextManager.h"
#include "Game.h"

TextManager::TextManager()
{
}

TextManager::~TextManager()
{
}

TextManager::TextManager(std::string fontName, int fontSize, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha)
{
	m_Red = red;
	m_Green = green;
	m_Blue = blue;
	m_Alpha = alpha;

	m_pRenderer = Game::Singleton()->getRenderer();
	m_pColour = { m_Red, m_Green, m_Blue, m_Alpha };
	m_pFont = TTF_OpenFont(fontName.c_str(), fontSize);
}

void TextManager::renderText(std::string msg, int x, int y)
{
	SDL_Surface* surface = nullptr;
	SDL_Texture* texture = nullptr;

	int texW = 0, texH = 0;

	surface = TTF_RenderText_Solid(m_pFont, msg.c_str(), m_pColour);

	// If the surface is not loaded
	if (!surface)
	{
		printf("SURFACE for font not loaded! \n");
		printf("%s\n", SDL_GetError());
	}
	else
	{
		texture = SDL_CreateTextureFromSurface(m_pRenderer, surface);
		if (!texture)
		{
			printf("TEXTURE for font not loaded! \n");
			printf("%s\n", SDL_GetError());
		}
		else
		{
			SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
			SDL_Rect textRect = { x, y, texW, texH };

			SDL_RenderCopy(m_pRenderer, texture, NULL, &textRect);
		}
	}

	if (texture)
		SDL_DestroyTexture(texture);

	if (surface)
		SDL_FreeSurface(surface);
}