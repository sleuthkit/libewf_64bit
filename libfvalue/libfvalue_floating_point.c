/*
 * Floating point value (IEEE 754) functions
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libfvalue_definitions.h"
#include "libfvalue_floating_point.h"
#include "libfvalue_libcerror.h"

/* Initialize a floating point
 * Returns 1 if successful or -1 on error
 */
int libfvalue_floating_point_initialize(
     libfvalue_floating_point_t **floating_point,
     libcerror_error_t **error )
{
	static char *function = "libfvalue_floating_point_initialize";

	if( floating_point == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid floating point.",
		 function );

		return( -1 );
	}
	if( *floating_point != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid floating point value already set.",
		 function );

		return( -1 );
	}
	*floating_point = memory_allocate_structure(
	            libfvalue_floating_point_t );

	if( *floating_point == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create floating point.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *floating_point,
	     0,
	     sizeof( libfvalue_floating_point_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear floating point.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *floating_point != NULL )
	{
		memory_free(
		 *floating_point );

		*floating_point = NULL;
	}
	return( -1 );
}

/* Frees a floating point
 * Returns 1 if successful or -1 on error
 */
int libfvalue_floating_point_free(
     libfvalue_floating_point_t **floating_point,
     libcerror_error_t **error )
{
	static char *function = "libfvalue_floating_point_free";

	if( floating_point == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid floating point.",
		 function );

		return( -1 );
	}
	if( *floating_point != NULL )
	{
		memory_free(
		 *floating_point );

		*floating_point = NULL;
	}
	return( 1 );
}

/* Clones a floating point
 * Returns 1 if successful or -1 on error
 */
int libfvalue_floating_point_clone(
     libfvalue_floating_point_t **destination_floating_point,
     libfvalue_floating_point_t *source_floating_point,
     libcerror_error_t **error )
{
	static char *function = "libfvalue_floating_point_clone";

	if( destination_floating_point == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination floating point.",
		 function );

		return( -1 );
	}
	if( *destination_floating_point != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: destination floating point already set.",
		 function );

		return( -1 );
	}
	if( source_floating_point == NULL )
	{
		*destination_floating_point = NULL;

		return( 1 );
	}
	*destination_floating_point = memory_allocate_structure(
	                               libfvalue_floating_point_t );

	if( *destination_floating_point == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination floating point.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_floating_point,
	     source_floating_point,
	     sizeof( libfvalue_floating_point_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy floating point.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_floating_point != NULL )
	{
		memory_free(
		 *destination_floating_point );

		*destination_floating_point = NULL;
	}
	return( -1 );
}

/* Copies the floating point from a byte stream
 * Returns 1 if successful or -1 on error
 */
int libfvalue_floating_point_copy_from_byte_stream(
     libfvalue_floating_point_t *floating_point,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     int encoding,
     libcerror_error_t **error )
{
	static char *function = "libfvalue_floating_point_copy_from_byte_stream";

	if( floating_point == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid floating point.",
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
	if( ( encoding != LIBFVALUE_ENDIAN_BIG )
	 && ( encoding != LIBFVALUE_ENDIAN_LITTLE )
	 && ( encoding != LIBFVALUE_ENDIAN_NATIVE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported encoding.",
		 function );

		return( -1 );
	}
	switch( byte_stream_size )
	{
		case 4:
			if( encoding == LIBFVALUE_ENDIAN_BIG )
			{
				byte_stream_copy_to_uint32_big_endian(
				 byte_stream,
				 floating_point->value );
			}
			else if( encoding == LIBFVALUE_ENDIAN_LITTLE )
			{
				byte_stream_copy_to_uint32_little_endian(
				 byte_stream,
				 floating_point->value );
			}
			else
			{
				floating_point->value = (uint64_t) *( (uint32_t *) byte_stream );
			}
			break;

		case 8:
			if( encoding == LIBFVALUE_ENDIAN_BIG )
			{
				byte_stream_copy_to_uint64_big_endian(
				 byte_stream,
				 floating_point->value );
			}
			else if( encoding == LIBFVALUE_ENDIAN_LITTLE )
			{
				byte_stream_copy_to_uint64_little_endian(
				 byte_stream,
				 floating_point->value );
			}
			else
			{
				floating_point->value = (uint64_t) *( (uint64_t *) byte_stream );
			}
			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported byte stream size: %" PRIzd ".",
			 function,
			 byte_stream_size );

			return( -1 );
	}
	floating_point->value_size = byte_stream_size * 8;

	return( 1 );
}

/* Copies the floating point from an integer value
 * Returns 1 if successful or -1 on error
 */
int libfvalue_floating_point_copy_from_integer(
     libfvalue_floating_point_t *floating_point,
     uint64_t integer_value,
     size_t integer_value_size,
     libcerror_error_t **error )
{
	byte_stream_float64_t value_float64;

	static char *function = "libfvalue_floating_point_copy_from_integer";

	if( floating_point == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid floating point.",
		 function );

		return( -1 );
	}
	if( ( integer_value_size != 32 )
	 && ( integer_value_size != 64 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported integer value size.",
		 function );

		return( -1 );
	}
	value_float64.floating_point = (double) integer_value;
	floating_point->value        = value_float64.integer;
	floating_point->value_size   = 64;

	return( 1 );
}

/* Copies the floating point to an integer value
 * Returns 1 if successful or -1 on error
 */
int libfvalue_floating_point_copy_to_integer(
     libfvalue_floating_point_t *floating_point,
     uint64_t *integer_value,
     size_t *integer_value_size,
     libcerror_error_t **error )
{
	byte_stream_float64_t value_float64;

	static char *function = "libfvalue_floating_point_copy_to_integer";

	if( floating_point == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid floating point.",
		 function );

		return( -1 );
	}
	if( integer_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid integer value.",
		 function );

		return( -1 );
	}
	if( integer_value_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid integer value size.",
		 function );

		return( -1 );
	}
	value_float64.integer = floating_point->value;
	*integer_value        = (uint64_t) value_float64.floating_point;
	*integer_value_size   = 64;

	return( 1 );
}

/* Retrieves the size of a string of the floating point
 * Returns 1 if successful or -1 on error
 */
int libfvalue_floating_point_get_string_size(
     libfvalue_floating_point_t *floating_point,
     size_t *string_size,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	static char *function = "libfvalue_floating_point_get_string_size";

	if( floating_point == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid floating point.",
		 function );

		return( -1 );
	}
	if( libfvalue_string_size_from_floating_point(
	     string_size,
	     floating_point->value,
	     floating_point->value_size,
	     string_format_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to determine size of string of %" PRIzd "-bit floating point.",
		 function,
		 floating_point->value_size );

		return( -1 );
	}
	return( 1 );
}

/* Copies the floating point from an UTF-8 encoded string
 * Returns 1 if successful or -1 on error
 */
int libfvalue_floating_point_copy_from_utf8_string_with_index(
     libfvalue_floating_point_t *floating_point,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     size_t *utf8_string_index,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	static char *function = "libfvalue_floating_point_copy_from_utf8_string_with_index";

	if( floating_point == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid floating point.",
		 function );

		return( -1 );
	}
	if( libfvalue_utf8_string_with_index_copy_to_floating_point(
	     utf8_string,
	     utf8_string_size,
	     utf8_string_index,
	     &( floating_point->value ),
	     floating_point->value_size,
	     string_format_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy %" PRIzd "-bit floating point from UTF-8 string.",
		 function,
		 floating_point->value_size );

		return( -1 );
	}
	return( 1 );
}

/* Copies the floating point to an UTF-8 encoded string
 * Returns 1 if successful or -1 on error
 */
int libfvalue_floating_point_copy_to_utf8_string_with_index(
     libfvalue_floating_point_t *floating_point,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     size_t *utf8_string_index,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	static char *function = "libfvalue_floating_point_copy_to_utf8_string_with_index";

	if( floating_point == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid floating point.",
		 function );

		return( -1 );
	}
	if( libfvalue_utf8_string_with_index_copy_from_floating_point(
	     utf8_string,
	     utf8_string_size,
	     utf8_string_index,
	     floating_point->value,
	     floating_point->value_size,
	     string_format_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy %" PRIzd "-bit floating point to UTF-8 string.",
		 function,
		 floating_point->value_size );

		return( -1 );
	}
	return( 1 );
}

/* Copies the floating point from an UTF-16 encoded string
 * Returns 1 if successful or -1 on error
 */
int libfvalue_floating_point_copy_from_utf16_string_with_index(
     libfvalue_floating_point_t *floating_point,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     size_t *utf16_string_index,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	static char *function = "libfvalue_floating_point_copy_from_utf16_string_with_index";

	if( floating_point == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid floating point.",
		 function );

		return( -1 );
	}
	if( libfvalue_utf16_string_with_index_copy_to_floating_point(
	     utf16_string,
	     utf16_string_size,
	     utf16_string_index,
	     &( floating_point->value ),
	     floating_point->value_size,
	     string_format_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy %" PRIzd "-bit floating point from UTF-16 string.",
		 function,
		 floating_point->value_size );

		return( -1 );
	}
	return( 1 );
}

/* Copies the floating point to an UTF-16 encoded string
 * Returns 1 if successful or -1 on error
 */
