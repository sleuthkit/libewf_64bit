/*
 * Value functions
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

#include "libfvalue_binary_data.h"
#include "libfvalue_data_handle.h"
#include "libfvalue_definitions.h"
#include "libfvalue_floating_point.h"
#include "libfvalue_integer.h"
#include "libfvalue_libcerror.h"
#include "libfvalue_libcnotify.h"
#include "libfvalue_string.h"
#include "libfvalue_types.h"
#include "libfvalue_unused.h"
#include "libfvalue_value.h"
#include "libfvalue_value_type.h"

#if defined( HAVE_LIBFDATETIME_H ) || defined( HAVE_LOCAL_LIBFDATETIME )
#include "libfvalue_libfdatetime.h"
#endif

#if defined( HAVE_LIBFGUID_H ) || defined( HAVE_LOCAL_LIBFGUID )
#include "libfvalue_libfguid.h"
#endif

#if defined( HAVE_LIBFWNT_H ) || defined( HAVE_LOCAL_LIBFWNT )
#include "libfvalue_libfwnt.h"
#endif

const char *libfvalue_value_type_strings[ 27 ] = {
	NULL,

	"binary-data",

	"boolean",

	"enum",

	"int8",
	"uint8",
	"int16",
	"uint16",
	"int32",
	"uint32",
	"int64",
	"uint64",

	"float32",
	"float64",

	"byte-stream-string",
	"utf8-string",
	"utf16-string",
	"utf32-string",

	"fat-date-time",
	"filetime",
	"floatingtime",
	"nsf-timedate",
	"hfs-time",
	"posix-time",
	"systemtime",

	"guid",

	"nt-sid",
};

const char *libfvalue_value_type_descriptions[ 27 ] = {
	NULL,

	"Binary data",

	"Boolean",

	"Enumeration",

	"Integer 8-bit signed",
	"Integer 8-bit unsigned",
	"Integer 16-bit signed",
	"Integer 16-bit unsigned",
	"Integer 32-bit signed",
	"Integer 32-bit unsigned",
	"Integer 64-bit signed",
	"Integer 64-bit unsigned",

	"Floating point 32-bit (single precision)",
	"Floating point 64-bit (double precision)",

	"Byte stream string",
	"UTF-8 string",
	"UTF-16 string",
	"UTF-32 string",

	"FAT date time",
	"Filetime",
	"FLoatingtime",
	"NSF timedate",
	"HFS time",
	"POSIX time",
	"Systemtime",

	"Globally Unique Identifier (GUID)",

	"NT Security Identifier (SID)",
};

/* Initialize a value of a specific type
 * Returns 1 if successful or -1 on error
 */
