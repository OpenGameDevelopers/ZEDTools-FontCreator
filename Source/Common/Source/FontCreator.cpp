#include <FontCreator.hpp>
#include <GitVersion.hpp>
#include <cstring>

namespace ZEDTool
{
	FontCreator::FontCreator( ) :
		m_pWindow( NULL ),
		m_pRenderer( NULL )
	{
	}

	FontCreator::~FontCreator( )
	{
		this->CleanUp( );
	}

	int FontCreator::Initialise( )
	{
		if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
		{
			return 1;
		}

		char WindowTitle[ 1024 ];
		char Buffer[ 1024 ];
		memset( WindowTitle, '\0',
			sizeof( WindowTitle ) * sizeof( WindowTitle[ 0 ] ) );
		memset( Buffer, '\0', sizeof( Buffer ) * sizeof( Buffer[ 0 ] ) );

		sprintf( WindowTitle, "ZED Font Creator | Build %s [%s]",
			GIT_BUILD_VERSION, GIT_TAG_NAME );

#if defined ZED_BUILD_DEBUG
		sprintf( Buffer, " | DEBUG" );
		strcat( WindowTitle, Buffer );
#endif

		m_pWindow = SDL_CreateWindow( WindowTitle,
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,
			SDL_WINDOW_RESIZABLE );

		if( m_pWindow == NULL )
		{
			return 1;
		}

		m_pRenderer = SDL_CreateRenderer( m_pWindow, -1,
			SDL_RENDERER_ACCELERATED );

		if( m_pRenderer == NULL )
		{
			return 1;
		}

		SDL_MaximizeWindow( m_pWindow );
		SDL_SetRenderDrawColor( m_pRenderer, 32, 0, 32, 255 );

		SDL_RaiseWindow( m_pWindow );

		return 0;
	}

	int FontCreator::Execute( )
	{
		bool Run = true;
		SDL_Event Event;

		while( Run )
		{
			while( SDL_PollEvent( &Event ) != 0 )
			{
				if( Event.type == SDL_QUIT )
				{
					Run = false;
				}
				else if( Event.type == SDL_KEYDOWN )
				{
					switch( Event.key.keysym.sym )
					{
						case SDLK_ESCAPE:
						{
							Run = false;
							break;
						}
						default:
						{
							break;
						}
					}
				}
			}

			SDL_RenderClear( m_pRenderer );
			SDL_RenderPresent( m_pRenderer );
		}

		return 0;
	}

	void FontCreator::CleanUp( )
	{
		SDL_DestroyRenderer( m_pRenderer );
		SDL_DestroyWindow( m_pWindow );
		SDL_Quit( );
	}
}

