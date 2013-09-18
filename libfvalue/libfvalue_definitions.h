/*
 * The internal definitions
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

#if !defined( LIBFVALUE_INTERNAL_DEFINITIONS_H )
#define LIBFVALUE_INTERNAL_DEFINITIONS_H

#include <common.h>
#include <types.h>

/* Define HAVE_LOCAL_LIBFVALUE for local use of libfvalue
 */
#if !defined( HAVE_LOCAL_LIBFVALUE )
#include <libfvalue/definitions.h>

/* The definitions in <libfvalue/definitions.h> are copied here
 * for local use of libfvalue
 */
#else
#include <byte_stream.h>

#define LIBFVALUE_VERSION					20121214

/* The libfvalue version string
 */
#define LIBFVALUE_VERSION_STRING				"20121214"

/* The endian definitions
 */
#define LIBFVALUE_ENDIAN_BIG					_BYTE_STREAM_ENDIAN_BIG
#define LIBFVALUE_ENDIAN_LITTLE					_BYTE_STREAM_ENDIAN_LITTLE
#define LIBFVALUE_ENDIAN_NATIVE					(uint8_t) 'n'

/* The value identifier flags definitions
 */
enum LIBFVALUE_VALUE_IDENTIFIER_FLAGS
{
	/* The identifier is not managed by the value
	 *
	 * if LIBFVALUE_VALUE_FLAG_IDENTIFIER_MANAGED is not set
	 * the identifier is expected to be available
	 * during the life-time of the value
	 *
	 * if LIBFVALUE_VALUE_FLAG_IDENTIFIER_MANAGED is set
	 * a copy of the identifier will be managed by the value
	 */
        LIBFVALUE_VALUE_IDENTIFIER_FLAG_NON_MANAGED		= 0x00,

	/* The identifier is managed by the value
	 * This will also set LIBFVALUE_VALUE_FLAG_IDENTIFIER_MANAGED in the value
	 */
        LIBFVALUE_VALUE_IDENTIFIER_FLAG_MANAGED			= 0x01,

	/* The identifier is not cloned, but passed as a reference
	 */
        LIBFVALUE_VALUE_IDENTIFIER_FLAG_CLONE_BY_REFERENCE	= 0x10
};

/* The value data flags definitions
 */
enum LIBFVALUE_VALUE_DATA_FLAGS
{
	/* The data is not managed by the value
	 *
	 * if LIBFVALUE_VALUE_FLAG_DATA_MANAGED is not set
	 * the data is expected to be available
	 * during the life-time of the value
	 *
	 * if LIBFVALUE_VALUE_FLAG_DATA_MANAGED is set
	 * a copy of the data will be managed by the value
	 */
        LIBFVALUE_VALUE_DATA_FLAG_NON_MANAGED			= 0x00,

	/* The data is managed by the value
	 * This will also set LIBFVALUE_VALUE_FLAG_DATA_MANAGED in the value
	 */
        LIBFVALUE_VALUE_DATA_FLAG_MANAGED			= 0x02,

	/* The data is not cloned, but passed as a reference
	 */
        LIBFVALUE_VALUE_DATA_FLAG_CLONE_BY_REFERENCE		= 0x10
};

/* The value class definitions
 */
enum LIBFVALUE_VALUE_CLASSES
{
	LIBFVALUE_VALUE_CLASS_UNDEFINED				= 0,
	LIBFVALUE_VALUE_CLASS_BINARY,
	LIBFVALUE_VALUE_CLASS_BOOLEAN,
	LIBFVALUE_VALUE_CLASS_DATETIME,
	LIBFVALUE_VALUE_CLASS_ENUMERATION,
	LIBFVALUE_VALUE_CLASS_FLOATING_POINT,
	LIBFVALUE_VALUE_CLASS_INTEGER,
	LIBFVALUE_VALUE_CLASS_STRING
};

/* The value type definitions
 */
enum LIBFVALUE_VALUE_TYPES
{
	LIBFVALUE_VALUE_TYPE_UNDEFINED				= 0,

	LIBFVALUE_VALUE_TYPE_BINARY_DATA,

	LIBFVALUE_VALUE_TYPE_BOOLEAN,

	LIBFVALUE_VALUE_TYPE_ENUMERATION,

