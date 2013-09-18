/*
 * Range list
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
#include <memory.h>
#include <types.h>

#include "libcdata_definitions.h"
#include "libcdata_libcerror.h"
#include "libcdata_list_element.h"
#include "libcdata_range_list.h"
#include "libcdata_types.h"

/* Creates an range list value
 * Returns 1 if successful or -1 on error
 */
int libcdata_range_list_value_initialize(
     libcdata_range_list_value_t **range_list_value,
     libcerror_error_t **error )
{
	static char *function = "libcdata_range_list_value_initialize";

	if( range_list_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list value.",
		 function );

		return( -1 );
	}
	if( *range_list_value != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid range list value value already set.",
		 function );

		return( -1 );
	}
	*range_list_value = memory_allocate_structure(
	                     libcdata_range_list_value_t );

	if( *range_list_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create range list value.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *range_list_value,
	     0,
	     sizeof( libcdata_range_list_value_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear range list value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *range_list_value != NULL )
	{
		memory_free(
		 *range_list_value );

		*range_list_value = NULL;
	}
	return( -1 );
}

/* Frees an range list value
 */
int libcdata_range_list_value_free(
     libcdata_range_list_value_t **range_list_value,
     libcerror_error_t **error )
{
	static char *function = "libcdata_range_list_value_free";

	if( range_list_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list value.",
		 function );

		return( -1 );
	}
	if( *range_list_value != NULL )
	{
		memory_free(
		 *range_list_value );

		*range_list_value = NULL;
	}
	return( 1 );
}

/* Clones the range list value
 * Returns 1 if successful or -1 on error
 */
int libcdata_range_list_value_clone(
     libcdata_range_list_value_t **destination_range_list_value,
     libcdata_range_list_value_t *source_range_list_value,
     libcerror_error_t **error )
{
	static char *function = "libcdata_range_list_value_clone";

	if( destination_range_list_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination range list value.",
		 function );

		return( -1 );
	}
	if( *destination_range_list_value != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination range list value value already set.",
		 function );

		return( -1 );
	}
	if( source_range_list_value == NULL )
	{
		*destination_range_list_value = NULL;

		return( 1 );
	}
	*destination_range_list_value = memory_allocate_structure(
	                                 libcdata_range_list_value_t );

	if( *destination_range_list_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination range list value.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_range_list_value,
	     source_range_list_value,
	     sizeof( libcdata_range_list_value_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy source to destination range list value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_range_list_value != NULL )
	{
		memory_free(
		 *destination_range_list_value );

		*destination_range_list_value = NULL;
	}
	return( -1 );
}

/* Creates an range list
 * Returns 1 if successful or -1 on error
 */
int libcdata_range_list_initialize(
     libcdata_range_list_t **range_list,
     libcerror_error_t **error )
{
	libcdata_internal_range_list_t *internal_range_list = NULL;
	static char *function                               = "libcdata_range_list_initialize";

	if( range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	if( *range_list != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid range list value already set.",
		 function );

		return( -1 );
	}
	internal_range_list = memory_allocate_structure(
	                       libcdata_internal_range_list_t );

	if( internal_range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create range list.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_range_list,
	     0,
	     sizeof( libcdata_internal_range_list_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear range list.",
		 function );

		goto on_error;
	}
	*range_list = (libcdata_range_list_t *) internal_range_list;

	return( 1 );

on_error:
	if( internal_range_list != NULL )
	{
		memory_free(
		 internal_range_list );
	}
	return( -1 );
}

/* Frees an range list including the elements
 * Returns 1 if successful or -1 on error
 */
int libcdata_range_list_free(
     libcdata_range_list_t **range_list,
     libcerror_error_t **error )
{
	static char *function = "libcdata_range_list_free";
	int result            = 1;

	if( range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	if( *range_list != NULL )
	{
		result = libcdata_range_list_empty(
		          *range_list,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to empty range list.",
			 function );
		}
		memory_free(
		 *range_list );

		*range_list = NULL;
	}
	return( result );
}

/* Empties an range list and frees the elements
 * Returns 1 if successful or -1 on error
 */