int libfvalue_floating_point_copy_to_utf16_string_with_index(
     libfvalue_floating_point_t *floating_point,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     size_t *utf16_string_index,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	static char *function = "libfvalue_floating_point_copy_to_utf16_string_with_index";

	if( floating_point == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid floating point.",
		 function );

		return( -1 );
	}
	if( libfvalue_utf16_string_with_index_copy_from_floating_point(
	     utf16_string,
	     utf16_string_size,
	     utf16_string_index,
	     floating_point->value,
	     floating_point->value_size,
	     string_format_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy %" PRIzd "-bit floating point to UTF-16 string.",
		 function,
		 floating_point->value_size );

		return( -1 );
	}
	return( 1 );
}

/* Copies the floating point from an UTF-32 encoded string
 * Returns 1 if successful or -1 on error
 */
int libfvalue_floating_point_copy_from_utf32_string_with_index(
     libfvalue_floating_point_t *floating_point,
     uint32_t *utf32_string,
     size_t utf32_string_size,
     size_t *utf32_string_index,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	static char *function = "libfvalue_floating_point_copy_from_utf32_string_with_index";

	if( floating_point == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid floating point.",
		 function );

		return( -1 );
	}
	if( libfvalue_utf32_string_with_index_copy_to_floating_point(
	     utf32_string,
	     utf32_string_size,
	     utf32_string_index,
	     &( floating_point->value ),
	     floating_point->value_size,
	     string_format_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy %" PRIzd "-bit floating point from UTF-32 string.",
		 function,
		 floating_point->value_size );

		return( -1 );
	}
	return( 1 );
}

/* Copies the floating point to an UTF-32 encoded string
 * Returns 1 if successful or -1 on error
 */
int libfvalue_floating_point_copy_to_utf32_string_with_index(
     libfvalue_floating_point_t *floating_point,
     uint32_t *utf32_string,
     size_t utf32_string_size,
     size_t *utf32_string_index,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	static char *function = "libfvalue_floating_point_copy_to_utf32_string_with_index";

	if( floating_point == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid floating point.",
		 function );

		return( -1 );
	}
	if( libfvalue_utf32_string_with_index_copy_from_floating_point(
	     utf32_string,
	     utf32_string_size,
	     utf32_string_index,
	     floating_point->value,
	     floating_point->value_size,
	     string_format_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy %" PRIzd "-bit floating point to UTF-32 string.",
		 function,
		 floating_point->value_size );

		return( -1 );
	}
	return( 1 );
}

/* Determines the size of a string of a floating point value
 * The floating_point value size is in bits
 * Returns 1 if successful or -1 on error
 */
int libfvalue_string_size_from_floating_point(
     size_t *string_size,
     uint64_t floating_point_value,
     size_t floating_point_value_size,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	char value_string[ 16 ];

	byte_stream_float32_t value_float32;
	byte_stream_float64_t value_float64;

	static char *function       = "libfvalue_string_size_from_floating_point";
	uint64_t divider            = 0;
	uint64_t multiplier         = 0;
	uint64_t value_64bit        = 0;
	uint32_t string_format_type = 0;
	uint32_t supported_flags    = 0;
	uint16_t exponent_divider   = 0;
	int16_t exponent            = 0;
	uint8_t is_denormalized     = 0;
	uint8_t is_indeterminate    = 0;
	uint8_t is_infinite         = 0;
	uint8_t is_not_a_number     = 0;
	uint8_t is_signed           = 0;
	uint8_t is_zero             = 0;
	uint8_t use_value_string    = 0;
	int8_t bit_shift            = 0;
	int print_count             = 0;

	if( string_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string size.",
		 function );

		return( -1 );
	}
	if( ( floating_point_value_size != 32 )
	 && ( floating_point_value_size != 64 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported floating point value size.",
		 function );

		return( -1 );
	}
	supported_flags = 0x000000ffUL;

	if( ( string_format_flags & ~( supported_flags ) ) != 0 )
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
	string_format_type = string_format_flags & 0x000000ffUL;

	if( ( string_format_type != LIBFVALUE_FLOATING_POINT_FORMAT_TYPE_DECIMAL )
	 && ( string_format_type != LIBFVALUE_FLOATING_POINT_FORMAT_TYPE_HEXADECIMAL ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string format type.",
		 function );

		return( -1 );
	}
	if( string_format_type == LIBFVALUE_FLOATING_POINT_FORMAT_TYPE_HEXADECIMAL )
	{
		*string_size = ( floating_point_value_size >> 2 ) + 3;
	}
	else
	{
		bit_shift = (uint8_t) ( floating_point_value_size - 1 );
		is_signed = (uint8_t) ( floating_point_value >> bit_shift );

		if( is_signed != 0 )
		{
			floating_point_value &= ~( (uint64_t) 1 << bit_shift );
		}
		switch( floating_point_value_size )
		{
			case 32:
				if( floating_point_value == 0x7f800000UL )
				{
					is_infinite = 1;
				}
				else if( ( is_signed != 0 )
				      && ( floating_point_value == 0x7fc00000UL ) )
				{
					is_indeterminate = 1;
				}
				else if( ( floating_point_value >= 0x7f800001UL )
				      && ( floating_point_value <= 0x7fffffffUL ) )
				{
					is_not_a_number = 1;
				}
				else if( floating_point_value != 0 )
				{
					value_float32.integer = (uint32_t) floating_point_value;

					print_count = libcstring_narrow_string_snprintf(
					               value_string,
					               16,
					               "%06e",
					               value_float32.floating_point );

					if( ( print_count < 0 )
					 || ( print_count > 16 ) )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
						 "%s: unable to print floating point value to string.",
						 function );

						return( -1 );
					}
					exponent = (int16_t) ( floating_point_value >> 23 );

					if( exponent == 0 )
					{
						is_denormalized = 1;

						exponent = -126;
					}
					else
					{
						exponent -= 127;
					}
					floating_point_value &= 0x007fffffUL;
					bit_shift             = 23;
				}
				else
				{
					is_zero = 1;
				}
				break;

			case 64:
				if( floating_point_value == 0x7ff0000000000000ULL )
				{
					is_infinite = 1;
				}
				else if( ( is_signed != 0 )
				      && ( floating_point_value == 0x7ff8000000000000ULL ) )
				{
					is_indeterminate = 1;
				}
				else if( ( floating_point_value >= 0x7ff0000000000001ULL )
				      && ( floating_point_value <= 0x7fffffffffffffffULL ) )
				{
					is_not_a_number = 1;
				}
				else if( floating_point_value != 0 )
				{
					value_float64.integer = floating_point_value;

					print_count = libcstring_narrow_string_snprintf(
					               value_string,
					               16,
					               "%06e",
					               value_float64.floating_point );

					if( ( print_count < 0 )
					 || ( print_count > 16 ) )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
						 "%s: unable to print floating point value to string.",
						 function );

						return( -1 );
					}
					exponent = (int16_t) ( floating_point_value >> 52 );

					if( exponent == 0 )
					{
						is_denormalized = 1;

						exponent = -1023;
					}
					else
					{
						exponent -= 1023;
					}
					floating_point_value &= 0x000fffffffffffffULL;
					bit_shift             = 52;
				}
				else
				{
					is_zero = 1;
				}
				break;
		}
		/* The string is at least a single digit with an end of string character
		 */
		*string_size = 2;

		if( is_signed != 0 )
		{
			*string_size += 1;
		}
		if( is_indeterminate != 0 )
		{
			*string_size += 3;
		}
		else if( is_infinite != 0 )
		{
			*string_size += 3;
		}
		else if( is_not_a_number != 0 )
		{
			*string_size += 3;
		}
		else if( is_zero == 0 )
		{
			if( is_denormalized != 0 )
			{
				while( bit_shift >= 0 )
				{
					if( ( ( floating_point_value >> bit_shift ) & 0x01 ) != 0 )
					{
						break;
					}
					bit_shift--;
					exponent--;
				}
				use_value_string = 1;
			}
			else
			{
				if( floating_point_value != 0 )
				{
					use_value_string = 1;
				}
				if( exponent != 0 )
				{
					use_value_string = 1;
				}
				floating_point_value |= (uint64_t) 1 << bit_shift;
			}
			if( use_value_string == 0 )
			{
				multiplier = 1;

				while( bit_shift > 0 )
				{
					value_64bit *= 10;
					value_64bit += ( ( floating_point_value >> bit_shift ) & 0x01 ) * multiplier;
					multiplier  *= 5;

					floating_point_value ^= (uint64_t) 1 << bit_shift;

					if( floating_point_value == 0 )
					{
						break;
					}
					if( multiplier > 15625 )
					{
						use_value_string = 1;

						break;
					}
					bit_shift--;
				}
				floating_point_value = value_64bit;
			}
			if( use_value_string != 0 )
			{
				*string_size += 12;
			}
			else
			{
				divider = 1;

				while( ( floating_point_value / divider ) >= 10 )
				{
					divider *= 10;

					*string_size += 1;
				}
				if( exponent != 0 )
				{
					if( exponent < 0 )
					{
						exponent *= -1;
					}
					exponent_divider = 1;

					while( ( exponent / exponent_divider ) >= 10 )
					{
						exponent_divider *= 10;

						*string_size += 1;
					}
					*string_size += 3;
				}
			}
		}
	}
	return( 1 );
}

