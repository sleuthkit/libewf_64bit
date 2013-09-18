/*
 * Data handle functions
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
#include <memory.h>
#include <types.h>

#include "libfvalue_data_handle.h"
#include "libfvalue_definitions.h"
#include "libfvalue_libcdata.h"
#include "libfvalue_libcerror.h"
#include "libfvalue_types.h"
#include "libfvalue_value_entry.h"

/* Creates a data handle
 * Returns 1 if successful or -1 on error
 */
int libfvalue_data_handle_initialize(
     libfvalue_data_handle_t **data_handle,
     libcerror_error_t **error )
{
	libfvalue_internal_data_handle_t *internal_data_handle = NULL;
	static char *function                                  = "libfvalue_data_handle_initialize";

	if( data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data handle.",
		 function );

		return( -1 );
	}
	if( *data_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid data handle value already set.",
		 function );

		return( -1 );
	}
	internal_data_handle = memory_allocate_structure(
	                        libfvalue_internal_data_handle_t );

	if( internal_data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_data_handle,
	     0,
	     sizeof( libfvalue_internal_data_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear data handle.",
		 function );

		goto on_error;
	}
	*data_handle = (libfvalue_data_handle_t *) internal_data_handle;

	return( 1 );

on_error:
	if( internal_data_handle != NULL )
	{
		memory_free(
		 internal_data_handle );
	}
	return( -1 );
}

/* Frees the data handle
 * Returns 1 if successful or -1 on error
 */
