/*
 * String value functions
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

#include <common.h>
#include <types.h>

#include "libfvalue_codepage.h"
#include "libfvalue_definitions.h"
#include "libfvalue_libcerror.h"
#include "libfvalue_libcnotify.h"
#include "libfvalue_libcstring.h"
#include "libfvalue_libuna.h"
#include "libfvalue_split_string.h"
#include "libfvalue_string.h"
#include "libfvalue_types.h"

/* Initialize a string
 * Returns 1 if successful or -1 on error
 */
int libfvalue_string_initialize(
     libfvalue_string_t **string,
     libcerror_error_t **error )
{
	static char *function = "libfvalue_string_initialize";

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
	if( *string != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid string value already set.",
		 function );

		return( -1 );
	}
	*string = memory_allocate_structure(
	           libfvalue_string_t );

	if( *string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create string.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *string,
	     0,
	     sizeof( libfvalue_string_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear string.",
		 function );

		goto on_error;
	}
	( *string )->codepage = LIBFVALUE_CODEPAGE_UTF8;

	return( 1 );

on_error:
	if( *string != NULL )
	{
		memory_free(
		 *string );

		*string = NULL;
	}
	return( -1 );
}

/* Frees a string
 * Returns 1 if successful or -1 on error
 */
int libfvalue_string_free(
     libfvalue_string_t **string,
     libcerror_error_t **error )
{
	static char *function = "libfvalue_string_free";

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
	if( *string != NULL )
	{
		if( ( *string )->data != NULL )
		{
			if( ( ( *string )->flags & LIBFVALUE_VALUE_FLAG_DATA_MANAGED ) != 0 )
			{
				memory_free(
				 ( *string )->data );
			}
		}
		memory_free(
		 *string );

		*string = NULL;
	}
	return( 1 );
}

/* Clones a string
 * Returns 1 if successful or -1 on error
 */
int libfvalue_string_clone(
     libfvalue_string_t **destination_string,
     libfvalue_string_t *source_string,
     libcerror_error_t **error )
{
	static char *function = "libfvalue_string_clone";

	if( destination_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination string.",
		 function );

		return( -1 );
	}
	if( *destination_string != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: destination string already set.",
		 function );

		return( -1 );
	}
	if( source_string == NULL )
	{
		*destination_string = NULL;

		return( 1 );
	}
	*destination_string = memory_allocate_structure(
	                       libfvalue_string_t );

	if( *destination_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination string.",
		 function );

		goto on_error;
	}
	if( ( source_string->flags & LIBFVALUE_VALUE_FLAG_DATA_MANAGED ) == 0 )
	{
		( *destination_string )->data      = source_string->data;
		( *destination_string )->data_size = source_string->data_size;
	}
	else
	{
		( *destination_string )->data = (uint8_t *) memory_allocate(
		                                             sizeof( uint8_t ) * source_string->data_size );

		if( ( *destination_string )->data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create destination string data.",
			 function );

			goto on_error;
		}
		( *destination_string )->data_size = source_string->data_size;

		( *destination_string )->flags |= LIBFVALUE_VALUE_FLAG_DATA_MANAGED;

		if( memory_copy(
		     ( *destination_string )->data,
		     source_string->data,
		     sizeof( uint8_t ) * source_string->data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy string data.",
			 function );

			goto on_error;
		}
	}
	( *destination_string )->codepage   = source_string->codepage;

	return( 1 );

on_error:
	if( *destination_string != NULL )
	{
		if( ( ( *destination_string )->data != NULL )
		 && ( ( *destination_string )->data != source_string->data ) )
		{
			memory_free(
			 ( *destination_string )->data );
		}
		memory_free(
		 *destination_string );

		*destination_string = NULL;
	}
	return( -1 );
}

/* Copies the string from a byte stream
 * Returns 1 if successful or -1 on error
 */
int libfvalue_string_copy_from_byte_stream(
     libfvalue_string_t *string,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     int encoding,
     libcerror_error_t **error )
{
	static char *function = "libfvalue_string_copy_from_byte_stream";

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
	if( ( encoding != LIBFVALUE_CODEPAGE_ASCII )
	 && ( encoding != LIBFVALUE_CODEPAGE_ISO_8859_1 )
	 && ( encoding != LIBFVALUE_CODEPAGE_ISO_8859_2 )
	 && ( encoding != LIBFVALUE_CODEPAGE_ISO_8859_3 )
	 && ( encoding != LIBFVALUE_CODEPAGE_ISO_8859_4 )
	 && ( encoding != LIBFVALUE_CODEPAGE_ISO_8859_5 )
	 && ( encoding != LIBFVALUE_CODEPAGE_ISO_8859_6 )
	 && ( encoding != LIBFVALUE_CODEPAGE_ISO_8859_7 )
	 && ( encoding != LIBFVALUE_CODEPAGE_ISO_8859_8 )
	 && ( encoding != LIBFVALUE_CODEPAGE_ISO_8859_9 )
	 && ( encoding != LIBFVALUE_CODEPAGE_ISO_8859_10 )
	 && ( encoding != LIBFVALUE_CODEPAGE_ISO_8859_11 )
	 && ( encoding != LIBFVALUE_CODEPAGE_ISO_8859_13 )
	 && ( encoding != LIBFVALUE_CODEPAGE_ISO_8859_14 )
	 && ( encoding != LIBFVALUE_CODEPAGE_ISO_8859_15 )
	 && ( encoding != LIBFVALUE_CODEPAGE_ISO_8859_16 )
	 && ( encoding != LIBFVALUE_CODEPAGE_KOI8_R )
	 && ( encoding != LIBFVALUE_CODEPAGE_KOI8_U )
	 && ( encoding != LIBFVALUE_CODEPAGE_UTF16_BIG_ENDIAN )
	 && ( encoding != LIBFVALUE_CODEPAGE_UTF16_LITTLE_ENDIAN )
	 && ( encoding != LIBFVALUE_CODEPAGE_UTF32_BIG_ENDIAN )
	 && ( encoding != LIBFVALUE_CODEPAGE_UTF32_LITTLE_ENDIAN )
	 && ( encoding != LIBFVALUE_CODEPAGE_UTF7 )
	 && ( encoding != LIBFVALUE_CODEPAGE_UTF8 )
	 && ( encoding != LIBFVALUE_CODEPAGE_WINDOWS_874 )
	 && ( encoding != LIBFVALUE_CODEPAGE_WINDOWS_932 )
	 && ( encoding != LIBFVALUE_CODEPAGE_WINDOWS_936 )
	 && ( encoding != LIBFVALUE_CODEPAGE_WINDOWS_1250 )
	 && ( encoding != LIBFVALUE_CODEPAGE_WINDOWS_1251 )
	 && ( encoding != LIBFVALUE_CODEPAGE_WINDOWS_1252 )
	 && ( encoding != LIBFVALUE_CODEPAGE_WINDOWS_1253 )
	 && ( encoding != LIBFVALUE_CODEPAGE_WINDOWS_1254 )
	 && ( encoding != LIBFVALUE_CODEPAGE_WINDOWS_1256 )
	 && ( encoding != LIBFVALUE_CODEPAGE_WINDOWS_1257 )
	 && ( encoding != LIBFVALUE_CODEPAGE_WINDOWS_1258 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported encoding.",
		 function );

		return( -1 );
	}
	string->data      = (uint8_t *) byte_stream;
	string->data_size = byte_stream_size;
	string->codepage  = encoding;

	return( 1 );
}