/* Copies an UTF-8 encoded string from a floating point value
 * The floating_point value size is in bits
 * Returns 1 if successful or -1 on error
 */
int libfvalue_utf8_string_copy_from_floating_point(
     uint8_t *utf8_string,
     size_t utf8_string_size,
     uint64_t floating_point_value,
     size_t floating_point_value_size,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	static char *function    = "libfvalue_utf8_string_copy_from_floating_point";
	size_t utf8_string_index = 0;

	if( libfvalue_utf8_string_with_index_copy_from_floating_point(
	     utf8_string,
	     utf8_string_size,
	     &utf8_string_index,
	     floating_point_value,
	     floating_point_value_size,
	     string_format_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy floating point value to UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Copies an UTF-8 encoded string of from floating_point value
 * The floating_point value size is in bits
 * Returns 1 if successful or -1 on error
 */
int libfvalue_utf8_string_with_index_copy_from_floating_point(
     uint8_t *utf8_string,
     size_t utf8_string_size,
     size_t *utf8_string_index,
     uint64_t floating_point_value,
     size_t floating_point_value_size,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	char value_string[ 16 ];

	byte_stream_float32_t value_float32;
	byte_stream_float64_t value_float64;

	static char *function        = "libfvalue_utf8_string_with_index_copy_from_floating_point";
	size_t string_index          = 0;
	uint64_t divider             = 0;
	uint64_t multiplier          = 0;
	uint64_t value_64bit         = 0;
	uint32_t string_format_type  = 0;
	uint32_t supported_flags     = 0;
	uint16_t exponent_divider    = 0;
	int16_t exponent             = 0;
	int16_t exponent_value       = 0;
	uint8_t byte_value           = 0;
	uint8_t is_denormalized      = 0;
	uint8_t is_indeterminate     = 0;
	uint8_t is_infinite          = 0;
	uint8_t is_not_a_number      = 0;
	uint8_t is_signed            = 0;
	uint8_t is_zero              = 0;
	uint8_t number_of_characters = 0;
	uint8_t use_value_string     = 0;
	int8_t bit_shift             = 0;
	int print_count              = 0;

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
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid UTF-8 string index value out of bounds.",
		 function );

		return( -1 );
	}
	string_index = *utf8_string_index;

	if( ( floating_point_value_size != 32 )
	 && ( floating_point_value_size != 64 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported floating point value size.",
		 function );

		return( -1 );
	}
	supported_flags = 0x000000ffUL;

	if( ( string_format_flags & ~( supported_flags ) ) != 0 )
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
	string_format_type = string_format_flags & 0x000000ffUL;

	if( ( string_format_type != LIBFVALUE_FLOATING_POINT_FORMAT_TYPE_DECIMAL )
	 && ( string_format_type != LIBFVALUE_FLOATING_POINT_FORMAT_TYPE_HEXADECIMAL ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string format type.",
		 function );

		return( -1 );
	}
	if( string_format_type == LIBFVALUE_FLOATING_POINT_FORMAT_TYPE_HEXADECIMAL )
	{
		number_of_characters = (uint8_t) ( floating_point_value_size >> 2 ) + 3;
	}
	else
	{
		bit_shift = (uint8_t) ( floating_point_value_size - 1 );
		is_signed = (uint8_t) ( floating_point_value >> bit_shift );

		if( is_signed != 0 )
		{
			floating_point_value &= ~( (uint64_t) 1 << bit_shift );
		}
		switch( floating_point_value_size )
		{
			case 32:
				if( floating_point_value == 0x7f800000UL )
				{
					is_infinite = 1;
				}
				else if( ( is_signed != 0 )
				      && ( floating_point_value == 0x7fc00000UL ) )
				{
					is_indeterminate = 1;
				}
				else if( ( floating_point_value >= 0x7f800001UL )
				      && ( floating_point_value <= 0x7fffffffUL ) )
				{
					is_not_a_number = 1;
				}
				else if( floating_point_value != 0 )
				{
					value_float32.integer = (uint32_t) floating_point_value;

					print_count = libcstring_narrow_string_snprintf(
					               value_string,
					               16,
					               "%06e",
					               value_float32.floating_point );

					if( ( print_count < 0 )
					 || ( print_count > 16 ) )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
						 "%s: unable to print floating point value to string.",
						 function );

						return( -1 );
					}
					exponent = (int16_t) ( floating_point_value >> 23 );

					if( exponent == 0 )
					{
						is_denormalized = 1;

						exponent = -126;
					}
					else
					{
						exponent -= 127;
					}
					floating_point_value &= 0x007fffffUL;
					bit_shift             = 23;
				}
				else
				{
					is_zero = 1;
				}
				break;

			case 64:
				if( floating_point_value == 0x7ff0000000000000ULL )
				{
					is_infinite = 1;
				}
				else if( ( is_signed != 0 )
				      && ( floating_point_value == 0x7ff8000000000000ULL ) )
				{
					is_indeterminate = 1;
				}
				else if( ( floating_point_value >= 0x7ff0000000000001ULL )
				      && ( floating_point_value <= 0x7fffffffffffffffULL ) )
				{
					is_not_a_number = 1;
				}
				else if( floating_point_value != 0 )
				{
					value_float64.integer = floating_point_value;

					print_count = libcstring_narrow_string_snprintf(
					               value_string,
					               16,
					               "%06e",
					               value_float64.floating_point );

					if( ( print_count < 0 )
					 || ( print_count > 16 ) )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
						 "%s: unable to print floating point value to string.",
						 function );

						return( -1 );
					}
					exponent = (int16_t) ( floating_point_value >> 52 );

					if( exponent == 0 )
					{
						is_denormalized = 1;

						exponent = -1023;
					}
					else
					{
						exponent -= 1023;
					}
					floating_point_value &= 0x000fffffffffffffULL;
					bit_shift             = 52;
				}
				else
				{
					is_zero = 1;
				}
				break;
		}
		/* The string is at least a single digit with an end of string character
		 */
		number_of_characters = 2;

		if( is_signed != 0 )
		{
			number_of_characters += 1;
		}
		if( is_indeterminate != 0 )
		{
			number_of_characters += 3;
		}
		else if( is_infinite != 0 )
		{
			number_of_characters += 3;
		}
		else if( is_not_a_number != 0 )
		{
			number_of_characters += 3;
		}
		else if( is_zero == 0 )
		{
			if( is_denormalized != 0 )
			{
				while( bit_shift >= 0 )
				{
					if( ( ( floating_point_value >> bit_shift ) & 0x01 ) != 0 )
					{
						break;
					}
					bit_shift--;
					exponent--;
				}
				use_value_string = 1;
			}
			else
			{
				if( floating_point_value != 0 )
				{
					use_value_string = 1;
				}
				if( exponent != 0 )
				{
					use_value_string = 1;
				}
				floating_point_value |= (uint64_t) 1 << bit_shift;
			}
			if( use_value_string == 0 )
			{
				multiplier = 1;

				while( bit_shift >= 0 )
				{
					value_64bit *= 10;
					value_64bit += ( ( floating_point_value >> bit_shift ) & 0x01 ) * multiplier;
					multiplier  *= 5;

					floating_point_value ^= (uint64_t) 1 << bit_shift;

					if( floating_point_value == 0 )
					{
						break;
					}
					if( multiplier > 15625 )
					{
						use_value_string = 1;

						break;
					}
					bit_shift--;
				}
				floating_point_value = value_64bit;
			}
			if( use_value_string != 0 )
			{
				number_of_characters += 12;
			}
			else
			{
				divider = 1;

				while( ( floating_point_value / divider ) >= 10 )
				{
					divider *= 10;

					number_of_characters += 1;
				}
				exponent_value = exponent;

				if( exponent_value != 0 )
				{
					if( exponent_value < 0 )
					{
						exponent_value *= -1;
					}
					exponent_divider = 1;

					while( ( exponent_value / exponent_divider ) >= 10 )
					{
						exponent_divider *= 10;

						number_of_characters += 1;
					}
					number_of_characters += 3;
				}
			}
		}
	}
	if( ( string_index + number_of_characters ) > utf8_string_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-8 string size too small.",
		 function );

		return( -1 );
	}
	if( string_format_type == LIBFVALUE_FLOATING_POINT_FORMAT_TYPE_HEXADECIMAL )
	{
		utf8_string[ string_index++ ] = (uint8_t) '0';
		utf8_string[ string_index++ ] = (uint8_t) 'x';

		bit_shift = (uint8_t) ( floating_point_value_size - 4 );

		do
		{
			byte_value = (uint8_t) ( ( floating_point_value >> bit_shift ) & 0x0f );

			if( byte_value <= 9 )
			{
				utf8_string[ string_index++ ] = (uint8_t) '0' + byte_value;
			}
			else
			{
				utf8_string[ string_index++ ] = (uint8_t) 'a' + byte_value - 10;
			}
			bit_shift -= 4;
		}
		while( bit_shift >= 0 );
	}
	else
	{
		if( is_signed != 0 )
		{
			utf8_string[ string_index++ ] = (uint8_t) '-';
		}
		if( is_indeterminate != 0 )
		{
			utf8_string[ string_index++ ] = (uint8_t) 'I';
			utf8_string[ string_index++ ] = (uint8_t) 'n';
			utf8_string[ string_index++ ] = (uint8_t) 'd';
		}
		else if( is_infinite != 0 )
		{
			utf8_string[ string_index++ ] = (uint8_t) 'I';
			utf8_string[ string_index++ ] = (uint8_t) 'n';
			utf8_string[ string_index++ ] = (uint8_t) 'f';
		}
		else if( is_not_a_number != 0 )
		{
			utf8_string[ string_index++ ] = (uint8_t) 'N';
			utf8_string[ string_index++ ] = (uint8_t) 'a';
			utf8_string[ string_index++ ] = (uint8_t) 'N';
		}
		else if( is_zero == 0 )
		{
			if( use_value_string != 0 )
			{
				utf8_string[ string_index++ ] = (uint8_t) value_string[ 0 ];
				utf8_string[ string_index++ ] = (uint8_t) value_string[ 1 ];
				utf8_string[ string_index++ ] = (uint8_t) value_string[ 2 ];
				utf8_string[ string_index++ ] = (uint8_t) value_string[ 3 ];
				utf8_string[ string_index++ ] = (uint8_t) value_string[ 4 ];
				utf8_string[ string_index++ ] = (uint8_t) value_string[ 5 ];
				utf8_string[ string_index++ ] = (uint8_t) value_string[ 6 ];
				utf8_string[ string_index++ ] = (uint8_t) value_string[ 7 ];
				utf8_string[ string_index++ ] = (uint8_t) value_string[ 8 ];
				utf8_string[ string_index++ ] = (uint8_t) value_string[ 9 ];
				utf8_string[ string_index++ ] = (uint8_t) value_string[ 10 ];
				utf8_string[ string_index++ ] = (uint8_t) value_string[ 11 ];
			}
			else
			{
				if( divider > 1 )
				{
					utf8_string[ string_index++ ] = (uint8_t) '0' + (uint8_t) ( floating_point_value / divider );

					floating_point_value %= divider;

					divider /= 10;

					utf8_string[ string_index++ ] = (uint8_t) '.';

					while( divider > 1 )
					{
						utf8_string[ string_index++ ] = (uint8_t) '0' + (uint8_t) ( floating_point_value / divider );

						floating_point_value %= divider;

						divider /= 10;
					}
				}
				utf8_string[ string_index++ ] = (uint8_t) '0' + (uint8_t) ( floating_point_value / divider );

				if( exponent != 0 )
				{
					utf8_string[ string_index++ ] = (uint8_t) 'e';

					if( exponent < 0 )
					{
						exponent *= -1;

						utf8_string[ string_index++ ] = (uint8_t) '-';
					}
					else
					{
						utf8_string[ string_index++ ] = (uint8_t) '+';
					}
					while( exponent_divider > 1 )
					{
						utf8_string[ string_index++ ] = (uint8_t) '0' + (uint8_t) ( exponent / exponent_divider );

						exponent %= exponent_divider;

						exponent_divider /= 10;
					}
					utf8_string[ string_index++ ] = (uint8_t) '0' + (uint8_t) ( exponent / exponent_divider );
				}
			}
		}
		else
		{
			utf8_string[ string_index++ ] = (uint8_t) '0';
		}
	}
	utf8_string[ string_index++ ] = 0;

	*utf8_string_index = string_index;

	return( 1 );
}