int libcdata_range_list_empty(
     libcdata_range_list_t *range_list,
     libcerror_error_t **error )
{
	libcdata_internal_range_list_t *internal_range_list = NULL;
	libcdata_list_element_t *list_element               = NULL;
	libcdata_list_element_t *next_element               = NULL;
	static char *function                               = "libcdata_range_list_empty";
	int element_index                                   = 0;
	int number_of_elements                              = 0;
	int result                                          = 1;

	if( range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	internal_range_list = (libcdata_internal_range_list_t *) range_list;

	if( internal_range_list->number_of_elements > 0 )
	{
		number_of_elements = internal_range_list->number_of_elements;

		list_element = internal_range_list->first_element;

		for( element_index = 0;
		     element_index < number_of_elements;
		     element_index++ )
		{
			if( libcdata_list_element_get_next_element(
			     list_element,
			     &next_element,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve next element from list element: %d.",
				 function,
				 element_index );

				return( -1 );
			}
			internal_range_list->first_element = next_element;

			if( internal_range_list->last_element == list_element )
			{
				internal_range_list->last_element = next_element;
			}
			internal_range_list->number_of_elements -= 1;

			if( next_element != NULL )
			{
				if( libcdata_list_element_set_previous_element(
				     next_element,
				     NULL,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set previous element of list element: %d.",
					 function,
					 element_index + 1 );

					return( -1 );
				}
			}
			if( libcdata_list_element_set_next_element(
			     list_element,
			     NULL,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set next element of list element: %d.",
				 function,
				 element_index );

				return( -1 );
			}
			if( libcdata_list_element_free(
			     &list_element,
			     (int (*)(intptr_t **, libcerror_error_t **)) &libcdata_range_list_value_free,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free element: %d.",
				 function,
				 element_index );

				result = -1;
			}
			list_element = next_element;
		}
		internal_range_list->current_element       = NULL;
		internal_range_list->current_element_index = 0;
	}
	return( result );
}

/* Clones the range list
 * Returns 1 if successful or -1 on error
 */
int libcdata_range_list_clone(
     libcdata_range_list_t **destination_range_list,
     libcdata_range_list_t *source_range_list,
     libcerror_error_t **error )
{
	libcdata_internal_range_list_t *source_internal_range_list = NULL;
	libcdata_list_element_t *source_list_element               = NULL;
	libcdata_range_list_value_t *destination_value             = NULL;
	libcdata_range_list_value_t *source_value                  = NULL;
	static char *function                                      = "libcdata_range_list_clone";
	int element_index                                          = 0;

	if( destination_range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination range list.",
		 function );

		return( -1 );
	}
	if( *destination_range_list != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination range list value already set.",
		 function );

		return( -1 );
	}
	if( source_range_list == NULL )
	{
		*destination_range_list = NULL;

		return( 1 );
	}
	source_internal_range_list = (libcdata_internal_range_list_t *) source_range_list;

	if( libcdata_range_list_initialize(
	     destination_range_list,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination range list.",
		 function );

		goto on_error;
	}
	if( *destination_range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing destination range list.",
		 function );

		goto on_error;
	}
	source_list_element = source_internal_range_list->first_element;

	for( element_index = 0;
	     element_index < source_internal_range_list->number_of_elements;
	     element_index++ )
	{
		if( libcdata_list_element_get_value(
		     source_list_element,
		     (intptr_t **) &source_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value from source list element: %d.",
			 function,
			 element_index );

			goto on_error;
		}
		if( libcdata_range_list_value_clone(
		     &destination_value,
		     source_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create destination range list value: %d.",
			 function,
			 element_index );

			goto on_error;
		}
		if( libcdata_range_list_append_value(
		     *destination_range_list,
		     destination_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to set value of destination element: %d.",
			 function,
			 element_index );

			goto on_error;
		}
		destination_value = NULL;

		if( libcdata_list_element_get_next_element(
		     source_list_element,
		     &source_list_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve next element from list element: %d.",
			 function,
			 element_index );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( destination_value != NULL )
	{
		libcdata_range_list_value_free(
		 &destination_value,
		 NULL );
	}
	if( *destination_range_list != NULL )
	{
		libcdata_range_list_free(
		 destination_range_list,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the number of elements in the range list
 * Returns 1 if successful or -1 on error
 */
int libcdata_range_list_get_number_of_elements(
     libcdata_range_list_t *range_list,
     int *number_of_elements,
     libcerror_error_t **error )
{
	libcdata_internal_range_list_t *internal_range_list = NULL;
	static char *function                               = "libcdata_range_list_get_number_of_elements";

	if( range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	internal_range_list = (libcdata_internal_range_list_t *) range_list;

	if( number_of_elements == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of elements.",
		 function );

		return( -1 );
	}
	*number_of_elements = internal_range_list->number_of_elements;

	return( 1 );
}

/* Sets the first element in the range list
 * Returns 1 if successful or -1 on error
 */
int libcdata_range_list_set_first_element(
     libcdata_range_list_t *range_list,
     libcdata_list_element_t *element,
     libcerror_error_t **error )
{
	libcdata_internal_range_list_t *internal_range_list = NULL;
	static char *function                               = "libcdata_range_list_set_first_element";

	if( range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	internal_range_list = (libcdata_internal_range_list_t *) range_list;

	if( element != NULL )
	{
		if( libcdata_list_element_set_next_element(
		     element,
		     internal_range_list->first_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set next element of list element.",
			 function );

			return( -1 );
		}
	}
	if( internal_range_list->first_element != NULL )
	{
		if( libcdata_list_element_set_previous_element(
		     internal_range_list->first_element,
		     element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set previous element of first element.",
			 function );

			return( -1 );
		}
	}
	internal_range_list->first_element = element;

	return( 1 );
}

/* Sets the last element in the list
 * Returns 1 if successful or -1 on error
 */
int libcdata_range_list_set_last_element(
     libcdata_range_list_t *range_list,
     libcdata_list_element_t *element,
     libcerror_error_t **error )
{
	libcdata_internal_range_list_t *internal_range_list = NULL;
	static char *function                               = "libcdata_list_set_last_element";

	if( range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	internal_range_list = (libcdata_internal_range_list_t *) range_list;

	if( element != NULL )
	{
		if( libcdata_list_element_set_previous_element(
		     element,
		     internal_range_list->last_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set previous element of list element.",
			 function );

			return( -1 );
		}
	}
	if( internal_range_list->last_element != NULL )
	{
		if( libcdata_list_element_set_next_element(
		     internal_range_list->last_element,
		     element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set next element of last element.",
			 function );

			return( -1 );
		}
	}
	internal_range_list->last_element = element;

	return( 1 );
}

/* Append a list element to the list
 * Returns 1 if successful or -1 on error
 */
int libcdata_range_list_append_element(
     libcdata_range_list_t *range_list,
     libcdata_list_element_t *element,
     libcerror_error_t **error )
{
	libcdata_internal_range_list_t *internal_range_list = NULL;
	static char *function                               = "libcdata_range_list_append_element";

	if( range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	internal_range_list = (libcdata_internal_range_list_t *) range_list;

	if( element == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list element.",
		 function );

		return( -1 );
	}
	if( internal_range_list->first_element == NULL )
	{
		internal_range_list->first_element = element;
	}
	if( libcdata_range_list_set_last_element(
	     range_list,
	     element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set last element.",
		 function );

		return( -1 );
	}
	internal_range_list->number_of_elements += 1;

	return( 1 );
}

/* Append a value to the list
 * Creates a new list element
 * Returns 1 if successful or -1 on error
 */
int libcdata_range_list_append_value(
     libcdata_range_list_t *range_list,
     libcdata_range_list_value_t *value,
     libcerror_error_t **error )
{
	libcdata_list_element_t *list_element = NULL;
	static char *function                 = "libcdata_range_list_append_value";

	if( libcdata_list_element_initialize(
	     &list_element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create list element.",
		 function );

		goto on_error;
	}
	if( libcdata_range_list_append_element(
	     range_list,
	     list_element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append element to list.",
		 function );

		goto on_error;
	}
	if( libcdata_list_element_set_value(
	     list_element,
	     (intptr_t *) value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to set value of list element.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( list_element != NULL )
	{
		libcdata_list_element_free(
		 &list_element,
		 NULL,
		 NULL );
	}
	return( -1 );
}

/* Appends a range
 * Returns 1 if successful, or -1 on error
 */
int libcdata_range_list_append_range(
     libcdata_range_list_t *range_list,
     uint64_t range_start,
     uint64_t range_size,
     libcerror_error_t **error )
{
	libcdata_internal_range_list_t *internal_range_list    = NULL;
	libcdata_list_element_t *last_element                  = NULL;
	libcdata_list_element_t *list_element                  = NULL;
	libcdata_list_element_t *next_element                  = NULL;
	libcdata_list_element_t *previous_element              = NULL;
	libcdata_range_list_value_t *next_range_list_value     = NULL;
	libcdata_range_list_value_t *range_list_value          = NULL;
	libcdata_range_list_value_t *previous_range_list_value = NULL;
	libcdata_range_list_value_t *split_range_list_value    = NULL;
	static char *function                                  = "libcdata_range_list_append_range";
	uint64_t range_end                                     = 0;
	int create_element                                     = 0;
	int element_index                                      = 0;
	int merge_next_element_check                           = 0;
	int merge_previous_element_check                       = 0;

	if( range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	internal_range_list = (libcdata_internal_range_list_t *) range_list;

	if( range_start > (uint64_t) INT64_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid range start value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( range_size > (uint64_t) INT64_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid range size value exceeds maximum.",
		 function );

		return( -1 );
	}
	range_end = range_start + range_size;

	if( range_end < range_start )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid range end value out of bounds.",
		 function );

		goto on_error;
	}
	create_element = 1;

	if( internal_range_list->number_of_elements > 0 )
	{
		/* Check the last element first, most often the list will be filled linear
		 */
		list_element  = internal_range_list->last_element;
		element_index = internal_range_list->number_of_elements - 1;

		if( libcdata_list_element_get_value(
		     list_element,
		     (intptr_t **) &range_list_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value from list element: %d.",
			 function,
			 element_index );

			goto on_error;
		}
		if( range_list_value == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing range list value element: %d.",
			 function,
			 element_index );

			goto on_error;
		}
		/* Check if the range overlaps at the end of the existing range
		 */
		if( ( range_start >= range_list_value->start )
		 && ( range_start <= range_list_value->end ) )
		{
			if( range_end > range_list_value->end )
			{
				range_list_value->size += range_end - range_list_value->end;
				range_list_value->end   = range_end;
			}
			create_element           = 0;
			merge_next_element_check = 1;
		}
		/* Check if the range overlaps at the beginning of the existing range
		 */
		else if( ( range_end >= range_list_value->start )
		      && ( range_end <= range_list_value->end ) )
		{
			if( range_start < range_list_value->start )
			{
				range_list_value->size += range_list_value->start - range_start;
				range_list_value->start = range_start;
			}
			create_element               = 0;
			merge_previous_element_check = 1;
		}
		/* Check if the range overlaps the existing range entirely
		 */
		else if( ( range_start < range_list_value->start )
		      && ( range_end > range_list_value->end ) )
		{
			range_list_value->start = range_start;
			range_list_value->size  = range_size;
			range_list_value->end   = range_end;

			create_element               = 0;
			merge_previous_element_check = 1;
		}
		/* Check if the range is after the existing range
		 */
		else if( range_end > range_list_value->end )
		{
			last_element = list_element;
		}
		else if( internal_range_list->number_of_elements > 1 )
		{
			if( range_end > ( range_list_value->end / 2 ) )
			{
				if( libcdata_list_element_get_previous_element(
				     list_element,
				     &list_element,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve previous element from list element: %d.",
					 function,
					 element_index );

					goto on_error;
				}
				for( element_index = ( internal_range_list->number_of_elements - 2 );
				     element_index >= 0;
				     element_index-- )
				{
					if( libcdata_list_element_get_value(
					     list_element,
					     (intptr_t **) &range_list_value,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
						 "%s: unable to retrieve value from list element: %d.",
						 function,
						 element_index );

						goto on_error;
					}
					if( range_list_value == NULL )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
						 "%s: missing range list value element: %d.",
						 function,
						 element_index );

						goto on_error;
					}
					/* Check if the range overlaps at the end of the existing range
					 */
					if( ( range_start >= range_list_value->start )
					 && ( range_start <= range_list_value->end ) )
					{
						if( range_end > range_list_value->end )
						{
							range_list_value->size += range_end - range_list_value->end;
							range_list_value->end   = range_end;
						}
						create_element           = 0;
						merge_next_element_check = 1;
					}
					/* Check if the range overlaps at the beginning of the existing range
					 */
					else if( ( range_end >= range_list_value->start )
					      && ( range_end <= range_list_value->end ) )
					{
						if( range_start < range_list_value->start )
						{
							range_list_value->size += range_list_value->start - range_start;
							range_list_value->start = range_start;
						}
						create_element               = 0;
						merge_previous_element_check = 1;
					}
					/* Check if the range overlaps the existing range entirely
					 */
					else if( ( range_start < range_list_value->start )
					      && ( range_end > range_list_value->end ) )
					{
						range_list_value->start = range_start;
						range_list_value->size  = range_size;
						range_list_value->end   = range_end;

						create_element               = 0;
						merge_next_element_check     = 1;
						merge_previous_element_check = 1;
					}
					if( create_element == 0 )
					{
						break;
					}
					/* Check if the range is after the existing range
					 */
					if( range_end > range_list_value->end )
					{
						last_element = list_element;

						break;
					}
					last_element = list_element;

					if( libcdata_list_element_get_previous_element(
					     list_element,
					     &list_element,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
						 "%s: unable to retrieve previous element from list element: %d.",
						 function,
						 element_index );

						goto on_error;
					}
				}
			}
			else
			{
				list_element = internal_range_list->first_element;

				for( element_index = 0;
				     element_index < ( internal_range_list->number_of_elements - 1 );
				     element_index++ )
				{
					if( libcdata_list_element_get_value(
					     list_element,
					     (intptr_t **) &range_list_value,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
						 "%s: unable to retrieve value from list element: %d.",
						 function,
						 element_index );

						goto on_error;
					}
					if( range_list_value == NULL )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
						 "%s: missing range list value element: %d.",
						 function,
						 element_index );

						goto on_error;
					}
					/* Check if the range overlaps at the end of the existing range
					 */
					if( ( range_start >= range_list_value->start )
					 && ( range_start <= range_list_value->end ) )
					{
						if( range_end > range_list_value->end )
						{
							range_list_value->size += range_end - range_list_value->end;
							range_list_value->end   = range_end;
						}
						create_element           = 0;
						merge_next_element_check = 1;
					}
					/* Check if the range overlaps at the beginning of the existing range
					 */
					else if( ( range_end >= range_list_value->start )
					      && ( range_end <= range_list_value->end ) )
					{
						if( range_start < range_list_value->start )
						{
							range_list_value->size += range_list_value->start - range_start;
							range_list_value->start = range_start;
						}
						create_element               = 0;
						merge_previous_element_check = 1;
					}
					/* Check if the range overlaps the existing range entirely
					 */
					else if( ( range_start < range_list_value->start )
					      && ( range_end > range_list_value->end ) )
					{
						range_list_value->start = range_start;
						range_list_value->size  = range_size;
						range_list_value->end   = range_end;

						create_element               = 0;
						merge_next_element_check     = 1;
						merge_previous_element_check = 1;
					}
					if( create_element == 0 )
					{
						break;
					}
					/* Check if the range belongs before the existing range
					 */
					if( range_end < range_list_value->end )
					{
						if( libcdata_list_element_get_previous_element(
						     list_element,
						     &last_element,
						     error ) != 1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
							 "%s: unable to retrieve previous element from list element: %d.",
							 function,
							 element_index );

							goto on_error;
						}
						break;
					}
					last_element = list_element;

					if( libcdata_list_element_get_next_element(
					     list_element,
					     &list_element,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
						 "%s: unable to retrieve next element from list element: %d.",
						 function,
						 element_index );

						goto on_error;
					}
				}
			}
		}
		/* Check if the current range should be merged with the previous range
		 */
		if( merge_previous_element_check != 0 )
		{
			if( libcdata_list_element_get_previous_element(
			     list_element,
			     &previous_element,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve previous element from list element: %d.",
				 function,
				 element_index );

				goto on_error;
			}
			if( previous_element != NULL )
			{
				if( libcdata_list_element_get_value(
				     previous_element,
				     (intptr_t **) &previous_range_list_value,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve value from list element: %d.",
					 function,
					 element_index - 1 );

					goto on_error;
				}
				if( previous_range_list_value == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
					 "%s: missing range list value element: %d.",
					 function,
					 element_index - 1 );

					goto on_error;
				}
				if( range_list_value->start <= previous_range_list_value->end )
				{
					/* Merge range with previous
					 */
					range_list_value->size += range_list_value->start - previous_range_list_value->start;
					range_list_value->start = previous_range_list_value->start;

					/* Remove previous list element
					 */
					if( libcdata_range_list_remove_element(
					     range_list,
					     previous_element,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_REMOVE_FAILED,
						 "%s: unable to remove list element: %d.",
						 function,
						 element_index - 1 );

						goto on_error;
					}
					if( libcdata_list_element_free(
					     &previous_element,
					     (int (*)(intptr_t **, libcerror_error_t **)) &libcdata_range_list_value_free,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
						 "%s: unable to free list element: %d.",
						 function,
						 element_index - 1 );

						goto on_error;
					}
				}
			}
		}
		/* Check if the current range should be merged with the next range
		 */
		if( merge_next_element_check != 0 )
		{
			if( libcdata_list_element_get_next_element(
			     list_element,
			     &next_element,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve next element from list element: %d.",
				 function,
				 element_index );

				goto on_error;
			}
			if( next_element != NULL )
			{
				if( libcdata_list_element_get_value(
				     next_element,
				     (intptr_t **) &next_range_list_value,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve value from list element: %d.",
					 function,
					 element_index + 1 );

					goto on_error;
				}
				if( next_range_list_value == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
					 "%s: missing range list value element: %d.",
					 function,
					 element_index + 1 );

					goto on_error;
				}
				if( range_list_value->end >= next_range_list_value->start )
				{
					/* Merge range with next
					 */
					range_list_value->size += next_range_list_value->end - range_list_value->end;
					range_list_value->end   = next_range_list_value->end;

					/* Remove next list element
					 */
					if( libcdata_range_list_remove_element(
					     range_list,
					     next_element,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_REMOVE_FAILED,
						 "%s: unable to remove list element: %d.",
						 function,
						 element_index + 1 );

						goto on_error;
					}
					if( libcdata_list_element_free(
					     &next_element,
					     (int (*)(intptr_t **, libcerror_error_t **)) &libcdata_range_list_value_free,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
						 "%s: unable to free list element: %d.",
						 function,
						 element_index + 1 );

						goto on_error;
					}
				}
			}
		}
	}
	if( create_element != 0 )
	{
		if( libcdata_range_list_value_initialize(
		     &split_range_list_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create range list value.",
			 function );

			goto on_error;
		}
		if( split_range_list_value == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing range list value.",
			 function );

			goto on_error;
		}
		split_range_list_value->start = range_start;
		split_range_list_value->size  = range_size;
		split_range_list_value->end   = range_end;

		if( libcdata_range_list_insert_value(
		     range_list,
		     last_element,
		     split_range_list_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to insert range list value in range list.",
			 function );

			goto on_error;
		}
		split_range_list_value = NULL;
	}
	return( 1 );

