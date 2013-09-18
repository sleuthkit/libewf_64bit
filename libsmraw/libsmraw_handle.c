/*
 * Handle functions
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
#include "libsmraw_information_file.h"
#include "libsmraw_filename.h"
#include "libsmraw_handle.h"
#include "libsmraw_libbfio.h"
#include "libsmraw_libcerror.h"
#include "libsmraw_libclocale.h"
#include "libsmraw_libcnotify.h"
#include "libsmraw_libcstring.h"
#include "libsmraw_libfvalue.h"
#include "libsmraw_libmfdata.h"
#include "libsmraw_libuna.h"
#include "libsmraw_types.h"

/* Initializes the handle
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_initialize(
     libsmraw_handle_t **handle,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_initialize";

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
	if( *handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle value already set.",
		 function );

		return( -1 );
	}
	internal_handle = memory_allocate_structure(
	                   libsmraw_internal_handle_t );

	if( internal_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create internal handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_handle,
	     0,
	     sizeof( libsmraw_internal_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear handle.",
		 function );

		memory_free(
		 internal_handle );

		return( -1 );
	}
	if( libmfdata_segment_table_initialize(
	     &( internal_handle->segment_table ),
	     (intptr_t *) internal_handle,
	     NULL,
	     NULL,
	     &libsmraw_handle_set_segment_name,
	     &libmfdata_segment_table_read_segment_data,
	     &libmfdata_segment_table_write_segment_data,
	     &libmfdata_segment_table_seek_segment_offset,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create segment table.",
		 function );

		goto on_error;
	}
	if( libmfdata_segment_table_set_maximum_segment_size(
	     internal_handle->segment_table,
	     LIBSMRAW_DEFAULT_MAXIMUM_SEGMENT_SIZE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set maximum segment size in segment table.",
		 function );

		goto on_error;
	}
	if( libfvalue_table_initialize(
	     &( internal_handle->media_values ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create media values table.",
		 function );

		goto on_error;
	}
	if( libfvalue_table_initialize(
	     &( internal_handle->information_values ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create information values table.",
		 function );

		goto on_error;
	}
	if( libfvalue_table_initialize(
	     &( internal_handle->integrity_hash_values ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create integrity hash values table.",
		 function );

		goto on_error;
	}
	internal_handle->maximum_number_of_open_handles = LIBBFIO_POOL_UNLIMITED_NUMBER_OF_OPEN_HANDLES;

	*handle = (libsmraw_handle_t *) internal_handle;

	return( 1 );

on_error:
	if( internal_handle != NULL )
	{
		if( internal_handle->information_values != NULL )
		{
			libfvalue_table_free(
			 &( internal_handle->information_values ),
			 NULL );
		}
		if( internal_handle->media_values != NULL )
		{
			libfvalue_table_free(
			 &( internal_handle->media_values ),
			 NULL );
		}
		if( internal_handle->segment_table != NULL )
		{
			libmfdata_segment_table_free(
			 &( internal_handle->segment_table ),
			 NULL );
		}
		memory_free(
		 internal_handle );
	}
	return( -1 );

}

/* Frees the handle
 * Returns 1 if succesful or -1 on error
 */