int libfvalue_data_handle_free(
     libfvalue_data_handle_t **data_handle,
     libcerror_error_t **error )
{
	libfvalue_internal_data_handle_t *internal_data_handle = NULL;
	static char *function                                  = "libfvalue_data_handle_free";
	int result                                             = 1;

	if( data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data handle.",
		 function );

		return( -1 );
	}
	if( *data_handle != NULL )
	{
		internal_data_handle = (libfvalue_internal_data_handle_t *) *data_handle;
		*data_handle         = NULL;

		if( ( internal_data_handle->flags & LIBFVALUE_VALUE_DATA_FLAG_MANAGED ) != 0 )
		{
			if( internal_data_handle->data != NULL )
			{
				memory_free(
				 internal_data_handle->data );
			}
		}
		if( internal_data_handle->value_entries != NULL )
		{
			if( libcdata_array_free(
			     &( internal_data_handle->value_entries ),
			     (int (*)(intptr_t **, libcerror_error_t **)) &libfvalue_value_entry_free,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free value entries array.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 internal_data_handle );
	}
	return( result );
}

/* Clones a data handle
 * Returns 1 if successful or -1 on error
 */
int libfvalue_data_handle_clone(
     libfvalue_data_handle_t **destination_data_handle,
     libfvalue_data_handle_t *source_data_handle,
     libcerror_error_t **error )
{
	libfvalue_internal_data_handle_t *internal_source_data_handle = NULL;
	static char *function                                         = "libfvalue_data_handle_clone";

	if( destination_data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination data handle.",
		 function );

		return( -1 );
	}
	if( *destination_data_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: destination data handle already set.",
		 function );

		return( -1 );
	}
	if( source_data_handle == NULL )
	{
		*destination_data_handle = NULL;

		return( 1 );
	}
	internal_source_data_handle = (libfvalue_internal_data_handle_t *) source_data_handle;
	
	if( libfvalue_data_handle_initialize(
	     destination_data_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination data handle.",
		 function );

		goto on_error;
	}
	if( *destination_data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing destination data handle.",
		 function );

		goto on_error;
	}
	if( internal_source_data_handle->data != NULL )
	{
		if( libfvalue_data_handle_set_data(
		     *destination_data_handle,
		     internal_source_data_handle->data,
		     internal_source_data_handle->data_size,
		     internal_source_data_handle->encoding,
		     LIBFVALUE_VALUE_DATA_FLAG_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set data in destination data handle.",
			 function );

			goto on_error;
		}
	}
	if( internal_source_data_handle->value_entries != NULL )
	{
		if( libcdata_array_clone(
		     &( ( (libfvalue_internal_data_handle_t *) *destination_data_handle )->value_entries ),
		     internal_source_data_handle->value_entries,
		     (int (*)(intptr_t **, libcerror_error_t **)) &libfvalue_value_entry_free,
		     (int (*)(intptr_t **, intptr_t *, libcerror_error_t **)) &libfvalue_value_entry_clone,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create destination value entries array.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( *destination_data_handle != NULL )
	{
		libfvalue_data_handle_free(
		 destination_data_handle,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the data
 * Returns 1 if successful or -1 on error
 */
int libfvalue_data_handle_get_data(
     libfvalue_data_handle_t *data_handle,
     uint8_t **data,
     size_t *data_size,
     int *encoding,
     libcerror_error_t **error )
{
	libfvalue_internal_data_handle_t *internal_data_handle = NULL;
	static char *function                                  = "libfvalue_data_handle_get_data";

	if( data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data handle.",
		 function );

		return( -1 );
	}
	internal_data_handle = (libfvalue_internal_data_handle_t *) data_handle;
	
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
	if( data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data size.",
		 function );

		return( -1 );
	}
	if( encoding == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid byte order.",
		 function );

		return( -1 );
	}
	*data      = internal_data_handle->data;
	*data_size = internal_data_handle->data_size;
	*encoding  = internal_data_handle->encoding;

	return( 1 );
}

/* Sets the data
 * Returns 1 if successful or -1 on error
 */
int libfvalue_data_handle_set_data(
     libfvalue_data_handle_t *data_handle,
     const uint8_t *data,
     size_t data_size,
     int encoding,
     uint8_t flags,
     libcerror_error_t **error )
{
	libfvalue_internal_data_handle_t *internal_data_handle = NULL;
	static char *function                                  = "libfvalue_data_handle_set_data";

	if( data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data handle.",
		 function );

		return( -1 );
	}
	internal_data_handle = (libfvalue_internal_data_handle_t *) data_handle;
	
	if( ( flags & ~( LIBFVALUE_VALUE_DATA_FLAG_MANAGED | LIBFVALUE_VALUE_DATA_FLAG_CLONE_BY_REFERENCE ) ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported flags: 0x%02" PRIx8 ".",
		 function,
		 flags );

		return( -1 );
	}
	if( ( internal_data_handle->flags & LIBFVALUE_VALUE_DATA_FLAG_MANAGED ) != 0 )
	{
		if( internal_data_handle->data != NULL )
		{
			memory_free(
			 internal_data_handle->data );

			internal_data_handle->data      = NULL;
			internal_data_handle->data_size = 0;
		}
		internal_data_handle->flags &= ~( LIBFVALUE_VALUE_DATA_FLAG_MANAGED );
	}
	/* Make sure empty values have data that refers to NULL
	 */
	if( ( data == NULL )
	 || ( data_size == 0 ) )
	{
		internal_data_handle->data = NULL;
	}
	else if( ( flags & LIBFVALUE_VALUE_DATA_FLAG_CLONE_BY_REFERENCE ) != 0 )
	{
		internal_data_handle->data = (uint8_t *) data;

		if( ( flags & LIBFVALUE_VALUE_DATA_FLAG_MANAGED ) != 0 )
		{
			internal_data_handle->flags |= LIBFVALUE_VALUE_DATA_FLAG_MANAGED;
		}
	}
	else
	{
		internal_data_handle->data = (uint8_t *) memory_allocate(
		                                          sizeof( uint8_t ) * data_size );

		if( internal_data_handle->data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create data.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     internal_data_handle->data,
		     data,
		     data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy data.",
			 function );

			goto on_error;
		}
		internal_data_handle->flags |= LIBFVALUE_VALUE_DATA_FLAG_MANAGED;
	}
	internal_data_handle->data_size = data_size;
	internal_data_handle->encoding  = encoding;

	return( 1 );

on_error:
	if( internal_data_handle->data != NULL )
	{
		memory_free(
		 internal_data_handle->data );

		internal_data_handle->data = NULL;
	}
	return( -1 );
}

/* Retrieves the number of value entries
 * Returns if successful or -1 on error
 */