/* Copies the string from an UTF-8 encoded string
 * Returns 1 if successful or -1 on error
 */
int libfvalue_string_copy_from_utf8_string_with_index(
     libfvalue_string_t *string,
     const uint8_t *utf8_string,
     size_t utf8_string_size,
     size_t *utf8_string_index,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	static char *function  = "libfvalue_string_copy_from_utf8_string_with_index";
	size_t value_data_size = 0;
	int byte_order         = 0;

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
	if( utf8_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf8_string_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string index.",
		 function );

		return( -1 );
	}
	if( *utf8_string_index >= utf8_string_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-8 string is too small.",
		 function );

		return( -1 );
	}
	if( string_format_flags != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string format flags: 0x%08" PRIx32 ".",
		 function,
		 string_format_flags );

		return( -1 );
	}
	switch( string->codepage )
	{
		case LIBFVALUE_CODEPAGE_UTF16_BIG_ENDIAN:
		case LIBFVALUE_CODEPAGE_UTF16_LITTLE_ENDIAN:
			if( libuna_utf16_stream_size_from_utf8(
			     utf8_string,
			     utf8_string_size,
			     &value_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine UTF-16 stream size of UTF-8 string.",
				 function );

				goto on_error;
			}
			break;

		case LIBFVALUE_CODEPAGE_UTF32_BIG_ENDIAN:
		case LIBFVALUE_CODEPAGE_UTF32_LITTLE_ENDIAN:
			if( libuna_utf32_stream_size_from_utf8(
			     utf8_string,
			     utf8_string_size,
			     &value_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine UTF-32 stream size of UTF-8 string.",
				 function );

				goto on_error;
			}
			break;

		case LIBFVALUE_CODEPAGE_UTF7:
			if( libuna_utf7_stream_size_from_utf8(
			     utf8_string,
			     utf8_string_size,
			     &value_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine UTF-7 stream size of UTF-8 string.",
				 function );

				goto on_error;
			}
			break;

		case LIBFVALUE_CODEPAGE_UTF8:
			if( libuna_utf8_stream_size_from_utf8(
			     utf8_string,
			     utf8_string_size,
			     &value_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine UTF-8 stream size of UTF-8 string.",
				 function );

				goto on_error;
			}
			break;

		default:
			if( libuna_byte_stream_size_from_utf8(
			     utf8_string,
			     utf8_string_size,
			     string->codepage,
			     &value_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine byte stream size of UTF-8 string.",
				 function );

				goto on_error;
			}
			break;

	}
	if( string->data != NULL )
	{
		if( ( string->flags & LIBFVALUE_VALUE_FLAG_DATA_MANAGED ) != 0 )
		{
			memory_free(
			 string->data );

			string->flags &= ~( LIBFVALUE_VALUE_FLAG_DATA_MANAGED );
		}
		string->data      = NULL;
		string->data_size = 0;
	}
	string->data_size = value_data_size;

	string->data = (uint8_t *) memory_allocate(
	                            sizeof( uint8_t ) * string->data_size );

	if( string->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create string data.",
		 function );

		goto on_error;
	}
	string->flags |= LIBFVALUE_VALUE_FLAG_DATA_MANAGED;

	switch( string->codepage )
	{
		case LIBFVALUE_CODEPAGE_UTF16_BIG_ENDIAN:
		case LIBFVALUE_CODEPAGE_UTF16_LITTLE_ENDIAN:
			if( string->codepage == LIBFVALUE_CODEPAGE_UTF16_BIG_ENDIAN )
			{
				byte_order = LIBFVALUE_ENDIAN_BIG;
			}
			else
			{
				byte_order = LIBFVALUE_ENDIAN_LITTLE;
			}
			if( libuna_utf16_stream_copy_from_utf8(
			     string->data,
			     string->data_size,
			     byte_order,
			     utf8_string,
			     utf8_string_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy UTF-16 stream from UTF-8 string.",
				 function );

				goto on_error;
			}
			break;

		case LIBFVALUE_CODEPAGE_UTF32_BIG_ENDIAN:
		case LIBFVALUE_CODEPAGE_UTF32_LITTLE_ENDIAN:
			if( string->codepage == LIBFVALUE_CODEPAGE_UTF32_BIG_ENDIAN )
			{
				byte_order = LIBFVALUE_ENDIAN_BIG;
			}
			else
			{
				byte_order = LIBFVALUE_ENDIAN_LITTLE;
			}
			if( libuna_utf32_stream_copy_from_utf8(
			     string->data,
			     string->data_size,
			     byte_order,
			     utf8_string,
			     utf8_string_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy UTF-32 stream from UTF-8 string.",
				 function );

				goto on_error;
			}
			break;

		case LIBFVALUE_CODEPAGE_UTF7:
			if( libuna_utf7_stream_copy_from_utf8(
			     string->data,
			     string->data_size,
			     utf8_string,
			     utf8_string_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy UTF-7 stream from UTF-8 string.",
				 function );

				goto on_error;
			}
			break;

		case LIBFVALUE_CODEPAGE_UTF8:
			if( libuna_utf8_stream_copy_from_utf8(
			     string->data,
			     string->data_size,
			     utf8_string,
			     utf8_string_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy UTF-8 stream from UTF-8 string.",
				 function );

				goto on_error;
			}
			break;

		default:
			if( libuna_byte_stream_copy_from_utf8(
			     string->data,
			     string->data_size,
			     string->codepage,
			     utf8_string,
			     utf8_string_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy byte stream from UTF-8 string.",
				 function );

				goto on_error;
			}
			break;
	}
	return( 1 );

on_error:
	if( string->data != NULL )
	{
		if( ( string->flags & LIBFVALUE_VALUE_FLAG_DATA_MANAGED ) != 0 )
		{
			memory_free(
			 string->data );

			string->flags &= ~( LIBFVALUE_VALUE_FLAG_DATA_MANAGED );
		}
		string->data      = NULL;
		string->data_size = 0;
	}
	return( -1 );
}

/* Retrieves the size of an UTF-8 encoded string of the string
 * Returns 1 if successful or -1 on error
 */