int libsmraw_handle_free(
     libsmraw_handle_t **handle,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_internal_handle_free";
	int result                                  = 1;

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
	if( *handle != NULL )
	{
		internal_handle = (libsmraw_internal_handle_t *) *handle;

		if( internal_handle->file_io_pool != NULL )
		{
			if( libsmraw_handle_close(
			     *handle,
			     error ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_CLOSE_FAILED,
				 "%s: unable to close handle.",
				 function );

				result = -1;
			}
		}
		*handle = NULL;

		if( libmfdata_segment_table_free(
		     &( internal_handle->segment_table ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free segment table.",
			 function );

			result = -1;
		}
		if( internal_handle->information_file != NULL )
		{
			if( libsmraw_information_file_free(
			     &( internal_handle->information_file ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free information file.",
				 function );

				result = -1;
			}
		}
		if( internal_handle->media_values != NULL )
		{
			if( libfvalue_table_free(
			     &( internal_handle->media_values ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free media values table.",
				 function );

				result = -1;
			}
		}
		if( internal_handle->information_values != NULL )
		{
			if( libfvalue_table_free(
			     &( internal_handle->information_values ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free information values table.",
				 function );

				result = -1;
			}
		}
		if( internal_handle->integrity_hash_values != NULL )
		{
			if( libfvalue_table_free(
			     &( internal_handle->integrity_hash_values ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free integrity hash values table.",
				 function );

				result = -1;
			}
		}
		if( internal_handle->basename != NULL )
		{
			memory_free(
			 internal_handle->basename );
		}
		memory_free(
		 internal_handle );
	}
	return( result );
}

/* Signals the handle to abort its current activity
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_signal_abort(
     libsmraw_handle_t *handle,
     libcerror_error_t **error )
{
	static char *function = "libsmraw_handle_signal_abort";

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
	( (libsmraw_internal_handle_t *) handle )->abort = 1;

	return( 1 );
}

/* Initializes the write values
 * Returns 1 if successful or -1 on error
 */
int libsmraw_internal_handle_initialize_write_values(
     libsmraw_internal_handle_t *internal_handle,
     libcerror_error_t **error )
{
	static char *function         = "libsmraw_internal_handle_initialize_write_values";
	size64_t maximum_segment_size = 0;

	if( internal_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->write_values_initialized != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: write values were initialized and cannot be initialized anymore.",
		 function );

		return( -1 );
	}
	if( libmfdata_segment_table_get_maximum_segment_size(
	     internal_handle->segment_table,
	     &maximum_segment_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve maximum segment size from segment table.",
		 function );

		return( -1 );
	}
	if( maximum_segment_size > 0 )
	{
		if( internal_handle->media_size > 0 )
		{
			internal_handle->total_number_of_segments = (int) ( internal_handle->media_size / maximum_segment_size );

			if( ( internal_handle->media_size % maximum_segment_size ) != 0 )
			{
				internal_handle->total_number_of_segments += 1;
			}
		}
		else
		{
			internal_handle->total_number_of_segments = 0;
		}
	}
	else
	{
		internal_handle->total_number_of_segments = 1;
	}
	internal_handle->write_values_initialized = 1;

	return( 1 );
}

/* Opens a set of storage media RAW files
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_open(
     libsmraw_handle_t *handle,
     char * const filenames[],
     int number_of_filenames,
     int access_flags,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle                    = NULL;
	libbfio_pool_t *file_io_pool                        = NULL;
	libsmraw_internal_handle_t *internal_handle         = NULL;
	libcstring_system_character_t *information_filename = NULL;
	static char *function                               = "libsmraw_handle_open";
	size_t filename_index                               = 0;
	size_t filename_length                              = 0;
	size_t information_filename_length                  = 0;
	int bfio_access_flags                               = 0;
	int filename_iterator                               = 0;

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

	if( internal_handle->basename != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - basename already exists.",
		 function );

		return( -1 );
	}
	if( filenames == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filenames.",
		 function );

		return( -1 );
	}
	if( number_of_filenames <= 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of filenames value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( ( access_flags & LIBSMRAW_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBSMRAW_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		return( -1 );
	}
	/* Open for read only or read/write
	 */
	if( ( access_flags & LIBSMRAW_ACCESS_FLAG_READ ) != 0 )
	{
		if( ( access_flags & LIBSMRAW_ACCESS_FLAG_READ ) != 0 )
		{
			bfio_access_flags |= LIBBFIO_ACCESS_FLAG_READ;
		}
		if( ( access_flags & LIBSMRAW_ACCESS_FLAG_WRITE ) != 0 )
		{
			bfio_access_flags |= LIBBFIO_ACCESS_FLAG_WRITE;
		}
		/* Set the basename
		 */
		filename_length = libcstring_narrow_string_length(
				   filenames[ 0 ] );

		if( filename_length <= 4 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: missing basename.",
			 function );

			goto on_error;
		}
		if( libsmraw_handle_set_segment_filename(
		     handle,
		     filenames[ 0 ],
		     filename_length - 4,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to set basename.",
			function );

			goto on_error;
		}
		if( libbfio_pool_initialize(
		     &file_io_pool,
		     number_of_filenames,
		     internal_handle->maximum_number_of_open_handles,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create file IO pool.",
			 function );

			goto on_error;
		}
		for( filename_iterator = 0;
		     filename_iterator < number_of_filenames;
		     filename_iterator++ )
		{
			filename_length = libcstring_narrow_string_length(
					   filenames[ filename_iterator ] );

			if( filename_length == 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing filename: %d.",
				 function,
				 filename_iterator );

				goto on_error;
			}
			if( libbfio_file_initialize(
			     &file_io_handle,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create file IO handle: %d.",
				 function,
				 filename_iterator );

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libbfio_handle_set_track_offsets_read(
			     file_io_handle,
			     1,
			     error ) != 1 )
			{
		                libcerror_error_set(
		                 error,
		                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		                 "%s: unable to set track offsets read in file IO handle: %d.",
		                 function,
				 filename_iterator );

				goto on_error;
			}
#endif
			if( libbfio_file_set_name(
			     file_io_handle,
			     filenames[ filename_iterator ],
			     filename_length + 1,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set name in file IO handle: %d.",
				 function,
				 filename_iterator );

				goto on_error;
			}
			if( libbfio_pool_set_handle(
			     file_io_pool,
			     filename_iterator,
			     file_io_handle,
			     bfio_access_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set file IO handle: %d in pool.",
				 function,
				 filename_iterator );

				goto on_error;
			}
			file_io_handle = NULL;
		}
	}
	/* Open for write only
	 */
	else if( ( access_flags & LIBSMRAW_ACCESS_FLAG_WRITE ) != 0 )
	{
		/* Set the basename
		 */
		filename_length = libcstring_narrow_string_length(
				   filenames[ 0 ] );

		if( filename_length == 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: missing basename.",
			 function );

			goto on_error;
		}
		if( libsmraw_handle_set_segment_filename(
		     handle,
		     filenames[ 0 ],
		     filename_length,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to set basename.",
			function );

			goto on_error;
		}
		/* Initialize the file IO pool
		 */
		if( libbfio_pool_initialize(
		     &file_io_pool,
		     0,
		     internal_handle->maximum_number_of_open_handles,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create file IO pool.",
			 function );

			goto on_error;
		}
	}
	if( libsmraw_handle_open_file_io_pool(
	     handle,
	     file_io_pool,
	     access_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open handle using file IO pool.",
		 function );

		goto on_error;
	}
	internal_handle->file_io_pool_created_in_library = 1;

	/* Open the information file
	 */
	if( internal_handle->basename != NULL )
	{
		information_filename_length = internal_handle->basename_size + 8;

		information_filename = libcstring_system_string_allocate(
		                        information_filename_length + 1 );

		if( information_filename == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create information filename.",
			 function );

			return( -1 );
		}
		if( libcstring_system_string_copy(
		     information_filename,
		     internal_handle->basename,
		     internal_handle->basename_size - 1 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy basename to information filename.",
			 function );

			memory_free(
			 information_filename );

			return( -1 );
		}
		filename_index = internal_handle->basename_size - 1;

		if( libcstring_system_string_copy(
		     &( information_filename[ filename_index ] ),
		     _LIBCSTRING_SYSTEM_STRING( ".raw.info" ),
		     9 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy extension to information filename.",
			 function );

			memory_free(
			 information_filename );

			return( -1 );
		}
		filename_index += 9;

		information_filename[ filename_index ] = 0;

		if( libsmraw_information_file_initialize(
		     &( internal_handle->information_file ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create information file.",
			 function );

			memory_free(
			 information_filename );

			return( -1 );
		}
		if( libsmraw_information_file_set_name(
		     internal_handle->information_file,
		     information_filename,
		     libcstring_system_string_length(
		      information_filename ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to set information filename.",
			 function );

			memory_free(
			 information_filename );

			return( -1 );
		}
		if( ( access_flags & LIBSMRAW_ACCESS_FLAG_READ ) != 0 )
		{
			if( libsmraw_handle_read_information_file(
			     internal_handle,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read information file.",
				 function );

				memory_free(
				 information_filename );

				return( -1 );
			}
		}
		memory_free(
		 information_filename );

		if( ( access_flags & LIBSMRAW_ACCESS_FLAG_WRITE ) != 0 )
		{
			internal_handle->write_information_on_close = 1;
		}
	}
	return( 1 );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	if( file_io_pool != NULL )
	{
		libbfio_pool_free(
		 &file_io_pool,
		 NULL );
	}
	return( -1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Opens a set of storage media RAW files
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_open_wide(
     libsmraw_handle_t *handle,
     wchar_t * const filenames[],
     int number_of_filenames,
     int access_flags,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle                    = NULL;
	libbfio_pool_t *file_io_pool                        = NULL;
	libsmraw_internal_handle_t *internal_handle         = NULL;
	libcstring_system_character_t *information_filename = NULL;
	static char *function                               = "libsmraw_handle_open_wide";
	size_t filename_index                               = 0;
	size_t filename_length                              = 0;
	size_t information_filename_length                  = 0;
	int bfio_access_flags                               = 0;
	int filename_iterator                               = 0;

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

	if( internal_handle->basename != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - basename already exists.",
		 function );

		return( -1 );
	}
	if( filenames == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filenames.",
		 function );

		return( -1 );
	}
	if( number_of_filenames <= 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of filenames value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( ( access_flags & LIBSMRAW_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBSMRAW_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		return( -1 );
	}
	/* Open for read only or read/write
	 */
	if( ( access_flags & LIBSMRAW_ACCESS_FLAG_READ ) != 0 )
	{
		if( ( access_flags & LIBSMRAW_ACCESS_FLAG_READ ) != 0 )
		{
			bfio_access_flags |= LIBBFIO_ACCESS_FLAG_READ;
		}
		if( ( access_flags & LIBSMRAW_ACCESS_FLAG_WRITE ) != 0 )
		{
			bfio_access_flags |= LIBBFIO_ACCESS_FLAG_WRITE;
		}
		/* Set the basename
		 */
		filename_length = libcstring_wide_string_length(
				   filenames[ 0 ] );

		if( filename_length <= 4 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: missing basename.",
			 function );

			goto on_error;
		}
		if( libsmraw_handle_set_segment_filename_wide(
		     handle,
		     filenames[ 0 ],
		     filename_length - 4,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to set basename.",
			function );

			goto on_error;
		}
		if( libbfio_pool_initialize(
		     &file_io_pool,
		     number_of_filenames,
		     internal_handle->maximum_number_of_open_handles,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create file IO pool.",
			 function );

			goto on_error;
		}
		for( filename_iterator = 0;
		     filename_iterator < number_of_filenames;
		     filename_iterator++ )
		{
			filename_length = libcstring_wide_string_length(
					   filenames[ filename_iterator ] );

			if( filename_length == 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing filename: %d.",
				 function,
				 filename_iterator );

				goto on_error;
			}
			if( libbfio_file_initialize(
			     &file_io_handle,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create file IO handle: %d.",
				 function,
				 filename_iterator );

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libbfio_handle_set_track_offsets_read(
			     file_io_handle,
			     1,
			     error ) != 1 )
			{
		                libcerror_error_set(
		                 error,
		                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		                 "%s: unable to set track offsets read in file IO handle: %d.",
		                 function,
				 filename_iterator );

		                goto on_error;
			}
#endif
			if( libbfio_file_set_name_wide(
			     file_io_handle,
			     filenames[ filename_iterator ],
			     filename_length + 1,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set name in file IO handle: %d.",
				 function,
				 filename_iterator );

				goto on_error;
			}
			if( libbfio_pool_set_handle(
			     file_io_pool,
			     filename_iterator,
			     file_io_handle,
			     bfio_access_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set file IO handle: %d in pool.",
				 function,
				 filename_iterator );

				goto on_error;
			}
			file_io_handle = NULL;
		}
	}
	/* Open for write only
	 */
	else if( ( access_flags & LIBSMRAW_ACCESS_FLAG_WRITE ) != 0 )
	{
		/* Set the basename
		 */
		filename_length = libcstring_wide_string_length(
				   filenames[ 0 ] );

		if( filename_length == 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: missing basename.",
			 function );

			goto on_error;
		}
		if( libsmraw_handle_set_segment_filename_wide(
		     handle,
		     filenames[ 0 ],
		     filename_length,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to set basename.",
			function );

			goto on_error;
		}
		/* Initialize the file IO pool
		 */
		if( libbfio_pool_initialize(
		     &file_io_pool,
		     0,
		     internal_handle->maximum_number_of_open_handles,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create file IO pool.",
			 function );

			goto on_error;
		}
	}
	if( libsmraw_handle_open_file_io_pool(
	     handle,
	     file_io_pool,
	     access_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open handle using file IO pool.",
		 function );

		goto on_error;
	}
	internal_handle->file_io_pool_created_in_library = 1;

	/* Open the information file
	 */
	if( internal_handle->basename != NULL )
	{
		information_filename_length = internal_handle->basename_size + 8;

		information_filename = libcstring_system_string_allocate(
		                        information_filename_length + 1 );

		if( information_filename == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create information filename.",
			 function );

			return( -1 );
		}
		if( libcstring_system_string_copy(
		     information_filename,
		     internal_handle->basename,
		     internal_handle->basename_size - 1 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy basename to information filename.",
			 function );

			memory_free(
			 information_filename );

			return( -1 );
		}
		filename_index = internal_handle->basename_size - 1;

		if( libcstring_system_string_copy(
		     &( information_filename[ filename_index ] ),
		     _LIBCSTRING_SYSTEM_STRING( ".raw.info" ),
		     9 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy extension to information filename.",
			 function );

			memory_free(
			 information_filename );

			return( -1 );
		}
		filename_index += 9;

		information_filename[ filename_index ] = 0;

		if( libsmraw_information_file_initialize(
		     &( internal_handle->information_file ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create information file.",
			 function );

			memory_free(
			 information_filename );

			return( -1 );
		}
		if( libsmraw_information_file_set_name(
		     internal_handle->information_file,
		     information_filename,
		     libcstring_system_string_length(
		      information_filename ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to set information filename.",
			 function );

			memory_free(
			 information_filename );

			return( -1 );
		}
		if( ( access_flags & LIBSMRAW_ACCESS_FLAG_READ ) != 0 )
		{
			if( libsmraw_handle_read_information_file(
			     internal_handle,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read information file.",
				 function );

				memory_free(
				 information_filename );

				return( -1 );
			}
		}
		memory_free(
		 information_filename );

		if( ( access_flags & LIBSMRAW_ACCESS_FLAG_WRITE ) != 0 )
		{
			internal_handle->write_information_on_close = 1;
		}
	}
	return( 1 );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	if( file_io_pool != NULL )
	{
		libbfio_pool_free(
		 &file_io_pool,
		 NULL );
	}
	return( -1 );
}

#endif

/* Opens a set of storage media RAW files using a Basic File IO (bfio) pool
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_open_file_io_pool(
     libsmraw_handle_t *handle,
     libbfio_pool_t *file_io_pool,
     int access_flags,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle            = NULL;
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_open_file_io_pool";
	size64_t file_io_handle_size                = 0;
	size64_t maximum_segment_size               = 0;
	int number_of_file_io_handles               = 0;
	int bfio_access_flags                       = 0;
	int file_io_handle_iterator                 = 0;

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

	if( internal_handle->file_io_pool != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - file IO pool already exists.",
		 function );

		return( -1 );
	}
	if( file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO pool.",
		 function );

		return( -1 );
	}
	if( ( ( access_flags & LIBSMRAW_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBSMRAW_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		return( -1 );
	}
	if( libbfio_pool_get_number_of_handles(
	     file_io_pool,
	     &number_of_file_io_handles,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the number of handles in the file IO pool.",
		 function );

		goto on_error;
	}
	if( ( access_flags & LIBSMRAW_ACCESS_FLAG_READ ) != 0 )
	{
		if( number_of_file_io_handles <= 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing file IO handles.",
			 function );

			goto on_error;
		}
		if( libmfdata_segment_table_resize(
		     internal_handle->segment_table,
		     number_of_file_io_handles,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to resize segment table.",
			 function );

			goto on_error;
		}
		if( ( access_flags & LIBSMRAW_ACCESS_FLAG_WRITE ) != 0 )
		{
			bfio_access_flags = LIBBFIO_OPEN_READ_WRITE;
		}
		else
		{
			bfio_access_flags = LIBBFIO_OPEN_READ;
		}
		for( file_io_handle_iterator = 0;
		     file_io_handle_iterator < number_of_file_io_handles;
		     file_io_handle_iterator++ )
		{
			if( libbfio_pool_get_handle(
			     file_io_pool,
			     file_io_handle_iterator,
			     &file_io_handle,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to retrieve file IO handle from pool entry: %d.",
				 function,
				 file_io_handle_iterator );

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: processing pool entry: %d.\n",
				 function,
				 file_io_handle_iterator );
			}
#endif
			if( libbfio_pool_open(
			     file_io_pool,
			     file_io_handle_iterator,
			     bfio_access_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_OPEN_FAILED,
				 "%s: unable to open pool entry: %d.",
				 function,
				 file_io_handle_iterator );

				goto on_error;
			}
			if( libbfio_pool_get_size(
			     file_io_pool,
			     file_io_handle_iterator,
			     &file_io_handle_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve file size for pool entry: %d.",
				 function,
				 file_io_handle_iterator );

				goto on_error;
			}
			if( libmfdata_segment_table_set_segment_by_index(
			     internal_handle->segment_table,
			     file_io_handle_iterator,
			     file_io_handle_iterator,
			     file_io_handle_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set name in file IO handle.",
				 function );

				goto on_error;
			}
			if( file_io_handle_size > maximum_segment_size )
			{
				maximum_segment_size = file_io_handle_size;
			}
		}
		if( libmfdata_segment_table_set_maximum_segment_size(
		     internal_handle->segment_table,
		     maximum_segment_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to set maximum segment size in segment table.",
			 function );

			goto on_error;
		}
		internal_handle->read_values_initialized = 1;
	}
	if( libmfdata_segment_table_get_value_size(
	     internal_handle->segment_table,
	     &( internal_handle->media_size ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value size from segment table.",
		 function );

		goto on_error;
	}
	internal_handle->file_io_pool = file_io_pool;

	return( 1 );

on_error:
	libmfdata_segment_table_empty(
	 internal_handle->segment_table,
	 NULL );

	internal_handle->read_values_initialized = 0;

	return( -1 );
}

/* Read an information file using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_read_information_file(
     libsmraw_internal_handle_t *internal_handle,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle = NULL;
	static char *function            = "libsmraw_handle_read_information_file";
	int result                       = 0;

	if( internal_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid internal handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->information_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing information file.",
		 function );

		return( -1 );
	}
	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO handle.",
		 function );

		return( -1 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libbfio_file_set_name_wide(
	     file_io_handle,
	     internal_handle->information_file->name,
	     internal_handle->information_file->name_size - 1,
	     error ) != 1 )
#else
	if( libbfio_file_set_name(
	     file_io_handle,
	     internal_handle->information_file->name,
	     internal_handle->information_file->name_size - 1,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set name in file IO handle.",
		 function );

		goto on_error;
	}
	result = libbfio_handle_exists(
		  file_io_handle,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_GENERIC,
		 "%s: unable to determine if information filename exists.",
		 function );

		goto on_error;
	}
	if( libbfio_handle_free(
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to free file IO handle.",
		 function );

		goto on_error;
	}
	/* Only read the information file if it exists
	 */
	if( result == 1 )
	{
		if( libsmraw_information_file_open(
		     internal_handle->information_file,
		     _LIBCSTRING_SYSTEM_STRING( FILE_STREAM_OPEN_READ ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to open information file.",
			 function );

			goto on_error;
		}
		if( libsmraw_information_file_read_section(
		     internal_handle->information_file,
		     (uint8_t *) "media_values",
		     12,
		     internal_handle->media_values,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read media values from information file.",
			 function );

			libsmraw_information_file_close(
			 internal_handle->information_file,
			 NULL );

			goto on_error;
		}
		if( libsmraw_information_file_read_section(
		     internal_handle->information_file,
		     (uint8_t *) "information_values",
		     18,
		     internal_handle->information_values,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read information values from information file.",
			 function );

			libsmraw_information_file_close(
			 internal_handle->information_file,
			 NULL );

			goto on_error;
		}
		if( libsmraw_information_file_read_section(
		     internal_handle->information_file,
		     (uint8_t *) "integrity_hash_values",
		     21,
		     internal_handle->integrity_hash_values,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read integrity hash values from information file.",
			 function );

			libsmraw_information_file_close(
			 internal_handle->information_file,
			 NULL );

			goto on_error;
		}
		if( libsmraw_information_file_close(
		     internal_handle->information_file,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to close information file.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( -1 );
}

/* Closes a RAW handle
 * Returns the 0 if succesful or -1 on error
 */
int libsmraw_handle_close(
     libsmraw_handle_t *handle,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_close";
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

	if( internal_handle->write_information_on_close != 0 )
	{
		if( libsmraw_information_file_open(
		     internal_handle->information_file,
		     _LIBCSTRING_SYSTEM_STRING( FILE_STREAM_OPEN_WRITE ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to open information file.",
			 function );

			return( -1 );
		}
		if( libsmraw_information_file_write_section(
		     internal_handle->information_file,
		     (uint8_t *) "information_values",
		     18,
		     internal_handle->information_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write information values to information file.",
			 function );

			return( -1 );
		}
		if( libsmraw_information_file_write_section(
		     internal_handle->information_file,
		     (uint8_t *) "integrity_hash_values",
		     21,
		     internal_handle->integrity_hash_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write integrity hash values to information file.",
			 function );

			return( -1 );
		}
		if( libsmraw_information_file_close(
		     internal_handle->information_file,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close information file.",
			 function );

			result = -1;
		}
	}
	if( libmfdata_segment_table_empty(
	     internal_handle->segment_table,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to empty segment table.",
		 function );

		result = -1;
	}
	if( internal_handle->file_io_pool_created_in_library != 0 )
	{
		if( internal_handle->file_io_pool != NULL )
		{
			if( libbfio_pool_close_all(
			     internal_handle->file_io_pool,
			     error ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_GENERIC,
				 "%s: unable close file IO pool.",
				 function );

				result = -1;
			}
			if( libbfio_pool_free(
			     &( internal_handle->file_io_pool ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free file IO pool.",
				 function );

				result = -1;
			}
		}
	}
	internal_handle->file_io_pool                    = NULL;
	internal_handle->file_io_pool_created_in_library = 0;

	if( libmfdata_segment_table_set_maximum_segment_size(
	     internal_handle->segment_table,
	     LIBSMRAW_DEFAULT_MAXIMUM_SEGMENT_SIZE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set maximum segment size in segment table.",
		 function );

		result = -1;
	}
	internal_handle->read_values_initialized  = 0;
	internal_handle->write_values_initialized = 0;

	return( result );
}

/* Reads a buffer
 * Returns the number of bytes read or -1 on error
 */
ssize_t libsmraw_handle_read_buffer(
         libsmraw_handle_t *handle,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_read_buffer";
	ssize_t read_count                          = 0;

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
		 "%s: invalid handle - missing file IO pool.",
		 function );

		return( -1 );
	}
	read_count = libmfdata_segment_table_read_buffer(
	              internal_handle->segment_table,
	              internal_handle->file_io_pool,
	              (uint8_t *) buffer,
	              buffer_size,
	              error );

	if( read_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read buffer from segment table.",
		 function );

		return( -1 );
	}
	return( read_count );
}

/* Reads (media) data at a specific offset
 * Returns the number of bytes read or -1 on error
 */
ssize_t libsmraw_handle_read_random(
         libsmraw_handle_t *handle,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error )
{
	static char *function = "libsmraw_handle_read_random";
	ssize_t read_count    = 0;

	if( libsmraw_handle_seek_offset(
	     handle,
	     offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset.",
		 function );

		return( -1 );
	}
	read_count = libsmraw_handle_read_buffer(
	              handle,
	              buffer,
	              buffer_size,
	              error );

	if( read_count <= -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read buffer.",
		 function );

		return( -1 );
	}
	return( read_count );
}

/* Writes a buffer
 * Returns the number of bytes written or -1 on error
 */
ssize_t libsmraw_handle_write_buffer(
         libsmraw_handle_t *handle,
         const void *buffer,
         size_t buffer_size,
         libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_write_buffer";
	off64_t value_offset                        = 0;
	ssize_t write_count                         = 0;
	int number_of_segments                      = 0;

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
		 "%s: invalid handle - missing file IO pool.",
		 function );

		return( -1 );
	}
	if( internal_handle->write_values_initialized == 0 )
	{
		if( libsmraw_internal_handle_initialize_write_values(
		     internal_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize write values.",
			 function );

			return( -1 );
		}
	}
	if( libmfdata_segment_table_get_value_offset(
	     internal_handle->segment_table,
	     &value_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value offset from segment table.",
		 function );

		return( -1 );
	}
	if( value_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid value offset value out of bounds.",
		 function );

		return( -1 );
	}
	/* Do not write beyond the media size
	 */
	if( ( internal_handle->media_size > 0 )
	 && ( (size64_t) value_offset >= internal_handle->media_size ) )
	{
		return( 0 );
	}
	if( libmfdata_segment_table_get_number_of_segments(
	     internal_handle->segment_table,
	     &number_of_segments,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of segments from segment table.",
		 function );

		return( -1 );
	}
	if( internal_handle->total_number_of_segments != 0 )
	{
		if( ( number_of_segments < 0 )
		 || ( number_of_segments > internal_handle->total_number_of_segments ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid number of segments value out of bounds.",
			 function );

			return( -1 );
		}
	}
	if( ( internal_handle->media_size > 0 )
	 && ( (size64_t) value_offset + buffer_size ) >= internal_handle->media_size )
	{
		buffer_size = (size_t) ( internal_handle->media_size - (size64_t) value_offset );
	}
	write_count = libmfdata_segment_table_write_buffer(
	               internal_handle->segment_table,
	               internal_handle->file_io_pool,
	               (uint8_t *) buffer,
	               buffer_size,
	               error );

	if( write_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to write buffer to segment table.",
		 function );

		return( -1 );
	}
	return( write_count );
}

