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

	void GUIElement::GetBoundingBox( SDL_Rect *p_pBoundingBox ) const
	{
		memcpy( p_pBoundingBox, &m_BoundingBox, sizeof( m_BoundingBox ) );
	}

	void GUIElement::SetPosition( const int p_X, const int p_Y )
	{
		m_BoundingBox.x += p_X;
		m_BoundingBox.y += p_Y;

		m_BoundingBox.w += p_X;
		m_BoundingBox.h += p_Y;
	}
}