int libfvalue_string_get_utf8_string_size(
     libfvalue_string_t *string,
     size_t *utf8_string_size,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	static char *function = "libfvalue_string_get_utf8_string_size";
	int byte_order         = 0;

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
	if( string_format_flags != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string format flags: 0x%08" PRIx32 ".",
		 function,
		 string_format_flags );

		return( -1 );
	}
	if( ( string->data == NULL )
	 || ( string->data_size == 0 ) )
	{
		if( utf8_string_size == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid UTF-8 string size.",
			 function );

			return( -1 );
		}
		*utf8_string_size = 1;
	}
	else switch( string->codepage )
	{
		case LIBFVALUE_CODEPAGE_UTF16_BIG_ENDIAN:
		case LIBFVALUE_CODEPAGE_UTF16_LITTLE_ENDIAN:
			if( string->codepage == LIBFVALUE_CODEPAGE_UTF16_BIG_ENDIAN )
			{
				byte_order = LIBFVALUE_ENDIAN_BIG;
			}
			else
			{
				byte_order = LIBFVALUE_ENDIAN_LITTLE;
			}
			if( libuna_utf8_string_size_from_utf16_stream(
			     string->data,
			     string->data_size,
			     byte_order,
			     utf8_string_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine UTF-8 string size of UTF-16 stream.",
				 function );

				return( -1 );
			}
			break;

		case LIBFVALUE_CODEPAGE_UTF32_BIG_ENDIAN:
		case LIBFVALUE_CODEPAGE_UTF32_LITTLE_ENDIAN:
			if( string->codepage == LIBFVALUE_CODEPAGE_UTF32_BIG_ENDIAN )
			{
				byte_order = LIBFVALUE_ENDIAN_BIG;
			}
			else
			{
				byte_order = LIBFVALUE_ENDIAN_LITTLE;
			}
			if( libuna_utf8_string_size_from_utf32_stream(
			     string->data,
			     string->data_size,
			     byte_order,
			     utf8_string_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine UTF-8 string size of UTF-32 stream.",
				 function );

				return( -1 );
			}
			break;

		case LIBFVALUE_CODEPAGE_UTF7:
			if( libuna_utf8_string_size_from_utf7_stream(
			     string->data,
			     string->data_size,
			     utf8_string_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine UTF-8 string size of UTF-7 stream.",
				 function );

				return( -1 );
			}
			break;

		case LIBFVALUE_CODEPAGE_UTF8:
			if( libuna_utf8_string_size_from_utf8_stream(
			     string->data,
			     string->data_size,
			     utf8_string_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine UTF-8 string size of UTF-8 stream.",
				 function );

				return( -1 );
			}
			break;

		default:
			if( libuna_utf8_string_size_from_byte_stream(
			     string->data,
			     string->data_size,
			     string->codepage,
			     utf8_string_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine UTF-8 string size of byte stream.",
				 function );

				return( -1 );
			}
			break;
	}
	return( 1 );
}

/* Copies the string to an UTF-8 encoded string
 * Returns 1 if successful or -1 on error
 */
int libfvalue_string_copy_to_utf8_string_with_index(
     libfvalue_string_t *string,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     size_t *utf8_string_index,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	static char *function = "libfvalue_string_copy_to_utf8_string_with_index";
	int byte_order         = 0;

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
	if( string_format_flags != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string format flags: 0x%08" PRIx32 ".",
		 function,
		 string_format_flags );

		return( -1 );
	}
	if( ( string->data == NULL )
	 || ( string->data_size == 0 ) )
	{
		if( utf8_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid UTF-8 string.",
			 function );

			return( -1 );
		}
		if( utf8_string_size > (size_t) SSIZE_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid UTF-8 string size value exceeds maximum.",
			 function );

			return( -1 );
		}
		if( utf8_string_index == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid UTF-8 string index.",
			 function );

			return( -1 );
		}
		if( *utf8_string_index >= utf8_string_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: UTF-8 string is too small.",
			 function );

			return( -1 );
		}
		utf8_string[ *utf8_string_index ] = 0;

		*utf8_string_index += 1;
	}
	else switch( string->codepage )
	{
		case LIBFVALUE_CODEPAGE_UTF16_BIG_ENDIAN:
		case LIBFVALUE_CODEPAGE_UTF16_LITTLE_ENDIAN:
			if( string->codepage == LIBFVALUE_CODEPAGE_UTF16_BIG_ENDIAN )
			{
				byte_order = LIBFVALUE_ENDIAN_BIG;
			}
			else
			{
				byte_order = LIBFVALUE_ENDIAN_LITTLE;
			}
			if( libuna_utf8_string_with_index_copy_from_utf16_stream(
			     utf8_string,
			     utf8_string_size,
			     utf8_string_index,
			     string->data,
			     string->data_size,
			     byte_order,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy UTF-16 stream to UTF-8 string.",
				 function );

				return( -1 );
			}
			break;

		case LIBFVALUE_CODEPAGE_UTF32_BIG_ENDIAN:
		case LIBFVALUE_CODEPAGE_UTF32_LITTLE_ENDIAN:
			if( string->codepage == LIBFVALUE_CODEPAGE_UTF32_BIG_ENDIAN )
			{
				byte_order = LIBFVALUE_ENDIAN_BIG;
			}
			else
			{
				byte_order = LIBFVALUE_ENDIAN_LITTLE;
			}
			if( libuna_utf8_string_with_index_copy_from_utf32_stream(
			     utf8_string,
			     utf8_string_size,
			     utf8_string_index,
			     string->data,
			     string->data_size,
			     byte_order,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy UTF-32 stream to UTF-8 string.",
				 function );

				return( -1 );
			}
			break;

		case LIBFVALUE_CODEPAGE_UTF7:
			if( libuna_utf8_string_with_index_copy_from_utf7_stream(
			     utf8_string,
			     utf8_string_size,
			     utf8_string_index,
			     string->data,
			     string->data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy UTF-7 stream to UTF-8 string.",
				 function );

				return( -1 );
			}
			break;

		case LIBFVALUE_CODEPAGE_UTF8:
			if( libuna_utf8_string_with_index_copy_from_utf8_stream(
			     utf8_string,
			     utf8_string_size,
			     utf8_string_index,
			     string->data,
			     string->data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy UTF-8 stream to UTF-8 string.",
				 function );

				return( -1 );
			}
			break;

		default:
			if( libuna_utf8_string_with_index_copy_from_byte_stream(
			     utf8_string,
			     utf8_string_size,
			     utf8_string_index,
			     string->data,
			     string->data_size,
			     string->codepage,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy byte stream to UTF-8 string.",
				 function );

				return( -1 );
			}
			break;
	}
	return( 1 );
}

/* Copies the string from an UTF-16 encoded string
 * Returns 1 if successful or -1 on error
 */
