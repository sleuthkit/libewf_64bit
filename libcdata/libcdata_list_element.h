/*
 * List element functions
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

#if !defined( _LIBCDATA_INTERNAL_LIST_ELEMENT_H )
#define _LIBCDATA_INTERNAL_LIST_ELEMENT_H

#include <common.h>
#include <types.h>

#include "libcdata_extern.h"
#include "libcdata_libcerror.h"
#include "libcdata_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libcdata_internal_list_element libcdata_internal_list_element_t;

struct libcdata_internal_list_element
{
	/* The previous list element
	 */
	libcdata_list_element_t *previous_element;

	/* The next list element
	 */
	libcdata_list_element_t *next_element;

	/* The value
	 */
	intptr_t *value;
};

LIBCDATA_EXTERN \
int libcdata_list_element_initialize(
     libcdata_list_element_t **element,
     libcerror_error_t **error );

LIBCDATA_EXTERN \
int libcdata_list_element_free(
     libcdata_list_element_t **element,
     int (*value_free_function)(
            intptr_t **value,
            libcerror_error_t **error ),
     libcerror_error_t **error );

LIBCDATA_EXTERN \
int libcdata_list_element_get_value(
     libcdata_list_element_t *element,
     intptr_t **value,
     libcerror_error_t **error );

LIBCDATA_EXTERN \
int libcdata_list_element_set_value(
     libcdata_list_element_t *element,
     intptr_t *value,
     libcerror_error_t **error );

LIBCDATA_EXTERN \
int libcdata_list_element_get_previous_element(
     libcdata_list_element_t *element,
     libcdata_list_element_t **previous_element,
     libcerror_error_t **error );

LIBCDATA_EXTERN \
int libcdata_list_element_set_previous_element(
     libcdata_list_element_t *element,
     libcdata_list_element_t *previous_element,
     libcerror_error_t **error );

LIBCDATA_EXTERN \
int libcdata_list_element_get_next_element(
     libcdata_list_element_t *element,
     libcdata_list_element_t **next_element,
     libcerror_error_t **error );

LIBCDATA_EXTERN \
int libcdata_list_element_set_next_element(
     libcdata_list_element_t *element,
     libcdata_list_element_t *next_element,
     libcerror_error_t **error );

LIBCDATA_EXTERN \
int libcdata_list_element_get_elements(
     libcdata_list_element_t *element,
     libcdata_list_element_t **previous_element,
     libcdata_list_element_t **next_element,
     libcerror_error_t **error );

LIBCDATA_EXTERN \
int libcdata_list_element_set_elements(
     libcdata_list_element_t *element,
     libcdata_list_element_t *previous_element,
     libcdata_list_element_t *next_element,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