on_error:
	if( split_range_list_value != NULL )
	{
		libcdata_range_list_value_free(
		 &split_range_list_value,
		 NULL );
	}
	return( -1 );
}

/* Appends an range list
 * Returns 1 if successful, or -1 on error
 */
int libcdata_range_list_append_range_list(
     libcdata_range_list_t *range_list,
     libcdata_range_list_t *source_range_list,
     libcerror_error_t **error )
{
	libcdata_internal_range_list_t *source_internal_range_list = NULL;
	libcdata_list_element_t *source_list_element               = NULL;
	libcdata_range_list_value_t *source_range_list_value       = NULL;
	static char *function                                      = "libcdata_range_list_append_range_list";
	int element_index                                          = 0;

	if( range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	if( source_range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source range list.",
		 function );

		return( -1 );
	}
	source_internal_range_list = (libcdata_internal_range_list_t *) source_range_list;

	source_list_element = source_internal_range_list->first_element;

	for( element_index = 0;
	     element_index < source_internal_range_list->number_of_elements;
	     element_index++ )
	{
		if( libcdata_list_element_get_value(
		     source_list_element,
		     (intptr_t **) &source_range_list_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value from source list element: %d.",
			 function,
			 element_index );

			return( -1 );
		}
		if( libcdata_range_list_append_range(
		     range_list,
		     source_range_list_value->start,
		     source_range_list_value->size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append source range list value: %d to range list.",
			 function,
			 element_index );

			return( -1 );
		}
		if( libcdata_list_element_get_next_element(
		     source_list_element,
		     &source_list_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve next element from source list element: %d.",
			 function,
			 element_index );

			return( -1 );
		}
	}
	return( 1 );
}