/* Copies an UTF-8 encoded string to a floating point value
 * The floating_point value size is in bits
 * Returns 1 if successful or -1 on error
 */
int libfvalue_utf8_string_with_index_copy_to_floating_point(
     uint8_t *utf8_string,
     size_t utf8_string_length,
     size_t *utf8_string_index,
     uint64_t *floating_point_value,
     size_t floating_point_value_size,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	static char *function       = "libfvalue_utf8_string_with_index_copy_to_floating_point";
	size_t maximum_string_index = 0;
	size_t string_index         = 0;
	uint64_t divider            = 0;
	uint64_t value_64bit        = 0;
	uint32_t string_format_type = 0;
	uint32_t supported_flags    = 0;
	uint8_t byte_value          = 0;
	uint8_t character_value     = 0;
	int8_t bit_shift            = 0;
	int8_t sign                 = 1;

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
	if( utf8_string_length > (size_t) SSIZE_MAX )
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
	if( *utf8_string_index >= utf8_string_length )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid UTF-8 string index value out of bounds.",
		 function );

		return( -1 );
	}
	string_index = *utf8_string_index;

	if( floating_point_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid floating point value.",
		 function );

		return( -1 );
	}
	if( ( floating_point_value_size != 32 )
	 && ( floating_point_value_size != 64 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported floating point value size.",
		 function );

		return( -1 );
	}
	supported_flags = 0x000000ffUL;

	if( ( string_format_flags & ~( supported_flags ) ) != 0 )
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
	string_format_type = string_format_flags & 0x000000ffUL;

	if( ( string_format_type != LIBFVALUE_FLOATING_POINT_FORMAT_TYPE_DECIMAL )
	 && ( string_format_type != LIBFVALUE_FLOATING_POINT_FORMAT_TYPE_HEXADECIMAL ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string format type.",
		 function );

		return( -1 );
	}
	if( string_format_type == LIBFVALUE_FLOATING_POINT_FORMAT_TYPE_HEXADECIMAL )
	{
		maximum_string_index = (size_t) ( floating_point_value_size >> 2 ) + 3;
	}
	else
	{
		/* The string is at least a single digit with an end of string character
		 */
		maximum_string_index = 2;

		bit_shift = (uint8_t) ( floating_point_value_size - 1 );

		divider = 1;

		value_64bit = ~( ( ~( (uint64_t) 1 << bit_shift ) >> bit_shift ) << bit_shift );

		while( ( value_64bit / divider ) >= 10 )
		{
			divider *= 10;

			maximum_string_index += 1;
		}
	}
	maximum_string_index += string_index;

	if( maximum_string_index > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid maximum string index value exceeds maximum.",
		 function );

		return( -1 );
	}
	value_64bit = 0;

	if( string_format_type == LIBFVALUE_FLOATING_POINT_FORMAT_TYPE_HEXADECIMAL )
	{
		if( utf8_string[ string_index ] != (uint8_t) '0' )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported character value: 0x%02" PRIx8 " at index: %d.",
			 function,
			 utf8_string[ string_index ],
			 string_index );

			return( -1 );
		}
		string_index++;

		if( utf8_string[ string_index ] != (uint8_t) 'x' )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported character value: 0x%02" PRIx8 " at index: %d.",
			 function,
			 utf8_string[ string_index ],
			 string_index );

			return( -1 );
		}
		string_index++;

		while( string_index < utf8_string_length )
		{
			if( utf8_string[ string_index ] == 0 )
			{
				break;
			}
			if( string_index > (size_t) maximum_string_index )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_LARGE,
				 "%s: string too large.",
				 function );

				return( -1 );
			}
			value_64bit <<= 4;

			if( ( utf8_string[ string_index ] >= (uint8_t) '0' )
			 && ( utf8_string[ string_index ] <= (uint8_t) '9' ) )
			{
				byte_value = (uint8_t) ( utf8_string[ string_index ] - (uint8_t) '0' );
			}
			else if( ( utf8_string[ string_index ] >= (uint8_t) 'A' )
			      && ( utf8_string[ string_index ] <= (uint8_t) 'F' ) )
			{
				byte_value = (uint8_t) ( utf8_string[ string_index ] - (uint8_t) 'A' + 10 );
			}
			else if( ( utf8_string[ string_index ] >= (uint8_t) 'a' )
			      && ( utf8_string[ string_index ] <= (uint8_t) 'f' ) )
			{
				byte_value = (uint8_t) ( utf8_string[ string_index ] - (uint8_t) 'a' + 10 );
			}
			else
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported character value: 0x%02" PRIx8 " at index: %d.",
				 function,
				 utf8_string[ string_index ],
				 string_index );

				return( -1 );
			}
			value_64bit += byte_value;

			string_index++;
		}
	}
	else
	{
		/* In the maximum possible string one character is substituted for the sign
		 */
		if( utf8_string[ string_index ] == (uint8_t) '-' )
		{
			string_index++;

			sign = -1;
		}
		else if( utf8_string[ string_index ] == (uint8_t) '+' )
		{
			string_index++;
		}
		while( string_index < utf8_string_length )
		{
			if( utf8_string[ string_index ] == 0 )
			{
				break;
			}
			if( string_index > (size_t) maximum_string_index )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_LARGE,
				 "%s: string too large.",
				 function );

				return( -1 );
			}
			value_64bit *= 10;

			if( ( utf8_string[ string_index ] < (uint8_t) '0' )
			 && ( utf8_string[ string_index ] > (uint8_t) '9' ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported character value: 0x%02" PRIx8 " at index: %d.",
				 function,
				 utf8_string[ string_index ],
				 string_index );

				return( -1 );
			}
			character_value = (uint8_t) ( utf8_string[ string_index ] - (uint8_t) '0' );

			value_64bit += character_value;

			string_index++;
		}
		if( sign == -1 )
		{
			value_64bit |= (uint64_t) 1 << bit_shift;
		}
	}
	*utf8_string_index = string_index;
	*floating_point_value     = value_64bit;

	return( 1 );
}