int libfvalue_value_type_initialize(
     libfvalue_value_t **value,
     int type,
     libcerror_error_t **error )
{
	libfvalue_data_handle_t *data_handle = NULL;
	static char *function                = "libfvalue_value_type_initialize";

	if( libfvalue_data_handle_initialize(
	     &data_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create data handle.",
		 function );

		goto on_error;
	}
	if( libfvalue_value_type_initialize_with_data_handle(
	     value,
	     type,
	     (intptr_t *) data_handle,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libfvalue_data_handle_free,
	     (int (*)(intptr_t **, intptr_t *, libcerror_error_t **)) &libfvalue_data_handle_clone,

	     (int (*)(intptr_t *, uint8_t **, size_t *, int *, libcerror_error_t **)) &libfvalue_data_handle_get_data,
	     (int (*)(intptr_t *, const uint8_t *, size_t, int, uint8_t, libcerror_error_t **)) &libfvalue_data_handle_set_data,

	     (int (*)(intptr_t *, int *, libcerror_error_t **)) &libfvalue_data_handle_get_number_of_value_entries,
	     (int (*)(intptr_t *, int, uint8_t **, size_t *, int *, libcerror_error_t **)) &libfvalue_data_handle_get_value_entry,
	     (int (*)(intptr_t *, int, const uint8_t *, size_t, int, libcerror_error_t **)) &libfvalue_data_handle_set_value_entry,
	     (int (*)(intptr_t *, int *, const uint8_t *, size_t, int, libcerror_error_t **)) &libfvalue_data_handle_append_value_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create value with data handle.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( data_handle != NULL )
	{
		libfvalue_data_handle_free(
		 &data_handle,
		 NULL );
	}
	return( -1 );
}

/* Initialize a value of a specific type with a custom data handle
 * Returns 1 if successful or -1 on error
 */
int libfvalue_value_type_initialize_with_data_handle(
     libfvalue_value_t **value,
     int type,
     intptr_t *data_handle,
     int (*free_data_handle)(
           intptr_t **data_handle,
           libcerror_error_t **error ),
     int (*clone_data_handle)(
           intptr_t **destination_data_handle,
           intptr_t *source_data_handle,
           libcerror_error_t **error ),
     int (*get_data)(
           intptr_t *data_handle,
           uint8_t **data,
           size_t *data_size,
           int *encoding,
           libcerror_error_t **error ),
     int (*set_data)(
           intptr_t *data_handle,
           const uint8_t *data,
           size_t data_size,
           int encoding,
           uint8_t flags,
           libcerror_error_t **error ),
     int (*get_number_of_value_entries)(
           intptr_t *data_handle,
           int *number_of_value_entries,
           libcerror_error_t **error ),
     int (*get_value_entry)(
           intptr_t *data_handle,
           int value_entry_index,
           uint8_t **data,
           size_t *data_size,
           int *encoding,
           libcerror_error_t **error ),
     int (*set_value_entry)(
           intptr_t *data_handle,
           int value_entry_index,
           const uint8_t *data,
           size_t data_size,
           int encoding,
           libcerror_error_t **error ),
     int (*append_value_entry)(
           intptr_t *data_handle,
           int *value_entry_index,
           const uint8_t *data,
           size_t data_size,
           int encoding,
           libcerror_error_t **error ),
     libcerror_error_t **error )
{
	static char *function = "libfvalue_value_type_initialize_with_data_handle";
	int result            = 0;

	if( value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	switch( type )
	{
		case LIBFVALUE_VALUE_TYPE_BINARY_DATA:
			result = libfvalue_value_initialize(
			          value,
			          libfvalue_value_type_strings[ type ],
			          libfvalue_value_type_descriptions[ type ],

			          data_handle,
			          free_data_handle,
			          clone_data_handle,

			          get_data,
			          set_data,

			          get_number_of_value_entries,
			          get_value_entry,
			          set_value_entry,
			          append_value_entry,

			          (int (*)(intptr_t **, libcerror_error_t **)) &libfvalue_binary_data_initialize,
			          (int (*)(intptr_t **, libcerror_error_t **)) &libfvalue_binary_data_free,
			          (int (*)(intptr_t **, intptr_t *, libcerror_error_t **)) &libfvalue_binary_data_clone,

			          (int (*)(intptr_t *, const uint8_t *, size_t, int, libcerror_error_t **)) &libfvalue_binary_data_copy_from_byte_stream,
			          NULL,

			          NULL,
			          NULL,

			          NULL,
			          NULL,

			          NULL,
			          (int (*)(intptr_t *, size_t *, uint32_t, libcerror_error_t **)) &libfvalue_binary_data_get_string_size,
			          (int (*)(intptr_t *, uint8_t *, size_t, size_t *, uint32_t, libcerror_error_t **)) &libfvalue_binary_data_copy_to_utf8_string_with_index,

			          NULL,
			          (int (*)(intptr_t *, size_t *, uint32_t, libcerror_error_t **)) &libfvalue_binary_data_get_string_size,
			          (int (*)(intptr_t *, uint16_t *, size_t, size_t *, uint32_t, libcerror_error_t **)) &libfvalue_binary_data_copy_to_utf16_string_with_index,

			          NULL,
			          (int (*)(intptr_t *, size_t *, uint32_t, libcerror_error_t **)) &libfvalue_binary_data_get_string_size,
			          (int (*)(intptr_t *, uint32_t *, size_t, size_t *, uint32_t, libcerror_error_t **)) &libfvalue_binary_data_copy_to_utf32_string_with_index,

			          error );
			break;

		case LIBFVALUE_VALUE_TYPE_FLOATING_POINT_32BIT:
		case LIBFVALUE_VALUE_TYPE_FLOATING_POINT_64BIT:
			result = libfvalue_value_initialize(
			          value,
			          libfvalue_value_type_strings[ type ],
			          libfvalue_value_type_descriptions[ type ],

			          data_handle,
			          free_data_handle,
			          clone_data_handle,

			          get_data,
			          set_data,

			          get_number_of_value_entries,
			          get_value_entry,
			          set_value_entry,
			          append_value_entry,

			          (int (*)(intptr_t **, libcerror_error_t **)) &libfvalue_floating_point_initialize,
			          (int (*)(intptr_t **, libcerror_error_t **)) &libfvalue_floating_point_free,
			          (int (*)(intptr_t **, intptr_t *, libcerror_error_t **)) &libfvalue_floating_point_clone,

			          (int (*)(intptr_t *, const uint8_t *, size_t, int, libcerror_error_t **)) &libfvalue_floating_point_copy_from_byte_stream,
			          NULL,

			          (int (*)(intptr_t *, uint64_t, size_t, libcerror_error_t **)) &libfvalue_floating_point_copy_from_integer,
			          (int (*)(intptr_t *, uint64_t *, size_t *, libcerror_error_t **)) &libfvalue_floating_point_copy_to_integer,

			          NULL,
			          NULL,

			          (int (*)(intptr_t *, const uint8_t *, size_t, size_t *, uint32_t, libcerror_error_t **)) &libfvalue_floating_point_copy_from_utf8_string_with_index,
			          (int (*)(intptr_t *, size_t *, uint32_t, libcerror_error_t **)) &libfvalue_floating_point_get_string_size,
			          (int (*)(intptr_t *, uint8_t *, size_t, size_t *, uint32_t, libcerror_error_t **)) &libfvalue_floating_point_copy_to_utf8_string_with_index,

			          (int (*)(intptr_t *, const uint16_t *, size_t, size_t *, uint32_t, libcerror_error_t **)) &libfvalue_floating_point_copy_from_utf16_string_with_index,
			          (int (*)(intptr_t *, size_t *, uint32_t, libcerror_error_t **)) &libfvalue_floating_point_get_string_size,
			          (int (*)(intptr_t *, uint16_t *, size_t, size_t *, uint32_t, libcerror_error_t **)) &libfvalue_floating_point_copy_to_utf16_string_with_index,

			          (int (*)(intptr_t *, const uint32_t *, size_t, size_t *, uint32_t, libcerror_error_t **)) &libfvalue_floating_point_copy_from_utf32_string_with_index,
			          (int (*)(intptr_t *, size_t *, uint32_t, libcerror_error_t **)) &libfvalue_floating_point_get_string_size,
			          (int (*)(intptr_t *, uint32_t *, size_t, size_t *, uint32_t, libcerror_error_t **)) &libfvalue_floating_point_copy_to_utf32_string_with_index,

			          error );
			break;

		case LIBFVALUE_VALUE_TYPE_BOOLEAN:
		case LIBFVALUE_VALUE_TYPE_INTEGER_8BIT:
		case LIBFVALUE_VALUE_TYPE_UNSIGNED_INTEGER_8BIT:
		case LIBFVALUE_VALUE_TYPE_INTEGER_16BIT:
		case LIBFVALUE_VALUE_TYPE_UNSIGNED_INTEGER_16BIT:
		case LIBFVALUE_VALUE_TYPE_INTEGER_32BIT:
		case LIBFVALUE_VALUE_TYPE_UNSIGNED_INTEGER_32BIT:
		case LIBFVALUE_VALUE_TYPE_INTEGER_64BIT:
		case LIBFVALUE_VALUE_TYPE_UNSIGNED_INTEGER_64BIT:
			result = libfvalue_value_initialize(
			          value,
			          libfvalue_value_type_strings[ type ],
			          libfvalue_value_type_descriptions[ type ],

			          data_handle,
			          free_data_handle,
			          clone_data_handle,

			          get_data,
			          set_data,

			          get_number_of_value_entries,
			          get_value_entry,
			          set_value_entry,
			          append_value_entry,

			          (int (*)(intptr_t **, libcerror_error_t **)) &libfvalue_integer_initialize,
			          (int (*)(intptr_t **, libcerror_error_t **)) &libfvalue_integer_free,
			          (int (*)(intptr_t **, intptr_t *, libcerror_error_t **)) &libfvalue_integer_clone,

			          (int (*)(intptr_t *, const uint8_t *, size_t, int, libcerror_error_t **)) &libfvalue_integer_copy_from_byte_stream,
			          NULL,

			          (int (*)(intptr_t *, uint64_t, size_t, libcerror_error_t **)) &libfvalue_integer_copy_from_integer,
			          (int (*)(intptr_t *, uint64_t *, size_t *, libcerror_error_t **)) &libfvalue_integer_copy_to_integer,

			          NULL,
			          NULL,

			          (int (*)(intptr_t *, const uint8_t *, size_t, size_t *, uint32_t, libcerror_error_t **)) &libfvalue_integer_copy_from_utf8_string_with_index,
			          (int (*)(intptr_t *, size_t *, uint32_t, libcerror_error_t **)) &libfvalue_integer_get_string_size,
			          (int (*)(intptr_t *, uint8_t *, size_t, size_t *, uint32_t, libcerror_error_t **)) &libfvalue_integer_copy_to_utf8_string_with_index,

			          (int (*)(intptr_t *, const uint16_t *, size_t, size_t *, uint32_t, libcerror_error_t **)) &libfvalue_integer_copy_from_utf16_string_with_index,
			          (int (*)(intptr_t *, size_t *, uint32_t, libcerror_error_t **)) &libfvalue_integer_get_string_size,
			          (int (*)(intptr_t *, uint16_t *, size_t, size_t *, uint32_t, libcerror_error_t **)) &libfvalue_integer_copy_to_utf16_string_with_index,

			          (int (*)(intptr_t *, const uint32_t *, size_t, size_t *, uint32_t, libcerror_error_t **)) &libfvalue_integer_copy_from_utf32_string_with_index,
			          (int (*)(intptr_t *, size_t *, uint32_t, libcerror_error_t **)) &libfvalue_integer_get_string_size,
			          (int (*)(intptr_t *, uint32_t *, size_t, size_t *, uint32_t, libcerror_error_t **)) &libfvalue_integer_copy_to_utf32_string_with_index,

			          error );
			break;

		case LIBFVALUE_VALUE_TYPE_STRING_BYTE_STREAM:
		case LIBFVALUE_VALUE_TYPE_STRING_UTF8:
		case LIBFVALUE_VALUE_TYPE_STRING_UTF16:
		case LIBFVALUE_VALUE_TYPE_STRING_UTF32:
			result = libfvalue_value_initialize(
			          value,
			          libfvalue_value_type_strings[ type ],
			          libfvalue_value_type_descriptions[ type ],

			          data_handle,
			          free_data_handle,
			          clone_data_handle,

			          get_data,
			          set_data,

			          get_number_of_value_entries,
			          get_value_entry,
			          set_value_entry,
			          append_value_entry,

			          (int (*)(intptr_t **, libcerror_error_t **)) &libfvalue_string_initialize,
			          (int (*)(intptr_t **, libcerror_error_t **)) &libfvalue_string_free,
			          (int (*)(intptr_t **, intptr_t *, libcerror_error_t **)) &libfvalue_string_clone,

			          (int (*)(intptr_t *, const uint8_t *, size_t, int, libcerror_error_t **)) &libfvalue_string_copy_from_byte_stream,
			          NULL,

			          NULL,
			          NULL,

			          NULL,
			          NULL,

			          (int (*)(intptr_t *, const uint8_t *, size_t, size_t *, uint32_t, libcerror_error_t **)) &libfvalue_string_copy_from_utf8_string_with_index,
			          (int (*)(intptr_t *, size_t *, uint32_t, libcerror_error_t **)) &libfvalue_string_get_utf8_string_size,
			          (int (*)(intptr_t *, uint8_t *, size_t, size_t *, uint32_t, libcerror_error_t **)) &libfvalue_string_copy_to_utf8_string_with_index,

			          (int (*)(intptr_t *, const uint16_t *, size_t, size_t *, uint32_t, libcerror_error_t **)) &libfvalue_string_copy_from_utf16_string_with_index,
			          (int (*)(intptr_t *, size_t *, uint32_t, libcerror_error_t **)) &libfvalue_string_get_utf16_string_size,
			          (int (*)(intptr_t *, uint16_t *, size_t, size_t *, uint32_t, libcerror_error_t **)) &libfvalue_string_copy_to_utf16_string_with_index,

			          (int (*)(intptr_t *, const uint32_t *, size_t, size_t *, uint32_t, libcerror_error_t **)) &libfvalue_string_copy_from_utf32_string_with_index,
			          (int (*)(intptr_t *, size_t *, uint32_t, libcerror_error_t **)) &libfvalue_string_get_utf32_string_size,
			          (int (*)(intptr_t *, uint32_t *, size_t, size_t *, uint32_t, libcerror_error_t **)) &libfvalue_string_copy_to_utf32_string_with_index,

			          error );
			break;

#if defined( HAVE_LIBFDATETIME_H ) || defined( HAVE_LOCAL_LIBFDATETIME )
		case LIBFVALUE_VALUE_TYPE_FAT_DATE_TIME:
			result = libfvalue_value_initialize(
			          value,
			          libfvalue_value_type_strings[ type ],
			          libfvalue_value_type_descriptions[ type ],

			          data_handle,
			          free_data_handle,
			          clone_data_handle,

			          get_data,
			          set_data,

			          get_number_of_value_entries,
			          get_value_entry,
			          set_value_entry,
			          append_value_entry,

			          (int (*)(intptr_t **, libcerror_error_t **)) &libfdatetime_fat_date_time_initialize,
			          (int (*)(intptr_t **, libcerror_error_t **)) &libfdatetime_fat_date_time_free,
			          NULL,

			          (int (*)(intptr_t *, const uint8_t *, size_t, int, libcerror_error_t **)) &libfdatetime_fat_date_time_copy_from_byte_stream,
			          NULL,

			          NULL,
			          NULL,

			          NULL,
			          NULL,

			          NULL,
			          (int (*)(intptr_t *, size_t *, uint32_t, libcerror_error_t **)) &libfdatetime_fat_date_time_get_string_size,
			          (int (*)(intptr_t *, uint8_t *, size_t, size_t *, uint32_t, libcerror_error_t **)) &libfdatetime_fat_date_time_copy_to_utf8_string_with_index,

			          NULL,
			          (int (*)(intptr_t *, size_t *, uint32_t, libcerror_error_t **)) &libfdatetime_fat_date_time_get_string_size,
			          (int (*)(intptr_t *, uint16_t *, size_t, size_t *, uint32_t, libcerror_error_t **)) &libfdatetime_fat_date_time_copy_to_utf16_string_with_index,

			          NULL,
			          (int (*)(intptr_t *, size_t *, uint32_t, libcerror_error_t **)) &libfdatetime_fat_date_time_get_string_size,
			          (int (*)(intptr_t *, uint32_t *, size_t, size_t *, uint32_t, libcerror_error_t **)) &libfdatetime_fat_date_time_copy_to_utf32_string_with_index,

			          error );
			break;

		case LIBFVALUE_VALUE_TYPE_FILETIME:
			result = libfvalue_value_initialize(
			          value,
			          libfvalue_value_type_strings[ type ],
			          libfvalue_value_type_descriptions[ type ],

			          data_handle,
			          free_data_handle,
			          clone_data_handle,

			          get_data,
			          set_data,

			          get_number_of_value_entries,
			          get_value_entry,
			          set_value_entry,
			          append_value_entry,

			          (int (*)(intptr_t **, libcerror_error_t **)) &libfdatetime_filetime_initialize,
			          (int (*)(intptr_t **, libcerror_error_t **)) &libfdatetime_filetime_free,
			          NULL,

			          (int (*)(intptr_t *, const uint8_t *, size_t, int, libcerror_error_t **)) &libfdatetime_filetime_copy_from_byte_stream,
			          NULL,

			          NULL,
			          NULL,

			          NULL,
			          NULL,

			          NULL,
			          (int (*)(intptr_t *, size_t *, uint32_t, libcerror_error_t **)) &libfdatetime_filetime_get_string_size,
			          (int (*)(intptr_t *, uint8_t *, size_t, size_t *, uint32_t, libcerror_error_t **)) &libfdatetime_filetime_copy_to_utf8_string_with_index,

			          NULL,
			          (int (*)(intptr_t *, size_t *, uint32_t, libcerror_error_t **)) &libfdatetime_filetime_get_string_size,
			          (int (*)(intptr_t *, uint16_t *, size_t, size_t *, uint32_t, libcerror_error_t **)) &libfdatetime_filetime_copy_to_utf16_string_with_index,

			          NULL,
			          (int (*)(intptr_t *, size_t *, uint32_t, libcerror_error_t **)) &libfdatetime_filetime_get_string_size,
			          (int (*)(intptr_t *, uint32_t *, size_t, size_t *, uint32_t, libcerror_error_t **)) &libfdatetime_filetime_copy_to_utf32_string_with_index,

			          error );
			break;

		case LIBFVALUE_VALUE_TYPE_FLOATINGTIME:
			result = libfvalue_value_initialize(
			          value,
			          libfvalue_value_type_strings[ type ],
			          libfvalue_value_type_descriptions[ type ],

			          data_handle,
			          free_data_handle,
			          clone_data_handle,

			          get_data,
			          set_data,

			          get_number_of_value_entries,
			          get_value_entry,
			          set_value_entry,
			          append_value_entry,

			          (int (*)(intptr_t **, libcerror_error_t **)) &libfdatetime_floatingtime_initialize,
			          (int (*)(intptr_t **, libcerror_error_t **)) &libfdatetime_floatingtime_free,
			          NULL,

			          (int (*)(intptr_t *, const uint8_t *, size_t, int, libcerror_error_t **)) &libfdatetime_floatingtime_copy_from_byte_stream,
			          NULL,

			          NULL,
			          NULL,

			          NULL,
			          NULL,

			          NULL,
			          (int (*)(intptr_t *, size_t *, uint32_t, libcerror_error_t **)) &libfdatetime_floatingtime_get_string_size,
			          (int (*)(intptr_t *, uint8_t *, size_t, size_t *, uint32_t, libcerror_error_t **)) &libfdatetime_floatingtime_copy_to_utf8_string_with_index,

			          NULL,
			          (int (*)(intptr_t *, size_t *, uint32_t, libcerror_error_t **)) &libfdatetime_floatingtime_get_string_size,
			          (int (*)(intptr_t *, uint16_t *, size_t, size_t *, uint32_t, libcerror_error_t **)) &libfdatetime_floatingtime_copy_to_utf16_string_with_index,

			          NULL,
			          (int (*)(intptr_t *, size_t *, uint32_t, libcerror_error_t **)) &libfdatetime_floatingtime_get_string_size,
			          (int (*)(intptr_t *, uint32_t *, size_t, size_t *, uint32_t, libcerror_error_t **)) &libfdatetime_floatingtime_copy_to_utf32_string_with_index,

			          error );
			break;

		case LIBFVALUE_VALUE_TYPE_NSF_TIMEDATE:
			result = libfvalue_value_initialize(
			          value,
			          libfvalue_value_type_strings[ type ],
			          libfvalue_value_type_descriptions[ type ],

			          data_handle,
			          free_data_handle,
			          clone_data_handle,

			          get_data,
			          set_data,

			          get_number_of_value_entries,
			          get_value_entry,
			          set_value_entry,
			          append_value_entry,

			          (int (*)(intptr_t **, libcerror_error_t **)) &libfdatetime_nsf_timedate_initialize,
			          (int (*)(intptr_t **, libcerror_error_t **)) &libfdatetime_nsf_timedate_free,
			          NULL,

			          (int (*)(intptr_t *, const uint8_t *, size_t, int, libcerror_error_t **)) &libfdatetime_nsf_timedate_copy_from_byte_stream,
			          NULL,

			          NULL,
			          NULL,

			          NULL,
			          NULL,

			          NULL,
			          (int (*)(intptr_t *, size_t *, uint32_t, libcerror_error_t **)) &libfdatetime_nsf_timedate_get_string_size,
			          (int (*)(intptr_t *, uint8_t *, size_t, size_t *, uint32_t, libcerror_error_t **)) &libfdatetime_nsf_timedate_copy_to_utf8_string_with_index,

			          NULL,
			          (int (*)(intptr_t *, size_t *, uint32_t, libcerror_error_t **)) &libfdatetime_nsf_timedate_get_string_size,
			          (int (*)(intptr_t *, uint16_t *, size_t, size_t *, uint32_t, libcerror_error_t **)) &libfdatetime_nsf_timedate_copy_to_utf16_string_with_index,

			          NULL,
			          (int (*)(intptr_t *, size_t *, uint32_t, libcerror_error_t **)) &libfdatetime_nsf_timedate_get_string_size,
			          (int (*)(intptr_t *, uint32_t *, size_t, size_t *, uint32_t, libcerror_error_t **)) &libfdatetime_nsf_timedate_copy_to_utf32_string_with_index,

			          error );
			break;

		case LIBFVALUE_VALUE_TYPE_POSIX_TIME:
			result = libfvalue_value_initialize(
			          value,
			          libfvalue_value_type_strings[ type ],
			          libfvalue_value_type_descriptions[ type ],

			          data_handle,
			          free_data_handle,
			          clone_data_handle,

			          get_data,
			          set_data,

			          get_number_of_value_entries,
			          get_value_entry,
			          set_value_entry,
			          append_value_entry,

			          (int (*)(intptr_t **, libcerror_error_t **)) &libfdatetime_posix_time_initialize,
			          (int (*)(intptr_t **, libcerror_error_t **)) &libfdatetime_posix_time_free,
			          NULL,

			          (int (*)(intptr_t *, const uint8_t *, size_t, int, libcerror_error_t **)) &libfdatetime_posix_time_copy_from_byte_stream,
			          NULL,

			          NULL,
			          NULL,

			          NULL,
			          NULL,

			          NULL,
			          (int (*)(intptr_t *, size_t *, uint32_t, libcerror_error_t **)) &libfdatetime_posix_time_get_string_size,
			          (int (*)(intptr_t *, uint8_t *, size_t, size_t *, uint32_t, libcerror_error_t **)) &libfdatetime_posix_time_copy_to_utf8_string_with_index,

			          NULL,
			          (int (*)(intptr_t *, size_t *, uint32_t, libcerror_error_t **)) &libfdatetime_posix_time_get_string_size,
			          (int (*)(intptr_t *, uint16_t *, size_t, size_t *, uint32_t, libcerror_error_t **)) &libfdatetime_posix_time_copy_to_utf16_string_with_index,

			          NULL,
			          (int (*)(intptr_t *, size_t *, uint32_t, libcerror_error_t **)) &libfdatetime_posix_time_get_string_size,
			          (int (*)(intptr_t *, uint32_t *, size_t, size_t *, uint32_t, libcerror_error_t **)) &libfdatetime_posix_time_copy_to_utf32_string_with_index,

			          error );
			break;

		case LIBFVALUE_VALUE_TYPE_SYSTEMTIME:
			result = libfvalue_value_initialize(
			          value,
			          libfvalue_value_type_strings[ type ],
			          libfvalue_value_type_descriptions[ type ],

			          data_handle,
			          free_data_handle,
			          clone_data_handle,

			          get_data,
			          set_data,

			          get_number_of_value_entries,
			          get_value_entry,
			          set_value_entry,
			          append_value_entry,

			          (int (*)(intptr_t **, libcerror_error_t **)) &libfdatetime_systemtime_initialize,
			          (int (*)(intptr_t **, libcerror_error_t **)) &libfdatetime_systemtime_free,
			          NULL,

			          (int (*)(intptr_t *, const uint8_t *, size_t, int, libcerror_error_t **)) &libfdatetime_systemtime_copy_from_byte_stream,
			          NULL,

			          NULL,
			          NULL,

			          NULL,
			          NULL,

			          NULL,
			          (int (*)(intptr_t *, size_t *, uint32_t, libcerror_error_t **)) &libfdatetime_systemtime_get_string_size,
			          (int (*)(intptr_t *, uint8_t *, size_t, size_t *, uint32_t, libcerror_error_t **)) &libfdatetime_systemtime_copy_to_utf8_string_with_index,

			          NULL,
			          (int (*)(intptr_t *, size_t *, uint32_t, libcerror_error_t **)) &libfdatetime_systemtime_get_string_size,
			          (int (*)(intptr_t *, uint16_t *, size_t, size_t *, uint32_t, libcerror_error_t **)) &libfdatetime_systemtime_copy_to_utf16_string_with_index,

			          NULL,
			          (int (*)(intptr_t *, size_t *, uint32_t, libcerror_error_t **)) &libfdatetime_systemtime_get_string_size,
			          (int (*)(intptr_t *, uint32_t *, size_t, size_t *, uint32_t, libcerror_error_t **)) &libfdatetime_systemtime_copy_to_utf32_string_with_index,

			          error );
			break;
#endif
#if defined( HAVE_LIBFGUID_H ) || defined( HAVE_LOCAL_LIBFGUID )
		case LIBFVALUE_VALUE_TYPE_GUID:
			result = libfvalue_value_initialize(
			          value,
			          libfvalue_value_type_strings[ type ],
			          libfvalue_value_type_descriptions[ type ],

			          data_handle,
			          free_data_handle,
			          clone_data_handle,

			          get_data,
			          set_data,

			          get_number_of_value_entries,
			          get_value_entry,
			          set_value_entry,
			          append_value_entry,

			          (int (*)(intptr_t **, libcerror_error_t **)) &libfguid_identifier_initialize,
			          (int (*)(intptr_t **, libcerror_error_t **)) &libfguid_identifier_free,
			          NULL,

			          (int (*)(intptr_t *, const uint8_t *, size_t, int, libcerror_error_t **)) &libfguid_identifier_copy_from_byte_stream,
			          (int (*)(intptr_t *, uint8_t *, size_t, int, libcerror_error_t **)) &libfguid_identifier_copy_to_byte_stream,

			          NULL,
			          NULL,

			          NULL,
			          NULL,

			          (int (*)(intptr_t *, const uint8_t *, size_t, size_t *, uint32_t, libcerror_error_t **)) &libfguid_identifier_copy_from_utf8_string_with_index,
			          (int (*)(intptr_t *, size_t *, uint32_t, libcerror_error_t **)) &libfguid_identifier_get_string_size,
			          (int (*)(intptr_t *, uint8_t *, size_t, size_t *, uint32_t, libcerror_error_t **)) &libfguid_identifier_copy_to_utf8_string_with_index,

			          (int (*)(intptr_t *, const uint16_t *, size_t, size_t *, uint32_t, libcerror_error_t **)) &libfguid_identifier_copy_from_utf16_string_with_index,
			          (int (*)(intptr_t *, size_t *, uint32_t, libcerror_error_t **)) &libfguid_identifier_get_string_size,
			          (int (*)(intptr_t *, uint16_t *, size_t, size_t *, uint32_t, libcerror_error_t **)) &libfguid_identifier_copy_to_utf16_string_with_index,

			          (int (*)(intptr_t *, const uint32_t *, size_t, size_t *, uint32_t, libcerror_error_t **)) &libfguid_identifier_copy_from_utf32_string_with_index,
			          (int (*)(intptr_t *, size_t *, uint32_t, libcerror_error_t **)) &libfguid_identifier_get_string_size,
			          (int (*)(intptr_t *, uint32_t *, size_t, size_t *, uint32_t, libcerror_error_t **)) &libfguid_identifier_copy_to_utf32_string_with_index,

			          error );
			break;
#endif
#if defined( HAVE_LIBFWNT_H ) || defined( HAVE_LOCAL_LIBFWNT )
		case LIBFVALUE_VALUE_TYPE_NT_SECURITY_IDENTIFIER:
			result = libfvalue_value_initialize(
			          value,
			          libfvalue_value_type_strings[ type ],
			          libfvalue_value_type_descriptions[ type ],

			          data_handle,
			          free_data_handle,
			          clone_data_handle,

			          get_data,
			          set_data,

			          get_number_of_value_entries,
			          get_value_entry,
			          set_value_entry,
			          append_value_entry,

			          (int (*)(intptr_t **, libcerror_error_t **)) &libfwnt_security_identifier_initialize,
			          (int (*)(intptr_t **, libcerror_error_t **)) &libfwnt_security_identifier_free,
			          NULL,

			          (int (*)(intptr_t *, const uint8_t *, size_t, int, libcerror_error_t **)) &libfwnt_security_identifier_copy_from_byte_stream,
			          NULL,

			          NULL,
			          NULL,

			          NULL,
			          NULL,

			          NULL,
			          (int (*)(intptr_t *, size_t *, uint32_t, libcerror_error_t **)) &libfwnt_security_identifier_get_string_size,
			          (int (*)(intptr_t *, uint8_t *, size_t, size_t *, uint32_t, libcerror_error_t **)) &libfwnt_security_identifier_copy_to_utf8_string_with_index,

			          NULL,
			          (int (*)(intptr_t *, size_t *, uint32_t, libcerror_error_t **)) &libfwnt_security_identifier_get_string_size,
			          (int (*)(intptr_t *, uint16_t *, size_t, size_t *, uint32_t, libcerror_error_t **)) &libfwnt_security_identifier_copy_to_utf16_string_with_index,

			          NULL,
			          (int (*)(intptr_t *, size_t *, uint32_t, libcerror_error_t **)) &libfwnt_security_identifier_get_string_size,
			          (int (*)(intptr_t *, uint32_t *, size_t, size_t *, uint32_t, libcerror_error_t **)) &libfwnt_security_identifier_copy_to_utf32_string_with_index,

			          error );
			break;
#endif
		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported type: 0x%02" PRIx8 ".",
			 function,
			 type );

			break;
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create value.",
		 function );

		return( -1 );
	}
	if( *value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing value.",
		 function );

		return( -1 );
	}
	( (libfvalue_internal_value_t *) *value )->type = type;

	return( 1 );
}