int libfvalue_string_copy_from_utf16_string_with_index(
     libfvalue_string_t *string,
     const uint16_t *utf16_string,
     size_t utf16_string_size,
     size_t *utf16_string_index,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	static char *function  = "libfvalue_string_copy_from_utf16_string_with_index";
	size_t value_data_size = 0;
	int byte_order         = 0;

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
	if( utf16_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-16 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf16_string_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string index.",
		 function );

		return( -1 );
	}
	if( *utf16_string_index >= utf16_string_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-16 string is too small.",
		 function );

		return( -1 );
	}
	if( string_format_flags != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string format flags: 0x%08" PRIx32 ".",
		 function,
		 string_format_flags );

		return( -1 );
	}
	switch( string->codepage )
	{
		case LIBFVALUE_CODEPAGE_UTF16_BIG_ENDIAN:
		case LIBFVALUE_CODEPAGE_UTF16_LITTLE_ENDIAN:
			if( libuna_utf16_stream_size_from_utf16(
			     utf16_string,
			     utf16_string_size,
			     &value_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine UTF-16 stream size of UTF-16 string.",
				 function );

				goto on_error;
			}
			break;

		case LIBFVALUE_CODEPAGE_UTF32_BIG_ENDIAN:
		case LIBFVALUE_CODEPAGE_UTF32_LITTLE_ENDIAN:
			if( libuna_utf32_stream_size_from_utf16(
			     utf16_string,
			     utf16_string_size,
			     &value_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine UTF-32 stream size of UTF-16 string.",
				 function );

				goto on_error;
			}
			break;

		case LIBFVALUE_CODEPAGE_UTF7:
			if( libuna_utf7_stream_size_from_utf16(
			     utf16_string,
			     utf16_string_size,
			     &value_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine UTF-7 stream size of UTF-16 string.",
				 function );

				goto on_error;
			}
			break;

		case LIBFVALUE_CODEPAGE_UTF8:
			if( libuna_utf8_stream_size_from_utf16(
			     utf16_string,
			     utf16_string_size,
			     &value_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine UTF-8 stream size of UTF-16 string.",
				 function );

				goto on_error;
			}
			break;

		default:
			if( libuna_byte_stream_size_from_utf16(
			     utf16_string,
			     utf16_string_size,
			     string->codepage,
			     &value_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine byte stream size of UTF-16 string.",
				 function );

				goto on_error;
			}
			break;

	}
	if( string->data != NULL )
	{
		if( ( string->flags & LIBFVALUE_VALUE_FLAG_DATA_MANAGED ) != 0 )
		{
			memory_free(
			 string->data );

			string->flags &= ~( LIBFVALUE_VALUE_FLAG_DATA_MANAGED );
		}
		string->data      = NULL;
		string->data_size = 0;
	}
	string->data_size = value_data_size;

	string->data = (uint8_t *) memory_allocate(
	                            sizeof( uint8_t ) * string->data_size );

	if( string->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create string data.",
		 function );

		goto on_error;
	}
	string->flags |= LIBFVALUE_VALUE_FLAG_DATA_MANAGED;

	switch( string->codepage )
	{
		case LIBFVALUE_CODEPAGE_UTF16_BIG_ENDIAN:
		case LIBFVALUE_CODEPAGE_UTF16_LITTLE_ENDIAN:
			if( string->codepage == LIBFVALUE_CODEPAGE_UTF16_BIG_ENDIAN )
			{
				byte_order = LIBFVALUE_ENDIAN_BIG;
			}
			else
			{
				byte_order = LIBFVALUE_ENDIAN_LITTLE;
			}
			if( libuna_utf16_stream_copy_from_utf16(
			     string->data,
			     string->data_size,
			     byte_order,
			     utf16_string,
			     utf16_string_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy UTF-16 stream from UTF-16 string.",
				 function );

				goto on_error;
			}
			break;

		case LIBFVALUE_CODEPAGE_UTF32_BIG_ENDIAN:
		case LIBFVALUE_CODEPAGE_UTF32_LITTLE_ENDIAN:
			if( string->codepage == LIBFVALUE_CODEPAGE_UTF32_BIG_ENDIAN )
			{
				byte_order = LIBFVALUE_ENDIAN_BIG;
			}
			else
			{
				byte_order = LIBFVALUE_ENDIAN_LITTLE;
			}
			if( libuna_utf32_stream_copy_from_utf16(
			     string->data,
			     string->data_size,
			     byte_order,
			     utf16_string,
			     utf16_string_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy UTF-32 stream from UTF-16 string.",
				 function );

				goto on_error;
			}
			break;

		case LIBFVALUE_CODEPAGE_UTF7:
			if( libuna_utf7_stream_copy_from_utf16(
			     string->data,
			     string->data_size,
			     utf16_string,
			     utf16_string_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy UTF-7 stream from UTF-16 string.",
				 function );

				goto on_error;
			}
			break;

		case LIBFVALUE_CODEPAGE_UTF8:
			if( libuna_utf8_stream_copy_from_utf16(
			     string->data,
			     string->data_size,
			     utf16_string,
			     utf16_string_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy UTF-8 stream from UTF-16 string.",
				 function );

				goto on_error;
			}
			break;

		default:
			if( libuna_byte_stream_copy_from_utf16(
			     string->data,
			     string->data_size,
			     string->codepage,
			     utf16_string,
			     utf16_string_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy byte stream from UTF-16 string.",
				 function );

				goto on_error;
			}
			break;
	}
	return( 1 );

on_error:
	if( string->data != NULL )
	{
		if( ( string->flags & LIBFVALUE_VALUE_FLAG_DATA_MANAGED ) != 0 )
		{
			memory_free(
			 string->data );

			string->flags &= ~( LIBFVALUE_VALUE_FLAG_DATA_MANAGED );
		}
		string->data      = NULL;
		string->data_size = 0;
	}
	return( -1 );
}

/* Retrieves the size of an UTF-16 encoded string of the string
 * Returns 1 if successful or -1 on error
 */
