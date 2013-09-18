/*
 * Meta data functions
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

#include "libsmraw_definitions.h"
#include "libsmraw_handle.h"
#include "libsmraw_libbfio.h"
#include "libsmraw_libcerror.h"
#include "libsmraw_libcnotify.h"
#include "libsmraw_libcstring.h"
#include "libsmraw_libfvalue.h"
#include "libsmraw_types.h"

/* Retrieves the media size
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_get_media_size(
     libsmraw_handle_t *handle,
     size64_t *media_size,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_get_media_size";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file io pool.",
		 function );

		return( -1 );
	}
	if( media_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media size.",
		 function );

		return( -1 );
	}
	*media_size = internal_handle->media_size;

	return( 1 );
}

/* Sets the media size
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_set_media_size(
     libsmraw_handle_t *handle,
     size64_t media_size,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_set_media_size";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( ( internal_handle->read_values_initialized != 0 )
	 || ( internal_handle->write_values_initialized != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: media size cannot be changed.",
		 function );

		return( -1 );
	}
	internal_handle->media_size = media_size;

	return( 1 );
}

/* Retrieves the bytes per sector
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_get_bytes_per_sector(
     libsmraw_handle_t *handle,
     uint32_t *bytes_per_sector,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	libfvalue_value_t *value                    = NULL;
	static char *function                       = "libsmraw_handle_get_bytes_per_sector";
	const char *identifier                      = "bytes_per_sector";
	size_t identifier_size                      = 16;
	uint64_t value_64bit                        = 0;
	int result                                  = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( bytes_per_sector == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid bytes per sector.",
		 function );

		return( -1 );
	}
	*bytes_per_sector = 0;

	result = libfvalue_table_get_value_by_identifier(
	          internal_handle->media_values,
	          (uint8_t *) identifier,
	          identifier_size,
	          &value,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %s from media values table.",
		 function,
		 identifier );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfvalue_value_copy_to_64bit(
		     value,
		     0,
		     &value_64bit,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy value to a 64-bit value.",
			 function );

			return( -1 );
		}
		if( value_64bit > (uint64_t) UINT32_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: 64-bit bytes per sector value out of bounds.",
			 function );

			return( -1 );
		}
		*bytes_per_sector = (size_t) value_64bit;
	}
	return( 1 );
}

/* Sets the bytes per sector
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_set_bytes_per_sector(
     libsmraw_handle_t *handle,
     uint32_t bytes_per_sector,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	libfvalue_value_t *value                    = NULL;
	static char *function                       = "libsmraw_handle_set_bytes_per_sector";
	const char *identifier                      = "bytes_per_sector";
	size_t identifier_size                      = 16;
	int result                                  = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( ( internal_handle->read_values_initialized != 0 )
	 || ( internal_handle->write_values_initialized != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: media values cannot be changed.",
		 function );

		return( -1 );
	}
	result = libfvalue_table_get_value_by_identifier(
	          internal_handle->media_values,
	          (uint8_t *) identifier,
	          identifier_size,
	          &value,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %s from media values table.",
		 function,
		 identifier );

		return( -1 );
	}
	else if( result == 0 )
	{
		if( libfvalue_value_type_initialize(
		     &value,
		     LIBFVALUE_VALUE_TYPE_UNSIGNED_INTEGER_64BIT,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create value.",
			 function );

			return( -1 );
		}
		if( libfvalue_value_set_identifier(
		     value,
		     (uint8_t *) identifier,
		     identifier_size,
		     LIBFVALUE_VALUE_FLAG_IDENTIFIER_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to set identifier in value.",
			 function );

			libfvalue_value_free(
			 &value,
			 NULL );

			return( -1 );
		}
		if( libfvalue_table_set_value(
		     internal_handle->media_values,
		     value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set value: %s in media values table.",
			 function,
			 identifier );

			libfvalue_value_free(
			 &value,
			 NULL );

			return( -1 );
		}
	}
	if( libfvalue_value_copy_from_64bit(
	     value,
	     0,
	     (uint64_t) bytes_per_sector,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy value from a 64-bit value.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the media type
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_get_media_type(
     libsmraw_handle_t *handle,
     uint8_t *media_type,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	libfvalue_value_t *value                    = NULL;
	uint8_t *value_data                         = NULL;
	static char *function                       = "libsmraw_handle_get_media_type";
	const char *identifier                      = "media_type";
	size_t identifier_size                      = 10;
	size_t value_data_size                      = 0;
	int encoding                                = 0;
	int result                                  = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file io pool.",
		 function );

		return( -1 );
	}
	if( media_type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media type.",
		 function );

		return( -1 );
	}
	*media_type = LIBSMRAW_MEDIA_TYPE_UNKNOWN;

	result = libfvalue_table_get_value_by_identifier(
	          internal_handle->media_values,
	          (uint8_t *) identifier,
	          identifier_size,
	          &value,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %s from media values table.",
		 function,
		 identifier );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfvalue_value_get_data(
		     value,
		     &value_data,
		     &value_data_size,
		     &encoding,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value data.",
			 function );

			return( -1 );
		}
		if( value_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing value data.",
			 function );

			return( -1 );
		}
		if( value_data_size == 6 )
		{
			if( libcstring_narrow_string_compare(
			     "fixed",
			     value_data,
			     5 ) == 0 )
			{
				*media_type = LIBSMRAW_MEDIA_TYPE_FIXED;
			}
		}
		else if( value_data_size == 7 )
		{
			if( libcstring_narrow_string_compare(
			     "memory",
			     value_data,
			     6 ) == 0 )
			{
				*media_type = LIBSMRAW_MEDIA_TYPE_MEMORY;
			}
		}
		else if( value_data_size == 8 )
		{
			if( libcstring_narrow_string_compare(
			     "optical",
			     value_data,
			     7 ) == 0 )
			{
				*media_type = LIBSMRAW_MEDIA_TYPE_OPTICAL;
			}
		}
		else if( value_data_size == 10 )
		{
			if( libcstring_narrow_string_compare(
			     "removable",
			     value_data,
			     9 ) == 0 )
			{
				*media_type = LIBSMRAW_MEDIA_TYPE_REMOVABLE;
			}
		}
	}
	return( 1 );
}

/* Sets the media type
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_set_media_type(
     libsmraw_handle_t *handle,
     uint8_t media_type,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	libfvalue_value_t *value                    = NULL;
	static char *function                       = "libsmraw_handle_set_media_type";
	const char *identifier                      = "media_type";
	const char *value_string                    = NULL;
	size_t identifier_size                      = 10;
	size_t value_string_length                  = 0;
	int result                                  = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( ( internal_handle->read_values_initialized != 0 )
	 || ( internal_handle->write_values_initialized != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: media values cannot be changed.",
		 function );

		return( -1 );
	}
	switch( media_type )
	{
		case LIBSMRAW_MEDIA_TYPE_FIXED:
			value_string        = "fixed";
			value_string_length = 5;

			break;

		case LIBSMRAW_MEDIA_TYPE_MEMORY:
			value_string        = "memory";
			value_string_length = 6;

			break;

		case LIBSMRAW_MEDIA_TYPE_OPTICAL:
			value_string        = "optical";
			value_string_length = 7;

			break;

		case LIBSMRAW_MEDIA_TYPE_REMOVABLE:
			value_string        = "removable";
			value_string_length = 9;

			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported media type: 0x%02x.",
			 function,
			 media_type );

			return( -1 );
	}
	result = libfvalue_table_get_value_by_identifier(
	          internal_handle->media_values,
	          (uint8_t *) identifier,
	          identifier_size,
	          &value,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %s from media values table.",
		 function,
		 identifier );

		return( -1 );
	}
	else if( result == 0 )
	{
		if( libfvalue_value_type_initialize(
		     &value,
		     LIBFVALUE_VALUE_TYPE_STRING_UTF8,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create value.",
			 function );

			return( -1 );
		}
		if( libfvalue_value_set_identifier(
		     value,
		     (uint8_t *) identifier,
		     identifier_size,
		     LIBFVALUE_VALUE_FLAG_IDENTIFIER_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to set identifier in value.",
			 function );

			libfvalue_value_free(
			 &value,
			 NULL );

			return( -1 );
		}
		if( libfvalue_table_set_value(
		     internal_handle->media_values,
		     value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set value: %s in media values table.",
			 function,
			 identifier );

			libfvalue_value_free(
			 &value,
			 NULL );

			return( -1 );
		}
	}
	if( libfvalue_value_copy_from_utf8_string(
	     value,
	     0,
	     (uint8_t *) value_string,
	     value_string_length,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy value from an UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the media flags
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_get_media_flags(
     libsmraw_handle_t *handle,
     uint8_t *media_flags,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	libfvalue_value_t *value                    = NULL;
	uint8_t *value_data                         = NULL;
	static char *function                       = "libsmraw_handle_get_media_flags";
	const char *identifier                      = "media_flags";
	size_t identifier_size                      = 11;
	size_t value_data_size                      = 0;
	int encoding                                = 0;
	int result                                  = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file io pool.",
		 function );

		return( -1 );
	}
	if( media_flags == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media flags.",
		 function );

		return( -1 );
	}
	result = libfvalue_table_get_value_by_identifier(
	          internal_handle->media_values,
	          (uint8_t *) identifier,
	          identifier_size,
	          &value,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %s from media values table.",
		 function,
		 identifier );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfvalue_value_get_data(
		     value,
		     &value_data,
		     &value_data_size,
		     &encoding,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value data.",
			 function );

			return( -1 );
		}
		if( value_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing value data.",
			 function );

			return( -1 );
		}
		if( value_data_size == 8 )
		{
			if( libcstring_narrow_string_compare(
			     "logical",
			     value_data,
			     7 ) == 0 )
			{
				*media_flags &= ~( LIBSMRAW_MEDIA_FLAG_PHYSICAL );
			}
		}
		else if( value_data_size == 9 )
		{
			if( libcstring_narrow_string_compare(
			     "physical",
			     value_data,
			     8 ) == 0 )
			{
				*media_flags |= LIBSMRAW_MEDIA_FLAG_PHYSICAL;
			}
		}
	}
	return( 1 );
}

/* Sets the media flags
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_set_media_flags(
     libsmraw_handle_t *handle,
     uint8_t media_flags,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	libfvalue_value_t *value                    = NULL;
	static char *function                       = "libsmraw_handle_set_media_flags";
	const char *identifier                      = "media_flags";
	const char *value_string                    = NULL;
	size_t identifier_size                      = 11;
	size_t value_string_length                  = 0;
	int result                                  = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( ( internal_handle->read_values_initialized != 0 )
	 || ( internal_handle->write_values_initialized != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: media size cannot be changed.",
		 function );

		return( -1 );
	}
	if( ( media_flags & ~( LIBSMRAW_MEDIA_FLAG_PHYSICAL ) ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported media flags: 0x%02x.",
		 function,
		 media_flags );

		return( -1 );
	}
	if( ( media_flags & LIBSMRAW_MEDIA_FLAG_PHYSICAL ) != 0 )
	{
		value_string        = "physical";
		value_string_length = 8;
	}
	else
	{
		value_string        = "logical";
		value_string_length = 7;
	}
	result = libfvalue_table_get_value_by_identifier(
	          internal_handle->media_values,
	          (uint8_t *) identifier,
	          identifier_size,
	          &value,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %s from media values table.",
		 function,
		 identifier );

		return( -1 );
	}
	else if( result == 0 )
	{
		if( libfvalue_value_type_initialize(
		     &value,
		     LIBFVALUE_VALUE_TYPE_STRING_UTF8,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create value.",
			 function );

			return( -1 );
		}
		if( libfvalue_value_set_identifier(
		     value,
		     (uint8_t *) identifier,
		     identifier_size,
		     LIBFVALUE_VALUE_FLAG_IDENTIFIER_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to set identifier in value.",
			 function );

			libfvalue_value_free(
			 &value,
			 NULL );

			return( -1 );
		}
		if( libfvalue_table_set_value(
		     internal_handle->media_values,
		     value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set value: %s in media values table.",
			 function,
			 identifier );

			libfvalue_value_free(
			 &value,
			 NULL );

			return( -1 );
		}
	}
	if( libfvalue_value_copy_from_utf8_string(
	     value,
	     0,
	     (uint8_t *) value_string,
	     value_string_length,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy value from an UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the number of information values
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_get_number_of_information_values(
     libsmraw_handle_t *handle,
     int *number_of_information_values,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_get_number_of_information_values";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file io pool.",
		 function );

		return( -1 );
	}
	if( libfvalue_table_get_number_of_values(
	     internal_handle->information_values,
	     number_of_information_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of information values.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of a specific information value identifier
 * The identifier size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_get_information_value_identifier_size(
     libsmraw_handle_t *handle,
     int information_value_index,
     size_t *identifier_size,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	libfvalue_value_t *value                    = NULL;
	uint8_t *value_identifier                   = NULL;
	static char *function                       = "libsmraw_handle_get_information_value_identifier_size";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file io pool.",
		 function );

		return( -1 );
	}
	if( libfvalue_table_get_value_by_index(
	     internal_handle->information_values,
	     information_value_index,
	     &value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d from information values table.",
		 function,
		 information_value_index );

		return( -1 );
	}
	if( libfvalue_value_get_identifier(
	     value,
	     &value_identifier,
	     identifier_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value identifier: %d.",
		 function,
		 information_value_index );

		return( -1 );
	}
	if( value_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing value identifier: %d.",
		 function,
		 information_value_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific information value identifier
 * The identifier size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_get_information_value_identifier(
     libsmraw_handle_t *handle,
     int information_value_index,
     uint8_t *identifier,
     size_t identifier_size,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	libfvalue_value_t *value                    = NULL;
	uint8_t *value_identifier                   = NULL;
	static char *function                       = "libsmraw_handle_get_information_value_identifier";
	size_t value_identifier_size                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file io pool.",
		 function );

		return( -1 );
	}
	if( libfvalue_table_get_value_by_index(
	     internal_handle->information_values,
	     information_value_index,
	     &value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d from information values table.",
		 function,
		 information_value_index );

		return( -1 );
	}
	if( libfvalue_value_get_identifier(
	     value,
	     &value_identifier,
	     &value_identifier_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value identifier: %d.",
		 function,
		 information_value_index );

		return( -1 );
	}
	if( value_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing value identifier: %d.",
		 function,
		 information_value_index );

		return( -1 );
	}
	if( identifier_size < value_identifier_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: identifier too small.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     identifier,
	     value_identifier,
	     value_identifier_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy identifier.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of an UTF-8 encoded information value for the specific identifier
 * The value size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libsmraw_handle_get_utf8_information_value_size(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	libfvalue_value_t *value                    = NULL;
	static char *function                       = "libsmraw_handle_get_utf8_information_value_size";
	int result                                  = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file io pool.",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
	result = libfvalue_table_get_value_by_identifier(
	          internal_handle->information_values,
	          identifier,
	          identifier_length + 1,
	          &value,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %s from information values table.",
		 function,
		 (char *) identifier );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfvalue_value_get_utf8_string_size(
		     value,
		     0,
		     utf8_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value: %s UTF-8 string size.",
			 function,
			 (char *) identifier );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves an UTF-8 encoded information value for the specific identifier
 * The value size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libsmraw_handle_get_utf8_information_value(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	libfvalue_value_t *value                    = NULL;
	static char *function                       = "libsmraw_handle_get_utf8_information_value";
	int result                                  = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file io pool.",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
	result = libfvalue_table_get_value_by_identifier(
	          internal_handle->information_values,
	          identifier,
	          identifier_length + 1,
	          &value,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %s from information values table.",
		 function,
		 (char *) identifier );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfvalue_value_copy_to_utf8_string(
		     value,
		     0,
		     utf8_string,
		     utf8_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy value: %s to an UTF-8 string.",
			 function,
			 (char *) identifier );

			return( -1 );
		}
	}
	return( result );
}

/* Sets an UTF-8 encoded information value for the specific identifier
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_set_utf8_information_value(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	libfvalue_value_t *value                    = NULL;
	static char *function                       = "libsmraw_handle_set_utf8_information_value";
	int result                                  = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->read_values_initialized != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: information values cannot be changed.",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
	result = libfvalue_table_get_value_by_identifier(
	          internal_handle->information_values,
	          identifier,
	          identifier_length + 1,
	          &value,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %s from information values table.",
		 function,
		 (char *) identifier );

		return( -1 );
	}
	else if( result == 0 )
	{
		if( libfvalue_value_type_initialize(
		     &value,
		     LIBFVALUE_VALUE_TYPE_STRING_UTF8,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create value.",
			 function );

			return( -1 );
		}
		if( libfvalue_value_set_identifier(
		     value,
		     identifier,
		     identifier_length + 1,
		     LIBFVALUE_VALUE_FLAG_IDENTIFIER_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to set identifier in value.",
			 function );

			libfvalue_value_free(
			 &value,
			 NULL );

			return( -1 );
		}
		if( libfvalue_table_set_value(
		     internal_handle->information_values,
		     value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set value: %s in information values table.",
			 function,
			 (char *) identifier );

			libfvalue_value_free(
			 &value,
			 NULL );

			return( -1 );
		}
	}
	if( libfvalue_value_copy_from_utf8_string(
	     value,
	     0,
	     utf8_string,
	     utf8_string_length + 1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy value: %s from an UTF-8 string.",
		 function,
		 (char *) identifier );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of an UTF-16 encoded information value for the specific identifier
 * The value size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libsmraw_handle_get_utf16_information_value_size(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	libfvalue_value_t *value                    = NULL;
	static char *function                       = "libsmraw_handle_get_utf16_information_value_size";
	int result                                  = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file io pool.",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
	result = libfvalue_table_get_value_by_identifier(
	          internal_handle->information_values,
	          identifier,
	          identifier_length + 1,
	          &value,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %s from information values table.",
		 function,
		 (char *) identifier );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfvalue_value_get_utf16_string_size(
		     value,
		     0,
		     utf16_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value: %s UTF-16 string size.",
			 function,
			 (char *) identifier );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves an UTF-16 encoded information value for the specific identifier
 * The value size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libsmraw_handle_get_utf16_information_value(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	libfvalue_value_t *value                    = NULL;
	static char *function                       = "libsmraw_handle_get_utf16_information_value";
	int result                                  = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file io pool.",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
	result = libfvalue_table_get_value_by_identifier(
	          internal_handle->information_values,
	          identifier,
	          identifier_length + 1,
	          &value,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %s from information values table.",
		 function,
		 (char *) identifier );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfvalue_value_copy_to_utf16_string(
		     value,
		     0,
		     utf16_string,
		     utf16_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy value: %s to an UTF-16 string.",
			 function,
			 (char *) identifier );

			return( -1 );
		}
	}
	return( result );
}

/* Sets an UTF-16 encoded information value for the specific identifier
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_set_utf16_information_value(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	libfvalue_value_t *value                    = NULL;
	static char *function                       = "libsmraw_handle_set_utf16_information_value";
	int result                                  = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->read_values_initialized != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: information values cannot be changed.",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
	result = libfvalue_table_get_value_by_identifier(
	          internal_handle->information_values,
	          identifier,
	          identifier_length + 1,
	          &value,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %s from information values table.",
		 function,
		 (char *) identifier );

		return( -1 );
	}
	else if( result == 0 )
	{
		if( libfvalue_value_type_initialize(
		     &value,
		     LIBFVALUE_VALUE_TYPE_STRING_UTF8,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create value.",
			 function );

			return( -1 );
		}
		if( libfvalue_value_set_identifier(
		     value,
		     identifier,
		     identifier_length + 1,
		     LIBFVALUE_VALUE_FLAG_IDENTIFIER_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to set identifier in value.",
			 function );

			libfvalue_value_free(
			 &value,
			 NULL );

			return( -1 );
		}
		if( libfvalue_table_set_value(
		     internal_handle->information_values,
		     value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set value: %s in information values table.",
			 function,
			 (char *) identifier );

			libfvalue_value_free(
			 &value,
			 NULL );

			return( -1 );
		}
	}
	if( libfvalue_value_copy_from_utf16_string(
	     value,
	     0,
	     utf16_string,
	     utf16_string_length + 1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy value: %s from an UTF-16 string.",
		 function,
		 (char *) identifier );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the number of integrity hash values
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_get_number_of_integrity_hash_values(
     libsmraw_handle_t *handle,
     int *number_of_integrity_hash_values,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_get_number_of_integrity_hash_values";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file io pool.",
		 function );

		return( -1 );
	}
	if( libfvalue_table_get_number_of_values(
	     internal_handle->integrity_hash_values,
	     number_of_integrity_hash_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of integrity hash values.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of a specific integrity hash value identifier
 * The identifier size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_get_integrity_hash_value_identifier_size(
     libsmraw_handle_t *handle,
     int integrity_hash_value_index,
     size_t *identifier_size,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	libfvalue_value_t *value                    = NULL;
	uint8_t *value_identifier                   = NULL;
	static char *function                       = "libsmraw_handle_get_integrity_hash_value_identifier_size";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file io pool.",
		 function );

		return( -1 );
	}
	if( libfvalue_table_get_value_by_index(
	     internal_handle->integrity_hash_values,
	     integrity_hash_value_index,
	     &value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d from integrity hash values table.",
		 function,
		 integrity_hash_value_index );

		return( -1 );
	}
	if( libfvalue_value_get_identifier(
	     value,
	     &value_identifier,
	     identifier_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value identifier: %d.",
		 function,
		 integrity_hash_value_index );

		return( -1 );
	}
	if( value_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing value identifier: %d.",
		 function,
		 integrity_hash_value_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific integrity hash value identifier
 * The identifier size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_get_integrity_hash_value_identifier(
     libsmraw_handle_t *handle,
     int integrity_hash_value_index,
     uint8_t *identifier,
     size_t identifier_size,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	libfvalue_value_t *value                    = NULL;
	uint8_t *value_identifier                   = NULL;
	static char *function                       = "libsmraw_handle_get_integrity_hash_value_identifier";
	size_t value_identifier_size                = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file io pool.",
		 function );

		return( -1 );
	}
	if( libfvalue_table_get_value_by_index(
	     internal_handle->integrity_hash_values,
	     integrity_hash_value_index,
	     &value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d from integrity hash values table.",
		 function,
		 integrity_hash_value_index );

		return( -1 );
	}
	if( libfvalue_value_get_identifier(
	     value,
	     &value_identifier,
	     &value_identifier_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value identifier: %d.",
		 function,
		 integrity_hash_value_index );

		return( -1 );
	}
	if( value_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing value identifier: %d.",
		 function,
		 integrity_hash_value_index );

		return( -1 );
	}
	if( identifier_size < value_identifier_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: identifier too small.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     identifier,
	     value_identifier,
	     value_identifier_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy identifier.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of an UTF-8 encoded integrity hash value for the specific identifier
 * The value size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libsmraw_handle_get_utf8_integrity_hash_value_size(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	libfvalue_value_t *value                    = NULL;
	static char *function                       = "libsmraw_handle_get_utf8_integrity_hash_value_size";
	int result                                  = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file io pool.",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
	result = libfvalue_table_get_value_by_identifier(
	          internal_handle->integrity_hash_values,
	          identifier,
	          identifier_length + 1,
	          &value,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %s from integrity hash values table.",
		 function,
		 (char *) identifier );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfvalue_value_get_utf8_string_size(
		     value,
		     0,
		     utf8_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value: %s UTF-8 string size.",
			 function,
			 (char *) identifier );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves an UTF-8 encoded integrity hash value for the specific identifier
 * The value size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libsmraw_handle_get_utf8_integrity_hash_value(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	libfvalue_value_t *value                    = NULL;
	static char *function                       = "libsmraw_handle_get_utf8_integrity_hash_value";
	int result                                  = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file io pool.",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
	result = libfvalue_table_get_value_by_identifier(
	          internal_handle->integrity_hash_values,
	          identifier,
	          identifier_length + 1,
	          &value,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %s from integrity hash values table.",
		 function,
		 (char *) identifier );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfvalue_value_copy_to_utf8_string(
		     value,
		     0,
		     utf8_string,
		     utf8_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy value: %s to an UTF-8 string.",
			 function,
			 (char *) identifier );

			return( -1 );
		}
	}
	return( result );
}

/* Sets an UTF-8 encoded integrity hash value for the specific identifier
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_set_utf8_integrity_hash_value(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	libfvalue_value_t *value                    = NULL;
	static char *function                       = "libsmraw_handle_set_utf8_integrity_hash_value";
	int result                                  = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->read_values_initialized != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: integrity hash values cannot be changed.",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
	result = libfvalue_table_get_value_by_identifier(
	          internal_handle->integrity_hash_values,
	          identifier,
	          identifier_length + 1,
	          &value,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %s from integrity hash values table.",
		 function,
		 (char *) identifier );

		return( -1 );
	}
	else if( result == 0 )
	{
		if( libfvalue_value_type_initialize(
		     &value,
		     LIBFVALUE_VALUE_TYPE_STRING_UTF8,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create value.",
			 function );

			return( -1 );
		}
		if( libfvalue_value_set_identifier(
		     value,
		     identifier,
		     identifier_length + 1,
		     LIBFVALUE_VALUE_FLAG_IDENTIFIER_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to set identifier in value.",
			 function );

			libfvalue_value_free(
			 &value,
			 NULL );

			return( -1 );
		}
		if( libfvalue_table_set_value(
		     internal_handle->integrity_hash_values,
		     value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set value: %s in integrity hash values table.",
			 function,
			 (char *) identifier );

			libfvalue_value_free(
			 &value,
			 NULL );

			return( -1 );
		}
	}
	if( libfvalue_value_copy_from_utf8_string(
	     value,
	     0,
	     utf8_string,
	     utf8_string_length + 1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy value: %s from an UTF-8 string.",
		 function,
		 (char *) identifier );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of an UTF-16 encoded integrity hash value for the specific identifier
 * The value size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libsmraw_handle_get_utf16_integrity_hash_value_size(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	libfvalue_value_t *value                    = NULL;
	static char *function                       = "libsmraw_handle_get_utf16_integrity_hash_value_size";
	int result                                  = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file io pool.",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
	result = libfvalue_table_get_value_by_identifier(
	          internal_handle->integrity_hash_values,
	          identifier,
	          identifier_length + 1,
	          &value,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %s from integrity hash values table.",
		 function,
		 (char *) identifier );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfvalue_value_get_utf16_string_size(
		     value,
		     0,
		     utf16_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value: %s UTF-16 string size.",
			 function,
			 (char *) identifier );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves an UTF-16 encoded integrity hash value for the specific identifier
 * The value size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libsmraw_handle_get_utf16_integrity_hash_value(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	libfvalue_value_t *value                    = NULL;
	static char *function                       = "libsmraw_handle_get_utf16_integrity_hash_value";
	int result                                  = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file io pool.",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
	result = libfvalue_table_get_value_by_identifier(
	          internal_handle->integrity_hash_values,
	          identifier,
	          identifier_length + 1,
	          &value,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %s from integrity hash values table.",
		 function,
		 (char *) identifier );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfvalue_value_copy_to_utf16_string(
		     value,
		     0,
		     utf16_string,
		     utf16_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy value: %s to an UTF-16 string.",
			 function,
			 (char *) identifier );

			return( -1 );
		}
	}
	return( result );
}

/* Sets an UTF-16 encoded integrity hash value for the specific identifier
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_handle_set_utf16_integrity_hash_value(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	libfvalue_value_t *value                    = NULL;
	static char *function                       = "libsmraw_handle_set_utf16_integrity_hash_value";
	int result                                  = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) handle;

	if( internal_handle->read_values_initialized != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: integrity hash values cannot be changed.",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
	result = libfvalue_table_get_value_by_identifier(
	          internal_handle->integrity_hash_values,
	          identifier,
	          identifier_length + 1,
	          &value,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %s from integrity hash values table.",
		 function,
		 (char *) identifier );

		return( -1 );
	}
	else if( result == 0 )
	{
		if( libfvalue_value_type_initialize(
		     &value,
		     LIBFVALUE_VALUE_TYPE_STRING_UTF8,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create value.",
			 function );

			return( -1 );
		}
		if( libfvalue_value_set_identifier(
		     value,
		     identifier,
		     identifier_length + 1,
		     LIBFVALUE_VALUE_FLAG_IDENTIFIER_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to set identifier in value.",
			 function );

			libfvalue_value_free(
			 &value,
			 NULL );

			return( -1 );
		}
		if( libfvalue_table_set_value(
		     internal_handle->integrity_hash_values,
		     value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set value: %s in integrity hash values table.",
			 function,
			 (char *) identifier );

			libfvalue_value_free(
			 &value,
			 NULL );

			return( -1 );
		}
	}
	if( libfvalue_value_copy_from_utf16_string(
	     value,
	     0,
	     utf16_string,
	     utf16_string_length + 1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy value: %s from an UTF-16 string.",
		 function,
		 (char *) identifier );

		return( -1 );
	}
	return( 1 );
}