/* Inserts the element in the range list after the range list element
 * If range_list_element is NULL the element is inserted before the first element in the list
 * Returns 1 if successful, or -1 on error
 */
int libcdata_range_list_insert_element(
     libcdata_range_list_t *range_list,
     libcdata_list_element_t *range_list_element,
     libcdata_list_element_t *element,
     libcerror_error_t **error )
{
	libcdata_internal_range_list_t *internal_range_list = NULL;
	libcdata_list_element_t *next_element               = NULL;
	libcdata_list_element_t *previous_element           = NULL;
	static char *function                               = "libcdata_range_list_insert_element";

	if( range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	internal_range_list = (libcdata_internal_range_list_t *) range_list;

	if( libcdata_list_element_get_elements(
	     element,
	     &previous_element,
	     &next_element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve previous and next element from list element.",
		 function );

		return( -1 );
	}
	if( ( previous_element != NULL )
	 || ( next_element != NULL ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: list element already part of a list.",
		 function );

		return( -1 );
	}
	if( internal_range_list->number_of_elements == 0 )
	{
		if( internal_range_list->first_element != NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid range list - first element already set.",
			 function );

			return( -1 );
		}
		if( internal_range_list->last_element != NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid range list - last element already set.",
			 function );

			return( -1 );
		}
		internal_range_list->first_element = element;
		internal_range_list->last_element  = element;
	}
	else
	{
		if( internal_range_list->first_element == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid range list - missing first element.",
			 function );

			return( -1 );
		}
		if( internal_range_list->last_element == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid range list - missing last element.",
			 function );

			return( -1 );
		}
		if( range_list_element == NULL )
		{
			if( libcdata_range_list_set_first_element(
			     range_list,
			     element,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set first element.",
				 function );

				return( -1 );
			}
		}
		else
		{
			if( libcdata_list_element_get_next_element(
			     range_list_element,
			     &next_element,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve next element from range list element.",
				 function );

				return( -1 );
			}
			if( libcdata_list_element_set_elements(
			     element,
			     range_list_element,
			     next_element,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set previous and next element of list element.",
				 function );

				return( -1 );
			}
			if( range_list_element == internal_range_list->last_element )
			{
				internal_range_list->last_element = element;
			}
			else if( next_element == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid range list element - missing next element.",
				 function );

				return( -1 );
			}
			else
			{
				if( libcdata_list_element_set_previous_element(
				     next_element,
				     element,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set previous element of next element.",
					 function );

					return( -1 );
				}
			}
			if( libcdata_list_element_set_next_element(
			     range_list_element,
			     element,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set next element of range list element.",
				 function );

				return( -1 );
			}
		}
	}
	internal_range_list->number_of_elements += 1;

	return( 1 );
}

