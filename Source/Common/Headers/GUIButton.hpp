#ifndef __ZEDTOOL_GUIBUTTON_HPP__
#define __ZEDTOOL_GUIBUTTON_HPP__

#include <GUIElement.hpp>

namespace ZEDTool
{
	class GUIButton : public GUIElement
	{
	public:
		GUIButton( );
		virtual ~GUIButton( );

		void SetText( const char *p_pText );

	private:
	};
}

#endif // __ZEDTOOL_GUIBUTTON_HPP__

