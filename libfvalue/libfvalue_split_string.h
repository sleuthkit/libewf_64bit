/*
 * Split string functions
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

#if !defined( _LIBFVALUE_INTERNAL_SPLIT_STRING_H )
#define _LIBFVALUE_INTERNAL_SPLIT_STRING_H

#include <common.h>
#include <types.h>

#include "libfvalue_extern.h"
#include "libfvalue_libcerror.h"
#include "libfvalue_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfvalue_internal_split_utf8_string libfvalue_internal_split_utf8_string_t;

struct libfvalue_internal_split_utf8_string
{
	/* The number of string segments
	 */
	int number_of_segments;

	/* The string
	 */
	uint8_t *string;

	/* The string size
	 */
	size_t string_size;

	/* The segments
	 */
	uint8_t **segments;

	/* The segment sizes
	 */
	size_t *segment_sizes;
};

typedef struct libfvalue_internal_split_utf16_string libfvalue_internal_split_utf16_string_t;

struct libfvalue_internal_split_utf16_string
{
	/* The number of string segments
	 */
	int number_of_segments;

	/* The string
	 */
	uint16_t *string;

	/* The string size
	 */
	size_t string_size;

	/* The segments
	 */
	uint16_t **segments;

	/* The segment sizes
	 */
	size_t *segment_sizes;
};

int libfvalue_split_utf8_string_initialize(
     libfvalue_split_utf8_string_t **split_string,
     const uint8_t *utf8_string,
     size_t utf8_string_size,
     int number_of_segments,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_split_utf8_string_free(
     libfvalue_split_utf8_string_t **split_string,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_split_utf8_string_get_string(
     libfvalue_split_utf8_string_t *split_string,
     uint8_t **utf8_string,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_split_utf8_string_get_number_of_segments(
     libfvalue_split_utf8_string_t *split_string,
     int *number_of_segments,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_split_utf8_string_get_segment_by_index(
     libfvalue_split_utf8_string_t *split_string,
     int segment_index,
     uint8_t **utf8_string_segment,
     size_t *utf8_string_segment_size,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_split_utf8_string_set_segment_by_index(
     libfvalue_split_utf8_string_t *split_string,
     int segment_index,
     uint8_t *utf8_string_segment,
     size_t utf8_string_segment_size,
     libcerror_error_t **error );

int libfvalue_split_utf16_string_initialize(
     libfvalue_split_utf16_string_t **split_string,
     const uint16_t *utf16_string,
     size_t utf16_string_size,
     int number_of_segments,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_split_utf16_string_free(
     libfvalue_split_utf16_string_t **split_string,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_split_utf16_string_get_string(
     libfvalue_split_utf16_string_t *split_string,
     uint16_t **utf16_string,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_split_utf16_string_get_number_of_segments(
     libfvalue_split_utf16_string_t *split_string,
     int *number_of_segments,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_split_utf16_string_get_segment_by_index(
     libfvalue_split_utf16_string_t *split_string,
     int segment_index,
     uint16_t **utf16_string_segment,
     size_t *utf16_string_segment_size,
     libcerror_error_t **error );

LIBFVALUE_EXTERN \
int libfvalue_split_utf16_string_set_segment_by_index(
     libfvalue_split_utf16_string_t *split_string,
     int segment_index,
     uint16_t *utf16_string_segment,
     size_t utf16_string_segment_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