/* Inserts the range list value in the range list after the range list element
 * Returns 1 if successful, or -1 on error
 */
int libcdata_range_list_insert_value(
     libcdata_range_list_t *range_list,
     libcdata_list_element_t *range_list_element,
     libcdata_range_list_value_t *range_list_value,
     libcerror_error_t **error )
{
	libcdata_list_element_t *list_element = NULL;
	static char *function                 = "libcdata_range_list_insert_value";

	if( range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	if( libcdata_list_element_initialize(
	     &list_element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create list element.",
		 function );

		goto on_error;
	}
	if( libcdata_list_element_set_value(
	     list_element,
	     (intptr_t *) range_list_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set value in list element.",
		 function );

		goto on_error;
	}
	if( libcdata_range_list_insert_element(
	     range_list,
	     range_list_element,
	     list_element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to insert list element in range list.",
		 function );

		goto on_error;
	}
	list_element = NULL;

	return( 1 );

on_error:
	if( list_element != NULL )
	{
		libcdata_list_element_free(
		 &list_element,
		 NULL,
		 NULL );
	}
	return( -1 );
}

/* Removes an element from the range list
 * Returns 1 if successful, or -1 on error
 */
int libcdata_range_list_remove_element(
     libcdata_range_list_t *range_list,
     libcdata_list_element_t *element,
     libcerror_error_t **error )
{
	libcdata_internal_range_list_t *internal_range_list = NULL;
	libcdata_list_element_t *next_element               = NULL;
	libcdata_list_element_t *previous_element           = NULL;
	static char *function                               = "libcdata_range_list_remove_element";

	if( range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	internal_range_list = (libcdata_internal_range_list_t *) range_list;

	if( libcdata_list_element_get_elements(
	     element,
	     &previous_element,
	     &next_element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve previous and next element from list element.",
		 function );

		return( -1 );
	}
	if( element == internal_range_list->first_element )
	{
		internal_range_list->first_element = next_element;
	}
	if( element == internal_range_list->last_element )
	{
		internal_range_list->last_element = previous_element;
	}
	if( next_element != NULL )
	{
		if( libcdata_list_element_set_previous_element(
		     next_element,
		     previous_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set previous element of next element.",
			 function );

			return( -1 );
		}
	}
	if( previous_element != NULL )
	{
		if( libcdata_list_element_set_next_element(
		     previous_element,
		     next_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set next element of previous element.",
			 function );

			return( -1 );
		}
	}
	if( libcdata_list_element_set_elements(
	     element,
	     NULL,
	     NULL,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set previous and next element of list element.",
		 function );

		return( -1 );
	}
	internal_range_list->number_of_elements -= 1;

	return( 1 );
}

/* Removes a range
 * Returns 1 if successful, or -1 on error
 */
int libcdata_range_list_remove_range(
     libcdata_range_list_t *range_list,
     uint64_t range_start,
     uint64_t range_size,
     libcerror_error_t **error )
{
	libcdata_list_element_t *list_element               = NULL;
	libcdata_range_list_value_t *range_list_value       = NULL;
	libcdata_range_list_value_t *split_range_list_value = NULL;
	static char *function                               = "libcdata_range_list_remove_range";
	uint64_t range_end                                  = 0;
	int result                                          = 0;

	if( range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	if( range_start > (uint64_t) INT64_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid range start value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( range_size > (uint64_t) INT64_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid range size value exceeds maximum.",
		 function );

		return( -1 );
	}
	range_end = range_start + range_size;

	if( range_end < range_start )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid range end value out of bounds.",
		 function );

		goto on_error;
	}
	result = libcdata_range_list_get_element_by_range_value(
	          range_list,
	          range_start,
	          &list_element,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve list element for range value: %" PRIu64 ".",
		 function,
		 range_start );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libcdata_list_element_get_value(
		     list_element,
		     (intptr_t **) &range_list_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value from list element for range value: %" PRIu64 ".",
			 function,
			 range_start );

			goto on_error;
		}
		if( range_list_value == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing range list value element for range value: %" PRIu64 ".",
			 function,
			 range_start );

			goto on_error;
		}
		if( range_end > range_list_value->end )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid range end value out of bounds.",
			 function );

			goto on_error;
		}
		if( range_start == range_list_value->start )
		{
			if( range_end == range_list_value->end )
			{
				if( libcdata_range_list_remove_element(
				     range_list,
				     list_element,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_REMOVE_FAILED,
					 "%s: unable to remove list element.",
					 function );

					goto on_error;
				}
				if( libcdata_list_element_free(
				     &list_element,
				     (int (*)(intptr_t **, libcerror_error_t **)) &libcdata_range_list_value_free,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free list element.",
					 function );

					goto on_error;
				}
			}
			else if( range_end < range_list_value->end )
			{
				range_list_value->start = range_end;
				range_list_value->size -= range_size;
			}
		}
		else if( range_end == range_list_value->end )
		{
			range_list_value->size -= range_size;
			range_list_value->end   = range_start;
		}
		else
		{
			if( libcdata_range_list_value_initialize(
			     &split_range_list_value,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create range list value.",
				 function );

				goto on_error;
			}
			if( split_range_list_value == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing range list value.",
				 function );

				goto on_error;
			}
			split_range_list_value->start = range_end;
			split_range_list_value->size  = range_list_value->end - range_end;
			split_range_list_value->end   = range_list_value->end;

			range_list_value->size = range_start - range_list_value->start;
			range_list_value->end  = range_start;

			if( libcdata_range_list_insert_value(
			     range_list,
			     list_element,
			     split_range_list_value,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to insert range list value in range list.",
				 function );

				goto on_error;
			}
			split_range_list_value = NULL;
		}
	}
	return( 1 );