int libfvalue_string_get_utf16_string_size(
     libfvalue_string_t *string,
     size_t *utf16_string_size,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	static char *function = "libfvalue_string_get_utf16_string_size";
	int byte_order         = 0;

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
	if( string_format_flags != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string format flags: 0x%08" PRIx32 ".",
		 function,
		 string_format_flags );

		return( -1 );
	}
	if( ( string->data == NULL )
	 || ( string->data_size == 0 ) )
	{
		if( utf16_string_size == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid UTF-16 string size.",
			 function );

			return( -1 );
		}
		*utf16_string_size = 1;
	}
	else switch( string->codepage )
	{
		case LIBFVALUE_CODEPAGE_UTF16_BIG_ENDIAN:
		case LIBFVALUE_CODEPAGE_UTF16_LITTLE_ENDIAN:
			if( string->codepage == LIBFVALUE_CODEPAGE_UTF16_BIG_ENDIAN )
			{
				byte_order = LIBFVALUE_ENDIAN_BIG;
			}
			else
			{
				byte_order = LIBFVALUE_ENDIAN_LITTLE;
			}
			if( libuna_utf16_string_size_from_utf16_stream(
			     string->data,
			     string->data_size,
			     byte_order,
			     utf16_string_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine UTF-16 string size of UTF-16 stream.",
				 function );

				return( -1 );
			}
			break;

		case LIBFVALUE_CODEPAGE_UTF32_BIG_ENDIAN:
		case LIBFVALUE_CODEPAGE_UTF32_LITTLE_ENDIAN:
			if( string->codepage == LIBFVALUE_CODEPAGE_UTF32_BIG_ENDIAN )
			{
				byte_order = LIBFVALUE_ENDIAN_BIG;
			}
			else
			{
				byte_order = LIBFVALUE_ENDIAN_LITTLE;
			}
			if( libuna_utf16_string_size_from_utf32_stream(
			     string->data,
			     string->data_size,
			     byte_order,
			     utf16_string_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine UTF-16 string size of UTF-32 stream.",
				 function );

				return( -1 );
			}
			break;

		case LIBFVALUE_CODEPAGE_UTF7:
			if( libuna_utf16_string_size_from_utf7_stream(
			     string->data,
			     string->data_size,
			     utf16_string_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine UTF-16 string size of UTF-7 stream.",
				 function );

				return( -1 );
			}
			break;

		case LIBFVALUE_CODEPAGE_UTF8:
			if( libuna_utf16_string_size_from_utf8_stream(
			     string->data,
			     string->data_size,
			     utf16_string_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine UTF-16 string size of UTF-8 stream.",
				 function );

				return( -1 );
			}
			break;

		default:
			if( libuna_utf16_string_size_from_byte_stream(
			     string->data,
			     string->data_size,
			     string->codepage,
			     utf16_string_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine UTF-16 string size of byte stream.",
				 function );

				return( -1 );
			}
			break;
	}
	return( 1 );
}

/* Copies the string to an UTF-16 encoded string
 * Returns 1 if successful or -1 on error
 */
int libfvalue_string_copy_to_utf16_string_with_index(
     libfvalue_string_t *string,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     size_t *utf16_string_index,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	static char *function = "libfvalue_string_copy_to_utf16_string_with_index";
	int byte_order         = 0;

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
	if( string_format_flags != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string format flags: 0x%08" PRIx32 ".",
		 function,
		 string_format_flags );

		return( -1 );
	}
	if( ( string->data == NULL )
	 || ( string->data_size == 0 ) )
	{
		if( utf16_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid UTF-16 string.",
			 function );

			return( -1 );
		}
		if( utf16_string_size > (size_t) SSIZE_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid UTF-16 string size value exceeds maximum.",
			 function );

			return( -1 );
		}
		if( utf16_string_index == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid UTF-16 string index.",
			 function );

			return( -1 );
		}
		if( *utf16_string_index >= utf16_string_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: UTF-16 string is too small.",
			 function );

			return( -1 );
		}
		utf16_string[ *utf16_string_index ] = 0;

		*utf16_string_index += 1;
	}
	else switch( string->codepage )
	{
		case LIBFVALUE_CODEPAGE_UTF16_BIG_ENDIAN:
		case LIBFVALUE_CODEPAGE_UTF16_LITTLE_ENDIAN:
			if( string->codepage == LIBFVALUE_CODEPAGE_UTF16_BIG_ENDIAN )
			{
				byte_order = LIBFVALUE_ENDIAN_BIG;
			}
			else
			{
				byte_order = LIBFVALUE_ENDIAN_LITTLE;
			}
			if( libuna_utf16_string_with_index_copy_from_utf16_stream(
			     utf16_string,
			     utf16_string_size,
			     utf16_string_index,
			     string->data,
			     string->data_size,
			     byte_order,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy UTF-16 stream to UTF-16 string.",
				 function );

				return( -1 );
			}
			break;

		case LIBFVALUE_CODEPAGE_UTF32_BIG_ENDIAN:
		case LIBFVALUE_CODEPAGE_UTF32_LITTLE_ENDIAN:
			if( string->codepage == LIBFVALUE_CODEPAGE_UTF32_BIG_ENDIAN )
			{
				byte_order = LIBFVALUE_ENDIAN_BIG;
			}
			else
			{
				byte_order = LIBFVALUE_ENDIAN_LITTLE;
			}
			if( libuna_utf16_string_with_index_copy_from_utf32_stream(
			     utf16_string,
			     utf16_string_size,
			     utf16_string_index,
			     string->data,
			     string->data_size,
			     byte_order,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy UTF-32 stream to UTF-16 string.",
				 function );

				return( -1 );
			}
			break;

		case LIBFVALUE_CODEPAGE_UTF7:
			if( libuna_utf16_string_with_index_copy_from_utf7_stream(
			     utf16_string,
			     utf16_string_size,
			     utf16_string_index,
			     string->data,
			     string->data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy UTF-7 stream to UTF-16 string.",
				 function );

				return( -1 );
			}
			break;

		case LIBFVALUE_CODEPAGE_UTF8:
			if( libuna_utf16_string_with_index_copy_from_utf8_stream(
			     utf16_string,
			     utf16_string_size,
			     utf16_string_index,
			     string->data,
			     string->data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy UTF-8 stream to UTF-16 string.",
				 function );

				return( -1 );
			}
			break;

		default:
			if( libuna_utf16_string_with_index_copy_from_byte_stream(
			     utf16_string,
			     utf16_string_size,
			     utf16_string_index,
			     string->data,
			     string->data_size,
			     string->codepage,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy byte stream to UTF-16 string.",
				 function );

				return( -1 );
			}
			break;
	}
	return( 1 );
}

/* Copies the string from an UTF-32 encoded string
 * Returns 1 if successful or -1 on error
 */
