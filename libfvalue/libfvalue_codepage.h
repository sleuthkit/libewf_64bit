/*
 * Codepage functions
 *
 * Copyright (c) 2010-2012, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBFVALUE_INTERNAL_CODEPAGE_H )
#define _LIBFVALUE_INTERNAL_CODEPAGE_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

#if !defined( HAVE_LOCAL_LIBFVALUE )

#include <libfvalue/codepage.h>

/* Define HAVE_LOCAL_LIBFVALUE for local use of libfvalue
 * The definitions in <libfvalue/codepage.h> are copied here
 * for local use of libfvalue
 */
#else

/* The codepage definitions
 */
enum LIBFVALUE_CODEPAGES
{
	LIBFVALUE_CODEPAGE_ASCII			= 20127,

	LIBFVALUE_CODEPAGE_ISO_8859_1			= 28591,
	LIBFVALUE_CODEPAGE_ISO_8859_2			= 28592,
	LIBFVALUE_CODEPAGE_ISO_8859_3			= 28593,
	LIBFVALUE_CODEPAGE_ISO_8859_4			= 28594,
	LIBFVALUE_CODEPAGE_ISO_8859_5			= 28595,
	LIBFVALUE_CODEPAGE_ISO_8859_6			= 28596,
	LIBFVALUE_CODEPAGE_ISO_8859_7			= 28597,
	LIBFVALUE_CODEPAGE_ISO_8859_8			= 28598,
	LIBFVALUE_CODEPAGE_ISO_8859_9			= 28599,
	LIBFVALUE_CODEPAGE_ISO_8859_10			= 28600,
	LIBFVALUE_CODEPAGE_ISO_8859_11			= 28601,
	LIBFVALUE_CODEPAGE_ISO_8859_13			= 28603,
	LIBFVALUE_CODEPAGE_ISO_8859_14			= 28604,
	LIBFVALUE_CODEPAGE_ISO_8859_15			= 28605,
	LIBFVALUE_CODEPAGE_ISO_8859_16			= 28606,

	LIBFVALUE_CODEPAGE_KOI8_R			= 20866,
	LIBFVALUE_CODEPAGE_KOI8_U			= 21866,

	LIBFVALUE_CODEPAGE_UTF16_LITTLE_ENDIAN		= 1200,
	LIBFVALUE_CODEPAGE_UTF16_BIG_ENDIAN		= 1201,
	LIBFVALUE_CODEPAGE_UTF32_LITTLE_ENDIAN		= 12000,
	LIBFVALUE_CODEPAGE_UTF32_BIG_ENDIAN		= 12001,
	LIBFVALUE_CODEPAGE_UTF7				= 65000,
	LIBFVALUE_CODEPAGE_UTF8				= 65001,

	LIBFVALUE_CODEPAGE_WINDOWS_874			= 874,
	LIBFVALUE_CODEPAGE_WINDOWS_932			= 932,
	LIBFVALUE_CODEPAGE_WINDOWS_936			= 936,
	LIBFVALUE_CODEPAGE_WINDOWS_1250			= 1250,
	LIBFVALUE_CODEPAGE_WINDOWS_1251			= 1251,
	LIBFVALUE_CODEPAGE_WINDOWS_1252			= 1252,
	LIBFVALUE_CODEPAGE_WINDOWS_1253			= 1253,
	LIBFVALUE_CODEPAGE_WINDOWS_1254			= 1254,
	LIBFVALUE_CODEPAGE_WINDOWS_1255			= 1255,
	LIBFVALUE_CODEPAGE_WINDOWS_1256			= 1256,
	LIBFVALUE_CODEPAGE_WINDOWS_1257			= 1257,
	LIBFVALUE_CODEPAGE_WINDOWS_1258			= 1258
};

#endif

#if defined( __cplusplus )
}
#endif

#endif

