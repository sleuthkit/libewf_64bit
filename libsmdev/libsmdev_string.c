/*
 * System character type string functions
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

#include <common.h>
#include <types.h>

#include "libsmdev_libcerror.h"
#include "libsmdev_libcstring.h"
#include "libsmdev_string.h"

#if defined( _cplusplus )
extern "C" {
#endif

/* Copies and trims the string from the byte stream
 * Returns the number of bytes copied or -1 on error
 */
ssize_t libsmdev_string_trim_copy_from_byte_stream(
         uint8_t *string,
         size_t string_size,
         const uint8_t *byte_stream,
         size_t byte_stream_size,
         libcerror_error_t **error )
{
	static char *function   = "libsmdev_string_trim_copy_from_byte_stream";
        size_t string_iterator  = 0;
        ssize_t first_character = 0;
        ssize_t last_character  = 0;

	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	if( string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( byte_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid byte stream.",
		 function );

		return( -1 );
	}
	if( byte_stream_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid byte stream size value exceeds maximum.",
		 function );

		return( -1 );
	}
	for( first_character = 0;
	     first_character < (ssize_t) byte_stream_size;
	     first_character++ )
	{
		if( ( byte_stream[ first_character ] >= (uint8_t) 0x21 )
		 && ( byte_stream[ first_character ] <= (uint8_t) 0x7e ) )
		{
			break;
		}
	}
	for( last_character = (ssize_t) byte_stream_size;
	     last_character >= 0;
	     last_character-- )
	{
		if( ( byte_stream[ last_character ] >= (uint8_t) 0x21 )
		 && ( byte_stream[ last_character ] <= (uint8_t) 0x7e ) )
		{
			break;
		}
	}
	if( first_character < last_character )
	{
		if( string_size < (size_t) ( last_character - first_character + 1 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: string too small.",
			 function );

			return( -1 );
		}
		while( first_character <= last_character )
		{
			string[ string_iterator++ ] = byte_stream[ first_character++ ];
		}
	}
	string[ string_iterator ] = 0;

	return( (ssize_t) string_iterator );
}