int libfvalue_data_handle_get_number_of_value_entries(
     libfvalue_data_handle_t *data_handle,
     int *number_of_value_entries,
     libcerror_error_t **error )
{
	libfvalue_internal_data_handle_t *internal_data_handle = NULL;
	static char *function                                  = "libfvalue_data_handle_get_number_of_value_entries";

	if( data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data handle.",
		 function );

		return( -1 );
	}
	internal_data_handle = (libfvalue_internal_data_handle_t *) data_handle;
	
	if( number_of_value_entries == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of value entries.",
		 function );

		return( -1 );
	}
	if( internal_data_handle->data == NULL )
	{
		*number_of_value_entries = 0;
	}
	else if( internal_data_handle->value_entries == NULL )
	{
		*number_of_value_entries = 1;
	}
	else
	{
		if( libcdata_array_get_number_of_entries(
		     internal_data_handle->value_entries,
		     number_of_value_entries,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of entries from value entries array.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves a specific value entry
 * Returns if successful or -1 on error
 */
int libfvalue_data_handle_get_value_entry(
     libfvalue_data_handle_t *data_handle,
     int value_entry_index,
     uint8_t **value_entry_data,
     size_t *value_entry_data_size,
     int *encoding,
     libcerror_error_t **error )
{
	libfvalue_internal_data_handle_t *internal_data_handle = NULL;
	libfvalue_value_entry_t *value_entry                   = NULL;
	static char *function                                  = "libfvalue_data_handle_get_value_entry";

	if( data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data handle.",
		 function );

		return( -1 );
	}
	internal_data_handle = (libfvalue_internal_data_handle_t *) data_handle;

	if( internal_data_handle->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data handle - missing data.",
		 function );

		return( -1 );
	}
	if( value_entry_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value entry data.",
		 function );

		return( -1 );
	}
	if( value_entry_data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value entry data size.",
		 function );

		return( -1 );
	}
	if( encoding == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid encoding.",
		 function );

		return( -1 );
	}
	if( internal_data_handle->value_entries == NULL )
	{
		if( value_entry_index != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid value index value out of bounds.",
			 function );

			return( -1 );
		}
		*value_entry_data      = internal_data_handle->data;
		*value_entry_data_size = internal_data_handle->data_size;
	}
	else
	{
		if( libcdata_array_get_entry_by_index(
		     internal_data_handle->value_entries,
		     value_entry_index,
		     (intptr_t **) &value_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve entry: %d from values entries array.",
			 function,
			 value_entry_index );

			return( -1 );
		}
		if( value_entry == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing value entry.",
			 function );

			return( -1 );
		}
		if( value_entry->offset > internal_data_handle->data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: value entry offset: %d out of bounds.",
			 function,
			 value_entry_index );

			return( -1 );
		}
		if( ( value_entry->offset + value_entry->size ) > internal_data_handle->data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: value entry size: %d out of bounds.",
			 function,
			 value_entry_index );

			return( -1 );
		}
		if( value_entry->size != 0 )
		{
			*value_entry_data = &( ( internal_data_handle->data )[ value_entry->offset ] );
		}
		else
		{
			*value_entry_data = NULL;
		}
		*value_entry_data_size = value_entry->size;
	}
	*encoding = internal_data_handle->encoding;

	return( 1 );
}

/* Sets a specific value entry
 * Returns if successful or -1 on error
 */
int libfvalue_data_handle_set_value_entry(
     libfvalue_data_handle_t *data_handle,
     int value_entry_index,
     const uint8_t *value_entry_data,
     size_t value_entry_data_size,
     int encoding,
     libcerror_error_t **error )
{
	libfvalue_internal_data_handle_t *internal_data_handle = NULL;
	libfvalue_value_entry_t *value_entry                   = NULL;
	static char *function                                  = "libfvalue_data_handle_set_value_entry";

	if( data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data handle.",
		 function );

		return( -1 );
	}
	internal_data_handle = (libfvalue_internal_data_handle_t *) data_handle;

	if( internal_data_handle->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data handle - missing data.",
		 function );

		return( -1 );
	}
	if( value_entry_index != 0 )	
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid value entry index value out of bounds.",
		 function );

		return( -1 );
	}
	if( value_entry_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value entry data.",
		 function );

		return( -1 );
	}
	if( value_entry_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid value entry data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( encoding != internal_data_handle->encoding )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid encoding value out of bounds.",
		 function );

		return( -1 );
	}
	if( internal_data_handle->value_entries == NULL )
	{
		if( value_entry_index != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid value index value out of bounds.",
			 function );

			return( -1 );
		}
		if( value_entry_data_size != internal_data_handle->data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid value entry data size value out of bounds.",
			 function );

			return( -1 );
		}
		if( internal_data_handle->data_size > 0 )
		{
			if( memory_copy(
			     internal_data_handle->data,
			     value_entry_data,
			     internal_data_handle->data_size ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy value entry data.",
				 function );

				return( -1 );
			}
		}
	}
	else
	{
		if( libcdata_array_get_entry_by_index(
		     internal_data_handle->value_entries,
		     value_entry_index,
		     (intptr_t **) &value_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve entry: %d from values entries array.",
			 function,
			 value_entry_index );

			return( -1 );
		}
		if( value_entry == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing value entry.",
			 function );

			return( -1 );
		}
		if( value_entry->offset > internal_data_handle->data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: value entry offset: %d out of bounds.",
			 function,
			 value_entry_index );

			return( -1 );
		}
		if( ( value_entry->offset + value_entry->size ) > internal_data_handle->data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: value entry size: %d out of bounds.",
			 function,
			 value_entry_index );

			return( -1 );
		}
		if( value_entry->size != internal_data_handle->data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid value entry data size value out of bounds.",
			 function );

			return( -1 );
		}
		if( value_entry->size > 0 )
		{
			if( memory_copy(
			     &( ( internal_data_handle->data )[ value_entry->offset ] ),
			     value_entry_data,
			     value_entry->size ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy value entry data.",
				 function );

				return( -1 );
			}
		}
	}
	return( 1 );
}