int libfvalue_string_copy_from_utf32_string_with_index(
     libfvalue_string_t *string,
     const uint32_t *utf32_string,
     size_t utf32_string_size,
     size_t *utf32_string_index,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	static char *function  = "libfvalue_string_copy_from_utf32_string_with_index";
	size_t value_data_size = 0;
	int byte_order         = 0;

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
	if( utf32_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-32 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf32_string_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-32 string index.",
		 function );

		return( -1 );
	}
	if( *utf32_string_index >= utf32_string_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-32 string is too small.",
		 function );

		return( -1 );
	}
	if( string_format_flags != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string format flags: 0x%08" PRIx32 ".",
		 function,
		 string_format_flags );

		return( -1 );
	}
	switch( string->codepage )
	{
		case LIBFVALUE_CODEPAGE_UTF16_BIG_ENDIAN:
		case LIBFVALUE_CODEPAGE_UTF16_LITTLE_ENDIAN:
			if( libuna_utf16_stream_size_from_utf32(
			     utf32_string,
			     utf32_string_size,
			     &value_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine UTF-16 stream size of UTF-32 string.",
				 function );

				goto on_error;
			}
			break;

		case LIBFVALUE_CODEPAGE_UTF32_BIG_ENDIAN:
		case LIBFVALUE_CODEPAGE_UTF32_LITTLE_ENDIAN:
			if( libuna_utf32_stream_size_from_utf32(
			     utf32_string,
			     utf32_string_size,
			     &value_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine UTF-32 stream size of UTF-32 string.",
				 function );

				goto on_error;
			}
			break;

		case LIBFVALUE_CODEPAGE_UTF7:
			if( libuna_utf7_stream_size_from_utf32(
			     utf32_string,
			     utf32_string_size,
			     &value_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine UTF-7 stream size of UTF-32 string.",
				 function );

				goto on_error;
			}
			break;

		case LIBFVALUE_CODEPAGE_UTF8:
			if( libuna_utf8_stream_size_from_utf32(
			     utf32_string,
			     utf32_string_size,
			     &value_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine UTF-8 stream size of UTF-32 string.",
				 function );

				goto on_error;
			}
			break;

		default:
			if( libuna_byte_stream_size_from_utf32(
			     utf32_string,
			     utf32_string_size,
			     string->codepage,
			     &value_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine byte stream size of UTF-32 string.",
				 function );

				goto on_error;
			}
			break;

	}
	if( string->data != NULL )
	{
		if( ( string->flags & LIBFVALUE_VALUE_FLAG_DATA_MANAGED ) != 0 )
		{
			memory_free(
			 string->data );

			string->flags &= ~( LIBFVALUE_VALUE_FLAG_DATA_MANAGED );
		}
		string->data      = NULL;
		string->data_size = 0;
	}
	string->data_size = value_data_size;

	string->data = (uint8_t *) memory_allocate(
	                            sizeof( uint8_t ) * string->data_size );

	if( string->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create string data.",
		 function );

		goto on_error;
	}
	string->flags |= LIBFVALUE_VALUE_FLAG_DATA_MANAGED;

	switch( string->codepage )
	{
		case LIBFVALUE_CODEPAGE_UTF16_BIG_ENDIAN:
		case LIBFVALUE_CODEPAGE_UTF16_LITTLE_ENDIAN:
			if( string->codepage == LIBFVALUE_CODEPAGE_UTF16_BIG_ENDIAN )
			{
				byte_order = LIBFVALUE_ENDIAN_BIG;
			}
			else
			{
				byte_order = LIBFVALUE_ENDIAN_LITTLE;
			}
			if( libuna_utf16_stream_copy_from_utf32(
			     string->data,
			     string->data_size,
			     byte_order,
			     utf32_string,
			     utf32_string_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy UTF-16 stream from UTF-32 string.",
				 function );

				goto on_error;
			}
			break;

		case LIBFVALUE_CODEPAGE_UTF32_BIG_ENDIAN:
		case LIBFVALUE_CODEPAGE_UTF32_LITTLE_ENDIAN:
			if( string->codepage == LIBFVALUE_CODEPAGE_UTF32_BIG_ENDIAN )
			{
				byte_order = LIBFVALUE_ENDIAN_BIG;
			}
			else
			{
				byte_order = LIBFVALUE_ENDIAN_LITTLE;
			}
			if( libuna_utf32_stream_copy_from_utf32(
			     string->data,
			     string->data_size,
			     byte_order,
			     utf32_string,
			     utf32_string_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy UTF-32 stream from UTF-32 string.",
				 function );

				goto on_error;
			}
			break;

		case LIBFVALUE_CODEPAGE_UTF7:
			if( libuna_utf7_stream_copy_from_utf32(
			     string->data,
			     string->data_size,
			     utf32_string,
			     utf32_string_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy UTF-7 stream from UTF-32 string.",
				 function );

				goto on_error;
			}
			break;

		case LIBFVALUE_CODEPAGE_UTF8:
			if( libuna_utf8_stream_copy_from_utf32(
			     string->data,
			     string->data_size,
			     utf32_string,
			     utf32_string_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy UTF-8 stream from UTF-32 string.",
				 function );

				goto on_error;
			}
			break;

		default:
			if( libuna_byte_stream_copy_from_utf32(
			     string->data,
			     string->data_size,
			     string->codepage,
			     utf32_string,
			     utf32_string_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy byte stream from UTF-32 string.",
				 function );

				goto on_error;
			}
			break;
	}
	return( 1 );

on_error:
	if( string->data != NULL )
	{
		if( ( string->flags & LIBFVALUE_VALUE_FLAG_DATA_MANAGED ) != 0 )
		{
			memory_free(
			 string->data );

			string->flags &= ~( LIBFVALUE_VALUE_FLAG_DATA_MANAGED );
		}
		string->data      = NULL;
		string->data_size = 0;
	}
	return( -1 );
}

/* Retrieves the size of an UTF-32 encoded string of the string
 * Returns 1 if successful or -1 on error
 */
int libfvalue_string_get_utf32_string_size(
     libfvalue_string_t *string,
     size_t *utf32_string_size,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	static char *function = "libfvalue_string_get_utf32_string_size";
	int byte_order         = 0;

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
	if( string_format_flags != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string format flags: 0x%08" PRIx32 ".",
		 function,
		 string_format_flags );

		return( -1 );
	}
	if( ( string->data == NULL )
	 || ( string->data_size == 0 ) )
	{
		if( utf32_string_size == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid UTF-32 string size.",
			 function );

			return( -1 );
		}
		*utf32_string_size = 1;
	}
	else switch( string->codepage )
	{
		case LIBFVALUE_CODEPAGE_UTF16_BIG_ENDIAN:
		case LIBFVALUE_CODEPAGE_UTF16_LITTLE_ENDIAN:
			if( string->codepage == LIBFVALUE_CODEPAGE_UTF16_BIG_ENDIAN )
			{
				byte_order = LIBFVALUE_ENDIAN_BIG;
			}
			else
			{
				byte_order = LIBFVALUE_ENDIAN_LITTLE;
			}
			if( libuna_utf32_string_size_from_utf16_stream(
			     string->data,
			     string->data_size,
			     byte_order,
			     utf32_string_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine UTF-32 string size of UTF-16 stream.",
				 function );

				return( -1 );
			}
			break;

		case LIBFVALUE_CODEPAGE_UTF32_BIG_ENDIAN:
		case LIBFVALUE_CODEPAGE_UTF32_LITTLE_ENDIAN:
			if( string->codepage == LIBFVALUE_CODEPAGE_UTF32_BIG_ENDIAN )
			{
				byte_order = LIBFVALUE_ENDIAN_BIG;
			}
			else
			{
				byte_order = LIBFVALUE_ENDIAN_LITTLE;
			}
			if( libuna_utf32_string_size_from_utf32_stream(
			     string->data,
			     string->data_size,
			     byte_order,
			     utf32_string_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine UTF-32 string size of UTF-32 stream.",
				 function );

				return( -1 );
			}
			break;

		case LIBFVALUE_CODEPAGE_UTF7:
			if( libuna_utf32_string_size_from_utf7_stream(
			     string->data,
			     string->data_size,
			     utf32_string_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine UTF-32 string size of UTF-7 stream.",
				 function );

				return( -1 );
			}
			break;

		case LIBFVALUE_CODEPAGE_UTF8:
			if( libuna_utf32_string_size_from_utf8_stream(
			     string->data,
			     string->data_size,
			     utf32_string_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine UTF-32 string size of UTF-8 stream.",
				 function );

				return( -1 );
			}
			break;

		default:
			if( libuna_utf32_string_size_from_byte_stream(
			     string->data,
			     string->data_size,
			     string->codepage,
			     utf32_string_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine UTF-32 string size of byte stream.",
				 function );

				return( -1 );
			}
			break;
	}
	return( 1 );
}