/* Writes (media) data at a specific offset,
 * Returns the number of input bytes written, 0 when no longer bytes can be written or -1 on error
 */
ssize_t libsmraw_handle_write_random(
         libsmraw_handle_t *handle,
         const void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error )
{
	static char *function = "libsmraw_handle_write_random";
	ssize_t write_count   = 0;

	if( libsmraw_handle_seek_offset(
	     handle,
	     offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset.",
		 function );

		return( -1 );
	}
	write_count = libsmraw_handle_write_buffer(
	               handle,
	               buffer,
	               buffer_size,
	               error );

	if( write_count <= -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write buffer.",
		 function );

		return( -1 );
	}
	return( write_count );
}

/* Seeks a certain offset
 * Returns the offset or -1 on error
 */
off64_t libsmraw_handle_seek_offset(
         libsmraw_handle_t *handle,
         off64_t offset,
         int whence,
         libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_seek_offset";

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
		 "%s: invalid handle - missing file IO pool.",
		 function );

		return( -1 );
	}
	offset = libmfdata_segment_table_seek_offset(
	          internal_handle->segment_table,
	          internal_handle->file_io_pool,
	          offset,
	          whence,
	          error );

	if( offset == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to seek offset in segment table.",
		 function );

		return( -1 );
	}
	return( offset );
}

