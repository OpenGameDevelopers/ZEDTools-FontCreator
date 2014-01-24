#include <Helper.hpp>
#include <errno.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <iostream>
#include <sys/stat.h>

namespace ZEDTool
{
	int GetExecutableDirectory( char **p_ppBuffer, ssize_t p_BufferSize )
	{
		char LinkName[ 64 ];
		pid_t PID;

		PID = getpid( );

		if( snprintf( LinkName, sizeof( LinkName ), "/proc/%i/exe", PID ) < 0 )
		{
			std::cout << "<ERROR> Failed to get executable process" <<
				std::endl;
			return 1;
		}

		char FullPath[ p_BufferSize ];
		int Return = readlink( LinkName, FullPath, p_BufferSize );

		if( Return == -1 )
		{
			std::cout << "<ERROR> Failed to get executable directory" <<
				std::endl;
			return 1;
		}

		if( Return >= p_BufferSize )
		{
			std::cout << "<ERROR> Directory path longer than expected" <<
				std::endl;
			errno = ERANGE;
			return 1;
		}

		FullPath[ Return ] = '\0';

		std::string ExeDirectory( FullPath );
		size_t LastSlash = ExeDirectory.find_last_of( "/" );
		ExeDirectory.resize( LastSlash );
		ExeDirectory.append( "/" );

		( *p_ppBuffer ) = new char[ ExeDirectory.size( ) + 1 ];
		strncpy( *p_ppBuffer, ExeDirectory.c_str( ), ExeDirectory.size( ) );
		( *p_ppBuffer )[ ExeDirectory.size( ) ] = '\0';

		return 0;
	}

	int FileExists( const char *p_pFilePath, const bool p_IncludeSymLinks )
	{
		struct stat FileStat;

		if( lstat( p_pFilePath, &FileStat ) < 0 )
		{
			switch( errno )
			{
				case EACCES:
				{
					std::cout << "<ERROR> Permission denied accessing " <<
						p_pFilePath << std::endl;
					break;
				}
				case EBADF:
				{
					std::cout << "<ERROR> Bad file descriptor for " <<
						p_pFilePath << std::endl;
					break;
				}
				case EFAULT:
				{
					std::cout << "<ERROR> Bad address for " << p_pFilePath <<
						std::endl;
					break;
				}
				case ELOOP:
				{
					std::cout << "<ERROR> Too many symbolic links for " <<
						p_pFilePath << std::endl;
					break;
				}
				case ENAMETOOLONG:
				{
					std::cout << "<ERROR> File name too long: " <<
						p_pFilePath << " " << strlen( p_pFilePath ) <<
						" characters" << std::endl;
					break;
				}
				case ENOENT:
				{
					std::cout << "<ERROR> File is not valid: " <<
						p_pFilePath << std::endl;
					break;
				}
				case ENOMEM:
				{
					std::cout << "<ERROR> Ran out of memory for " <<
						p_pFilePath << std::endl;
					break;
				}
				case EOVERFLOW:
				{
					std::cout <<
						"<ERROR> Overflow occurred (maybe file is too large?) "
						"for " << p_pFilePath << std::endl;
					break;
				}
				default:
				{
					std::cout << "<ERROR> Unknown erorr occurred while "
						"reading " << p_pFilePath << std::endl;
					break;
				}
			}
			return 1;
		}

		if( S_ISREG( FileStat.st_mode ) )
		{
			return 0;
		}

		if( p_IncludeSymLinks )
		{
			if( S_ISLNK( FileStat.st_mode ) )
			{
				return 0;
			}
		}

		std::cout << "<ERROR> File found but is of type ";

		if( S_ISLNK( FileStat.st_mode ) )
		{
			std::cout << "symbolic link (function did not request to include "
				"symbolic links";
		}
		else if( S_ISDIR( FileStat.st_mode ) )
		{
			std::cout << "directory";
		}
		else if( S_ISCHR( FileStat.st_mode ) )
		{
			std::cout << "character special file";
		}
		else if( S_ISBLK( FileStat.st_mode ) )
		{
			std::cout << "block special file";
		}
		else if( S_ISFIFO( FileStat.st_mode ) )
		{
			std::cout << "pipe or FIFO";
		}
		else if( S_ISSOCK( FileStat.st_mode ) )
		{
			std::cout << "socket";
		}
		else
		{
			std::cout << "unknown (not supported file type)";
		}

		std::cout << std::endl;

		return 1;
	}
}

