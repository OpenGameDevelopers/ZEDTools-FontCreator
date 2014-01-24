#include <FontCreator.hpp>
#include <GitVersion.hpp>
#include <cstring>
#include <Helper.hpp>
#include <iostream>

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
	
		// Attempt to load the settings from the previous session
		char *pProgramDirectory = new char[ 255 ];
		memset( pProgramDirectory, '\0', sizeof( char ) * 255 );

		if( GetExecutableDirectory( &pProgramDirectory, 255 ) != 0 )
		{
			SAFE_DELETE_ARRAY( pProgramDirectory );

			return 1;
		}

		char ConfigPath[ 255 ];
		memset( ConfigPath, '\0',
			sizeof( ConfigPath ) * sizeof( ConfigPath[ 0 ] ) );

		sprintf( ConfigPath, "%sdefault.config", pProgramDirectory );

		int X = SDL_WINDOWPOS_CENTERED, Y = SDL_WINDOWPOS_CENTERED;
		int Width = 1024, Height = 768;

		if( FileExists( ConfigPath, false ) == 0 )
		{
			// The file is just the x, y position and the width and height
			// When the configuration file needs to get more complex, this will
			// be revisited
			FILE *pConfigFile = NULL;

			pConfigFile = fopen( ConfigPath, "rb" );

			if( pConfigFile )
			{
				fread( &X, sizeof( int ), 1, pConfigFile );
				fread( &Y, sizeof( int ), 1, pConfigFile );
				fread( &Width, sizeof( int ), 1, pConfigFile );
				fread( &Height, sizeof( int ), 1, pConfigFile );

				fclose( pConfigFile );
				pConfigFile = NULL;
			}
		}
		else
		{
			std::cout << "<WARNING> Could not read configuration file: " <<
				ConfigPath << std::endl;
		}

		SAFE_DELETE_ARRAY( pProgramDirectory );

		m_pWindow = SDL_CreateWindow( WindowTitle,
			X, Y, Width, Height, SDL_WINDOW_RESIZABLE );

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
		SDL_SetRenderDrawColor( m_pRenderer, 140, 140, 140, 255 );

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

		// Save the current window size and position for the next time
		int X, Y, Width, Height;
		SDL_GetWindowPosition( m_pWindow, &X, &Y );
		SDL_GetWindowSize( m_pWindow, &Width, &Height );

		char *pProgramDirectory = new char[ 255 ];
		memset( pProgramDirectory, '\0', sizeof( char ) * 255 );

		if( GetExecutableDirectory( &pProgramDirectory, 255 ) != 0 )
		{
			SAFE_DELETE_ARRAY( pProgramDirectory );
			return 1;
		}

		char ConfigPath[ 255 ];
		memset( ConfigPath, '\0',
			sizeof( ConfigPath ) * sizeof( ConfigPath[ 0 ] ) );
		sprintf( ConfigPath, "%sdefault.config", pProgramDirectory );

		FILE *pConfigFile = NULL;
		pConfigFile = fopen( ConfigPath, "wb" );

		if( pConfigFile )
		{
			fwrite( &X, sizeof( int ), 1, pConfigFile );
			fwrite( &Y, sizeof( int ), 1, pConfigFile );
			fwrite( &Width, sizeof( int ), 1, pConfigFile );
			fwrite( &Height, sizeof( int ), 1, pConfigFile );

			fclose( pConfigFile );
			pConfigFile = NULL;
		}

		SAFE_DELETE_ARRAY( pProgramDirectory );

		return 0;
	}

	void FontCreator::CleanUp( )
	{
		SDL_DestroyRenderer( m_pRenderer );
		SDL_DestroyWindow( m_pWindow );
		SDL_Quit( );
	}
}