/* Copies an UTF-16 encoded string of a floating point value
 * The floating_point value size is in bits
 * Returns 1 if successful or -1 on error
 */
int libfvalue_utf16_string_copy_from_floating_point(
     uint16_t *utf16_string,
     size_t utf16_string_size,
     uint64_t floating_point_value,
     size_t floating_point_value_size,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	static char *function     = "libfvalue_utf16_string_copy_from_floating_point";
	size_t utf16_string_index = 0;

	if( libfvalue_utf16_string_with_index_copy_from_floating_point(
	     utf16_string,
	     utf16_string_size,
	     &utf16_string_index,
	     floating_point_value,
	     floating_point_value_size,
	     string_format_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy floating point value to UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Copies an UTF-16 encoded string of a floating point value
 * The floating_point value size is in bits
 * Returns 1 if successful or -1 on error
 */
int libfvalue_utf16_string_with_index_copy_from_floating_point(
     uint16_t *utf16_string,
     size_t utf16_string_size,
     size_t *utf16_string_index,
     uint64_t floating_point_value,
     size_t floating_point_value_size,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	char value_string[ 16 ];

	byte_stream_float32_t value_float32;
	byte_stream_float64_t value_float64;

	static char *function        = "libfvalue_utf16_string_with_index_copy_from_floating_point";
	size_t string_index          = 0;
	uint64_t divider             = 0;
	uint64_t multiplier          = 0;
	uint64_t value_64bit         = 0;
	uint32_t string_format_type  = 0;
	uint32_t supported_flags     = 0;
	uint16_t exponent_divider    = 0;
	int16_t exponent             = 0;
	int16_t exponent_value       = 0;
	uint8_t byte_value           = 0;
	uint8_t is_denormalized      = 0;
	uint8_t is_indeterminate     = 0;
	uint8_t is_infinite          = 0;
	uint8_t is_not_a_number      = 0;
	uint8_t is_signed            = 0;
	uint8_t is_zero              = 0;
	uint8_t number_of_characters = 0;
	uint8_t use_value_string     = 0;
	int8_t bit_shift             = 0;
	int print_count              = 0;

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
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid UTF-16 string index value out of bounds.",
		 function );

		return( -1 );
	}
	string_index = *utf16_string_index;

	if( ( floating_point_value_size != 32 )
	 && ( floating_point_value_size != 64 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported floating point value size.",
		 function );

		return( -1 );
	}
	supported_flags = 0x000000ffUL;

	if( ( string_format_flags & ~( supported_flags ) ) != 0 )
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
	string_format_type = string_format_flags & 0x000000ffUL;

	if( ( string_format_type != LIBFVALUE_FLOATING_POINT_FORMAT_TYPE_DECIMAL )
	 && ( string_format_type != LIBFVALUE_FLOATING_POINT_FORMAT_TYPE_HEXADECIMAL ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string format type.",
		 function );

		return( -1 );
	}
	if( string_format_type == LIBFVALUE_FLOATING_POINT_FORMAT_TYPE_HEXADECIMAL )
	{
		number_of_characters = (uint8_t) ( floating_point_value_size >> 2 ) + 3;
	}
	else
	{
		bit_shift = (uint8_t) ( floating_point_value_size - 1 );
		is_signed = (uint8_t) ( floating_point_value >> bit_shift );

		if( is_signed != 0 )
		{
			floating_point_value &= ~( (uint64_t) 1 << bit_shift );
		}
		switch( floating_point_value_size )
		{
			case 32:
				if( floating_point_value == 0x7f800000UL )
				{
					is_infinite = 1;
				}
				else if( ( is_signed != 0 )
				      && ( floating_point_value == 0x7fc00000UL ) )
				{
					is_indeterminate = 1;
				}
				else if( ( floating_point_value >= 0x7f800001UL )
				      && ( floating_point_value <= 0x7fffffffUL ) )
				{
					is_not_a_number = 1;
				}
				else if( floating_point_value != 0 )
				{
					value_float32.integer = (uint32_t) floating_point_value;

					print_count = libcstring_narrow_string_snprintf(
					               value_string,
					               16,
					               "%06e",
					               value_float32.floating_point );

					if( ( print_count < 0 )
					 || ( print_count > 16 ) )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
						 "%s: unable to print floating point value to string.",
						 function );

						return( -1 );
					}
					exponent = (int16_t) ( floating_point_value >> 23 );

					if( exponent == 0 )
					{
						is_denormalized = 1;

						exponent = -126;
					}
					else
					{
						exponent -= 127;
					}
					floating_point_value &= 0x007fffffUL;
					bit_shift             = 23;
				}
				else
				{
					is_zero = 1;
				}
				break;

			case 64:
				if( floating_point_value == 0x7ff0000000000000ULL )
				{
					is_infinite = 1;
				}
				else if( ( is_signed != 0 )
				      && ( floating_point_value == 0x7ff8000000000000ULL ) )
				{
					is_indeterminate = 1;
				}
				else if( ( floating_point_value >= 0x7ff0000000000001ULL )
				      && ( floating_point_value <= 0x7fffffffffffffffULL ) )
				{
					is_not_a_number = 1;
				}
				else if( floating_point_value != 0 )
				{
					value_float64.integer = floating_point_value;

					print_count = libcstring_narrow_string_snprintf(
					               value_string,
					               16,
					               "%06e",
					               value_float64.floating_point );

					if( ( print_count < 0 )
					 || ( print_count > 16 ) )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
						 "%s: unable to print floating point value to string.",
						 function );

						return( -1 );
					}
					exponent = (int16_t) ( floating_point_value >> 52 );

					if( exponent == 0 )
					{
						is_denormalized = 1;

						exponent = -1023;
					}
					else
					{
						exponent -= 1023;
					}
					floating_point_value &= 0x000fffffffffffffULL;
					bit_shift             = 52;
				}
				else
				{
					is_zero = 1;
				}
				break;
		}
		/* The string is at least a single digit with an end of string character
		 */
		number_of_characters = 2;

		if( is_signed != 0 )
		{
			number_of_characters += 1;
		}
		if( is_indeterminate != 0 )
		{
			number_of_characters += 3;
		}
		else if( is_infinite != 0 )
		{
			number_of_characters += 3;
		}
		else if( is_not_a_number != 0 )
		{
			number_of_characters += 3;
		}
		else if( is_zero == 0 )
		{
			if( is_denormalized != 0 )
			{
				while( bit_shift >= 0 )
				{
					if( ( ( floating_point_value >> bit_shift ) & 0x01 ) != 0 )
					{
						break;
					}
					bit_shift--;
					exponent--;
				}
				use_value_string = 1;
			}
			else
			{
				if( floating_point_value != 0 )
				{
					use_value_string = 1;
				}
				if( exponent != 0 )
				{
					use_value_string = 1;
				}
				floating_point_value |= (uint64_t) 1 << bit_shift;
			}
			if( use_value_string == 0 )
			{
				multiplier = 1;

				while( bit_shift >= 0 )
				{
					value_64bit *= 10;
					value_64bit += ( ( floating_point_value >> bit_shift ) & 0x01 ) * multiplier;
					multiplier  *= 5;

					floating_point_value ^= (uint64_t) 1 << bit_shift;

					if( floating_point_value == 0 )
					{
						break;
					}
					if( multiplier > 15625 )
					{
						use_value_string = 1;

						break;
					}
					bit_shift--;
				}
				floating_point_value = value_64bit;
			}
			if( use_value_string != 0 )
			{
				number_of_characters += 12;
			}
			else
			{
				divider = 1;

				while( ( floating_point_value / divider ) >= 10 )
				{
					divider *= 10;

					number_of_characters += 1;
				}
				exponent_value = exponent;

				if( exponent_value != 0 )
				{
					if( exponent_value < 0 )
					{
						exponent_value *= -1;
					}
					exponent_divider = 1;

					while( ( exponent_value / exponent_divider ) >= 10 )
					{
						exponent_divider *= 10;

						number_of_characters += 1;
					}
					number_of_characters += 3;
				}
			}
		}
	}
	if( ( string_index + number_of_characters ) > utf16_string_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-16 string size too small.",
		 function );

		return( -1 );
	}
	if( string_format_type == LIBFVALUE_FLOATING_POINT_FORMAT_TYPE_HEXADECIMAL )
	{
		utf16_string[ string_index++ ] = (uint16_t) '0';
		utf16_string[ string_index++ ] = (uint16_t) 'x';

		bit_shift = (uint8_t) ( floating_point_value_size - 4 );

		do
		{
			byte_value = (uint8_t) ( ( floating_point_value >> bit_shift ) & 0x0f );

			if( byte_value <= 9 )
			{
				utf16_string[ string_index++ ] = (uint16_t) '0' + byte_value;
			}
			else
			{
				utf16_string[ string_index++ ] = (uint16_t) 'a' + byte_value - 10;
			}
			bit_shift -= 4;
		}
		while( bit_shift >= 0 );
	}
	else
	{
		if( is_signed != 0 )
		{
			utf16_string[ string_index++ ] = (uint16_t) '-';
		}
		if( is_indeterminate != 0 )
		{
			utf16_string[ string_index++ ] = (uint16_t) 'I';
			utf16_string[ string_index++ ] = (uint16_t) 'n';
			utf16_string[ string_index++ ] = (uint16_t) 'd';
		}
		else if( is_infinite != 0 )
		{
			utf16_string[ string_index++ ] = (uint16_t) 'I';
			utf16_string[ string_index++ ] = (uint16_t) 'n';
			utf16_string[ string_index++ ] = (uint16_t) 'f';
		}
		else if( is_not_a_number != 0 )
		{
			utf16_string[ string_index++ ] = (uint16_t) 'N';
			utf16_string[ string_index++ ] = (uint16_t) 'a';
			utf16_string[ string_index++ ] = (uint16_t) 'N';
		}
		else if( is_zero == 0 )
		{
			if( use_value_string != 0 )
			{
				utf16_string[ string_index++ ] = (uint16_t) value_string[ 0 ];
				utf16_string[ string_index++ ] = (uint16_t) value_string[ 1 ];
				utf16_string[ string_index++ ] = (uint16_t) value_string[ 2 ];
				utf16_string[ string_index++ ] = (uint16_t) value_string[ 3 ];
				utf16_string[ string_index++ ] = (uint16_t) value_string[ 4 ];
				utf16_string[ string_index++ ] = (uint16_t) value_string[ 5 ];
				utf16_string[ string_index++ ] = (uint16_t) value_string[ 6 ];
				utf16_string[ string_index++ ] = (uint16_t) value_string[ 7 ];
				utf16_string[ string_index++ ] = (uint16_t) value_string[ 8 ];
				utf16_string[ string_index++ ] = (uint16_t) value_string[ 9 ];
				utf16_string[ string_index++ ] = (uint16_t) value_string[ 10 ];
				utf16_string[ string_index++ ] = (uint16_t) value_string[ 11 ];
			}
			else
			{
				if( divider > 1 )
				{
					utf16_string[ string_index++ ] = (uint16_t) '0' + (uint16_t) ( floating_point_value / divider );

					floating_point_value %= divider;

					divider /= 10;

					utf16_string[ string_index++ ] = (uint16_t) '.';

					while( divider > 1 )
					{
						utf16_string[ string_index++ ] = (uint16_t) '0' + (uint16_t) ( floating_point_value / divider );

						floating_point_value %= divider;

						divider /= 10;
					}
				}
				utf16_string[ string_index++ ] = (uint16_t) '0' + (uint16_t) ( floating_point_value / divider );

				if( exponent != 0 )
				{
					utf16_string[ string_index++ ] = (uint16_t) 'e';

					if( exponent < 0 )
					{
						exponent *= -1;

						utf16_string[ string_index++ ] = (uint16_t) '-';
					}
					else
					{
						utf16_string[ string_index++ ] = (uint16_t) '+';
					}
					while( exponent_divider > 1 )
					{
						utf16_string[ string_index++ ] = (uint16_t) '0' + (uint16_t) ( exponent / exponent_divider );

						exponent %= exponent_divider;

						exponent_divider /= 10;
					}
					utf16_string[ string_index++ ] = (uint16_t) '0' + (uint16_t) ( exponent / exponent_divider );
				}
			}
		}
		else
		{
			utf16_string[ string_index++ ] = (uint16_t) '0';
		}
	}
	utf16_string[ string_index++ ] = 0;

	*utf16_string_index = string_index;

	return( 1 );
}

