/*
 * The internal definitions
 *
 * Copyright (c) 2010-2013, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined( _LIBSMDEV_DEFINITIONS_H )
#define _LIBSMDEV_DEFINITIONS_H

#include <common.h>
#include <byte_stream.h>

#define LIBSMDEV_ENDIAN_BIG				_BYTE_STREAM_ENDIAN_BIG
#define LIBSMDEV_ENDIAN_LITTLE				_BYTE_STREAM_ENDIAN_LITTLE

/* Define HAVE_LOCAL_LIBSMDEV for local use of libsmdev
 */
#if !defined( HAVE_LOCAL_LIBSMDEV )
#include <libsmdev/definitions.h>

#else

#define LIBSMDEV_VERSION				20130331

/* The version string
 */
#define LIBSMDEV_VERSION_STRING				"20130331"

/* The access flags definitions
 * bit 1        set to 1 for read access
 * bit 2        set to 1 for write access
 * bit 3        set to 1 to truncate an existing file on write
 * bit 4-8      not used
 */
enum LIBSMDEV_ACCESS_FLAGS
{
	LIBSMDEV_ACCESS_FLAG_READ			= 0x01,
	LIBSMDEV_ACCESS_FLAG_WRITE			= 0x02,
	LIBSMDEV_ACCESS_FLAG_TRUNCATE			= 0x04
};

/* The file access macros
 */
#define LIBSMDEV_OPEN_READ				( LIBSMDEV_ACCESS_FLAG_READ )
#define LIBSMDEV_OPEN_WRITE				( LIBSMDEV_ACCESS_FLAG_WRITE )
#define LIBSMDEV_OPEN_READ_WRITE			( LIBSMDEV_ACCESS_FLAG_READ | LIBSMDEV_ACCESS_FLAG_WRITE )
#define LIBSMDEV_OPEN_WRITE_TRUNCATE			( LIBSMDEV_ACCESS_FLAG_WRITE | LIBSMDEV_ACCESS_FLAG_TRUNCATE )
#define LIBSMDEV_OPEN_READ_WRITE_TRUNCATE		( LIBSMDEV_ACCESS_FLAG_READ | LIBSMDEV_ACCESS_FLAG_WRITE | LIBSMDEV_ACCESS_FLAG_TRUNCATE )

/* The error flags definitions
 */
enum LIBSMDEV_ERROR_FLAGS
{
	LIBSMDEV_ERROR_FLAG_ZERO_ON_ERROR		= 0x01
};

/* The media type definitions
 */
enum LIBSMDEV_MEDIA_TYPES
{
	LIBSMDEV_MEDIA_TYPE_REMOVABLE			= 0x00,
	LIBSMDEV_MEDIA_TYPE_FIXED			= 0x01,
	LIBSMDEV_MEDIA_TYPE_OPTICAL			= 0x03,
	LIBSMDEV_MEDIA_TYPE_MEMORY			= 0x10
};

/* The bus type definitions
 */
enum LIBSMDEV_BUS_TYPES
{
	LIBSMDEV_BUS_TYPE_UNKNOWN			= 0,
	LIBSMDEV_BUS_TYPE_ATA				= (uint8_t) 'a',
	LIBSMDEV_BUS_TYPE_FIREWIRE			= (uint8_t) 'f',
	LIBSMDEV_BUS_TYPE_SCSI				= (uint8_t) 's',
	LIBSMDEV_BUS_TYPE_USB				= (uint8_t) 'u'
};

/* The track types
 */
enum LIBSMDEV_TRACK_TYPES
{
	LIBSMDEV_TRACK_TYPE_UNKNOWN,
	LIBSMDEV_TRACK_TYPE_AUDIO,
	LIBSMDEV_TRACK_TYPE_CDG,
	LIBSMDEV_TRACK_TYPE_MODE1_2048,
	LIBSMDEV_TRACK_TYPE_MODE1_2352,
	LIBSMDEV_TRACK_TYPE_MODE2_2048,
	LIBSMDEV_TRACK_TYPE_MODE2_2324,
	LIBSMDEV_TRACK_TYPE_MODE2_2336,
	LIBSMDEV_TRACK_TYPE_MODE2_2352,
	LIBSMDEV_TRACK_TYPE_CDI_2336,
	LIBSMDEV_TRACK_TYPE_CDI_2352,
};

#endif

/* Platform specific macros
 */
#if defined( _MSC_VER )
#define LIBSMDEV_LARGE_INTEGER_ZERO			{ 0, 0 }

#elif defined( __BORLANDC__ )
#define LIBSMDEV_LARGE_INTEGER_ZERO			{ 0 }

#elif defined( __CYGWIN__ )
#define LIBSMDEV_LARGE_INTEGER_ZERO			{ { 0, 0 } }

#elif defined( __MINGW32_VERSION ) || defined( __MINGW64_VERSION_MAJOR )
#define LIBSMDEV_LARGE_INTEGER_ZERO			{ { 0, 0 } }

#endif

#endif

