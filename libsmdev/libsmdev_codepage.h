/*
 * Codepage functions
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

#if !defined( _LIBSMDEV_INTERNAL_CODEPAGE_H )
#define _LIBSMDEV_INTERNAL_CODEPAGE_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

#if !defined( HAVE_LOCAL_LIBSMDEV )

#include <libsmdev/codepage.h>

/* Define HAVE_LOCAL_LIBSMDEV for local use of libsmdev
 * The definitions in <libsmdev/codepage.h> are copied here
 * for local use of libsmdev
 */
#else

/* The codepage definitions
 */
enum LIBSMDEV_CODEPAGES
{
	LIBSMDEV_CODEPAGE_ASCII				= 20127,

	LIBSMDEV_CODEPAGE_ISO_8859_1			= 28591,
	LIBSMDEV_CODEPAGE_ISO_8859_2			= 28592,
	LIBSMDEV_CODEPAGE_ISO_8859_3			= 28593,
	LIBSMDEV_CODEPAGE_ISO_8859_4			= 28594,
	LIBSMDEV_CODEPAGE_ISO_8859_5			= 28595,
	LIBSMDEV_CODEPAGE_ISO_8859_6			= 28596,
	LIBSMDEV_CODEPAGE_ISO_8859_7			= 28597,
	LIBSMDEV_CODEPAGE_ISO_8859_8			= 28598,
	LIBSMDEV_CODEPAGE_ISO_8859_9			= 28599,
	LIBSMDEV_CODEPAGE_ISO_8859_10			= 28600,
	LIBSMDEV_CODEPAGE_ISO_8859_11			= 28601,
	LIBSMDEV_CODEPAGE_ISO_8859_13			= 28603,
	LIBSMDEV_CODEPAGE_ISO_8859_14			= 28604,
	LIBSMDEV_CODEPAGE_ISO_8859_15			= 28605,
	LIBSMDEV_CODEPAGE_ISO_8859_16			= 28606,

	LIBSMDEV_CODEPAGE_KOI8_R			= 20866,
	LIBSMDEV_CODEPAGE_KOI8_U			= 21866,

	LIBSMDEV_CODEPAGE_WINDOWS_874			= 874,
	LIBSMDEV_CODEPAGE_WINDOWS_932			= 932,
	LIBSMDEV_CODEPAGE_WINDOWS_936			= 936,
	LIBSMDEV_CODEPAGE_WINDOWS_949			= 949,
	LIBSMDEV_CODEPAGE_WINDOWS_950			= 950,
	LIBSMDEV_CODEPAGE_WINDOWS_1250			= 1250,
	LIBSMDEV_CODEPAGE_WINDOWS_1251			= 1251,
	LIBSMDEV_CODEPAGE_WINDOWS_1252			= 1252,
	LIBSMDEV_CODEPAGE_WINDOWS_1253			= 1253,
	LIBSMDEV_CODEPAGE_WINDOWS_1254			= 1254,
	LIBSMDEV_CODEPAGE_WINDOWS_1255			= 1255,
	LIBSMDEV_CODEPAGE_WINDOWS_1256			= 1256,
	LIBSMDEV_CODEPAGE_WINDOWS_1257			= 1257,
	LIBSMDEV_CODEPAGE_WINDOWS_1258			= 1258
};

#endif

#if defined( __cplusplus )
}
#endif

#endif