on_error:
	if( split_range_list_value != NULL )
	{
		libcdata_range_list_value_free(
		 &split_range_list_value,
		 NULL );
	}
	return( -1 );
}

/* Retrieves a specific element from the range list
 * Returns 1 if successful or -1 on error
 */
int libcdata_range_list_get_element_by_index(
     libcdata_range_list_t *range_list,
     int element_index,
     libcdata_list_element_t **element,
     libcerror_error_t **error )
{
	libcdata_internal_range_list_t *internal_range_list = NULL;
	static char *function                               = "libcdata_range_list_get_element_by_index";

	if( range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	internal_range_list = (libcdata_internal_range_list_t *) range_list;

	if( ( element_index < 0 )
	 || ( element_index >= internal_range_list->number_of_elements ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid element index value out of bounds.",
		 function );

		return( -1 );
	}
	if( element == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid element.",
		 function );

		return( -1 );
	}
	if( ( internal_range_list->current_element != NULL )
	 && ( internal_range_list->current_element_index != element_index ) )
	{
		if( element_index < internal_range_list->current_element_index )
		{
			if( ( internal_range_list->current_element_index - element_index ) < ( internal_range_list->number_of_elements / 2 ) )
			{
				while( internal_range_list->current_element_index > element_index )
				{
					if( libcdata_list_element_get_previous_element(
					     internal_range_list->current_element,
					     &( internal_range_list->current_element ),
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
						 "%s: unable to retrieve previous element from list element: %d.",
						 function,
						 internal_range_list->current_element_index );

						return( -1 );
					}
					internal_range_list->current_element_index--;
				}
			}
		}
		else
		{
			if( ( element_index - internal_range_list->current_element_index ) < ( internal_range_list->number_of_elements / 2 ) )
			{
				while( internal_range_list->current_element_index < element_index )
				{
					if( libcdata_list_element_get_next_element(
					     internal_range_list->current_element,
					     &( internal_range_list->current_element ),
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
						 "%s: unable to retrieve next element from list element: %d.",
						 function,
						 internal_range_list->current_element_index );

						return( -1 );
					}
					internal_range_list->current_element_index++;
				}
			}
		}
	}
	if( ( internal_range_list->current_element == NULL )
	 || ( internal_range_list->current_element_index != element_index ) )
	{
		if( element_index < ( internal_range_list->number_of_elements / 2 ) )
		{
			internal_range_list->current_element = internal_range_list->first_element;

			for( internal_range_list->current_element_index = 0;
			     internal_range_list->current_element_index < element_index;
			     internal_range_list->current_element_index++ )
			{
				if( libcdata_list_element_get_next_element(
				     internal_range_list->current_element,
				     &( internal_range_list->current_element ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve next element from list element: %d.",
					 function,
					 internal_range_list->current_element_index );

					return( -1 );
				}
			}
		}
		else
		{
			internal_range_list->current_element = internal_range_list->last_element;

			for( internal_range_list->current_element_index = ( internal_range_list->number_of_elements - 1 );
			     internal_range_list->current_element_index > element_index;
			     internal_range_list->current_element_index-- )
			{
				if( libcdata_list_element_get_previous_element(
				     internal_range_list->current_element,
				     &( internal_range_list->current_element ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve previous element from list element: %d.",
					 function,
					 internal_range_list->current_element_index );

					return( -1 );
				}
			}
		}
	}
	*element = internal_range_list->current_element;

	return( 1 );
}

/* Retrieves the element that contains the range value
 * Returns 1 if successful, 0 if no element was found or -1 on error
 */
int libcdata_range_list_get_element_by_range_value(
     libcdata_range_list_t *range_list,
     uint64_t range_value,
     libcdata_list_element_t **element,
     libcerror_error_t **error )
{
	libcdata_internal_range_list_t *internal_range_list = NULL;
	libcdata_range_list_value_t *range_list_value       = NULL;
	static char *function                               = "libcdata_range_list_get_element_by_range_value";
	int element_index                                   = 0;

	if( range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	internal_range_list = (libcdata_internal_range_list_t *) range_list;

	if( range_value > (uint64_t) INT64_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid range value value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( element == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid element.",
		 function );

		return( -1 );
	}
/* TODO add optimization using current element */
	*element = internal_range_list->first_element;

	for( element_index = 0;
	     element_index < internal_range_list->number_of_elements;
	     element_index++ )
	{
		if( libcdata_list_element_get_value(
		     *element,
		     (intptr_t **) &range_list_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value from list element: %d.",
			 function,
			 element_index );

			return( -1 );
		}
		if( range_list_value == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing range list value element: %d.",
			 function,
			 element_index );

			return( -1 );
		}
		if( ( range_value >= range_list_value->start )
		 && ( range_value < range_list_value->end ) )
		{
			return( 1 );
		}
		if( libcdata_list_element_get_next_element(
		     *element,
		     element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve next element from list element: %d.",
			 function,
			 element_index );

			return( -1 );
		}
	}
	return( 0 );
}

/* Retrieves a specific value from the range list
 * Returns 1 if successful or -1 on error
 */
int libcdata_range_list_get_value_by_index(
     libcdata_range_list_t *range_list,
     int element_index,
     libcdata_range_list_value_t **range_list_value,
     libcerror_error_t **error )
{
	libcdata_list_element_t *list_element = NULL;
	static char *function                 = "libcdata_range_list_get_value_by_index";

	if( range_list_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list value.",
		 function );

		return( -1 );
	}
	if( libcdata_range_list_get_element_by_index(
	     range_list,
	     element_index,
	     &list_element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve list element: %d.",
		 function,
		 element_index );

		return( -1 );
	}
	if( libcdata_list_element_get_value(
	     list_element,
	     (intptr_t **) range_list_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from list element: %d.",
		 function,
		 element_index );

		return( -1 );
	}
	if( range_list_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing range list value: %d .",
		 function,
		 element_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the value that contains the range value
 * Returns 1 if successful, 0 if no value was found or -1 on error
 */
int libcdata_range_list_get_value_by_range_value(
     libcdata_range_list_t *range_list,
     uint64_t range_value,
     libcdata_range_list_value_t **range_list_value,
     libcerror_error_t **error )
{
	libcdata_list_element_t *list_element = NULL;
	static char *function                 = "libcdata_range_list_get_value_by_range_value";
	int result                            = 0;

	if( range_list_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list value.",
		 function );

		return( -1 );
	}
	result = libcdata_range_list_get_element_by_range_value(
	          range_list,
	          range_value,
	          &list_element,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve list element for range value: %" PRIu64 ".",
		 function,
		 range_value );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libcdata_list_element_get_value(
		     list_element,
		     (intptr_t **) range_list_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value from list element for range value: %" PRIu64 ".",
			 function,
			 range_value );

			return( -1 );
		}
		if( *range_list_value == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing range list value for range value: %" PRIu64 ".",
			 function,
			 range_value );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves a specific range
 * Returns 1 if successful or -1 on error
 */
int libcdata_range_list_get_range(
     libcdata_range_list_t *range_list,
     int element_index,
     uint64_t *range_start,
     uint64_t *range_size,
     libcerror_error_t **error )
{
	libcdata_range_list_value_t *range_list_value = NULL;
	static char *function                         = "libcdata_range_list_get_range";

	if( range_start == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range start.",
		 function );

		return( -1 );
	}
	if( range_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range size.",
		 function );

		return( -1 );
	}
	if( libcdata_range_list_get_value_by_index(
	     range_list,
	     element_index,
	     &range_list_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve range list value: %d.",
		 function,
		 element_index );

		return( -1 );
	}
	if( range_list_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing range list value: %d.",
		 function,
		 element_index );

		return( -1 );
	}
	*range_start = range_list_value->start;
	*range_size  = range_list_value->size;

	return( 1 );
}