/* Copies an UTF-16 encoded string to a floating point value
 * The floating_point value size is in bits
 * Returns 1 if successful or -1 on error
 */
int libfvalue_utf16_string_with_index_copy_to_floating_point(
     uint16_t *utf16_string,
     size_t utf16_string_length,
     size_t *utf16_string_index,
     uint64_t *floating_point_value,
     size_t floating_point_value_size,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	static char *function       = "libfvalue_utf16_string_with_index_copy_to_floating_point";
	size_t maximum_string_index = 0;
	size_t string_index         = 0;
	uint64_t value_64bit        = 0;
	uint64_t divider            = 0;
	uint32_t string_format_type = 0;
	uint32_t supported_flags    = 0;
	uint8_t byte_value          = 0;
	uint8_t character_value     = 0;
	int8_t bit_shift            = 0;
	int8_t sign                 = 1;

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
	if( utf16_string_length > (size_t) SSIZE_MAX )
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
	if( *utf16_string_index >= utf16_string_length )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid UTF-16 string index value out of bounds.",
		 function );

		return( -1 );
	}
	string_index = *utf16_string_index;

	if( floating_point_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid floating point value.",
		 function );

		return( -1 );
	}
	if( ( floating_point_value_size != 32 )
	 && ( floating_point_value_size != 64 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported floating point value size.",
		 function );

		return( -1 );
	}
	supported_flags = 0x000000ffUL;

	if( ( string_format_flags & ~( supported_flags ) ) != 0 )
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
	string_format_type = string_format_flags & 0x000000ffUL;

	if( ( string_format_type != LIBFVALUE_FLOATING_POINT_FORMAT_TYPE_DECIMAL )
	 && ( string_format_type != LIBFVALUE_FLOATING_POINT_FORMAT_TYPE_HEXADECIMAL ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string format type.",
		 function );

		return( -1 );
	}
	if( string_format_type == LIBFVALUE_FLOATING_POINT_FORMAT_TYPE_HEXADECIMAL )
	{
		maximum_string_index = (size_t) ( floating_point_value_size >> 2 ) + 3;
	}
	else
	{
		/* The string is at least a single digit with an end of string character
		 */
		maximum_string_index = 2;

		bit_shift = (uint8_t) ( floating_point_value_size - 1 );

		divider = 1;

		value_64bit = ~( ( ~( (uint64_t) 1 << bit_shift ) >> bit_shift ) << bit_shift );

		while( ( value_64bit / divider ) >= 10 )
		{
			divider *= 10;

			maximum_string_index += 1;
		}
	}
	maximum_string_index += string_index;

	if( maximum_string_index > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid maximum string index value exceeds maximum.",
		 function );

		return( -1 );
	}
	value_64bit = 0;

	if( string_format_type == LIBFVALUE_FLOATING_POINT_FORMAT_TYPE_HEXADECIMAL )
	{
		if( utf16_string[ string_index ] != (uint16_t) '0' )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported character value: 0x04%" PRIx16 " at index: %d.",
			 function,
			 utf16_string[ string_index ],
			 string_index );

			return( -1 );
		}
		string_index++;

		if( utf16_string[ string_index ] != (uint16_t) 'x' )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported character value: 0x04%" PRIx16 " at index: %d.",
			 function,
			 utf16_string[ string_index ],
			 string_index );

			return( -1 );
		}
		string_index++;

		while( string_index < utf16_string_length )
		{
			if( utf16_string[ string_index ] == 0 )
			{
				break;
			}
			if( string_index > (size_t) maximum_string_index )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_LARGE,
				 "%s: string too large.",
				 function );

				return( -1 );
			}
			value_64bit <<= 4;

			if( ( utf16_string[ string_index ] >= (uint16_t) '0' )
			 && ( utf16_string[ string_index ] <= (uint16_t) '9' ) )
			{
				byte_value = (uint8_t) ( utf16_string[ string_index ] - (uint16_t) '0' );
			}
			else if( ( utf16_string[ string_index ] >= (uint16_t) 'A' )
			      && ( utf16_string[ string_index ] <= (uint16_t) 'F' ) )
			{
				byte_value = (uint8_t) ( utf16_string[ string_index ] - (uint16_t) 'A' + 10 );
			}
			else if( ( utf16_string[ string_index ] >= (uint16_t) 'a' )
			      && ( utf16_string[ string_index ] <= (uint16_t) 'f' ) )
			{
				byte_value = (uint8_t) ( utf16_string[ string_index ] - (uint16_t) 'a' + 10 );
			}
			else
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported character value: 0x04%" PRIx16 " at index: %d.",
				 function,
				 utf16_string[ string_index ],
				 string_index );

				return( -1 );
			}
			value_64bit += byte_value;

			string_index++;
		}
	}
	else
	{
		/* In the maximum possible string one character is substituted for the sign
		 */
		if( utf16_string[ string_index ] == (uint16_t) '-' )
		{
			string_index++;

			sign = -1;
		}
		else if( utf16_string[ string_index ] == (uint16_t) '+' )
		{
			string_index++;
		}
		while( string_index < utf16_string_length )
		{
			if( utf16_string[ string_index ] == 0 )
			{
				break;
			}
			if( string_index > (size_t) maximum_string_index )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_LARGE,
				 "%s: string too large.",
				 function );

				return( -1 );
			}
			value_64bit *= 10;

			if( ( utf16_string[ string_index ] < (uint16_t) '0' )
			 && ( utf16_string[ string_index ] > (uint16_t) '9' ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported character value: 0x04%" PRIx16 " at index: %d.",
				 function,
				 utf16_string[ string_index ],
				 string_index );

				return( -1 );
			}
			character_value = (uint8_t) ( utf16_string[ string_index ] - (uint16_t) '0' );

			value_64bit += character_value;

			string_index++;
		}
		if( sign == -1 )
		{
			value_64bit |= (uint64_t) 1 << bit_shift;
		}
	}
	*utf16_string_index = string_index;
	*floating_point_value      = value_64bit;

	return( 1 );
}