	LIBFVALUE_VALUE_TYPE_INTEGER_8BIT,
	LIBFVALUE_VALUE_TYPE_UNSIGNED_INTEGER_8BIT,
	LIBFVALUE_VALUE_TYPE_INTEGER_16BIT,
	LIBFVALUE_VALUE_TYPE_UNSIGNED_INTEGER_16BIT,
	LIBFVALUE_VALUE_TYPE_INTEGER_32BIT,
	LIBFVALUE_VALUE_TYPE_UNSIGNED_INTEGER_32BIT,
	LIBFVALUE_VALUE_TYPE_INTEGER_64BIT,
	LIBFVALUE_VALUE_TYPE_UNSIGNED_INTEGER_64BIT,

	LIBFVALUE_VALUE_TYPE_FLOATING_POINT_32BIT,
	LIBFVALUE_VALUE_TYPE_FLOATING_POINT_64BIT,

	LIBFVALUE_VALUE_TYPE_STRING_BYTE_STREAM,
	LIBFVALUE_VALUE_TYPE_STRING_UTF8,
	LIBFVALUE_VALUE_TYPE_STRING_UTF16,
	LIBFVALUE_VALUE_TYPE_STRING_UTF32,

	LIBFVALUE_VALUE_TYPE_FAT_DATE_TIME,
	LIBFVALUE_VALUE_TYPE_FILETIME,
	LIBFVALUE_VALUE_TYPE_FLOATINGTIME,
	LIBFVALUE_VALUE_TYPE_NSF_TIMEDATE,
	LIBFVALUE_VALUE_TYPE_HFSTIME,
	LIBFVALUE_VALUE_TYPE_POSIX_TIME,
	LIBFVALUE_VALUE_TYPE_SYSTEMTIME,

	LIBFVALUE_VALUE_TYPE_GUID,

	LIBFVALUE_VALUE_TYPE_NT_SECURITY_IDENTIFIER
};

/* The binary data value format flags
 */
enum LIBFVALUE_BINARY_DATA_FORMAT_FLAGS
{
	LIBFVALUE_BINARY_DATA_FORMAT_TYPE_BASE16		= 0x00000001UL,
	LIBFVALUE_BINARY_DATA_FORMAT_TYPE_BASE32		= 0x00000002UL,
	LIBFVALUE_BINARY_DATA_FORMAT_TYPE_BASE64		= 0x00000003UL,

	LIBFVALUE_BINARY_DATA_FORMAT_FLAG_CASE_LOWER		= 0x00000100UL,
	LIBFVALUE_BINARY_DATA_FORMAT_FLAG_CASE_UPPER		= 0x00000200UL,
};

/* The date time value format definition flags
 */
enum LIBFVALUE_DATE_TIME_FORMAT_FLAGS
{
	/* Format the date time value in C time
	 */
	LIBFVALUE_DATE_TIME_FORMAT_TYPE_CTIME			= 0x00000001UL,

	/* Format the date time value in ISO 8601
	 */
	LIBFVALUE_DATE_TIME_FORMAT_TYPE_ISO8601			= 0x00000002UL,

	/* Format the date time value as a date only
	 */
	LIBFVALUE_DATE_TIME_FORMAT_FLAG_DATE			= 0x00000100UL,

	/* Format the date time value as a time only
	 */
	LIBFVALUE_DATE_TIME_FORMAT_FLAG_TIME			= 0x00000200UL,

	/* Format the date time value as a duration
	 */
	LIBFVALUE_DATE_TIME_FORMAT_FLAG_DURATION		= 0x00000400UL,

	/* Format the date time value with a time in milli seconds
	 */
	LIBFVALUE_DATE_TIME_FORMAT_FLAG_TIME_MILLI_SECONDS	= 0x00010000UL,

	/* Format the date time value with a time in micro seconds
	 */
	LIBFVALUE_DATE_TIME_FORMAT_FLAG_TIME_MICRO_SECONDS	= 0x00020000UL,

	/* Format the date time value with a time in nano seconds
	 */
	LIBFVALUE_DATE_TIME_FORMAT_FLAG_TIME_NANO_SECONDS	= 0x00040000UL,

	/* Add a timezone indicator
	 */
	LIBFVALUE_DATE_TIME_FORMAT_FLAG_TIMEZONE_INDICATOR	= 0x80000000UL,
};

#define LIBFVALUE_DATE_TIME_FORMAT_FLAG_DATE_TIME \
	( LIBFVALUE_DATE_TIME_FORMAT_FLAG_DATE | LIBFVALUE_DATE_TIME_FORMAT_FLAG_TIME )

#define LIBFVALUE_DATE_TIME_FORMAT_FLAG_DATE_TIME_MILLI_SECONDS \
	( LIBFVALUE_DATE_TIME_FORMAT_FLAG_DATE | LIBFVALUE_DATE_TIME_FORMAT_FLAG_TIME | LIBFVALUE_DATE_TIME_FORMAT_FLAG_TIME_MILLI_SECONDS )