/* Retrieves a range for a specific range value
 * Returns 1 if successful, 0 if no range was found or -1 on error
 */
int libcdata_range_list_get_range_by_range_value(
     libcdata_range_list_t *range_list,
     uint64_t range_value,
     uint64_t *range_start,
     uint64_t *range_size,
     libcerror_error_t **error )
{
	libcdata_range_list_value_t *range_list_value = NULL;
	static char *function                         = "libcdata_range_list_get_range_by_range_value";
	int result                                    = 0;

	if( range_start == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range start.",
		 function );

		return( -1 );
	}
	if( range_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range size.",
		 function );

		return( -1 );
	}
	result = libcdata_range_list_get_value_by_range_value(
	          range_list,
	          range_value,
	          &range_list_value,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve range list value for range value: %" PRIu64 ".",
		 function,
		 range_value );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( range_list_value == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing range list value for range value: %" PRIu64 ".",
			 function,
			 range_value );

			return( -1 );
		}
		*range_start = range_list_value->start;
		*range_size  = range_list_value->size;
	}
	return( result );
}

/* Determines if a certain range is present
 * Returns 1 if present, 0 if not present or -1 on error
 */
int libcdata_range_list_range_is_present(
     libcdata_range_list_t *range_list,
     uint64_t range_start,
     uint64_t range_size,
     libcerror_error_t **error )
{
	libcdata_internal_range_list_t *internal_range_list = NULL;
	libcdata_list_element_t *list_element               = NULL;
	libcdata_range_list_value_t *range_list_value       = NULL;
	static char *function                               = "libcdata_range_list_range_is_present";
	uint64_t range_end                                  = 0;
	int element_index                                   = 0;

	if( range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	internal_range_list = (libcdata_internal_range_list_t *) range_list;

	if( range_start > (uint64_t) INT64_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid range start value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( range_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.",
		 function );

		return( -1 );
	}
/* TODO refactor to get by range function ? */
	range_end = range_start + range_size;

	if( range_end < range_start )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid range end value out of bounds.",
		 function );

		return( -1 );
	}
/* TODO add optimization using current element */
	list_element = internal_range_list->first_element;

	for( element_index = 0;
	     element_index < internal_range_list->number_of_elements;
	     element_index++ )
	{
		if( libcdata_list_element_get_value(
		     list_element,
		     (intptr_t **) &range_list_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value from list element: %d.",
			 function,
			 element_index );

			return( -1 );
		}
		if( ( range_start >= range_list_value->start )
		 && ( range_start < range_list_value->end ) )
		{
			return( 1 );
		}
/* TODO is this necessary ? */
		if( ( range_list_value->start >= range_start )
		 && ( range_list_value->start < range_end ) )
		{
			return( 1 );
		}
		if( libcdata_list_element_get_next_element(
		     list_element,
		     &list_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve next element from list element: %d.",
			 function,
			 element_index );

			return( -1 );
		}
	}
	return( 0 );
}