/* Copies an UTF-32 encoded string of a floating point value
 * The floating_point value size is in bits
 * Returns 1 if successful or -1 on error
 */
int libfvalue_utf32_string_copy_from_floating_point(
     uint32_t *utf32_string,
     size_t utf32_string_size,
     uint64_t floating_point_value,
     size_t floating_point_value_size,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	static char *function     = "libfvalue_utf32_string_copy_from_floating_point";
	size_t utf32_string_index = 0;

	if( libfvalue_utf32_string_with_index_copy_from_floating_point(
	     utf32_string,
	     utf32_string_size,
	     &utf32_string_index,
	     floating_point_value,
	     floating_point_value_size,
	     string_format_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy floating point value to UTF-32 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Copies an UTF-32 encoded string of from floating_point value
 * The floating_point value size is in bits
 * Returns 1 if successful or -1 on error
 */
int libfvalue_utf32_string_with_index_copy_from_floating_point(
     uint32_t *utf32_string,
     size_t utf32_string_size,
     size_t *utf32_string_index,
     uint64_t floating_point_value,
     size_t floating_point_value_size,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	char value_string[ 16 ];

	byte_stream_float32_t value_float32;
	byte_stream_float64_t value_float64;

	static char *function        = "libfvalue_utf32_string_with_index_copy_from_floating_point";
	size_t string_index          = 0;
	uint64_t divider             = 0;
	uint64_t multiplier          = 0;
	uint64_t value_64bit         = 0;
	uint32_t string_format_type  = 0;
	uint32_t supported_flags     = 0;
	uint16_t exponent_divider    = 0;
	int16_t exponent             = 0;
	int16_t exponent_value       = 0;
	uint8_t byte_value           = 0;
	uint8_t is_denormalized      = 0;
	uint8_t is_indeterminate     = 0;
	uint8_t is_infinite          = 0;
	uint8_t is_not_a_number      = 0;
	uint8_t is_signed            = 0;
	uint8_t is_zero              = 0;
	uint8_t number_of_characters = 0;
	uint8_t use_value_string     = 0;
	int8_t bit_shift             = 0;
	int print_count              = 0;

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
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid UTF-32 string index value out of bounds.",
		 function );

		return( -1 );
	}
	string_index = *utf32_string_index;

	if( ( floating_point_value_size != 32 )
	 && ( floating_point_value_size != 64 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported floating point value size.",
		 function );

		return( -1 );
	}
	supported_flags = 0x000000ffUL;

	if( ( string_format_flags & ~( supported_flags ) ) != 0 )
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
	string_format_type = string_format_flags & 0x000000ffUL;

	if( ( string_format_type != LIBFVALUE_FLOATING_POINT_FORMAT_TYPE_DECIMAL )
	 && ( string_format_type != LIBFVALUE_FLOATING_POINT_FORMAT_TYPE_HEXADECIMAL ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string format type.",
		 function );

		return( -1 );
	}
	if( string_format_type == LIBFVALUE_FLOATING_POINT_FORMAT_TYPE_HEXADECIMAL )
	{
		number_of_characters = (uint8_t) ( floating_point_value_size >> 2 ) + 3;
	}
	else
	{
		bit_shift = (uint8_t) ( floating_point_value_size - 1 );
		is_signed = (uint8_t) ( floating_point_value >> bit_shift );

		if( is_signed != 0 )
		{
			floating_point_value &= ~( (uint64_t) 1 << bit_shift );
		}
		switch( floating_point_value_size )
		{
			case 32:
				if( floating_point_value == 0x7f800000UL )
				{
					is_infinite = 1;
				}
				else if( ( is_signed != 0 )
				      && ( floating_point_value == 0x7fc00000UL ) )
				{
					is_indeterminate = 1;
				}
				else if( ( floating_point_value >= 0x7f800001UL )
				      && ( floating_point_value <= 0x7fffffffUL ) )
				{
					is_not_a_number = 1;
				}
				else if( floating_point_value != 0 )
				{
					value_float32.integer = (uint32_t) floating_point_value;

					print_count = libcstring_narrow_string_snprintf(
					               value_string,
					               16,
					               "%06e",
					               value_float32.floating_point );

					if( ( print_count < 0 )
					 || ( print_count > 16 ) )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
						 "%s: unable to print floating point value to string.",
						 function );

						return( -1 );
					}
					exponent = (int16_t) ( floating_point_value >> 23 );

					if( exponent == 0 )
					{
						is_denormalized = 1;

						exponent = -126;
					}
					else
					{
						exponent -= 127;
					}
					floating_point_value &= 0x007fffffUL;
					bit_shift             = 23;
				}
				else
				{
					is_zero = 1;
				}
				break;

			case 64:
				if( floating_point_value == 0x7ff0000000000000ULL )
				{
					is_infinite = 1;
				}
				else if( ( is_signed != 0 )
				      && ( floating_point_value == 0x7ff8000000000000ULL ) )
				{
					is_indeterminate = 1;
				}
				else if( ( floating_point_value >= 0x7ff0000000000001ULL )
				      && ( floating_point_value <= 0x7fffffffffffffffULL ) )
				{
					is_not_a_number = 1;
				}
				else if( floating_point_value != 0 )
				{
					value_float64.integer = floating_point_value;

					print_count = libcstring_narrow_string_snprintf(
					               value_string,
					               16,
					               "%06e",
					               value_float64.floating_point );

					if( ( print_count < 0 )
					 || ( print_count > 16 ) )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
						 "%s: unable to print floating point value to string.",
						 function );

						return( -1 );
					}
					exponent = (int16_t) ( floating_point_value >> 52 );

					if( exponent == 0 )
					{
						is_denormalized = 1;

						exponent = -1023;
					}
					else
					{
						exponent -= 1023;
					}
					floating_point_value &= 0x000fffffffffffffULL;
					bit_shift             = 52;
				}
				else
				{
					is_zero = 1;
				}
				break;
		}
		/* The string is at least a single digit with an end of string character
		 */
		number_of_characters = 2;

		if( is_signed != 0 )
		{
			number_of_characters += 1;
		}
		if( is_indeterminate != 0 )
		{
			number_of_characters += 3;
		}
		else if( is_infinite != 0 )
		{
			number_of_characters += 3;
		}
		else if( is_not_a_number != 0 )
		{
			number_of_characters += 3;
		}
		else if( is_zero == 0 )
		{
			if( is_denormalized != 0 )
			{
				while( bit_shift >= 0 )
				{
					if( ( ( floating_point_value >> bit_shift ) & 0x01 ) != 0 )
					{
						break;
					}
					bit_shift--;
					exponent--;
				}
				use_value_string = 1;
			}
			else
			{
				if( floating_point_value != 0 )
				{
					use_value_string = 1;
				}
				if( exponent != 0 )
				{
					use_value_string = 1;
				}
				floating_point_value |= (uint64_t) 1 << bit_shift;
			}
			if( use_value_string == 0 )
			{
				multiplier = 1;

				while( bit_shift >= 0 )
				{
					value_64bit *= 10;
					value_64bit += ( ( floating_point_value >> bit_shift ) & 0x01 ) * multiplier;
					multiplier  *= 5;

					floating_point_value ^= (uint64_t) 1 << bit_shift;

					if( floating_point_value == 0 )
					{
						break;
					}
					if( multiplier > 15625 )
					{
						use_value_string = 1;

						break;
					}
					bit_shift--;
				}
				floating_point_value = value_64bit;
			}
			if( use_value_string != 0 )
			{
				number_of_characters += 12;
			}
			else
			{
				divider = 1;

				while( ( floating_point_value / divider ) >= 10 )
				{
					divider *= 10;

					number_of_characters += 1;
				}
				exponent_value = exponent;

				if( exponent_value != 0 )
				{
					if( exponent_value < 0 )
					{
						exponent_value *= -1;
					}
					exponent_divider = 1;

					while( ( exponent_value / exponent_divider ) >= 10 )
					{
						exponent_divider *= 10;

						number_of_characters += 1;
					}
					number_of_characters += 3;
				}
			}
		}
	}
	if( ( string_index + number_of_characters ) > utf32_string_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-32 string size too small.",
		 function );

		return( -1 );
	}
	if( string_format_type == LIBFVALUE_FLOATING_POINT_FORMAT_TYPE_HEXADECIMAL )
	{
		utf32_string[ string_index++ ] = (uint32_t) '0';
		utf32_string[ string_index++ ] = (uint32_t) 'x';

		bit_shift = (uint8_t) ( floating_point_value_size - 4 );

		do
		{
			byte_value = (uint8_t) ( ( floating_point_value >> bit_shift ) & 0x0f );

			if( byte_value <= 9 )
			{
				utf32_string[ string_index++ ] = (uint32_t) '0' + byte_value;
			}
			else
			{
				utf32_string[ string_index++ ] = (uint32_t) 'a' + byte_value - 10;
			}
			bit_shift -= 4;
		}
		while( bit_shift >= 0 );
	}
	else
	{
		if( is_signed != 0 )
		{
			utf32_string[ string_index++ ] = (uint32_t) '-';
		}
		if( is_indeterminate != 0 )
		{
			utf32_string[ string_index++ ] = (uint32_t) 'I';
			utf32_string[ string_index++ ] = (uint32_t) 'n';
			utf32_string[ string_index++ ] = (uint32_t) 'd';
		}
		else if( is_infinite != 0 )
		{
			utf32_string[ string_index++ ] = (uint32_t) 'I';
			utf32_string[ string_index++ ] = (uint32_t) 'n';
			utf32_string[ string_index++ ] = (uint32_t) 'f';
		}
		else if( is_not_a_number != 0 )
		{
			utf32_string[ string_index++ ] = (uint32_t) 'N';
			utf32_string[ string_index++ ] = (uint32_t) 'a';
			utf32_string[ string_index++ ] = (uint32_t) 'N';
		}
		else if( is_zero == 0 )
		{
			if( use_value_string != 0 )
			{
				utf32_string[ string_index++ ] = (uint32_t) value_string[ 0 ];
				utf32_string[ string_index++ ] = (uint32_t) value_string[ 1 ];
				utf32_string[ string_index++ ] = (uint32_t) value_string[ 2 ];
				utf32_string[ string_index++ ] = (uint32_t) value_string[ 3 ];
				utf32_string[ string_index++ ] = (uint32_t) value_string[ 4 ];
				utf32_string[ string_index++ ] = (uint32_t) value_string[ 5 ];
				utf32_string[ string_index++ ] = (uint32_t) value_string[ 6 ];
				utf32_string[ string_index++ ] = (uint32_t) value_string[ 7 ];
				utf32_string[ string_index++ ] = (uint32_t) value_string[ 8 ];
				utf32_string[ string_index++ ] = (uint32_t) value_string[ 9 ];
				utf32_string[ string_index++ ] = (uint32_t) value_string[ 10 ];
				utf32_string[ string_index++ ] = (uint32_t) value_string[ 11 ];
			}
			else
			{
				if( divider > 1 )
				{
					utf32_string[ string_index++ ] = (uint32_t) '0' + (uint32_t) ( floating_point_value / divider );

					floating_point_value %= divider;

					divider /= 10;

					utf32_string[ string_index++ ] = (uint32_t) '.';

					while( divider > 1 )
					{
						utf32_string[ string_index++ ] = (uint32_t) '0' + (uint32_t) ( floating_point_value / divider );

						floating_point_value %= divider;

						divider /= 10;
					}
				}
				utf32_string[ string_index++ ] = (uint32_t) '0' + (uint32_t) ( floating_point_value / divider );

				if( exponent != 0 )
				{
					utf32_string[ string_index++ ] = (uint32_t) 'e';

					if( exponent < 0 )
					{
						exponent *= -1;

						utf32_string[ string_index++ ] = (uint32_t) '-';
					}
					else
					{
						utf32_string[ string_index++ ] = (uint32_t) '+';
					}
					while( exponent_divider > 1 )
					{
						utf32_string[ string_index++ ] = (uint32_t) '0' + (uint32_t) ( exponent / exponent_divider );

						exponent %= exponent_divider;

						exponent_divider /= 10;
					}
					utf32_string[ string_index++ ] = (uint32_t) '0' + (uint32_t) ( exponent / exponent_divider );
				}
			}
		}
		else
		{
			utf32_string[ string_index++ ] = (uint32_t) '0';
		}
	}
	utf32_string[ string_index++ ] = 0;

	*utf32_string_index = string_index;

	return( 1 );
}