/* Copies the string to an UTF-32 encoded string
 * Returns 1 if successful or -1 on error
 */
int libfvalue_string_copy_to_utf32_string_with_index(
     libfvalue_string_t *string,
     uint32_t *utf32_string,
     size_t utf32_string_size,
     size_t *utf32_string_index,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	static char *function = "libfvalue_string_copy_to_utf32_string_with_index";
	int byte_order         = 0;

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
	if( string_format_flags != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string format flags: 0x%08" PRIx32 ".",
		 function,
		 string_format_flags );

		return( -1 );
	}
	if( ( string->data == NULL )
	 || ( string->data_size == 0 ) )
	{
		if( utf32_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid UTF-32 string.",
			 function );

			return( -1 );
		}
		if( utf32_string_size > (size_t) SSIZE_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid UTF-32 string size value exceeds maximum.",
			 function );

			return( -1 );
		}
		if( utf32_string_index == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid UTF-32 string index.",
			 function );

			return( -1 );
		}
		if( *utf32_string_index >= utf32_string_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: UTF-32 string is too small.",
			 function );

			return( -1 );
		}
		utf32_string[ *utf32_string_index ] = 0;

		*utf32_string_index += 1;
	}
	else switch( string->codepage )
	{
		case LIBFVALUE_CODEPAGE_UTF16_BIG_ENDIAN:
		case LIBFVALUE_CODEPAGE_UTF16_LITTLE_ENDIAN:
			if( string->codepage == LIBFVALUE_CODEPAGE_UTF16_BIG_ENDIAN )
			{
				byte_order = LIBFVALUE_ENDIAN_BIG;
			}
			else
			{
				byte_order = LIBFVALUE_ENDIAN_LITTLE;
			}
			if( libuna_utf32_string_with_index_copy_from_utf16_stream(
			     utf32_string,
			     utf32_string_size,
			     utf32_string_index,
			     string->data,
			     string->data_size,
			     byte_order,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy UTF-16 stream to UTF-32 string.",
				 function );

				return( -1 );
			}
			break;

		case LIBFVALUE_CODEPAGE_UTF32_BIG_ENDIAN:
		case LIBFVALUE_CODEPAGE_UTF32_LITTLE_ENDIAN:
			if( string->codepage == LIBFVALUE_CODEPAGE_UTF32_BIG_ENDIAN )
			{
				byte_order = LIBFVALUE_ENDIAN_BIG;
			}
			else
			{
				byte_order = LIBFVALUE_ENDIAN_LITTLE;
			}
			if( libuna_utf32_string_with_index_copy_from_utf32_stream(
			     utf32_string,
			     utf32_string_size,
			     utf32_string_index,
			     string->data,
			     string->data_size,
			     byte_order,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy UTF-32 stream to UTF-32 string.",
				 function );

				return( -1 );
			}
			break;

		case LIBFVALUE_CODEPAGE_UTF7:
			if( libuna_utf32_string_with_index_copy_from_utf7_stream(
			     utf32_string,
			     utf32_string_size,
			     utf32_string_index,
			     string->data,
			     string->data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy UTF-7 stream to UTF-32 string.",
				 function );

				return( -1 );
			}
			break;

		case LIBFVALUE_CODEPAGE_UTF8:
			if( libuna_utf32_string_with_index_copy_from_utf8_stream(
			     utf32_string,
			     utf32_string_size,
			     utf32_string_index,
			     string->data,
			     string->data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy UTF-8 stream to UTF-32 string.",
				 function );

				return( -1 );
			}
			break;

		default:
			if( libuna_utf32_string_with_index_copy_from_byte_stream(
			     utf32_string,
			     utf32_string_size,
			     utf32_string_index,
			     string->data,
			     string->data_size,
			     string->codepage,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy byte stream to UTF-32 string.",
				 function );

				return( -1 );
			}
			break;
	}
	return( 1 );
}

/* Splits an UTF-8 string
 * Returns 1 if successful or -1 on error
 */