/* Determines the size of a variable sized string
 * This function will look for a end-of-string character as the indicator
 * of the end of the string upto data size
 * Returns data size of the string if successful or -1 on error
 */
ssize_t libfvalue_value_type_get_string_size(
         libfvalue_value_t *value,
         const uint8_t *data,
         size_t data_size,
         libcerror_error_t **error )
{
	libfvalue_internal_value_t *internal_value = NULL;
	static char *function                      = "libfvalue_value_type_get_string_size";
	size_t data_index                          = 0;

	if( value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	internal_value = (libfvalue_internal_value_t *) value;

	if( ( internal_value->type != LIBFVALUE_VALUE_TYPE_STRING_BYTE_STREAM )
	 && ( internal_value->type != LIBFVALUE_VALUE_TYPE_STRING_UTF8 )
	 && ( internal_value->type != LIBFVALUE_VALUE_TYPE_STRING_UTF16 )
	 && ( internal_value->type != LIBFVALUE_VALUE_TYPE_STRING_UTF32 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported type: 0x%02" PRIx8 ".",
		 function,
		 internal_value->type );

		return( -1 );
	}
	if( data != NULL )
	{
		if( ( internal_value->type == LIBFVALUE_VALUE_TYPE_STRING_BYTE_STREAM )
		 || ( internal_value->type == LIBFVALUE_VALUE_TYPE_STRING_UTF8 ) )
		{
			while( data_index < data_size )
			{
				if( data[ data_index ] == 0 )
				{
					data_index += 1;

					break;
				}
				data_index += 1;
			}
		}
		else if( internal_value->type == LIBFVALUE_VALUE_TYPE_STRING_UTF16 )
		{
			while( ( data_index + 1 ) < data_size )
			{
				if( ( data[ data_index ] == 0 )
				 && ( data[ data_index + 1 ] == 0 ) )
				{
					data_index += 2;

					break;
				}
				data_index += 2;
			}
		}
		else if( internal_value->type == LIBFVALUE_VALUE_TYPE_STRING_UTF32 )
		{
			while( ( data_index + 3 ) < data_size )
			{
				if( ( data[ data_index ] == 0 )
				 && ( data[ data_index + 1 ] == 0 )
				 && ( data[ data_index + 2 ] == 0 )
				 && ( data[ data_index + 3 ] == 0 ) )
				{
					data_index += 4;

					break;
				}
				data_index += 4;
			}
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: string data:\n",
			 function );
			libcnotify_print_data(
			 data,
			 data_index,
			 0 );
		}
#endif
	}
	return( (ssize_t) data_index );
}