/* Copies an UTF-32 encoded string to a floating point value
 * The floating_point value size is in bits
 * Returns 1 if successful or -1 on error
 */
int libfvalue_utf32_string_with_index_copy_to_floating_point(
     uint32_t *utf32_string,
     size_t utf32_string_length,
     size_t *utf32_string_index,
     uint64_t *floating_point_value,
     size_t floating_point_value_size,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	static char *function       = "libfvalue_utf32_string_with_index_copy_to_floating_point";
	size_t maximum_string_index = 0;
	size_t string_index         = 0;
	uint64_t value_64bit        = 0;
	uint64_t divider            = 0;
	uint32_t string_format_type = 0;
	uint32_t supported_flags    = 0;
	uint8_t byte_value          = 0;
	uint8_t character_value     = 0;
	int8_t bit_shift            = 0;
	int8_t sign                 = 1;

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
	if( utf32_string_length > (size_t) SSIZE_MAX )
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
	if( *utf32_string_index >= utf32_string_length )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid UTF-32 string index value out of bounds.",
		 function );

		return( -1 );
	}
	string_index = *utf32_string_index;

	if( floating_point_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid floating point value.",
		 function );

		return( -1 );
	}
	if( ( floating_point_value_size != 32 )
	 && ( floating_point_value_size != 64 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported floating point value size.",
		 function );

		return( -1 );
	}
	supported_flags = 0x000000ffUL;

	if( ( string_format_flags & ~( supported_flags ) ) != 0 )
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
	string_format_type = string_format_flags & 0x000000ffUL;

	if( ( string_format_type != LIBFVALUE_FLOATING_POINT_FORMAT_TYPE_DECIMAL )
	 && ( string_format_type != LIBFVALUE_FLOATING_POINT_FORMAT_TYPE_HEXADECIMAL ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string format type.",
		 function );

		return( -1 );
	}
	if( string_format_type == LIBFVALUE_FLOATING_POINT_FORMAT_TYPE_HEXADECIMAL )
	{
		maximum_string_index = (size_t) ( floating_point_value_size >> 2 ) + 3;
	}
	else
	{
		/* The string is at least a single digit with an end of string character
		 */
		maximum_string_index = 2;

		bit_shift = (uint8_t) ( floating_point_value_size - 1 );

		divider = 1;

		value_64bit = ~( ( ~( (uint64_t) 1 << bit_shift ) >> bit_shift ) << bit_shift );

		while( ( value_64bit / divider ) >= 10 )
		{
			divider *= 10;

			maximum_string_index += 1;
		}
	}
	maximum_string_index += string_index;

	if( maximum_string_index > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid maximum string index value exceeds maximum.",
		 function );

		return( -1 );
	}
	value_64bit = 0;

	if( string_format_type == LIBFVALUE_FLOATING_POINT_FORMAT_TYPE_HEXADECIMAL )
	{
		if( utf32_string[ string_index ] != (uint32_t) '0' )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported character value: 0x08%" PRIx32 " at index: %d.",
			 function,
			 utf32_string[ string_index ],
			 string_index );

			return( -1 );
		}
		string_index++;

		if( utf32_string[ string_index ] != (uint32_t) 'x' )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported character value: 0x08%" PRIx32 " at index: %d.",
			 function,
			 utf32_string[ string_index ],
			 string_index );

			return( -1 );
		}
		string_index++;

		while( string_index < utf32_string_length )
		{
			if( utf32_string[ string_index ] == 0 )
			{
				break;
			}
			if( string_index > (size_t) maximum_string_index )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_LARGE,
				 "%s: string too large.",
				 function );

				return( -1 );
			}
			value_64bit <<= 4;

			if( ( utf32_string[ string_index ] >= (uint32_t) '0' )
			 && ( utf32_string[ string_index ] <= (uint32_t) '9' ) )
			{
				byte_value = (uint8_t) ( utf32_string[ string_index ] - (uint32_t) '0' );
			}
			else if( ( utf32_string[ string_index ] >= (uint32_t) 'A' )
			      && ( utf32_string[ string_index ] <= (uint32_t) 'F' ) )
			{
				byte_value = (uint8_t) ( utf32_string[ string_index ] - (uint32_t) 'A' + 10 );
			}
			else if( ( utf32_string[ string_index ] >= (uint32_t) 'a' )
			      && ( utf32_string[ string_index ] <= (uint32_t) 'f' ) )
			{
				byte_value = (uint8_t) ( utf32_string[ string_index ] - (uint32_t) 'a' + 10 );
			}
			else
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported character value: 0x08%" PRIx32 " at index: %d.",
				 function,
				 utf32_string[ string_index ],
				 string_index );

				return( -1 );
			}
			value_64bit += byte_value;

			string_index++;
		}
	}
	else
	{
		/* In the maximum possible string one character is substituted for the sign
		 */
		if( utf32_string[ string_index ] == (uint32_t) '-' )
		{
			string_index++;

			sign = -1;
		}
		else if( utf32_string[ string_index ] == (uint32_t) '+' )
		{
			string_index++;
		}
		while( string_index < utf32_string_length )
		{
			if( utf32_string[ string_index ] == 0 )
			{
				break;
			}
			if( string_index > (size_t) maximum_string_index )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_LARGE,
				 "%s: string too large.",
				 function );

				return( -1 );
			}
			value_64bit *= 10;

			if( ( utf32_string[ string_index ] < (uint32_t) '0' )
			 && ( utf32_string[ string_index ] > (uint32_t) '9' ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported character value: 0x08%" PRIx32 " at index: %d.",
				 function,
				 utf32_string[ string_index ],
				 string_index );

				return( -1 );
			}
			character_value = (uint8_t) ( utf32_string[ string_index ] - (uint32_t) '0' );

			value_64bit += character_value;

			string_index++;
		}
		if( sign == -1 )
		{
			value_64bit |= (uint64_t) 1 << bit_shift;
		}
	}
	*utf32_string_index = string_index;
	*floating_point_value      = value_64bit;

	return( 1 );
}

