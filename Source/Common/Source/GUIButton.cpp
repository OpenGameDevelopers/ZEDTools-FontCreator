#include <GUIButton.hpp>
#include <Helper.hpp>
#include <iostream>

namespace ZEDTool
{
	GUIButton::GUIButton( ) :
		m_pText( NULL ),
		m_pFont( NULL )
	{
	}

	GUIButton::~GUIButton( )
	{
		TTF_CloseFont( m_pFont );
		SAFE_DELETE_ARRAY( m_pText );
	}

	void GUIButton::SetText( const char *p_pText )
	{
		if( p_pText )
		{
			if( strlen( p_pText ) > 0 )
			{
				SAFE_DELETE_ARRAY( m_pText );

				size_t TextSize = strlen( p_pText );
				m_pText = new char[ TextSize + 1 ];
				strncpy( m_pText, p_pText, TextSize );
				m_pText[ TextSize ] = '\0';
			}
		}
	}

	int GUIButton::SetFont( const char *p_pFontFile, const int p_FontSize )
	{
		m_pFont = TTF_OpenFont( p_pFontFile, p_FontSize );

		if( !m_pFont )
		{
			return 1;
		}

		return 0;
	}

	void GUIButton::Render( SDL_Renderer * const &p_pRenderer )
	{
		if( m_pFont )
		{
			// Changing this to black was pretty cool, theming should be
			// supported at some point, though it's questionable how useful
			// that would be
			SDL_SetRenderDrawColor( p_pRenderer, 180, 180, 180, 255 );
			SDL_Rect MessageBounds;

			// This is horribly inefficient, creating and destroying the
			// texture over and again is pointless
			// This should be done once, probably in SetText, which would rely
			// on a font being set
			SDL_Surface *Text;
			SDL_Texture *Texture;
			SDL_Color Colour = { 0, 0, 0 };
			if( m_pText )
			{
				Text = TTF_RenderText_Solid( m_pFont, m_pText, Colour );
			}
			else
			{
				Text = TTF_RenderText_Solid( m_pFont, " ", Colour );
			}
			Texture = SDL_CreateTextureFromSurface( p_pRenderer, Text );
			MessageBounds.x = m_BoundingBox.x+5;
			MessageBounds.y = m_BoundingBox.y+3;
			MessageBounds.w = Text->w;
			MessageBounds.h = Text->h;

			m_BoundingBox.w = Text->w + 10;
			m_BoundingBox.h = Text->h + 6;

			SDL_RenderFillRect( p_pRenderer, &m_BoundingBox );
			SDL_Rect InnerBorder;
			memcpy( &InnerBorder, &m_BoundingBox, sizeof( InnerBorder ) );

			InnerBorder.w -= 4;
			InnerBorder.x += 2;
			InnerBorder.h -= 4;
			InnerBorder.y += 2;

			SDL_SetRenderDrawColor( p_pRenderer, 130, 130, 130, 255 );
			SDL_RenderDrawRect( p_pRenderer, &InnerBorder );


			SDL_RenderCopy( p_pRenderer, Texture, NULL, &MessageBounds );
			SDL_FreeSurface( Text );
			SDL_DestroyTexture( Texture );
		}
	}
}