/* Appends a value entry
 * Returns if successful or -1 on error
 */
int libfvalue_data_handle_append_value_entry(
     libfvalue_data_handle_t *data_handle,
     int *value_entry_index,
     const uint8_t *value_entry_data,
     size_t value_entry_data_size,
     int encoding,
     libcerror_error_t **error )
{
	libfvalue_internal_data_handle_t *internal_data_handle = NULL;
	libfvalue_value_entry_t *value_entry                   = NULL;
	void *reallocation                                     = NULL;
	static char *function                                  = "libfvalue_data_handle_append_value_entry";

	if( data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data handle.",
		 function );

		return( -1 );
	}
	internal_data_handle = (libfvalue_internal_data_handle_t *) data_handle;

	if( value_entry_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value entry index.",
		 function );

		return( -1 );
	}
	if( value_entry_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value entry data.",
		 function );

		return( -1 );
	}
	if( value_entry_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid value entry data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( internal_data_handle->data == NULL )
	{
		if( libfvalue_data_handle_set_data(
		     data_handle,
		     value_entry_data,
		     value_entry_data_size,
		     encoding,
		     LIBFVALUE_VALUE_DATA_FLAG_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set data in data handle.",
			 function );

			goto on_error;
		}
		internal_data_handle->encoding = encoding;
	}
	else
	{
		if( encoding != internal_data_handle->encoding )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid encoding value out of bounds.",
			 function );

			return( -1 );
		}
		if( ( internal_data_handle->data_size + value_entry_data_size ) > (size_t) SSIZE_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid value entry data size value exceeds maximum.",
			 function );

			return( -1 );
		}
		if( internal_data_handle->value_entries == NULL )
		{
			if( libcdata_array_initialize(
			     &( internal_data_handle->value_entries ),
			     1,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create value entries array.",
				 function );

				goto on_error;
			}
			if( libfvalue_value_entry_initialize(
			     &value_entry,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create value entry.",
				 function );

				goto on_error;
			}
			value_entry->offset = 0;
			value_entry->size   = internal_data_handle->data_size;

			if( libcdata_array_set_entry_by_index(
			     internal_data_handle->value_entries,
			     0,
			     (intptr_t *) value_entry,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set entry: 0 in values entries array.",
				 function );

				goto on_error;
			}
			value_entry = NULL;
		}
		if( libfvalue_value_entry_initialize(
		     &value_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create value entry.",
			 function );

			goto on_error;
		}
		value_entry->offset = internal_data_handle->data_size;
		value_entry->size   = value_entry_data_size;

		reallocation = memory_reallocate(
		                internal_data_handle->data,
		                internal_data_handle->data_size + value_entry_data_size );

		if( reallocation == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to resize array entries.",
			 function );

			goto on_error;
		}
		internal_data_handle->data       = (uint8_t *) reallocation;
		internal_data_handle->data_size += value_entry_data_size;

		if( memory_copy(
		     &( ( internal_data_handle->data )[ value_entry->offset ] ),
		     value_entry_data,
		     value_entry->size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy value entry data.",
			 function );

			goto on_error;
		}
		if( libcdata_array_append_entry(
		     internal_data_handle->value_entries,
		     value_entry_index,
		     (intptr_t *) value_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append entry to values entries array.",
			 function );

			goto on_error;
		}
		value_entry = NULL;
	}
	return( 1 );

on_error:
	if( value_entry != NULL )
	{
		libfvalue_value_entry_free(
		 &value_entry,
		 NULL );
	}
	return( -1 );
}

