/*
 * Debugging functions
 *
 * Copyright (c) 2006-2013, Joachim Metz <joachim.metz@gmail.com>
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libewf_libcstring.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"

#include "libewf_debug.h"
#include "libewf_libuna.h"

#include "ewf_checksum.h"

/* Prints a dump of data
 * Returns 1 if successful or -1 on error
 */
int libewf_debug_dump_data(
     const char *header_string,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function        = "libewf_debug_dump_data";
	uint32_t stored_checksum     = 0;
	uint32_t calculated_checksum = 0;

	if( header_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header string.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	calculated_checksum = ewf_checksum_calculate(
	                       data,
	                       data_size - sizeof( uint32_t ),
	                       1 );


	byte_stream_copy_to_uint32_little_endian(
	 &( data[ data_size - sizeof( uint32_t ) ] ),
	 stored_checksum );

	libcnotify_printf(
	 "%s:\n",
	 header_string );

	libcnotify_print_data(
	 data,
	 data_size,
	 0 );

	libcnotify_printf(
	 "%s: possible checksum (in file: %" PRIu32 " calculated: %" PRIu32 ").\n",
	 function,	
	 stored_checksum,	
	 calculated_checksum );

	return( 1 );
}

/* Prints the byte stream data to the notify stream
 * Returns 1 if successful or -1 on error
 */
int libewf_debug_byte_stream_print(
     const char *header_string,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     libcerror_error_t **error )
{
	libcstring_system_character_t *string = NULL;
	static char *function                 = "libewf_debug_byte_stream_print";
	size_t string_size                    = 0;
	int result                            = 0;

	if( header_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header string.",
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
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libuna_utf16_string_size_from_byte_stream(
	          byte_stream,
	          byte_stream_size,
	          LIBUNA_CODEPAGE_ASCII,
	          &string_size,
	          error );
#else
	result = libuna_utf8_string_size_from_byte_stream(
	          byte_stream,
	          byte_stream_size,
	          LIBUNA_CODEPAGE_ASCII,
	          &string_size,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine string size.",
		 function );

		return( -1 );
	}
	string = libcstring_system_string_allocate(
	          string_size );

	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create string.",
		 function );

		return( -1 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libuna_utf16_string_copy_from_byte_stream(
	          (uint16_t *) string,
	          string_size,
	          byte_stream,
	          byte_stream_size,
	          LIBUNA_CODEPAGE_ASCII,
	          error );
#else
	result = libuna_utf8_string_copy_from_byte_stream(
	          (uint8_t *) string,
	          string_size,
	          byte_stream,
	          byte_stream_size,
	          LIBUNA_CODEPAGE_ASCII,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy string from byte stream.",
		 function );

		memory_free(
		 string );

		return( -1 );
	}
	libcnotify_printf(
	 "%s:\n"
	 "%" PRIs_LIBCSTRING_SYSTEM "",
	 header_string,
	 string );

	memory_free(
	 string );

	return( 1 );
}

/* Prints the UTF-8 stream data to the notify stream
 * Returns 1 if successful or -1 on error
 */
int libewf_debug_utf8_stream_print(
     const char *header_string,
     const uint8_t *utf8_stream,
     size_t utf8_stream_size,
     libcerror_error_t **error )
{
	libcstring_system_character_t *string = NULL;
	static char *function                 = "libewf_debug_utf8_stream_print";
	size_t string_size                    = 0;
	int result                            = 0;

	if( header_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header string.",
		 function );

		return( -1 );
	}
	if( utf8_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 stream.",
		 function );

		return( -1 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libuna_utf16_string_size_from_utf8_stream(
	          utf8_stream,
	          utf8_stream_size,
	          &string_size,
	          error );
#else
	result = libuna_utf8_string_size_from_utf8_stream(
	          utf8_stream,
	          utf8_stream_size,
	          &string_size,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine string size.",
		 function );

		return( -1 );
	}
	string = libcstring_system_string_allocate(
	          string_size );

	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create string.",
		 function );

		return( -1 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libuna_utf16_string_copy_from_utf8_stream(
	          (uint16_t *) string,
	          string_size,
	          utf8_stream,
	          utf8_stream_size,
	          error );
#else
	result = libuna_utf8_string_copy_from_utf8_stream(
	          (uint8_t *) string,
	          string_size,
	          utf8_stream,
	          utf8_stream_size,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy string from UTF-8 stream.",
		 function );

		memory_free(
		 string );

		return( -1 );
	}
	libcnotify_printf(
	 "%s:\n"
	 "%" PRIs_LIBCSTRING_SYSTEM "",
	 header_string,
	 string );

	memory_free(
	 string );

	return( 1 );
}

/* Prints the UTF-16 data to the notify stream
 * Returns 1 if successful or -1 on error
 */
int libewf_debug_utf16_stream_print(
     const char *header_string,
     const uint8_t *utf16_stream,
     size_t utf16_stream_size,
     libcerror_error_t **error )
{
	libcstring_system_character_t *string = NULL;
	static char *function                 = "libewf_debug_utf16_stream_print";
	size_t string_size                    = 0;
	int result                            = 0;

	if( header_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header string.",
		 function );

		return( -1 );
	}
	if( utf16_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 stream.",
		 function );

		return( -1 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libuna_utf16_string_size_from_utf16_stream(
	          utf16_stream,
	          utf16_stream_size,
	          LIBUNA_ENDIAN_LITTLE,
	          &string_size,
	          error );
#else
	result = libuna_utf8_string_size_from_utf16_stream(
	          utf16_stream,
	          utf16_stream_size,
	          LIBUNA_ENDIAN_LITTLE,
	          &string_size,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine string size.",
		 function );

		return( -1 );
	}
	string = libcstring_system_string_allocate(
	          string_size );

	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create string.",
		 function );

		return( -1 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libuna_utf16_string_copy_from_utf16_stream(
	          (uint16_t *) string,
	          string_size,
	          utf16_stream,
	          utf16_stream_size,
	          LIBUNA_ENDIAN_LITTLE,
	          error );
#else
	result = libuna_utf8_string_copy_from_utf16_stream(
	          (uint8_t *) string,
	          string_size,
	          utf16_stream,
	          utf16_stream_size,
	          LIBUNA_ENDIAN_LITTLE,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy string from UTF-16 stream.",
		 function );

		memory_free(
		 string );

		return( -1 );
	}
	libcnotify_printf(
	 "%s:\n"
	 "%" PRIs_LIBCSTRING_SYSTEM "",
	 header_string,
	 string );

	memory_free(
	 string );

	return( 1 );
}

