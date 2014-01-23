#include <GUIElement.hpp>
#include <cstring>

namespace ZEDTool
{
	GUIElement::GUIElement( )
	{
		m_BoundingBox.w = m_BoundingBox.h = m_BoundingBox.x =
			m_BoundingBox.y = 0;
	}

	GUIElement::~GUIElement( )
	{
	}

	void GUIElement::GetBoundingBox( SDL_Rect *p_pBoundingBox )
	{
		memcpy( p_pBoundingBox, &m_BoundingBox, sizeof( m_BoundingBox ) );
	}
}