/* Retrieves the current offset of the (media) data
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_get_offset(
     libsmraw_handle_t *handle,
     off64_t *offset,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_get_offset";

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

	if( libmfdata_segment_table_get_value_offset(
	     internal_handle->segment_table,
	     offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value offset from segment table.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the maximum number of (concurrent) open file handles
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_set_maximum_number_of_open_handles(
     libsmraw_handle_t *handle,
     int maximum_number_of_open_handles,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_set_maximum_number_of_open_handles";

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

	if( internal_handle->file_io_pool != NULL )
	{
		if( libbfio_pool_set_maximum_number_of_open_handles(
		     internal_handle->file_io_pool,
		     maximum_number_of_open_handles,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set maximum number of open handles in file IO handle.",
			 function );

			return( -1 );
		}
	}
	internal_handle->maximum_number_of_open_handles = maximum_number_of_open_handles;

	return( 1 );
}

/* Set the name of the segment
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_set_segment_name(
     intptr_t *io_handle,
     libbfio_handle_t *file_io_handle,
     int segment_index,
     libcerror_error_t **error )
{
	libcstring_system_character_t *segment_filename = NULL;
	libsmraw_internal_handle_t *internal_handle     = NULL;
	static char *function                           = "libsmraw_handle_set_segment_name";
	size_t segment_filename_size                    = 0;

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmraw_internal_handle_t *) io_handle;

	if( libsmraw_filename_create(
	     &segment_filename,
	     &segment_filename_size,
	     internal_handle->basename,
	     internal_handle->basename_size,
	     internal_handle->total_number_of_segments,
	     segment_index,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create segment filename: %d.",
		 function,
		 segment_index );

		return( -1 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libbfio_file_set_name_wide(
	     file_io_handle,
	     segment_filename,
	     segment_filename_size,
	     error ) != 1 )
#else
	if( libbfio_file_set_name(
	     file_io_handle,
	     segment_filename,
	     segment_filename_size,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set name in file IO handle.",
		 function );

		memory_free(
		 segment_filename );

		return( -1 );
	}
	memory_free(
	 segment_filename );

	return( 1 );
}

/* Retrieves the segment filenmae size of the file handle
 * The segment filenmae size includes the end of string character
 * Returns 1 if succesful or -1 on error
 */
