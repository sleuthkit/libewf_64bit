/*
 * Array type functions
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

#if !defined( _LIBODRAW_ARRAY_TYPE_H )
#define _LIBODRAW_ARRAY_TYPE_H

#include <common.h>
#include <types.h>

#include "libodraw_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

/* The array comparison definitions
 */
enum LIBODRAW_ARRAY_COMPARE_DEFINITIONS
{
	/* The first value is less than the second value
	 */
        LIBODRAW_ARRAY_COMPARE_LESS,

	/* The first and second values are equal
	 */
        LIBODRAW_ARRAY_COMPARE_EQUAL,

	/* The first value is greater than the second value
	 */
        LIBODRAW_ARRAY_COMPARE_GREATER
};

/* The array insert flag definitions
 */
enum LIBODRAW_ARRAY_INSERT_FLAGS
{
	/* Allow duplicate entries
	 */
	LIBODRAW_ARRAY_INSERT_FLAG_NON_UNIQUE_ENTRIES	= 0x00,

	/* Only allow unique entries, no duplicates
	 */
	LIBODRAW_ARRAY_INSERT_FLAG_UNIQUE_ENTRIES	= 0x01,
};

typedef struct libodraw_array libodraw_array_t;

struct libodraw_array
{
	/* The number of allocated entries
	 */
	int number_of_allocated_entries;

	/* The number of entries
	 */
	int number_of_entries;

	/* The entries
	 */
	intptr_t **entries;
};

int libodraw_array_initialize(
     libodraw_array_t **array,
     int number_of_entries,
     libcerror_error_t **error );

int libodraw_array_free(
     libodraw_array_t **array,
     int (*entry_free_function)(
            intptr_t **entry,
            libcerror_error_t **error ),
     libcerror_error_t **error );

int libodraw_array_empty(
     libodraw_array_t *array,
     int (*entry_free_function)(
            intptr_t **entry,
            libcerror_error_t **error ),
     libcerror_error_t **error );

int libodraw_array_clear(
     libodraw_array_t *array,
     int (*entry_free_function)(
            intptr_t **entry,
            libcerror_error_t **error ),
     libcerror_error_t **error );

int libodraw_array_clone(
     libodraw_array_t **destination_array,
     libodraw_array_t *source_array,
     int (*entry_free_function)(
            intptr_t **entry,
            libcerror_error_t **error ),
     int (*entry_clone_function)(
            intptr_t **destination,
            intptr_t *source,
            libcerror_error_t **error ),
     libcerror_error_t **error );

int libodraw_array_resize(
     libodraw_array_t *array,
     int number_of_entries,
     int (*entry_free_function)(
            intptr_t **entry,
            libcerror_error_t **error ),
     libcerror_error_t **error );

int libodraw_array_get_number_of_entries(
     libodraw_array_t *array,
     int *number_of_entries,
     libcerror_error_t **error );

int libodraw_array_get_entry_by_index(
     libodraw_array_t *array,
     int entry_index,
     intptr_t **entry,
     libcerror_error_t **error );

int libodraw_array_set_entry_by_index(
     libodraw_array_t *array,
     int entry_index,
     intptr_t *entry,
     libcerror_error_t **error );

int libodraw_array_append_entry(
     libodraw_array_t *array,
     int *entry_index,
     intptr_t *entry,
     libcerror_error_t **error );

int libodraw_array_insert_entry(
     libodraw_array_t *array,
     int *entry_index,
     intptr_t *entry,
     int (*entry_compare_function)(
            intptr_t *first_entry,
            intptr_t *second_entry,
            libcerror_error_t **error ),
     uint8_t insert_flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

