/*
 * Filename functions
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
#include <file_stream.h>
#include <memory.h>
#include <types.h>

#include "libsmraw_definitions.h"
#include "libsmraw_handle.h"
#include "libsmraw_information_file.h"
#include "libsmraw_libbfio.h"
#include "libsmraw_libcerror.h"
#include "libsmraw_libcnotify.h"
#include "libsmraw_libcstring.h"
#include "libsmraw_handle.h"
#include "libsmraw_types.h"

/* Creates a (split) RAW filename
 * Returns 1 if successful or -1 on error
 */
int libsmraw_filename_create(
     libcstring_system_character_t **filename,
     size_t *filename_size,
     libcstring_system_character_t *basename,
     size_t basename_size,
     int total_number_of_segments,
     int current_file_io_pool_entry,
     libcerror_error_t **error )
{
	static char *function    = "libsmraw_filename_create";
	size_t additional_length = 0;
	size_t filename_index    = 0;

	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment filename.",
		 function );

		return( -1 );
	}
	if( *filename != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid segment filename already set.",
		 function );

		return( -1 );
	}
	if( filename_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment filename size.",
		 function );

		return( -1 );
	}
	if( basename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid basename.",
		 function );

		return( -1 );
	}
	if( basename_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid basename size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( total_number_of_segments < 0 )
	 || ( total_number_of_segments >= 1000 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid maximum pool entry value out of bounds.",
		 function );

		return( -1 );
	}
	if( total_number_of_segments > 0 )
	{
		if( ( current_file_io_pool_entry < 0 )
		 || ( current_file_io_pool_entry > total_number_of_segments ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid current pool entry value out of bounds.",
			 function );

			return( -1 );
		}
	}
	if( total_number_of_segments == 1 )
	{
		additional_length = 4;
	}
	else
	{
		additional_length = 8;
	}
	*filename_size = basename_size + additional_length;

	*filename = libcstring_system_string_allocate(
	             *filename_size );

	if( *filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create segment filename.",
		 function );

		goto on_error;
	}
	if( libcstring_system_string_copy(
	     *filename,
	     basename,
	     basename_size - 1 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy basename to segment filename.",
		 function );

		goto on_error;
	}
	filename_index = basename_size - 1;

	if( libcstring_system_string_copy(
	     &( ( *filename )[ filename_index ] ),
	     _LIBCSTRING_SYSTEM_STRING( ".raw" ),
	     4 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy extension to segment filename.",
		 function );

		goto on_error;
	}
	filename_index += 4;

	if( total_number_of_segments != 1 )
	{
		( *filename )[ filename_index++ ] = (libcstring_system_character_t) '.';

		( *filename )[ filename_index++ ] = (libcstring_system_character_t) '0'
		                                  + (libcstring_system_character_t) ( current_file_io_pool_entry / 100 );

		current_file_io_pool_entry %= 100;

		( *filename )[ filename_index++ ] = (libcstring_system_character_t) '0'
		                                  + (libcstring_system_character_t) ( current_file_io_pool_entry / 10 );

		current_file_io_pool_entry %= 10;

		( *filename )[ filename_index++ ] = (libcstring_system_character_t) '0'
		                                  + (libcstring_system_character_t) current_file_io_pool_entry;
	}
	( *filename )[ filename_index ] = 0;

	return( 1 );

on_error:
	if( *filename != NULL )
	{
		memory_free(
		 *filename );

		*filename = NULL;
	}
	*filename_size = 0;

	return( -1 );
}

