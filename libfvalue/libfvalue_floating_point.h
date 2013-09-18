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

#if !defined( _LIBFVALUE_FLOATING_POINT_H )
#define _LIBFVALUE_FLOATING_POINT_H

#include <common.h>
#include <types.h>

#include "libfvalue_extern.h"
#include "libfvalue_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfvalue_floating_point libfvalue_floating_point_t;

struct libfvalue_floating_point
{
	/* The value
	 */
	uint64_t value;

	/* The value size
	 */
	size_t value_size;
};

int libfvalue_floating_point_initialize(
     libfvalue_floating_point_t **floating_point,
     libcerror_error_t **error );

int libfvalue_floating_point_free(
     libfvalue_floating_point_t **floating_point,
     libcerror_error_t **error );

int libfvalue_floating_point_clone(
     libfvalue_floating_point_t **destination_floating_point,
     libfvalue_floating_point_t *source_floating_point,
     libcerror_error_t **error );

int libfvalue_floating_point_copy_from_byte_stream(
     libfvalue_floating_point_t *floating_point,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     int encoding,
     libcerror_error_t **error );

int libfvalue_floating_point_copy_from_integer(
     libfvalue_floating_point_t *floating_point,
     uint64_t integer_value,
     size_t integer_value_size,
     libcerror_error_t **error );

int libfvalue_floating_point_copy_to_integer(
     libfvalue_floating_point_t *floating_point,
     uint64_t *integer_value,
     size_t *integer_value_size,
     libcerror_error_t **error );

int libfvalue_floating_point_get_string_size(
     libfvalue_floating_point_t *floating_point,
     size_t *string_size,
     uint32_t string_format_flags,
     libcerror_error_t **error );

int libfvalue_floating_point_copy_from_utf8_string_with_index(
     libfvalue_floating_point_t *floating_point,
     uint8_t *utf8_string,
     size_t utf8_string_length,
     size_t *utf8_string_index,
     uint32_t string_format_flags,
     libcerror_error_t **error );

int libfvalue_floating_point_copy_to_utf8_string_with_index(
     libfvalue_floating_point_t *floating_point,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     size_t *utf8_string_index,
     uint32_t string_format_flags,
     libcerror_error_t **error );

int libfvalue_floating_point_copy_from_utf16_string_with_index(
     libfvalue_floating_point_t *floating_point,
     uint16_t *utf16_string,
     size_t utf16_string_length,
     size_t *utf16_string_index,
     uint32_t string_format_flags,
     libcerror_error_t **error );

int libfvalue_floating_point_copy_to_utf16_string_with_index(
     libfvalue_floating_point_t *floating_point,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     size_t *utf16_string_index,
     uint32_t string_format_flags,
     libcerror_error_t **error );

int libfvalue_floating_point_copy_from_utf32_string_with_index(
     libfvalue_floating_point_t *floating_point,
     uint32_t *utf32_string,
     size_t utf32_string_length,
     size_t *utf32_string_index,
     uint32_t string_format_flags,
     libcerror_error_t **error );

int libfvalue_floating_point_copy_to_utf32_string_with_index(
     libfvalue_floating_point_t *floating_point,
     uint32_t *utf32_string,
     size_t utf32_string_size,
     size_t *utf32_string_index,
     uint32_t string_format_flags,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_string_size_from_floating_point(
     size_t *string_size,
     uint64_t floating_point_value,
     size_t floating_point_value_size,
     uint32_t string_format_flags,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_utf8_string_copy_from_floating_point(
     uint8_t *utf8_string,
     size_t utf8_string_size,
     uint64_t floating_point_value,
     size_t floating_point_value_size,
     uint32_t string_format_flags,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_utf8_string_with_index_copy_from_floating_point(
     uint8_t *utf8_string,
     size_t utf8_string_size,
     size_t *utf8_string_index,
     uint64_t floating_point_value,
     size_t floating_point_value_size,
     uint32_t string_format_flags,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_utf8_string_with_index_copy_to_floating_point(
     uint8_t *utf8_string,
     size_t utf8_string_length,
     size_t *utf8_string_index,
     uint64_t *floating_point_value,
     size_t floating_point_value_size,
     uint32_t string_format_flags,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_utf16_string_copy_from_floating_point(
     uint16_t *utf16_string,
     size_t utf16_string_size,
     uint64_t floating_point_value,
     size_t floating_point_value_size,
     uint32_t string_format_flags,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_utf16_string_with_index_copy_from_floating_point(
     uint16_t *utf16_string,
     size_t utf16_string_size,
     size_t *utf16_string_index,
     uint64_t floating_point_value,
     size_t floating_point_value_size,
     uint32_t string_format_flags,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_utf16_string_with_index_copy_to_floating_point(
     uint16_t *utf16_string,
     size_t utf16_string_length,
     size_t *utf16_string_index,
     uint64_t *floating_point_value,
     size_t floating_point_value_size,
     uint32_t string_format_flags,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_utf32_string_copy_from_floating_point(
     uint32_t *utf32_string,
     size_t utf32_string_size,
     uint64_t floating_point_value,
     size_t floating_point_value_size,
     uint32_t string_format_flags,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_utf32_string_with_index_copy_from_floating_point(
     uint32_t *utf32_string,
     size_t utf32_string_size,
     size_t *utf32_string_index,
     uint64_t floating_point_value,
     size_t floating_point_value_size,
     uint32_t string_format_flags,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_utf32_string_with_index_copy_to_floating_point(
     uint32_t *utf32_string,
     size_t utf32_string_length,
     size_t *utf32_string_index,
     uint64_t *floating_point_value,
     size_t floating_point_value_size,
     uint32_t string_format_flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