/* Sets the data of a variable sized string
 * This function will look for a end-of-string character as the indicator
 * of the end of the string upto data size
 * Returns data size of the string if successful or -1 on error
 */
ssize_t libfvalue_value_type_set_data_string(
         libfvalue_value_t *value,
         const uint8_t *data,
         size_t data_size,
         int encoding,
         uint8_t flags,
         libcerror_error_t **error )
{
	static char *function = "libfvalue_value_type_set_data_string";
	ssize_t data_index    = 0;

	if( value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	data_index = libfvalue_value_type_get_string_size(
	              value,
	              data,
	              data_size,
	              error );

	if( data_index == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to determine data string size.",
		 function );

		return( -1 );
	}
	if( libfvalue_value_set_data(
	     value,
	     data,
	     (size_t) data_index,
	     encoding,
	     flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set value entry.",
		 function );

		return( -1 );
	}
	return( data_index );
}

/* Append the data of a variable sized string
 * This function will look for a end-of-string character as the indicator
 * of the end of the string upto data size
 * Returns data size of the string if successful or -1 on error
 */
ssize_t libfvalue_value_type_append_data_string(
         libfvalue_value_t *value,
         const uint8_t *data,
         size_t data_size,
         int encoding,
         libcerror_error_t **error )
{
	static char *function = "libfvalue_value_type_append_data_string";
	ssize_t data_index    = 0;
	int value_entry_index = 0;

	if( value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	data_index = libfvalue_value_type_get_string_size(
	              value,
	              data,
	              data_size,
	              error );

	if( data_index == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to determine data string size.",
		 function );

		return( -1 );
	}
	if( libfvalue_value_append_entry_data(
	     value,
	     &value_entry_index,
	     data,
	     (size_t) data_index,
	     encoding,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append value entry.",
		 function );

		return( -1 );
	}
	return( data_index );
}