int libfvalue_utf8_string_split(
     const uint8_t *utf8_string,
     size_t utf8_string_size,
     uint8_t delimiter,
     libfvalue_split_utf8_string_t **split_string,
     libcerror_error_t **error )
{
	uint8_t *segment_start = NULL;
	uint8_t *segment_end   = NULL;
	uint8_t *string_end    = NULL;
	static char *function  = "libfvalue_utf8_string_split";
	size_t string_size     = 0;
	ssize_t segment_length = 0;
	int number_of_segments = 0;
	int segment_index      = 0;

	if( utf8_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( utf8_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( split_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid split string.",
		 function );

		return( -1 );
	}
	if( *split_string != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid split string already set.",
		 function );

		return( -1 );
	}
	/* An empty string has no segments
	 */
	if( ( utf8_string_size == 0 )
	 || ( utf8_string[ 0 ] == 0 ) )
	{
		return( 1 );
	}
	/* Determine the number of segments
	 */
	segment_start = (uint8_t *) utf8_string;
	string_end    = (uint8_t *) &( utf8_string[ utf8_string_size - 1 ] );

	do
	{
		segment_end = segment_start;

		while( segment_end <= string_end )
		{
			if( ( segment_end == string_end )
			 || ( *segment_end == 0 ) )
			{
				segment_end = NULL;

				break;
			}
			else if( *segment_end == delimiter )
			{
				break;
			}
			segment_end++;
		}
		if( segment_end > string_end )
		{
			break;
		}
		segment_index++;

		if( segment_end == NULL )
		{
			break;
		}
		if( segment_end == segment_start )
		{
			segment_start++;
		}
		else if( segment_end != utf8_string )
		{
			segment_start = segment_end + 1;
		}
	}
	while( segment_end != NULL );

	number_of_segments = segment_index;

	if( libfvalue_split_utf8_string_initialize(
	     split_string,
	     utf8_string,
	     utf8_string_size,
	     number_of_segments,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to intialize split string.",
		 function );

		goto on_error;
	}
	if( *split_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing split string.",
		 function );

		goto on_error;
	}
	/* Do not bother splitting empty strings
	 */
	if( number_of_segments == 0 )
	{
		return( 1 );
	}
	/* Determine the segments
	 * empty segments are stored as strings only containing the end of character
	 */
	if( libfvalue_split_utf8_string_get_string(
	     *split_string,
	     &segment_start,
	     &string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve split UTF-8 string.",
		 function );

		goto on_error;
	}
	if( segment_start == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing segment start.",
		 function );

		goto on_error;
	}
	if( string_size < 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid string size value out of bounds.",
		 function );

		goto on_error;
	}
	segment_end = segment_start;
	string_end  = &( segment_start[ string_size - 1 ] );

	for( segment_index = 0;
	     segment_index < number_of_segments;
	     segment_index++ )
	{
		segment_end = segment_start;

		while( segment_end <= string_end )
		{
			if( ( segment_end == string_end )
			 || ( *segment_end == 0 ) )
			{
				segment_end = NULL;

				break;
			}
			else if( *segment_end == delimiter )
			{
				break;
			}
			segment_end++;
		}
		if( segment_end == NULL )
		{
			segment_length = (ssize_t) ( string_end - segment_start );
		}
		else
		{
			segment_length = (ssize_t) ( segment_end - segment_start );
		}
		if( segment_length >= 0 )
		{
			segment_start[ segment_length ] = 0;

			if( libfvalue_split_utf8_string_set_segment_by_index(
			     *split_string,
			     segment_index,
			     segment_start,
			     segment_length + 1,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set split UTF-8 string segment: %d.",
				 function,
				 segment_index );

				goto on_error;
			}
		}
		if( segment_end == NULL )
		{
			break;
		}
		if( segment_end == string_end )
		{
			segment_start++;
		}
		if( segment_end != string_end )
		{
			segment_start = segment_end + 1;
		}
	}
	return( 1 );

on_error:
	if( *split_string != NULL )
	{
		libfvalue_split_utf8_string_free(
		 split_string,
		 NULL );
	}
	return( -1 );
}

/* Splits an UTF-16 string
 * Returns 1 if successful or -1 on error
 */
int libfvalue_utf16_string_split(
     const uint16_t *utf16_string,
     size_t utf16_string_size,
     uint16_t delimiter,
     libfvalue_split_utf16_string_t **split_string,
     libcerror_error_t **error )
{
	uint16_t *segment_start = NULL;
	uint16_t *segment_end   = NULL;
	uint16_t *string_end    = NULL;
	static char *function   = "libfvalue_utf16_string_split";
	size_t string_size      = 0;
	ssize_t segment_length  = 0;
	int number_of_segments  = 0;
	int segment_index       = 0;

	if( utf16_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string.",
		 function );

		return( -1 );
	}
	if( utf16_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-16 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( split_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid split string.",
		 function );

		return( -1 );
	}
	if( *split_string != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid split string already set.",
		 function );

		return( -1 );
	}
	/* An empty string has no segments
	 */
	if( ( utf16_string_size == 0 )
	 || ( utf16_string[ 0 ] == 0 ) )
	{
		return( 1 );
	}
	/* Determine the number of segments
	 */
	segment_start = (uint16_t *) utf16_string;
	string_end    = (uint16_t *) &( utf16_string[ utf16_string_size - 1 ] );

	do
	{
		segment_end = segment_start;

		while( segment_end <= string_end )
		{
			if( ( segment_end == string_end )
			 || ( *segment_end == 0 ) )
			{
				segment_end = NULL;

				break;
			}
			else if( *segment_end == delimiter )
			{
				break;
			}
			segment_end++;
		}
		if( segment_end > string_end )
		{
			break;
		}
		segment_index++;

		if( segment_end == NULL )
		{
			break;
		}
		if( segment_end == segment_start )
		{
			segment_start++;
		}
		else if( segment_end != utf16_string )
		{
			segment_start = segment_end + 1;
		}
	}
	while( segment_end != NULL );

	number_of_segments = segment_index;

	if( libfvalue_split_utf16_string_initialize(
	     split_string,
	     utf16_string,
	     utf16_string_size,
	     number_of_segments,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to intialize split string.",
		 function );

		goto on_error;
	}
	if( *split_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing split string.",
		 function );

		goto on_error;
	}
	/* Do not bother splitting empty strings
	 */
	if( number_of_segments == 0 )
	{
		return( 1 );
	}
	/* Determine the segments
	 * empty segments are stored as strings only containing the end of character
	 */
	if( libfvalue_split_utf16_string_get_string(
	     *split_string,
	     &segment_start,
	     &string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve split UTF-16 string.",
		 function );

		goto on_error;
	}
	if( segment_start == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing segment start.",
		 function );

		goto on_error;
	}
	if( string_size < 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid string size value out of bounds.",
		 function );

		goto on_error;
	}
	segment_end = segment_start;
	string_end  = &( segment_start[ string_size - 1 ] );

	for( segment_index = 0;
	     segment_index < number_of_segments;
	     segment_index++ )
	{
		segment_end = segment_start;

		while( segment_end <= string_end )
		{
			if( ( segment_end == string_end )
			 || ( *segment_end == 0 ) )
			{
				segment_end = NULL;

				break;
			}
			else if( *segment_end == delimiter )
			{
				break;
			}
			segment_end++;
		}
		if( segment_end == NULL )
		{
			segment_length = (ssize_t) ( string_end - segment_start );
		}
		else
		{
			segment_length = (ssize_t) ( segment_end - segment_start );
		}
		if( segment_length >= 0 )
		{
			segment_start[ segment_length ] = 0;

			if( libfvalue_split_utf16_string_set_segment_by_index(
			     *split_string,
			     segment_index,
			     segment_start,
			     segment_length + 1,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set split UTF-16 string segment: %d.",
				 function,
				 segment_index );

				goto on_error;
			}
		}
		if( segment_end == NULL )
		{
			break;
		}
		if( segment_end == string_end )
		{
			segment_start++;
		}
		if( segment_end != string_end )
		{
			segment_start = segment_end + 1;
		}
	}
	return( 1 );

on_error:
	if( *split_string != NULL )
	{
		libfvalue_split_utf16_string_free(
		 split_string,
		 NULL );
	}
	return( -1 );
}