#define LIBFVALUE_DATE_TIME_FORMAT_FLAG_DATE_TIME_MICRO_SECONDS \
	( LIBFVALUE_DATE_TIME_FORMAT_FLAG_DATE | LIBFVALUE_DATE_TIME_FORMAT_FLAG_TIME | LIBFVALUE_DATE_TIME_FORMAT_FLAG_TIME_MICRO_SECONDS )

#define LIBFVALUE_DATE_TIME_FORMAT_FLAG_DATE_TIME_NANO_SECONDS \
	( LIBFVALUE_DATE_TIME_FORMAT_FLAG_DATE | LIBFVALUE_DATE_TIME_FORMAT_FLAG_TIME | LIBFVALUE_DATE_TIME_FORMAT_FLAG_TIME_NANO_SECONDS )

/* The GUID value format definition flags
 */
enum LIBFVALUE_GUID_FORMAT_FLAGS
{
	/* Format the using lower case
	 */
	LIBFVALUE_GUID_FORMAT_FLAG_USE_LOWER_CASE		= 0x00000001UL,

	/* Format the using upper case
	 */
	LIBFVALUE_GUID_FORMAT_FLAG_USE_UPPER_CASE		= 0x00000002UL,

	/* Format the using surrounding braces { GUID }
	 */
	LIBFVALUE_GUID_FORMAT_FLAG_USE_SURROUNDING_BRACES	= 0x00000004UL
};

/* The floating point value format flags
 */
enum LIBFVALUE_FLOATING_POINT_FORMAT_FLAGS
{
	LIBFVALUE_FLOATING_POINT_FORMAT_TYPE_DECIMAL		= 0x00000001UL,
	LIBFVALUE_FLOATING_POINT_FORMAT_TYPE_HEXADECIMAL	= 0x00000002UL
};

/* The integer value format flags
 */
enum LIBFVALUE_INTEGER_FORMAT_FLAGS
{
	LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL			= 0x00000001UL,
	LIBFVALUE_INTEGER_FORMAT_TYPE_HEXADECIMAL		= 0x00000002UL,
	LIBFVALUE_INTEGER_FORMAT_TYPE_BOOLEAN			= 0x00000003UL,

	LIBFVALUE_INTEGER_FORMAT_FLAG_SIGNED			= 0x00000100UL,
	LIBFVALUE_INTEGER_FORMAT_FLAG_UNSIGNED			= 0x00000200UL,

	/* The integer has no base indicator e.g. 0x */
	LIBFVALUE_INTEGER_FORMAT_FLAG_NO_BASE_INDICATOR		= 0x00010000UL
};

#define LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_SIGNED \
	LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL | LIBFVALUE_INTEGER_FORMAT_FLAG_SIGNED

#define LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED \
	LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL | LIBFVALUE_INTEGER_FORMAT_FLAG_UNSIGNED

/* The table flag definitions
 */
enum LIBFVALUE_TABLE_FLAGS
{
	/* Allow a partial match of the identifier
	 */
        LIBFVALUE_TABLE_FLAG_ALLOW_PARTIAL_MATCH		= 0x01
};

/* The print flags definitions
 */
enum LIBFVALUE_PRINT_FLAGS
{
	/* Print the value with a type description
	 */
        LIBFVALUE_PRINT_FLAG_WITH_TYPE_DESCRIPTION		= 0x01
};

#endif

/* The value flags definitions
 */
enum LIBFVALUE_VALUE_FLAGS
{
	/* The identifier is not managed by the value
	 * the identifier is expected to be available
	 * during the life-time of the value
	 */
        LIBFVALUE_VALUE_FLAG_IDENTIFIER_NON_MANAGED		= 0x00,

	/* The identifier is managed by the value
	 */
        LIBFVALUE_VALUE_FLAG_IDENTIFIER_MANAGED			= 0x01,

	/* The data is not managed by the value
	 * the data is expected to be available
	 * during the life-time of the value
	 */
        LIBFVALUE_VALUE_FLAG_DATA_NON_MANAGED			= 0x00,

	/* The data is managed by the value
	 */
        LIBFVALUE_VALUE_FLAG_DATA_MANAGED			= 0x02,

	/* The value has a runtime instance of the value type
	 */
	LIBFVALUE_VALUE_FLAG_HAS_RUNTIME_INSTANCE		= 0x10,
};

#endif