/* Sets the data of an array of variable sized strings
 * This function will look for a NUL-character as the indicator of
 * the end of the string upto data_size
 * Returns data size of the string array if successful or -1 on error
 */
ssize_t libfvalue_value_type_set_data_strings_array(
         libfvalue_value_t *value,
         const uint8_t *data,
         size_t data_size,
         int encoding,
         libcerror_error_t **error )
{
	static char *function   = "libfvalue_value_type_set_data_strings_array";
	ssize_t data_index      = 0;
	ssize_t last_data_index = 0;
	int value_entry_index   = 0;

	if( value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	while( (size_t) last_data_index < data_size )
	{
		data_index = libfvalue_value_type_get_string_size(
			      value,
		              &( data[ last_data_index ] ),
		              data_size - last_data_index,
			      error );

		if( data_index == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to determine data string size.",
			 function );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: strings array value entry: %d data:\n",
			 function,
			 value_entry_index );
			libcnotify_print_data(
			 &( data[ last_data_index ] ),
			 (size_t) data_index,
			 0 );
		}
#endif
		if( libfvalue_value_append_entry_data(
		     value,
		     &value_entry_index,
		     &( data[ last_data_index ] ),
		     (size_t) data_index,
		     encoding,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append value entry.",
			 function );

			return( -1 );
		}
		last_data_index += data_index;
	}
	return( last_data_index );
}