int libsmraw_handle_get_segment_filename_size(
     libsmraw_handle_t *handle,
     size_t *filename_size,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_get_segment_filename_size";

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	int result                                  = 0;
#endif

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

	if( internal_handle->basename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing basename.",
		 function );

		return( -1 );
	}
	if( filename_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename size.",
		 function );

		return( -1 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf8_string_size_from_utf32(
		          (libuna_utf32_character_t *) internal_handle->basename,
		          internal_handle->basename_size,
		          filename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf8_string_size_from_utf16(
		          (libuna_utf16_character_t *) internal_handle->basename,
		          internal_handle->basename_size,
		          filename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_byte_stream_size_from_utf32(
		          (libuna_utf32_character_t *) internal_handle->basename,
		          internal_handle->basename_size,
		          libclocale_codepage,
		          filename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_byte_stream_size_from_utf16(
		          (libuna_utf16_character_t *) internal_handle->basename,
		          internal_handle->basename_size,
		          libclocale_codepage,
		          filename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine filename size.",
		 function );

		return( -1 );
	}
#else
	*filename_size = internal_handle->basename_size;
#endif
	return( 1 );
}

/* Retrieves the segment filename of the file handle
 * The segment filename size should include the end of string character
 * Returns 1 if succesful or -1 on error
 */
int libsmraw_handle_get_segment_filename(
     libsmraw_handle_t *handle,
     char *filename,
     size_t filename_size,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_get_segment_filename";
	size_t narrow_filename_size                 = 0;

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	int result                                  = 0;
#endif

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

	if( internal_handle->basename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing basename.",
		 function );

		return( -1 );
	}
	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf8_string_size_from_utf32(
		          (libuna_utf32_character_t *) internal_handle->basename,
		          internal_handle->basename_size,
		          &narrow_filename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf8_string_size_from_utf16(
		          (libuna_utf16_character_t *) internal_handle->basename,
		          internal_handle->basename_size,
		          &narrow_filename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_byte_stream_size_from_utf32(
		          (libuna_utf32_character_t *) internal_handle->basename,
		          internal_handle->basename_size,
		          libclocale_codepage,
		          &narrow_filename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_byte_stream_size_from_utf16(
		          (libuna_utf16_character_t *) internal_handle->basename,
		          internal_handle->basename_size,
		          libclocale_codepage,
		          &narrow_filename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine filename size.",
		 function );

		return( -1 );
	}
#else
	narrow_filename_size = internal_handle->basename_size;
#endif
	if( filename_size < narrow_filename_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: filename too small.",
		 function );

		return( -1 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf8_string_copy_from_utf32(
		          (libuna_utf8_character_t *) filename,
		          filename_size,
		          (libuna_utf32_character_t *) internal_handle->basename,
		          internal_handle->basename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf8_string_copy_from_utf16(
		          (libuna_utf8_character_t *) filename,
		          filename_size,
		          (libuna_utf16_character_t *) internal_handle->basename,
		          internal_handle->basename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_byte_stream_copy_from_utf32(
		          (uint8_t *) filename,
		          filename_size,
		          libclocale_codepage,
		          (libuna_utf32_character_t *) internal_handle->basename,
		          internal_handle->basename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_byte_stream_copy_from_utf16(
		          (uint8_t *) filename,
		          filename_size,
		          libclocale_codepage,
		          (libuna_utf16_character_t *) internal_handle->basename,
		          internal_handle->basename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set filename.",
		 function );

		return( -1 );
	}
#else
	if( libcstring_system_string_copy(
	     filename,
	     internal_handle->basename,
	     internal_handle->basename_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set filename.",
		 function );

		return( -1 );
	}
	filename[ internal_handle->basename_size - 1 ] = 0;
#endif
	return( 1 );
}

/* Sets the segment filename for the file handle
 * Returns 1 if succesful or -1 on error
 */
int libsmraw_handle_set_segment_filename(
     libsmraw_handle_t *handle,
     const char *filename,
     size_t filename_length,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_set_segment_filename";

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	int result                                  = 0;
#endif

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

	if( internal_handle->basename != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - basename already set.",
		 function );

		return( -1 );
	}
	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( filename_length == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid filename length is zero.",
		 function );

		return( -1 );
	}
	if( filename_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid filename length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( internal_handle->basename != NULL )
	{
		if( internal_handle->file_io_pool != NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: basename already set: %" PRIs_LIBCSTRING_SYSTEM ".",
			 function,
			 internal_handle->basename );

			return( -1 );
		}
		memory_free(
		  internal_handle->basename );

		 internal_handle->basename      = NULL;
		 internal_handle->basename_size = 0;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_size_from_utf8(
		          (libuna_utf8_character_t *) filename,
		          filename_length + 1,
		          &( internal_handle->basename_size ),
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_size_from_utf8(
		          (libuna_utf8_character_t *) filename,
		          filename_length + 1,
		          &( internal_handle->basename_size ),
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_size_from_byte_stream(
		          (uint8_t *) filename,
		          filename_length + 1,
		          libclocale_codepage,
		          &( internal_handle->basename_size ),
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_size_from_byte_stream(
		          (uint8_t *) filename,
		          filename_length + 1,
		          libclocale_codepage,
		          &( internal_handle->basename_size ),
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine name size.",
		 function );

		return( -1 );
	}
#else
	internal_handle->basename_size = filename_length + 1;
#endif
	internal_handle->basename = libcstring_system_string_allocate(
	                             internal_handle->basename_size );

	if( internal_handle->basename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create basename.",
		 function );

		return( -1 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_copy_from_utf8(
		          (libuna_utf32_character_t *) internal_handle->basename,
		          internal_handle->basename_size,
		          (libuna_utf8_character_t *) filename,
		          filename_length + 1,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_copy_from_utf8(
		          (libuna_utf16_character_t *) internal_handle->basename,
		          internal_handle->basename_size,
		          (libuna_utf8_character_t *) filename,
		          filename_length + 1,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_copy_from_byte_stream(
		          (libuna_utf32_character_t *) internal_handle->basename,
		          internal_handle->basename_size,
		          (uint8_t *) filename,
		          filename_length + 1,
		          libclocale_codepage,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_copy_from_byte_stream(
		          (libuna_utf16_character_t *) internal_handle->basename,
		          internal_handle->basename_size,
		          (uint8_t *) filename,
		          filename_length + 1,
		          libclocale_codepage,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set basename.",
		 function );

		memory_free(
		 internal_handle->basename );

		internal_handle->basename      = NULL;
		internal_handle->basename_size = 0;

		return( -1 );
	}
#else
	if( libcstring_system_string_copy(
	     internal_handle->basename,
	     filename,
	     filename_length + 1 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set basename.",
		 function );

		memory_free(
		 internal_handle->basename );

		internal_handle->basename      = NULL;
		internal_handle->basename_size = 0;

		return( -1 );
	}
	internal_handle->basename[ filename_length ] = 0;
#endif
	return( 1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Retrieves the segment filename size of the file handle
 * The segment filename size includes the end of string character
 * Returns 1 if succesful or -1 on error
 */
int libsmraw_handle_get_segment_filename_size_wide(
     libsmraw_handle_t *handle,
     size_t *filename_size,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_get_segment_filename_size_wide";

#if !defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	int result                                  = 0;
#endif

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

	if( internal_handle->basename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing basename.",
		 function );

		return( -1 );
	}
	if( filename_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename size.",
		 function );

		return( -1 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	*filename_size = internal_handle->basename_size;
#else
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_size_from_utf8(
		          (libuna_utf8_character_t *) internal_handle->basename,
		          internal_handle->basename_size,
		          filename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_size_from_utf8(
		          (libuna_utf8_character_t *) internal_handle->basename,
		          internal_handle->basename_size,
		          filename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_size_from_byte_stream(
		          (uint8_t *) internal_handle->basename,
		          internal_handle->basename_size,
		          libclocale_codepage,
		          filename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_size_from_byte_stream(
		          (uint8_t *) internal_handle->basename,
		          internal_handle->basename_size,
		          libclocale_codepage,
		          filename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine filename size.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Retrieves the segment filename of the file handle
 * The segment filename size should include the end of string character
 * Returns 1 if succesful or -1 on error
 */
int libsmraw_handle_get_segment_filename_wide(
     libsmraw_handle_t *handle,
     wchar_t *filename,
     size_t filename_size,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libsmraw_handle_get_segment_filename_wide";
	size_t wide_filename_size                  = 0;

#if !defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	int result                                 = 0;
#endif

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

	if( internal_handle->basename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing basename.",
		 function );

		return( -1 );
	}
	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	wide_filename_size = internal_handle->basename_size;
#else
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_size_from_utf8(
		          (libuna_utf8_character_t *) internal_handle->basename,
		          internal_handle->basename_size,
		          &wide_filename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_size_from_utf8(
		          (libuna_utf8_character_t *) internal_handle->basename,
		          internal_handle->basename_size,
		          &wide_filename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_size_from_byte_stream(
		          (uint8_t *) internal_handle->basename,
		          internal_handle->basename_size,
		          libclocale_codepage,
		          &wide_filename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_size_from_byte_stream(
		          (uint8_t *) internal_handle->basename,
		          internal_handle->basename_size,
		          libclocale_codepage,
		          &wide_filename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine filename size.",
		 function );

		return( -1 );
	}
#endif
	if( filename_size < wide_filename_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: filename too small.",
		 function );

		return( -1 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libcstring_system_string_copy(
	     filename,
	     internal_handle->basename,
	     internal_handle->basename_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set filename.",
		 function );

		return( -1 );
	}
	filename[ internal_handle->basename_size - 1 ] = 0;
#else
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_copy_from_utf8(
		          (libuna_utf32_character_t *) filename,
		          filename_size,
		          (libuna_utf8_character_t *) internal_handle->basename,
		          internal_handle->basename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_copy_from_utf8(
		          (libuna_utf16_character_t *) filename,
		          filename_size,
		          (libuna_utf8_character_t *) internal_handle->basename,
		          internal_handle->basename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_copy_from_byte_stream(
		          (libuna_utf32_character_t *) filename,
		          filename_size,
		          (uint8_t *) internal_handle->basename,
		          internal_handle->basename_size,
		          libclocale_codepage,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_copy_from_byte_stream(
		          (libuna_utf16_character_t *) filename,
		          filename_size,
		          (uint8_t *) internal_handle->basename,
		          internal_handle->basename_size,
		          libclocale_codepage,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set filename.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Sets the segment filename for the file handle
 * Returns 1 if succesful or -1 on error
 */
int libsmraw_handle_set_segment_filename_wide(
     libsmraw_handle_t *handle,
     const wchar_t *filename,
     size_t filename_length,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_set_segment_filename_wide";

#if !defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	int result                                  = 0;
#endif

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

	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( filename_length == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid filename length is zero.",
		 function );

		return( -1 );
	}
	if( filename_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid filename length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( internal_handle->basename != NULL )
	{
		if( internal_handle->file_io_pool != NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: basename already set: %" PRIs_LIBCSTRING_SYSTEM ".",
			 function,
			 internal_handle->basename );

			return( -1 );
		}
		memory_free(
		  internal_handle->basename );

		 internal_handle->basename      = NULL;
		 internal_handle->basename_size = 0;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	internal_handle->basename_size = filename_length + 1;
#else
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf8_string_size_from_utf32(
		          (libuna_utf32_character_t *) filename,
		          filename_length + 1,
		          &( internal_handle->basename_size ),
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf8_string_size_from_utf16(
		          (libuna_utf16_character_t *) filename,
		          filename_length + 1,
		          &( internal_handle->basename_size ),
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_byte_stream_size_from_utf32(
		          (libuna_utf32_character_t *) filename,
		          filename_length + 1,
		          libclocale_codepage,
		          &( internal_handle->basename_size ),
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_byte_stream_size_from_utf16(
		          (libuna_utf16_character_t *) filename,
		          filename_length + 1,
		          libclocale_codepage,
		          &( internal_handle->basename_size ),
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine name size.",
		 function );

		return( -1 );
	}
#endif /* defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER ) */

	internal_handle->basename = libcstring_system_string_allocate(
	                             internal_handle->basename_size );

	if( internal_handle->basename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create basename.",
		 function );

		return( -1 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libcstring_system_string_copy(
	     internal_handle->basename,
	     filename,
	     filename_length + 1 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set filename.",
		 function );

		memory_free(
		 internal_handle->basename );

		internal_handle->basename      = NULL;
		internal_handle->basename_size = 0;

		return( -1 );
	}
	internal_handle->basename[ filename_length ] = 0;
#else
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf8_string_copy_from_utf32(
		          (libuna_utf8_character_t *) internal_handle->basename,
		          internal_handle->basename_size,
		          (libuna_utf32_character_t *) filename,
		          filename_length + 1,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf8_string_copy_from_utf16(
		          (libuna_utf8_character_t *) internal_handle->basename,
		          internal_handle->basename_size,
		          (libuna_utf16_character_t *) filename,
		          filename_length + 1,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_byte_stream_copy_from_utf32(
		          (uint8_t *) internal_handle->basename,
		          internal_handle->basename_size,
		          libclocale_codepage,
		          (libuna_utf32_character_t *) filename,
		          filename_length + 1,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_byte_stream_copy_from_utf16(
		          (uint8_t *) internal_handle->basename,
		          internal_handle->basename_size,
		          libclocale_codepage,
		          (libuna_utf16_character_t *) filename,
		          filename_length + 1,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set filename.",
		 function );

		memory_free(
		 internal_handle->basename );

		internal_handle->basename      = NULL;
		internal_handle->basename_size = 0;

		return( -1 );
	}
#endif /* defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER ) */

	return( 1 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Retrieves the segment file size
 * 0 represents the maximum possible segment file size
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_get_maximum_segment_size(
     libsmraw_handle_t *handle,
     size64_t *maximum_segment_size,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_get_maximum_segment_size";

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

	if( libmfdata_segment_table_get_maximum_segment_size(
	     internal_handle->segment_table,
	     maximum_segment_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve maximum segment size from segment table.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the segment file size
 * 0 represents the maximum possible segment file size
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_set_maximum_segment_size(
     libsmraw_handle_t *handle,
     size64_t maximum_segment_size,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_set_maximum_segment_size";

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

	if( internal_handle->write_values_initialized != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: segment file size cannot be changed.",
		 function );

		return( -1 );
	}
	if( libmfdata_segment_table_set_maximum_segment_size(
	     internal_handle->segment_table,
	     maximum_segment_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to set maximum segment size in segment table.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the filename size of the segment file of the current offset
 * The filename size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_get_filename_size(
     libsmraw_handle_t *handle,
     size_t *filename_size,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle = NULL;
	static char *function            = "libsmraw_handle_get_filename_size";

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
	if( libsmraw_handle_get_file_io_handle(
	     handle,
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file IO handle for current offset.",
		 function );

		return( -1 );
	}
	if( libbfio_file_get_name_size(
	     file_io_handle,
	     filename_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve filename size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the filename of the segment file of the current offset
 * The filename size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_get_filename(
     libsmraw_handle_t *handle,
     char *filename,
     size_t filename_size,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle = NULL;
	static char *function            = "libsmraw_handle_get_filename";

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
	if( libsmraw_handle_get_file_io_handle(
	     handle,
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file IO handle for current offset.",
		 function );

		return( -1 );
	}
	if( libbfio_file_get_name(
	     file_io_handle,
	     filename,
	     filename_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve filename.",
		 function );

		return( -1 );
	}
	return( 1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Retrieves the filename size of the segment file of the current offset
 * The filename size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_get_filename_size_wide(
     libsmraw_handle_t *handle,
     size_t *filename_size,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle = NULL;
	static char *function            = "libsmraw_handle_get_filename_size_wide";

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
	if( libsmraw_handle_get_file_io_handle(
	     handle,
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file IO handle for current offset.",
		 function );

		return( -1 );
	}
	if( libbfio_file_get_name_size_wide(
	     file_io_handle,
	     filename_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve filename size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the filename of the segment file of the current offset
 * The filename size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_get_filename_wide(
     libsmraw_handle_t *handle,
     wchar_t *filename,
     size_t filename_size,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle = NULL;
	static char *function            = "libsmraw_handle_get_filename_wide";

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
	if( libsmraw_handle_get_file_io_handle(
	     handle,
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file IO handle for current offset.",
		 function );

		return( -1 );
	}
	if( libbfio_file_get_name_wide(
	     file_io_handle,
	     filename,
	     filename_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve filename.",
		 function );

		return( -1 );
	}
	return( 1 );
}

#endif

/* Retrieves the file IO handle of the segment file of the current offset
 * Returns 1 if successful or -1 on error
 */
int libsmraw_handle_get_file_io_handle(
     libsmraw_handle_t *handle,
     libbfio_handle_t **file_io_handle,
     libcerror_error_t **error )
{
	libsmraw_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmraw_handle_get_file_io_handle";

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

	if( libmfdata_segment_table_get_file_io_handle(
	     internal_handle->segment_table,
	     internal_handle->file_io_pool,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file IO handle from segment table.",
		 function );

		return( -1 );
	}
	return( 1 );
}

