/*****************************************************************************
 Yihsiang Liow
 Copyright
 *****************************************************************************/
#ifndef TEXTSURFACE_H
#define TEXTSURFACE_H

#include "Color.h"




class TextSurface
{
public:
	TextSurface( char str[]="Hello, World!", char fontfamily[]="fonts/FreeMonoOblique.ttf", int size=20, int R=255, int G=100, int B=100 )
	{
		atexit( TTF_Quit );
		if ( TTF_Init() != 0 )
		{
			printf("TTF_Init: %s\n", TTF_GetError());
			return;
		}

		TTF_Font * font = TTF_OpenFont( fontfamily, size );
		Color color = {R,G,B};
		text = TTF_RenderText_Solid( font, str, color );
		TTF_CloseFont( font );
	}

	~TextSurface()
	{
		SDL_FreeSurface( text );
	}

	SDL_Surface * get_surface()
	{
		return text;
	}

private:
	SDL_Surface * text;
};

#endif
