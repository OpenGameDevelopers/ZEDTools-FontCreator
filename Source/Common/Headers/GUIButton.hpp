#ifndef __ZEDTOOL_GUIBUTTON_HPP__
#define __ZEDTOOL_GUIBUTTON_HPP__

#include <GUIElement.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

namespace ZEDTool
{
	class GUIButton : public GUIElement
	{
	public:
		GUIButton( );
		virtual ~GUIButton( );

		void SetText( const char *p_pText );
		// Maybe the font should be set from the GUIManager?  Perhaps even
		// residing within that class.
		int SetFont( const char *p_pFontFile, const int p_FontSize );

		virtual void Render( SDL_Renderer * const &p_pRenderer );

	private:
		char		*m_pText;
		TTF_Font	*m_pFont;
	};
}

#endif // __ZEDTOOL_GUIBUTTON_HPP__

