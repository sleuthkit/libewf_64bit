/*
 * Export handle
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

#if defined( WINAPI )
#include <rpcdce.h>

#elif defined( HAVE_UUID_UUID_H )
#include <uuid/uuid.h>
#endif

#include "byte_size_string.h"
#include "digest_hash.h"
#include "ewfcommon.h"
#include "ewfinput.h"
#include "ewftools_libcerror.h"
#include "ewftools_libcfile.h"
#include "ewftools_libcpath.h"
#include "ewftools_libcsplit.h"
#include "ewftools_libcstring.h"
#include "ewftools_libcsystem.h"
#include "ewftools_libewf.h"
#include "ewftools_libsmraw.h"
#include "ewftools_libhmac.h"
#include "export_handle.h"
#include "guid.h"
#include "process_status.h"

#define EXPORT_HANDLE_BUFFER_SIZE		8192
#define EXPORT_HANDLE_INPUT_BUFFER_SIZE		64
#define EXPORT_HANDLE_STRING_SIZE		1024
#define EXPORT_HANDLE_NOTIFY_STREAM		stderr

/* Initializes the export handle
 * Returns 1 if successful or -1 on error
 */
int export_handle_initialize(
     export_handle_t **export_handle,
     uint8_t calculate_md5,
     libcerror_error_t **error )
{
	static char *function = "export_handle_initialize";

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( *export_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid export handle value already set.",
		 function );

		return( -1 );
	}
	*export_handle = memory_allocate_structure(
	                  export_handle_t );

	if( *export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create export handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *export_handle,
	     0,
	     sizeof( export_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear export handle.",
		 function );

		memory_free(
		 *export_handle );

		*export_handle = NULL;

		return( -1 );
	}
	if( libewf_handle_initialize(
	     &( ( *export_handle )->input_handle ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create input handle.",
		 function );

		goto on_error;
	}
	( *export_handle )->input_buffer = libcstring_system_string_allocate(
					    EXPORT_HANDLE_INPUT_BUFFER_SIZE );

	if( ( *export_handle )->input_buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create input buffer.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     ( *export_handle )->input_buffer,
	     0,
	     sizeof( libcstring_system_character_t ) * EXPORT_HANDLE_INPUT_BUFFER_SIZE ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear export handle.",
		 function );

		goto on_error;
	}
	if( calculate_md5 != 0 )
	{
		( *export_handle )->calculated_md5_hash_string = libcstring_system_string_allocate(
								  33 );

		if( ( *export_handle )->calculated_md5_hash_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create calculated MD5 digest hash string.",
			 function );

			goto on_error;
		}
	}
	( *export_handle )->calculate_md5       = calculate_md5;
	( *export_handle )->compression_method  = LIBEWF_COMPRESSION_METHOD_DEFLATE;
	( *export_handle )->compression_level   = LIBEWF_COMPRESSION_NONE;
	( *export_handle )->output_format       = EXPORT_HANDLE_OUTPUT_FORMAT_RAW;
	( *export_handle )->ewf_format          = LIBEWF_FORMAT_ENCASE6;
	( *export_handle )->sectors_per_chunk   = 64;
	( *export_handle )->header_codepage     = LIBEWF_CODEPAGE_ASCII;
	( *export_handle )->process_buffer_size = EWFCOMMON_PROCESS_BUFFER_SIZE;
	( *export_handle )->notify_stream       = EXPORT_HANDLE_NOTIFY_STREAM;

	return( 1 );

on_error:
	if( *export_handle != NULL )
	{
		if( ( *export_handle )->calculated_sha1_hash_string != NULL )
		{
			memory_free(
			 ( *export_handle )->calculated_sha1_hash_string );
		}
		if( ( *export_handle )->calculated_md5_hash_string != NULL )
		{
			memory_free(
			 ( *export_handle )->calculated_md5_hash_string );
		}
		if( ( *export_handle )->input_buffer != NULL )
		{
			memory_free(
			 ( *export_handle )->input_buffer );
		}
		memory_free(
		 *export_handle );

		*export_handle = NULL;
	}
	return( -1 );
}

/* Frees the export handle and its elements
 * Returns 1 if successful or -1 on error
 */
int export_handle_free(
     export_handle_t **export_handle,
     libcerror_error_t **error )
{
	static char *function = "export_handle_free";
	int result            = 1;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( *export_handle != NULL )
	{
		memory_free(
		 ( *export_handle )->input_buffer );

		if( ( *export_handle )->target_path != NULL )
		{
			memory_free(
			 ( *export_handle )->target_path );
		}
		if( ( *export_handle )->input_handle != NULL )
		{
			if( libewf_handle_free(
			     &( ( *export_handle )->input_handle ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free input handle.",
				 function );

				result = -1;
			}
		}
		if( ( *export_handle )->ewf_output_handle != NULL )
		{
			if( libewf_handle_free(
			     &( ( *export_handle )->ewf_output_handle ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free ewf output handle.",
				 function );

				result = -1;
			}
		}
		if( ( *export_handle )->raw_output_handle != NULL )
		{
			if( libsmraw_handle_free(
			     &( ( *export_handle )->raw_output_handle ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free raw output handle.",
				 function );

				result = -1;
			}
		}
		if( ( *export_handle )->md5_context != NULL )
		{
			if( libhmac_md5_free(
			     &( ( *export_handle )->md5_context ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free MD5 context.",
				 function );

				result = -1;
			}
		}
		if( ( *export_handle )->calculated_md5_hash_string != NULL )
		{
			memory_free(
			 ( *export_handle )->calculated_md5_hash_string );
		}
		if( ( *export_handle )->sha1_context != NULL )
		{
			if( libhmac_sha1_free(
			     &( ( *export_handle )->sha1_context ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free SHA1 context.",
				 function );

				result = -1;
			}
		}
		if( ( *export_handle )->calculated_sha1_hash_string != NULL )
		{
			memory_free(
			 ( *export_handle )->calculated_sha1_hash_string );
		}
		if( ( *export_handle )->sha256_context != NULL )
		{
			if( libhmac_sha256_free(
			     &( ( *export_handle )->sha256_context ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free SHA256 context.",
				 function );

				result = -1;
			}
		}
		if( ( *export_handle )->calculated_sha256_hash_string != NULL )
		{
			memory_free(
			 ( *export_handle )->calculated_sha256_hash_string );
		}
		memory_free(
		 *export_handle );

		*export_handle = NULL;
	}
	return( result );
}

/* Signals the export handle to abort
 * Returns 1 if successful or -1 on error
 */
int export_handle_signal_abort(
     export_handle_t *export_handle,
     libcerror_error_t **error )
{
	static char *function = "export_handle_signal_abort";

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( export_handle->input_handle != NULL )
	{
		if( libewf_handle_signal_abort(
		     export_handle->input_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to signal input handle to abort.",
			 function );

			return( -1 );
		}
	}
	if( export_handle->output_format == EXPORT_HANDLE_OUTPUT_FORMAT_EWF )
	{
		if( libewf_handle_signal_abort(
		     export_handle->ewf_output_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to signal ewf output handle to abort.",
			 function );

			return( -1 );
		}
	}
	else if( ( export_handle->output_format == EXPORT_HANDLE_OUTPUT_FORMAT_RAW )
	      && ( export_handle->use_stdout == 0 ) )
	{
		if( libsmraw_handle_signal_abort(
		     export_handle->raw_output_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to signal raw output handle to abort.",
			 function );

			return( -1 );
		}
	}
	export_handle->abort = 1;

	return( 1 );
}

/* Sets the maximum number of (concurrent) open file handles
 * Returns 1 if successful or -1 on error
 */
int export_handle_set_maximum_number_of_open_handles(
     export_handle_t *export_handle,
     int maximum_number_of_open_handles,
     libcerror_error_t **error )
{
	static char *function = "export_handle_set_maximum_number_of_open_handles";

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( libewf_handle_set_maximum_number_of_open_handles(
	     export_handle->input_handle,
	     maximum_number_of_open_handles,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set maximum number of open handles in input handle.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Opens the input of the export handle
 * Returns 1 if successful or -1 on error
 */
int export_handle_open_input(
     export_handle_t *export_handle,
     libcstring_system_character_t * const * filenames,
     int number_of_filenames,
     libcerror_error_t **error )
{
	libcstring_system_character_t **libewf_filenames = NULL;
	static char *function                            = "export_handle_open_input";
	size_t first_filename_length                     = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
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
		 LIBCERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid number of filenames.",
		 function );

		return( -1 );
	}
	if( number_of_filenames == 1 )
	{
		first_filename_length = libcstring_system_string_length(
		                         filenames[ 0 ] );

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		if( libewf_glob_wide(
		     filenames[ 0 ],
		     first_filename_length,
		     LIBEWF_FORMAT_UNKNOWN,
		     &libewf_filenames,
		     &number_of_filenames,
		     error ) != 1 )
#else
		if( libewf_glob(
		     filenames[ 0 ],
		     first_filename_length,
		     LIBEWF_FORMAT_UNKNOWN,
		     &libewf_filenames,
		     &number_of_filenames,
		     error ) != 1 )
#endif
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to resolve filename(s).",
			 function );

			return( -1 );
		}
		filenames = (libcstring_system_character_t * const *) libewf_filenames;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libewf_handle_open_wide(
	     export_handle->input_handle,
	     filenames,
	     number_of_filenames,
	     LIBEWF_OPEN_READ,
	     error ) != 1 )
#else
	if( libewf_handle_open(
	     export_handle->input_handle,
	     filenames,
	     number_of_filenames,
	     LIBEWF_OPEN_READ,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file(s).",
		 function );

		if( libewf_filenames != NULL )
		{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
			libewf_glob_wide_free(
			 libewf_filenames,
			 number_of_filenames,
			 NULL );
#else
			libewf_glob_free(
			 libewf_filenames,
			 number_of_filenames,
			 NULL );
#endif
		}
		return( -1 );
	}
	if( libewf_filenames != NULL )
	{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		if( libewf_glob_wide_free(
		     libewf_filenames,
		     number_of_filenames,
		     error ) != 1 )
#else
		if( libewf_glob_free(
		     libewf_filenames,
		     number_of_filenames,
		     error ) != 1 )
#endif
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free globbed filenames.",
			 function );

			return( -1 );
		}
	}
	if( libewf_handle_set_header_codepage(
	     export_handle->input_handle,
	     export_handle->header_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header codepage.",
		 function );

		return( -1 );
	}
	if( libewf_handle_get_chunk_size(
	     export_handle->input_handle,
	     &( export_handle->input_chunk_size ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve chunk size.",
		 function );

		return( -1 );
	}
	if( libewf_handle_get_media_size(
	     export_handle->input_handle,
	     &( export_handle->input_media_size ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve media size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Opens the output of the export handle
 * Returns 1 if successful or -1 on error
 */
int export_handle_open_output(
     export_handle_t *export_handle,
     const libcstring_system_character_t *filename,
     libcerror_error_t **error )
{
	libcstring_system_character_t *filenames[ 1 ] = { NULL };
	static char *function                         = "export_handle_open_output";
	size_t filename_length                        = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( ( export_handle->output_format != EXPORT_HANDLE_OUTPUT_FORMAT_EWF )
	 && ( export_handle->output_format != EXPORT_HANDLE_OUTPUT_FORMAT_RAW ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported output format.",
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
	if( export_handle->output_format == EXPORT_HANDLE_OUTPUT_FORMAT_EWF )
	{
		if( export_handle->ewf_output_handle != NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: invalid export handle - ewf output handle already set.",
			 function );

			return( -1 );
		}
		if( libewf_handle_initialize(
		     &( export_handle->ewf_output_handle ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create ewf output handle.",
			 function );

			return( -1 );
		}
		filenames[ 0 ] = (libcstring_system_character_t *) filename;

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		if( libewf_handle_open_wide(
		     export_handle->ewf_output_handle,
		     filenames,
		     1,
		     LIBEWF_OPEN_WRITE,
		     error ) != 1 )
#else
		if( libewf_handle_open(
		     export_handle->ewf_output_handle,
		     filenames,
		     1,
		     LIBEWF_OPEN_WRITE,
		     error ) != 1 )
#endif
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to open file: %" PRIs_LIBCSTRING_SYSTEM ".",
			 function,
			 filename );

			libewf_handle_free(
			 &( export_handle->ewf_output_handle ),
			 NULL );

			return( -1 );
		}
	}
	else if( export_handle->output_format == EXPORT_HANDLE_OUTPUT_FORMAT_RAW )
	{
		filename_length = libcstring_system_string_length(
		                   filename );

		if( ( filename_length == 1 )
		 && ( libcstring_system_string_compare(
		       filename,
		       _LIBCSTRING_SYSTEM_STRING( "-" ),
		       1 ) == 0 ) )
		{
			export_handle->use_stdout = 1;
		}
		else
		{
			if( export_handle->raw_output_handle != NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
				 "%s: invalid export handle - raw output handle already set.",
				 function );

				return( -1 );
			}
			if( libsmraw_handle_initialize(
			     &( export_handle->raw_output_handle ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create raw output handle.",
				 function );

				return( -1 );
			}
			filenames[ 0 ] = (libcstring_system_character_t *) filename;

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
			if( libsmraw_handle_open_wide(
			     export_handle->raw_output_handle,
			     filenames,
			     1,
			     LIBSMRAW_OPEN_WRITE,
			     error ) != 1 )
#else
			if( libsmraw_handle_open(
			     export_handle->raw_output_handle,
			     filenames,
			     1,
			     LIBSMRAW_OPEN_WRITE,
			     error ) != 1 )
#endif
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_OPEN_FAILED,
				 "%s: unable to open file: %" PRIs_LIBCSTRING_SYSTEM ".",
				 function,
				 filename );

				libsmraw_handle_free(
				 &( export_handle->raw_output_handle ),
				 NULL );

				return( -1 );
			}
		}
	}
	return( 1 );
}

/* Closes the export handle
 * Returns the 0 if succesful or -1 on error
 */
int export_handle_close(
     export_handle_t *export_handle,
     libcerror_error_t **error )
{
	static char *function = "export_handle_close";

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( libewf_handle_close(
	     export_handle->input_handle,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close input handle.",
		 function );

		return( -1 );
	}
	if( export_handle->ewf_output_handle != NULL )
	{
		if( libewf_handle_close(
		     export_handle->ewf_output_handle,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close ewf output handle.",
			 function );

			return( -1 );
		}
	}
	if( export_handle->raw_output_handle != NULL )
	{
		if( libsmraw_handle_close(
		     export_handle->raw_output_handle,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close raw output handle.",
			 function );

			return( -1 );
		}
	}
	return( 0 );
}

/* Prepares a buffer after reading the input of the export handle
 * Returns the resulting buffer size or -1 on error
 */
ssize_t export_handle_prepare_read_buffer(
         export_handle_t *export_handle,
         storage_media_buffer_t *storage_media_buffer,
         libcerror_error_t **error )
{
	static char *function = "export_handle_prepare_read_buffer";
	ssize_t process_count = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( storage_media_buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid storage media buffer.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
	storage_media_buffer->raw_buffer_data_size = storage_media_buffer->raw_buffer_size;

	process_count = libewf_handle_prepare_read_chunk(
	                 export_handle->input_handle,
	                 storage_media_buffer->compression_buffer,
	                 storage_media_buffer->compression_buffer_data_size,
	                 storage_media_buffer->raw_buffer,
	                 &( storage_media_buffer->raw_buffer_data_size ),
	                 storage_media_buffer->is_compressed,
	                 storage_media_buffer->checksum,
	                 storage_media_buffer->process_checksum,
	                 error );

	if( process_count == -1 )
	{
		libcerror_error_free(
		 error );

		/* Wipe the chunk if nescessary
		 */
		if( export_handle->zero_chunk_on_error != 0 )
		{
			if( ( storage_media_buffer->is_compressed != 0 )
			 && ( memory_set(
			       storage_media_buffer->compression_buffer,
			       0,
			       storage_media_buffer->compression_buffer_size ) == NULL ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to zero compression buffer.",
				 function );

				return( -1 );
			}
			if( memory_set(
			     storage_media_buffer->raw_buffer,
			     0,
			     storage_media_buffer->raw_buffer_size ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to zero raw buffer.",
				 function );

				return( -1 );
			}
		}
		process_count = export_handle->input_chunk_size;

		/* Appends a read error
		 */
		if( export_handle_append_read_error(
		     export_handle,
		     export_handle->input_offset,
		     process_count,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append read error.",
			 function );

			return( -1 );
		}
	}
	if( storage_media_buffer->is_compressed == 0 )
	{
		storage_media_buffer->data_in_compression_buffer = 1;
	}
	else
	{
		storage_media_buffer->data_in_compression_buffer = 0;
	}
	export_handle->input_offset += process_count;
#else
	process_count = (ssize_t) storage_media_buffer->raw_buffer_data_size;
#endif
	return( process_count );
}

/* Reads a buffer from the input of the export handle
 * Returns the number of bytes written or -1 on error
 */
ssize_t export_handle_read_buffer(
         export_handle_t *export_handle,
         storage_media_buffer_t *storage_media_buffer,
         size_t read_size,
         libcerror_error_t **error )
{
	static char *function = "export_handle_read_buffer";
	ssize_t read_count    = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( storage_media_buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid storage media buffer.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
	read_count = libewf_handle_read_chunk(
                      export_handle->input_handle,
                      storage_media_buffer->compression_buffer,
                      storage_media_buffer->compression_buffer_size,
	              &( storage_media_buffer->is_compressed ),
	              &( storage_media_buffer->compression_buffer[ storage_media_buffer->raw_buffer_size ] ),
	              &( storage_media_buffer->checksum ),
	              &( storage_media_buffer->process_checksum ),
	              error );
#else
	read_count = libewf_handle_read_buffer(
                      export_handle->input_handle,
                      storage_media_buffer->raw_buffer,
                      read_size,
	              error );
#endif

	if( read_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read storage media buffer.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
	storage_media_buffer->compression_buffer_data_size = (size_t) read_count;
#else
	storage_media_buffer->raw_buffer_data_size         = (size_t) read_count;
#endif

	return( read_count );
}

/* Prepares a buffer before writing the output of the export handle
 * Returns the resulting buffer size or -1 on error
 */
ssize_t export_handle_prepare_write_buffer(
         export_handle_t *export_handle,
         storage_media_buffer_t *storage_media_buffer,
         libcerror_error_t **error )
{
	static char *function = "export_handle_prepare_write_buffer";
	ssize_t process_count = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( storage_media_buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid storage media buffer.",
		 function );

		return( -1 );
	}
	if( export_handle->output_format == EXPORT_HANDLE_OUTPUT_FORMAT_EWF )
	{
		if( export_handle->ewf_output_handle == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid export handle - missing ewf output handle.",
			 function );

			return( -1 );
		}
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
		storage_media_buffer->compression_buffer_data_size = storage_media_buffer->compression_buffer_size;

		process_count = libewf_handle_prepare_write_chunk(
				 export_handle->ewf_output_handle,
				 storage_media_buffer->raw_buffer,
				 storage_media_buffer->raw_buffer_data_size,
				 storage_media_buffer->compression_buffer,
				 &( storage_media_buffer->compression_buffer_data_size ),
				 &( storage_media_buffer->is_compressed ),
				 &( storage_media_buffer->checksum ),
				 &( storage_media_buffer->process_checksum ),
				 error );

		if( process_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to prepare storage media buffer before writing.",
			 function );

			return( -1 );
		}
#else
		process_count = (ssize_t) storage_media_buffer->raw_buffer_data_size;
#endif
	}
	else if( export_handle->output_format == EXPORT_HANDLE_OUTPUT_FORMAT_RAW )
	{
		process_count = (ssize_t) storage_media_buffer->raw_buffer_data_size;
	}
	return( process_count );
}

/* Writes a buffer to the output of the export handle
 * Returns the number of bytes written or -1 on error
 */
ssize_t export_handle_write_buffer(
         export_handle_t *export_handle,
         storage_media_buffer_t *storage_media_buffer,
         size_t write_size,
         libcerror_error_t **error )
{
	static char *function        = "export_handle_write_buffer";
	ssize_t write_count          = 0;

#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
	uint8_t *raw_write_buffer    = NULL;
	size_t raw_write_buffer_size = 0;
#endif

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( storage_media_buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid storage media buffer.",
		 function );

		return( -1 );
	}
	if( write_size == 0 )
	{
		return( 0 );
	}
	if( export_handle->output_format == EXPORT_HANDLE_OUTPUT_FORMAT_EWF )
	{
		if( export_handle->ewf_output_handle == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid export handle - missing ewf output handle.",
			 function );

			return( -1 );
		}
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
		if( storage_media_buffer->is_compressed == 0 )
		{
			raw_write_buffer      = storage_media_buffer->raw_buffer;
			raw_write_buffer_size = storage_media_buffer->raw_buffer_data_size;
		}
		else
		{
			raw_write_buffer      = storage_media_buffer->compression_buffer;
			raw_write_buffer_size = storage_media_buffer->compression_buffer_data_size;
		}
		if( write_size != raw_write_buffer_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: mismatch in write size and number of bytes in storage media buffer.",
			 function );

			return( -1 );
		}
		write_count = libewf_handle_write_chunk(
			       export_handle->ewf_output_handle,
			       raw_write_buffer,
			       raw_write_buffer_size,
			       storage_media_buffer->raw_buffer_data_size,
			       storage_media_buffer->is_compressed,
			       storage_media_buffer->checksum_buffer,
			       storage_media_buffer->checksum,
			       storage_media_buffer->process_checksum,
			       error );
#else
		write_count = libewf_handle_write_buffer(
			       export_handle->ewf_output_handle,
			       storage_media_buffer->raw_buffer,
			       write_size,
			       error );
#endif

		if( write_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to write storage media buffer.",
			 function );

			return( -1 );
		}
	}
	else if( export_handle->output_format == EXPORT_HANDLE_OUTPUT_FORMAT_RAW )
	{
		if( export_handle->use_stdout != 0 )
		{
			write_count = libcsystem_file_io_write(
			               1,
				       storage_media_buffer->raw_buffer,
				       write_size );
		}
		else
		{
			write_count = libsmraw_handle_write_buffer(
				       export_handle->raw_output_handle,
				       storage_media_buffer->raw_buffer,
				       write_size,
				       error );
		}
		if( write_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to write storage media buffer.",
			 function );

			return( -1 );
		}
	}
	return( write_count );
}

/* Seeks the offset
 * Returns the resulting offset or -1 on error
 */
off64_t export_handle_seek_offset(
         export_handle_t *export_handle,
         off64_t offset,
         libcerror_error_t **error )
{
	static char *function = "export_handle_seek_offset";

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( libewf_handle_seek_offset(
	     export_handle->input_handle,
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
	return( offset );
}

/* Swaps the byte order of byte pairs within a buffer of a certain size
 * Returns 1 if successful, -1 on error
 */
int export_handle_swap_byte_pairs(
     export_handle_t *export_handle,
     storage_media_buffer_t *storage_media_buffer,
     size_t read_size,
     libcerror_error_t **error )
{
	uint8_t *data         = NULL;
	static char *function = "export_handle_swap_byte_pairs";
	size_t data_offset    = 0;
	size_t data_size      = 0;
	uint8_t byte          = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( storage_media_buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid storage media buffer.",
		 function );

		return( -1 );
	}
	if( ( read_size == 0 )
	 || ( read_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid read size value out of bounds.",
		 function );

		return( -1 );
	}
	/* If the last bit is set the value is odd
	 */
	if( ( read_size & 0x01 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid read size value is odd.",
		 function );

		return( -1 );
	}
	if( storage_media_buffer_get_data(
	     storage_media_buffer,
	     &data,
	     &data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve storage media buffer data.",
		 function );

		return( -1 );
	}
	if( read_size != data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: mismatch in read size and data size.",
		 function );

		return( -1 );
	}
	for( data_offset = 0;
	     data_offset < read_size;
	     data_offset += 2 )
	{
		byte                    = data[ data_offset ];
		data[ data_offset ]     = data[ data_offset + 1 ];
		data[ data_offset + 1 ] = byte;
	}
	return( 1 );
}

/* Initializes the integrity hash(es)
 * Returns 1 if successful or -1 on error
 */
int export_handle_initialize_integrity_hash(
     export_handle_t *export_handle,
     libcerror_error_t **error )
{
	static char *function = "export_handle_initialize_integrity_hash";

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( export_handle->calculate_md5 != 0 )
	{
		if( libhmac_md5_initialize(
		     &( export_handle->md5_context ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create MD5 context.",
			 function );

			goto on_error;
		}
		export_handle->md5_context_initialized = 1;
	}
	if( export_handle->calculate_sha1 != 0 )
	{
		if( libhmac_sha1_initialize(
		     &( export_handle->sha1_context ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create SHA1 context.",
			 function );

			goto on_error;
		}
		export_handle->sha1_context_initialized = 1;
	}
	if( export_handle->calculate_sha256 != 0 )
	{
		if( libhmac_sha256_initialize(
		     &( export_handle->sha256_context ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create SHA256 context.",
			 function );

			goto on_error;
		}
		export_handle->sha256_context_initialized = 1;
	}
	return( 1 );

on_error:
	if( export_handle->sha1_context != NULL )
	{
		libhmac_sha1_free(
		 &( export_handle->sha1_context ),
		 NULL );
	}
	if( export_handle->md5_context != NULL )
	{
		libhmac_md5_free(
		 &( export_handle->md5_context ),
		 NULL );
	}
	return( -1 );
}

/* Updates the integrity hash(es)
 * Returns 1 if successful or -1 on error
 */
int export_handle_update_integrity_hash(
     export_handle_t *export_handle,
     uint8_t *buffer,
     size_t buffer_size,
     libcerror_error_t **error )
{
	static char *function = "export_handle_update_integrity_hash";

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( ( buffer_size == 0 )
	 || ( buffer_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid buffer size value out of bounds.",
		 function );

		return( -1 );
	}
	if( export_handle->calculate_md5 != 0 )
	{
		if( libhmac_md5_update(
		     export_handle->md5_context,
		     buffer,
		     buffer_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to update MD5 digest hash.",
			 function );

			return( -1 );
		}
	}
	if( export_handle->calculate_sha1 != 0 )
	{
		if( libhmac_sha1_update(
		     export_handle->sha1_context,
		     buffer,
		     buffer_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to update SHA1 digest hash.",
			 function );

			return( -1 );
		}
	}
	if( export_handle->calculate_sha256 != 0 )
	{
		if( libhmac_sha256_update(
		     export_handle->sha256_context,
		     buffer,
		     buffer_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to update SHA256 digest hash.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Finalizes the integrity hash(es)
 * Returns 1 if successful or -1 on error
 */
int export_handle_finalize_integrity_hash(
     export_handle_t *export_handle,
     libcerror_error_t **error )
{
	uint8_t calculated_md5_hash[ LIBHMAC_MD5_HASH_SIZE ];
	uint8_t calculated_sha1_hash[ LIBHMAC_SHA1_HASH_SIZE ];
	uint8_t calculated_sha256_hash[ LIBHMAC_SHA256_HASH_SIZE ];

	static char *function = "export_handle_finalize_integrity_hash";

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( export_handle->calculate_md5 != 0 )
	{
		if( export_handle->calculated_md5_hash_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid export handle - missing calculated MD5 hash string.",
			 function );

			return( -1 );
		}
		if( libhmac_md5_finalize(
		     export_handle->md5_context,
		     calculated_md5_hash,
		     LIBHMAC_MD5_HASH_SIZE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to finalize MD5 hash.",
			 function );

			return( -1 );
		}
		if( digest_hash_copy_to_string(
		     calculated_md5_hash,
		     LIBHMAC_MD5_HASH_SIZE,
		     export_handle->calculated_md5_hash_string,
		     33,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBEWF_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set calculated MD5 hash string.",
			 function );

			return( -1 );
		}
	}
	if( export_handle->calculate_sha1 != 0 )
	{
		if( export_handle->calculated_sha1_hash_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid export handle - missing calculated SHA1 hash string.",
			 function );

			return( -1 );
		}
		if( libhmac_sha1_finalize(
		     export_handle->sha1_context,
		     calculated_sha1_hash,
		     LIBHMAC_SHA1_HASH_SIZE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to finalize SHA1 hash.",
			 function );

			return( -1 );
		}
		if( digest_hash_copy_to_string(
		     calculated_sha1_hash,
		     LIBHMAC_SHA1_HASH_SIZE,
		     export_handle->calculated_sha1_hash_string,
		     41,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create calculated SHA1 hash string.",
			 function );

			return( -1 );
		}
	}
	if( export_handle->calculate_sha256 != 0 )
	{
		if( export_handle->calculated_sha256_hash_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid export handle - missing calculated SHA256 hash string.",
			 function );

			return( -1 );
		}
		if( libhmac_sha256_finalize(
		     export_handle->sha256_context,
		     calculated_sha256_hash,
		     LIBHMAC_SHA256_HASH_SIZE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to finalize SHA256 hash.",
			 function );

			return( -1 );
		}
		if( digest_hash_copy_to_string(
		     calculated_sha256_hash,
		     LIBHMAC_SHA256_HASH_SIZE,
		     export_handle->calculated_sha256_hash_string,
		     41,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create calculated SHA256 hash string.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Determines if the input is corrupted
 * Returns 1 if corrupted, 0 if not or -1 on error
 */
int export_handle_input_is_corrupted(
     export_handle_t *export_handle,
     libcerror_error_t **error )
{
	static char *function = "export_handle_get_output_chunk_size";
	int result            = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	result = libewf_handle_segment_files_corrupted(
	          export_handle->input_handle,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if segment files are corrupted.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the chunk size
 * Returns 1 if successful or -1 on error
 */
int export_handle_get_output_chunk_size(
     export_handle_t *export_handle,
     size32_t *chunk_size,
     libcerror_error_t **error )
{
	static char *function = "export_handle_get_output_chunk_size";

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( chunk_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk size.",
		 function );

		return( -1 );
	}
	if( export_handle->output_format == EXPORT_HANDLE_OUTPUT_FORMAT_EWF )
	{
		if( export_handle->ewf_output_handle == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid export handle - missing ewf output handle.",
			 function );

			return( -1 );
		}
		if( libewf_handle_get_chunk_size(
		     export_handle->ewf_output_handle,
		     chunk_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve chunk size.",
			 function );

			return( -1 );
		}
	}
	else if( export_handle->output_format == EXPORT_HANDLE_OUTPUT_FORMAT_RAW )
	{
		*chunk_size = export_handle->input_chunk_size;
	}
	return( 1 );
}

/* Prompts the user for a string
 * Returns 1 if successful, 0 if no input was provided or -1 on error
 */
int export_handle_prompt_for_string(
     export_handle_t *export_handle,
     const libcstring_system_character_t *request_string,
     libcstring_system_character_t **internal_string,
     size_t *internal_string_size,
     libcerror_error_t **error )
{
	static char *function = "export_handle_prompt_for_string";
	int result            = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( internal_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid internal string.",
		 function );

		return( -1 );
	}
	if( internal_string_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid internal string size.",
		 function );

		return( -1 );
	}
	if( *internal_string != NULL )
	{
		memory_free(
		 *internal_string );

		*internal_string      = NULL;
		*internal_string_size = 0;
	}
	*internal_string_size = EXPORT_HANDLE_STRING_SIZE;

	*internal_string = libcstring_system_string_allocate(
	                    *internal_string_size );

	if( *internal_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create internal string.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *internal_string,
	     0,
	     *internal_string_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear internal string.",
		 function );

		goto on_error;
	}
	result = ewfinput_get_string_variable(
	          export_handle->notify_stream,
	          request_string,
	          *internal_string,
	          *internal_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve string variable.",
		 function );

		goto on_error;
	}
	return( result );

on_error:
	if( *internal_string != NULL )
	{
		memory_free(
		 *internal_string );

		*internal_string = NULL;
	}
	*internal_string_size = 0;

	return( -1 );
}

/* Prompts the user for the compression method
 * Returns 1 if successful, 0 if no input was provided or -1 on error
 */
int export_handle_prompt_for_compression_method(
     export_handle_t *export_handle,
     const libcstring_system_character_t *request_string,
     libcerror_error_t **error )
{
	libcstring_system_character_t *fixed_string_variable = NULL;
	static char *function                                = "export_handle_prompt_for_compression_method";
	uint8_t compression_methods_amount                   = 0;
	int result                                           = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
/* experimental version only
	if( export_handle->ewf_format != LIBEWF_FORMAT_V2_ENCASE7 )
*/
	{
		compression_methods_amount = 1;
	}
/* experimental version only
	else
	{
		compression_methods_amount = EWFINPUT_COMPRESSION_METHODS_AMOUNT;
	}
*/
	result = ewfinput_get_fixed_string_variable(
	          export_handle->notify_stream,
	          export_handle->input_buffer,
	          EXPORT_HANDLE_INPUT_BUFFER_SIZE,
	          request_string,
	          ewfinput_compression_methods,
	          compression_methods_amount,
	          EWFINPUT_COMPRESSION_METHODS_DEFAULT,
	          &fixed_string_variable,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve fixed string variable.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		result = ewfinput_determine_compression_method(
			  fixed_string_variable,
			  &( export_handle->compression_method ),
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine compression method.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Prompts the user for the compression level
 * Returns 1 if successful, 0 if no input was provided or -1 on error
 */
int export_handle_prompt_for_compression_level(
     export_handle_t *export_handle,
     const libcstring_system_character_t *request_string,
     libcerror_error_t **error )
{
	libcstring_system_character_t *fixed_string_variable = NULL;
	static char *function                                = "export_handle_prompt_for_compression_level";
	int result                                           = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	result = ewfinput_get_fixed_string_variable(
	          export_handle->notify_stream,
	          export_handle->input_buffer,
	          EXPORT_HANDLE_INPUT_BUFFER_SIZE,
	          request_string,
	          ewfinput_compression_levels,
	          EWFINPUT_COMPRESSION_LEVELS_AMOUNT,
	          EWFINPUT_COMPRESSION_LEVELS_DEFAULT,
	          &fixed_string_variable,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve fixed string variable.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		result = ewfinput_determine_compression_values(
			  fixed_string_variable,
			  &( export_handle->compression_level ),
			  &( export_handle->compression_flags ),
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine compression values.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Prompts the user for the output format
 * Returns 1 if successful, 0 if no input was provided or -1 on error
 */
int export_handle_prompt_for_output_format(
     export_handle_t *export_handle,
     const libcstring_system_character_t *request_string,
     libcerror_error_t **error )
{
	libcstring_system_character_t *fixed_string_variable = NULL;
	static char *function                                = "export_handle_prompt_for_output_format";
	size_t fixed_string_variable_length                  = 0;
	int result                                           = 0;

	libcstring_system_character_t *format_types[ 17 ] = \
	{ _LIBCSTRING_SYSTEM_STRING( "raw" ),
	  _LIBCSTRING_SYSTEM_STRING( "files" ),
	  _LIBCSTRING_SYSTEM_STRING( "ewf" ),
	  _LIBCSTRING_SYSTEM_STRING( "smart" ),
	  _LIBCSTRING_SYSTEM_STRING( "ftk" ),
	  _LIBCSTRING_SYSTEM_STRING( "encase1" ),
	  _LIBCSTRING_SYSTEM_STRING( "encase2" ),
	  _LIBCSTRING_SYSTEM_STRING( "encase3" ),
	  _LIBCSTRING_SYSTEM_STRING( "encase4" ),
	  _LIBCSTRING_SYSTEM_STRING( "encase5" ),
	  _LIBCSTRING_SYSTEM_STRING( "encase6" ),
	  _LIBCSTRING_SYSTEM_STRING( "encase7" ),
	  _LIBCSTRING_SYSTEM_STRING( "encase7-v2" ),
	  _LIBCSTRING_SYSTEM_STRING( "linen5" ),
	  _LIBCSTRING_SYSTEM_STRING( "linen6" ),
	  _LIBCSTRING_SYSTEM_STRING( "linen7" ),
	  _LIBCSTRING_SYSTEM_STRING( "ewfx" ) };

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	result = ewfinput_get_fixed_string_variable(
	          export_handle->notify_stream,
	          export_handle->input_buffer,
	          EXPORT_HANDLE_INPUT_BUFFER_SIZE,
	          request_string,
	          format_types,
	          17,
	          0,
	          &fixed_string_variable,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve fixed string variable.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		result = 0;

		fixed_string_variable_length = libcstring_system_string_length(
		                                fixed_string_variable );

		if( fixed_string_variable_length == 3 )
		{
			if( libcstring_system_string_compare(
			     fixed_string_variable,
			     _LIBCSTRING_SYSTEM_STRING( "raw" ),
			     3 ) == 0 )
			{
				export_handle->output_format = EXPORT_HANDLE_OUTPUT_FORMAT_RAW;
				result                       = 1;
			}
		}
		else if( fixed_string_variable_length == 5 )
		{
			if( libcstring_system_string_compare(
			     fixed_string_variable,
			     _LIBCSTRING_SYSTEM_STRING( "files" ),
			     5 ) == 0 )
			{
				export_handle->output_format = EXPORT_HANDLE_OUTPUT_FORMAT_FILES;
				result                       = 1;
			}
		}
		if( result == 0 )
		{
			result = ewfinput_determine_ewf_format(
				  fixed_string_variable,
				  &( export_handle->ewf_format ),
				  error );

			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine format.",
				 function );

				return( -1 );
			}
			export_handle->output_format = EXPORT_HANDLE_OUTPUT_FORMAT_EWF;
		}
	}
	return( result );
}

/* Prompts the user for the number of sectors per chunk
 * Returns 1 if successful, 0 if no input was provided or -1 on error
 */
int export_handle_prompt_for_sectors_per_chunk(
     export_handle_t *export_handle,
     const libcstring_system_character_t *request_string,
     libcerror_error_t **error )
{
	libcstring_system_character_t *fixed_string_variable = NULL;
	static char *function                                = "export_handle_prompt_for_sectors_per_chunk";
	int result                                           = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	result = ewfinput_get_fixed_string_variable(
	          export_handle->notify_stream,
	          export_handle->input_buffer,
	          EXPORT_HANDLE_INPUT_BUFFER_SIZE,
	          request_string,
	          ewfinput_sector_per_block_sizes,
	          EWFINPUT_SECTOR_PER_BLOCK_SIZES_AMOUNT,
	          EWFINPUT_SECTOR_PER_BLOCK_SIZES_DEFAULT,
	          &fixed_string_variable,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve fixed string variable.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		result = ewfinput_determine_sectors_per_chunk(
			  fixed_string_variable,
			  &( export_handle->sectors_per_chunk ),
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine sectors per chunk.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Prompts the user for the maximum segment size
 * Returns 1 if successful, 0 if no input was provided or -1 on error
 */
int export_handle_prompt_for_maximum_segment_size(
     export_handle_t *export_handle,
     const libcstring_system_character_t *request_string,
     libcerror_error_t **error )
{
	static char *function        = "export_handle_prompt_for_maximum_segment_size";
	uint64_t default_input_size  = 0;
	uint64_t input_size_variable = 0;
	uint64_t maximum_input_size  = 0;
	uint64_t minimum_input_size  = 0;
	int result                   = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	maximum_input_size = EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_64BIT;

	if( export_handle->output_format == EXPORT_HANDLE_OUTPUT_FORMAT_EWF )
	{
		if( export_handle->ewf_format != LIBEWF_FORMAT_ENCASE6 )
		{
			maximum_input_size = EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_32BIT;
		}
		minimum_input_size = EWFCOMMON_MINIMUM_SEGMENT_FILE_SIZE;

	       	if( default_input_size == 0 )
       		{
			default_input_size = EWFCOMMON_DEFAULT_SEGMENT_FILE_SIZE;
       		}
	}
	result = ewfinput_get_byte_size_variable(
	          export_handle->notify_stream,
	          export_handle->input_buffer,
	          EXPORT_HANDLE_INPUT_BUFFER_SIZE,
	          request_string,
	          minimum_input_size,
	          maximum_input_size,
	          default_input_size,
	          &input_size_variable,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve byte size variable.",
		 function );

		return( -1 );
	}
	export_handle->maximum_segment_size = input_size_variable;

	return( result );
}

/* Prompts the user for the export offset
 * Returns 1 if successful, 0 if no input was provided or -1 on error
 */
int export_handle_prompt_for_export_offset(
     export_handle_t *export_handle,
     const libcstring_system_character_t *request_string,
     libcerror_error_t **error )
{
	static char *function        = "export_handle_prompt_for_export_offset";
	uint64_t input_size_variable = 0;
	int result                   = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	result = ewfinput_get_size_variable(
	          export_handle->notify_stream,
	          export_handle->input_buffer,
	          EXPORT_HANDLE_INPUT_BUFFER_SIZE,
	          request_string,
	          0,
	          export_handle->input_media_size,
	          export_handle->export_offset,
	          &input_size_variable,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size variable.",
		 function );

		return( -1 );
	}
	export_handle->export_offset = input_size_variable;

	return( result );
}

/* Prompts the user for the export size
 * Returns 1 if successful, 0 if no input was provided or -1 on error
 */
int export_handle_prompt_for_export_size(
     export_handle_t *export_handle,
     const libcstring_system_character_t *request_string,
     libcerror_error_t **error )
{
	static char *function        = "export_handle_prompt_for_export_size";
	uint64_t default_input_size  = 0;
	uint64_t input_size_variable = 0;
	uint64_t maximum_input_size  = 0;
	int result                   = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	maximum_input_size = export_handle->input_media_size
	                   - export_handle->export_offset;

	if( ( export_handle->export_size == 0 )
	 || ( export_handle->export_size > maximum_input_size ) )
	{
		default_input_size = maximum_input_size;
	}
	else
	{
		default_input_size = export_handle->export_size;
	}
	result = ewfinput_get_size_variable(
	          export_handle->notify_stream,
	          export_handle->input_buffer,
	          EXPORT_HANDLE_INPUT_BUFFER_SIZE,
	          request_string,
	          0,
	          maximum_input_size,
	          default_input_size,
	          &input_size_variable,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size variable.",
		 function );

		return( -1 );
	}
	export_handle->export_size = input_size_variable;

	return( result );
}

/* Sets a string
 * Returns 1 if successful or -1 on error
 */
int export_handle_set_string(
     export_handle_t *export_handle,
     const libcstring_system_character_t *string,
     libcstring_system_character_t **internal_string,
     size_t *internal_string_size,
     libcerror_error_t **error )
{
	static char *function = "export_handle_set_string";
	size_t string_length  = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	if( internal_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid internal string.",
		 function );

		return( -1 );
	}
	if( internal_string_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid internal string size.",
		 function );

		return( -1 );
	}
	if( *internal_string != NULL )
	{
		memory_free(
		 *internal_string );

		*internal_string      = NULL;
		*internal_string_size = 0;
	}
	string_length = libcstring_system_string_length(
	                 string );

	if( string_length > 0 )
	{
		*internal_string = libcstring_system_string_allocate(
		                    string_length + 1 );

		if( *internal_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create internal string.",
			 function );

			goto on_error;
		}
		if( libcstring_system_string_copy(
		     *internal_string,
		     string,
		     string_length ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy string.",
			 function );

			goto on_error;
		}
		( *internal_string )[ string_length ] = 0;

		*internal_string_size = string_length + 1;
	}
	return( 1 );

on_error:
	if( *internal_string != NULL )
	{
		memory_free(
		 *internal_string );

		*internal_string = NULL;
	}
	*internal_string_size = 0;

	return( -1 );
}

/* Sets the compression values
 * Returns 1 if successful, 0 if unsupported value or -1 on error
 */
int export_handle_set_compression_values(
     export_handle_t *export_handle,
     const libcstring_system_character_t *string,
     libcerror_error_t **error )
{
	libcstring_system_character_t *string_segment    = NULL;
	static char *function                            = "export_handle_set_compression_values";
	size_t string_segment_size                       = 0;
	size_t string_length                             = 0;
	int number_of_segments                           = 0;
	int result                                       = 0;
	int segment_index                                = 0;

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	libcsplit_wide_split_string_t *string_elements   = NULL;
#else
	libcsplit_narrow_split_string_t *string_elements = NULL;
#endif

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	string_length = libcstring_system_string_length(
	                 string );

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libcsplit_wide_string_split(
	     string,
	     string_length + 1,
	     (wchar_t) ':',
	     &string_elements,
	     error ) != 1 )
#else
	if( libcsplit_narrow_string_split(
	     string,
	     string_length + 1,
	     (char) ':',
	     &string_elements,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to split string.",
		 function );

		goto on_error;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libcsplit_wide_split_string_get_number_of_segments(
	     string_elements,
	     &number_of_segments,
	     error ) != 1 )
#else
	if( libcsplit_narrow_split_string_get_number_of_segments(
	     string_elements,
	     &number_of_segments,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of segments.",
		 function );

		return( -1 );
	}
	if( ( number_of_segments != 1 )
	 && ( number_of_segments != 2 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: number of segments is out of bounds.",
		 function );

		return( -1 );
	}
	if( number_of_segments == 2 )
	{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		if( libcsplit_wide_split_string_get_segment_by_index(
		     string_elements,
		     segment_index,
		     &string_segment,
		     &string_segment_size,
		     error ) != 1 )
#else
		if( libcsplit_narrow_split_string_get_segment_by_index(
		     string_elements,
		     segment_index,
		     &string_segment,
		     &string_segment_size,
		     error ) != 1 )
#endif
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve string segment: %d.",
			 function,
			 segment_index );

			goto on_error;
		}
		if( string_segment == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing string segment: %d.",
			 function,
			 segment_index );

			goto on_error;
		}
		result = ewfinput_determine_compression_method(
			  string_segment,
			  &( export_handle->compression_method ),
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine compression method.",
			 function );

			goto on_error;
		}
/* experimental version only
		if( export_handle->ewf_format != LIBEWF_FORMAT_V2_ENCASE7 )
*/
		{
			if( export_handle->compression_method != LIBEWF_COMPRESSION_METHOD_DEFLATE )
			{
				export_handle->compression_method = LIBEWF_COMPRESSION_METHOD_DEFLATE;

				result = 0;
			}
		}
		segment_index++;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libcsplit_wide_split_string_get_segment_by_index(
	     string_elements,
	     segment_index,
	     &string_segment,
	     &string_segment_size,
	     error ) != 1 )
#else
	if( libcsplit_narrow_split_string_get_segment_by_index(
	     string_elements,
	     segment_index,
	     &string_segment,
	     &string_segment_size,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve string segment: %d.",
		 function,
		 segment_index );

		goto on_error;
	}
	if( string_segment == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing string segment: %d.",
		 function,
		 segment_index );

		goto on_error;
	}
	result = ewfinput_determine_compression_values(
	          string_segment,
	          &( export_handle->compression_level ),
	          &( export_handle->compression_flags ),
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine compression values.",
		 function );

		goto on_error;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libcsplit_wide_split_string_free(
	     &string_elements,
	     error ) != 1 )
#else
	if( libcsplit_narrow_split_string_free(
	     &string_elements,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free split string.",
		 function );

		goto on_error;
	}
	return( result );

on_error:
	if( string_elements != NULL )
	{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		libcsplit_wide_split_string_free(
		 &string_elements,
		 NULL );
#else
		libcsplit_narrow_split_string_free(
		 &string_elements,
		 NULL );
#endif
	}
	return( -1 );
}


/* Sets the output format
 * Returns 1 if successful, 0 if unsupported value or -1 on error
 */
int export_handle_set_output_format(
     export_handle_t *export_handle,
     const libcstring_system_character_t *string,
     libcerror_error_t **error )
{
	static char *function = "export_handle_set_output_format";
	size_t string_length  = 0;
	int result            = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	string_length = libcstring_system_string_length(
	                 string );

	if( string_length == 3 )
	{
		if( libcstring_system_string_compare(
		     string,
		     _LIBCSTRING_SYSTEM_STRING( "raw" ),
		     3 ) == 0 )
		{
			export_handle->output_format = EXPORT_HANDLE_OUTPUT_FORMAT_RAW;
			result                       = 1;
		}
	}
	else if( string_length == 5 )
	{
		if( libcstring_system_string_compare(
		     string,
		     _LIBCSTRING_SYSTEM_STRING( "files" ),
		     5 ) == 0 )
		{
			export_handle->output_format = EXPORT_HANDLE_OUTPUT_FORMAT_FILES;
			result                       = 1;
		}
	}
	if( result == 0 )
	{
		result = ewfinput_determine_ewf_format(
			  string,
			  &( export_handle->ewf_format ),
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine format.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Sets the number of sectors per chunk
 * Returns 1 if successful, 0 if unsupported value or -1 on error
 */
int export_handle_set_sectors_per_chunk(
     export_handle_t *export_handle,
     const libcstring_system_character_t *string,
     libcerror_error_t **error )
{
	static char *function = "export_handle_set_sectors_per_chunk";
	int result            = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	result = ewfinput_determine_sectors_per_chunk(
	          string,
	          &( export_handle->sectors_per_chunk ),
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine sectors per chunk.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Sets the maximum segment file size
 * Returns 1 if successful, 0 if unsupported value or -1 on error
 */
int export_handle_set_maximum_segment_size(
     export_handle_t *export_handle,
     const libcstring_system_character_t *string,
     libcerror_error_t **error )
{
	static char *function = "export_handle_set_maximum_segment_size";
	size_t string_length  = 0;
	int result            = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	string_length = libcstring_system_string_length(
	                 string );

	result = byte_size_string_convert(
	          string,
	          string_length,
	          &( export_handle->maximum_segment_size ),
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine maximum segment size.",
		 function );

		return( -1 );
	}
	if( result != 0 )
	{
		if( export_handle->output_format == EXPORT_HANDLE_OUTPUT_FORMAT_EWF )
		{
			if( ( export_handle->maximum_segment_size < EWFCOMMON_MINIMUM_SEGMENT_FILE_SIZE )
			 || ( ( export_handle->ewf_format == LIBEWF_FORMAT_ENCASE6 )
			  &&  ( export_handle->maximum_segment_size >= (uint64_t) EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_64BIT ) )
			 || ( ( export_handle->ewf_format != LIBEWF_FORMAT_ENCASE6 )
			  &&  ( export_handle->maximum_segment_size >= (uint64_t) EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_32BIT ) ) )
			{
				export_handle->maximum_segment_size = EWFCOMMON_DEFAULT_SEGMENT_FILE_SIZE;

				result = 0;
			}
		}
		else if( export_handle->output_format == EXPORT_HANDLE_OUTPUT_FORMAT_RAW )
		{
			if( ( ( export_handle->maximum_segment_size != 0 )
			  &&  ( export_handle->maximum_segment_size >= (uint64_t) EWFCOMMON_MAXIMUM_SEGMENT_FILE_SIZE_64BIT ) ) )
			{
				export_handle->maximum_segment_size = EWFCOMMON_DEFAULT_SEGMENT_FILE_SIZE;

				result = 0;
			}
		}
	}
	return( result );
}

/* Sets the export offset
 * Returns 1 if successful or -1 on error
 */
int export_handle_set_export_offset(
     export_handle_t *export_handle,
     const libcstring_system_character_t *string,
     libcerror_error_t **error )
{
	static char *function = "export_handle_set_export_offset";
	size_t string_length  = 0;
	int result            = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	if( string[ 0 ] != (libcstring_system_character_t) '-' )
	{
		string_length = libcstring_system_string_length(
				 string );

		if( libcsystem_string_decimal_copy_to_64_bit(
		     string,
		     string_length + 1,
		     &( export_handle->export_offset ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine export offset.",
			 function );

			return( -1 );
		}
		result = 1;
	}
	return( result );
}

/* Sets the export size
 * Returns 1 if successful or -1 on error
 */
int export_handle_set_export_size(
     export_handle_t *export_handle,
     const libcstring_system_character_t *string,
     libcerror_error_t **error )
{
	static char *function = "export_handle_set_export_size";
	size_t string_length  = 0;
	int result            = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	if( string[ 0 ] != (libcstring_system_character_t) '-' )
	{
		string_length = libcstring_system_string_length(
				 string );

		if( libcsystem_string_decimal_copy_to_64_bit(
		     string,
		     string_length + 1,
		     &( export_handle->export_size ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine export size.",
			 function );

			return( -1 );
		}
		result = 1;
	}
	return( result );
}

/* Sets the header codepage
 * Returns 1 if successful or -1 on error
 */
int export_handle_set_header_codepage(
     export_handle_t *export_handle,
     const libcstring_system_character_t *string,
     libcerror_error_t **error )
{
	static char *function = "export_handle_set_header_codepage";
	int result            = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	result = ewfinput_determine_header_codepage(
	          string,
	          &export_handle->header_codepage,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine header codepage.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Sets the process buffer size
 * Returns 1 if successful, 0 if unsupported value or -1 on error
 */
int export_handle_set_process_buffer_size(
     export_handle_t *export_handle,
     const libcstring_system_character_t *string,
     libcerror_error_t **error )
{
	static char *function  = "export_handle_set_process_buffer_size";
	size_t string_length   = 0;
	uint64_t size_variable = 0;
	int result             = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	string_length = libcstring_system_string_length(
	                 string );

	result = byte_size_string_convert(
	          string,
	          string_length,
	          &size_variable,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine process buffer size.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( size_variable > (uint64_t) SSIZE_MAX )
		{
			export_handle->process_buffer_size = 0;

			result = 0;
		}
		else
		{
			export_handle->process_buffer_size = (size_t) size_variable;
		}
	}
	return( result );
}

/* Sets the additional digest types
 * Returns 1 if successful or -1 on error
 */
int export_handle_set_additional_digest_types(
     export_handle_t *export_handle,
     const libcstring_system_character_t *string,
     libcerror_error_t **error )
{
	libcstring_system_character_t *string_segment    = NULL;
	static char *function                            = "export_handle_set_additional_digest_types";
	size_t string_length                             = 0;
	size_t string_segment_size                       = 0;
	uint8_t calculate_sha1                           = 0;
	uint8_t calculate_sha256                         = 0;
	int number_of_segments                           = 0;
	int result                                       = 0;
	int segment_index                                = 0;

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	libcsplit_wide_split_string_t *string_elements   = NULL;
#else
	libcsplit_narrow_split_string_t *string_elements = NULL;
#endif

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	string_length = libcstring_system_string_length(
	                 string );

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libcsplit_wide_string_split(
	     string,
	     string_length + 1,
	     (wchar_t) ',',
	     &string_elements,
	     error ) != 1 )
#else
	if( libcsplit_narrow_string_split(
	     string,
	     string_length + 1,
	     (char) ',',
	     &string_elements,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to split string.",
		 function );

		goto on_error;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libcsplit_wide_split_string_get_number_of_segments(
	     string_elements,
	     &number_of_segments,
	     error ) != 1 )
#else
	if( libcsplit_narrow_split_string_get_number_of_segments(
	     string_elements,
	     &number_of_segments,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of segments.",
		 function );

		goto on_error;
	}
	for( segment_index = 0;
	     segment_index < number_of_segments;
	     segment_index++ )
	{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		if( libcsplit_wide_split_string_get_segment_by_index(
		     string_elements,
		     segment_index,
		     &string_segment,
		     &string_segment_size,
		     error ) != 1 )
#else
		if( libcsplit_narrow_split_string_get_segment_by_index(
		     string_elements,
		     segment_index,
		     &string_segment,
		     &string_segment_size,
		     error ) != 1 )
#endif
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve string segment: %d.",
			 function,
			 segment_index );

			goto on_error;
		}
		if( string_segment == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing string segment: %d.",
			 function,
			 segment_index );

			goto on_error;
		}
		if( string_segment_size == 5 )
		{
			if( libcstring_system_string_compare(
			     string_segment,
			     _LIBCSTRING_SYSTEM_STRING( "sha1" ),
			     4 ) == 0 )
			{
				calculate_sha1 = 1;
			}
			else if( libcstring_system_string_compare(
			          string_segment,
			          _LIBCSTRING_SYSTEM_STRING( "SHA1" ),
			          4 ) == 0 )
			{
				calculate_sha1 = 1;
			}
		}
		else if( string_segment_size == 6 )
		{
			if( libcstring_system_string_compare(
			     string_segment,
			     _LIBCSTRING_SYSTEM_STRING( "sha-1" ),
			     5 ) == 0 )
			{
				calculate_sha1 = 1;
			}
			else if( libcstring_system_string_compare(
			          string_segment,
			          _LIBCSTRING_SYSTEM_STRING( "sha_1" ),
			          5 ) == 0 )
			{
				calculate_sha1 = 1;
			}
			else if( libcstring_system_string_compare(
			          string_segment,
			          _LIBCSTRING_SYSTEM_STRING( "SHA-1" ),
			          5 ) == 0 )
			{
				calculate_sha1 = 1;
			}
			else if( libcstring_system_string_compare(
			          string_segment,
			          _LIBCSTRING_SYSTEM_STRING( "SHA_1" ),
			          5 ) == 0 )
			{
				calculate_sha1 = 1;
			}
		}
		else if( string_segment_size == 7 )
		{
			if( libcstring_system_string_compare(
			     string_segment,
			     _LIBCSTRING_SYSTEM_STRING( "sha256" ),
			     6 ) == 0 )
			{
				calculate_sha256 = 1;
			}
			else if( libcstring_system_string_compare(
			          string_segment,
			          _LIBCSTRING_SYSTEM_STRING( "SHA256" ),
			          6 ) == 0 )
			{
				calculate_sha256 = 1;
			}
		}
		else if( string_segment_size == 8 )
		{
			if( libcstring_system_string_compare(
			     string_segment,
			     _LIBCSTRING_SYSTEM_STRING( "sha-256" ),
			     7 ) == 0 )
			{
				calculate_sha256 = 1;
			}
			else if( libcstring_system_string_compare(
			          string_segment,
			          _LIBCSTRING_SYSTEM_STRING( "sha_256" ),
			          7 ) == 0 )
			{
				calculate_sha256 = 1;
			}
			else if( libcstring_system_string_compare(
			          string_segment,
			          _LIBCSTRING_SYSTEM_STRING( "SHA-256" ),
			          7 ) == 0 )
			{
				calculate_sha256 = 1;
			}
			else if( libcstring_system_string_compare(
			          string_segment,
			          _LIBCSTRING_SYSTEM_STRING( "SHA_256" ),
			          7 ) == 0 )
			{
				calculate_sha256 = 1;
			}
		}
	}
	if( ( calculate_sha1 != 0 )
	 && ( export_handle->calculate_sha1 == 0 ) )
	{
		export_handle->calculated_sha1_hash_string = libcstring_system_string_allocate(
		                                              41 );

		if( export_handle->calculated_sha1_hash_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create calculated SHA1 digest hash string.",
			 function );

			goto on_error;
		}
		export_handle->calculate_sha1 = 1;
	}
	if( ( calculate_sha256 != 0 )
	 && ( export_handle->calculate_sha256 == 0 ) )
	{
		export_handle->calculated_sha256_hash_string = libcstring_system_string_allocate(
		                                                65 );

		if( export_handle->calculated_sha256_hash_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create calculated SHA256 digest hash string.",
			 function );

			goto on_error;
		}
		export_handle->calculate_sha256 = 1;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libcsplit_wide_split_string_free(
	     &string_elements,
	     error ) != 1 )
#else
	if( libcsplit_narrow_split_string_free(
	     &string_elements,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free split string.",
		 function );

		goto on_error;
	}
	return( result );

on_error:
	if( string_elements != NULL )
	{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		libcsplit_wide_split_string_free(
		 &string_elements,
		 NULL );
#else
		libcsplit_narrow_split_string_free(
		 &string_elements,
		 NULL );
#endif
	}
	return( -1 );
}

/* Sets the output values of the export handle
 * Returns 1 if successful or -1 on error
 */
int export_handle_set_output_values(
     export_handle_t *export_handle,
     libcstring_system_character_t *acquiry_operating_system,
     libcstring_system_character_t *acquiry_software,
     libcstring_system_character_t *acquiry_software_version,
     uint8_t zero_chunk_on_error,
     uint8_t copy_input_values,
     libcerror_error_t **error )
{
#if defined( HAVE_GUID_SUPPORT ) || defined( WINAPI )
	uint8_t guid[ GUID_SIZE ];

	uint8_t guid_type          = 0;
#endif

	static char *function      = "export_handle_set_output_values";
	size_t value_string_length = 0;
	int result                 = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( libewf_handle_get_bytes_per_sector(
	     export_handle->input_handle,
	     &( export_handle->bytes_per_sector ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve bytes per sector from input handle.",
		 function );

		return( -1 );
	}
	if( libewf_handle_set_read_zero_chunk_on_error(
	     export_handle->input_handle,
	     zero_chunk_on_error,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set zero chunk on error in input handle.",
		 function );

		return( -1 );
	}
	if( export_handle->output_format == EXPORT_HANDLE_OUTPUT_FORMAT_EWF )
	{
		if( export_handle->ewf_output_handle == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid export handle - missing ewf output handle.",
			 function );

			return( -1 );
		}
		if( libewf_handle_copy_header_values(
		     export_handle->ewf_output_handle,
		     export_handle->input_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy header values.",
			 function );

			return( -1 );
		}
		/* Set acquiry operating system, software and software version
		 */
		if( acquiry_operating_system != NULL )
		{
			value_string_length = libcstring_system_string_length(
			                       acquiry_operating_system );

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
			result = libewf_handle_set_utf16_header_value(
			          export_handle->ewf_output_handle,
			          (uint8_t *) "acquiry_operating_system",
			          24,
			          (uint16_t *) acquiry_operating_system,
			          value_string_length,
			          error );
#else
			result = libewf_handle_set_utf8_header_value(
			          export_handle->ewf_output_handle,
			          (uint8_t *) "acquiry_operating_system",
			          24,
			          (uint8_t *) acquiry_operating_system,
			          value_string_length,
			          error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set header value: acquiry operating system.",
				 function );

				return( -1 );
			}
		}
		value_string_length = libcstring_system_string_length(
				       acquiry_software );

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libewf_handle_set_utf16_header_value(
			  export_handle->ewf_output_handle,
			  (uint8_t *) "acquiry_software",
			  16,
			  (uint16_t *) acquiry_software,
			  value_string_length,
			  error );
#else
		result = libewf_handle_set_utf8_header_value(
			  export_handle->ewf_output_handle,
			  (uint8_t *) "acquiry_software",
			  16,
			  (uint8_t *) acquiry_software,
			  value_string_length,
			  error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set header value: acquiry software.",
			 function );

			return( -1 );
		}
		value_string_length = libcstring_system_string_length(
				       acquiry_software_version );

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libewf_handle_set_utf16_header_value(
			  export_handle->ewf_output_handle,
			  (uint8_t *) "acquiry_software_version",
			  24,
			  (uint16_t *) acquiry_software_version,
			  value_string_length,
			  error );
#else
		result = libewf_handle_set_utf8_header_value(
			  export_handle->ewf_output_handle,
			  (uint8_t *) "acquiry_software_version",
			  24,
			  (uint8_t *) acquiry_software_version,
			  value_string_length,
			  error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set header value: acquiry software version.",
			 function );

			return( -1 );
		}
		if( libewf_handle_set_header_codepage(
		     export_handle->ewf_output_handle,
		     export_handle->header_codepage,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set header codepage.",
			 function );

			return( -1 );
		}
		if( libewf_handle_copy_media_values(
		     export_handle->ewf_output_handle,
		     export_handle->input_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy media values.",
			 function );

			return( -1 );
		}
		if( copy_input_values == 0 )
		{
			if( libewf_handle_set_media_size(
			     export_handle->ewf_output_handle,
			     (size64_t) export_handle->export_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set media size.",
				 function );

				return( -1 );
			}
		}
		/* Format needs to be set before segment file size and compression values
		 */
		if( copy_input_values != 0 )
		{
			if( libewf_handle_get_format(
			     export_handle->input_handle,
			     &( export_handle->ewf_format ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve format.",
				 function );

				return( -1 );
			}
		}
		if( libewf_handle_set_format(
		     export_handle->ewf_output_handle,
		     export_handle->ewf_format,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set format.",
			 function );

			return( -1 );
		}
		if( copy_input_values != 0 )
		{
/* experimental version only
			if( libewf_handle_get_compression_method(
			     export_handle->input_handle,
			     &( export_handle->compression_method ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve compression method.",
				 function );

				return( -1 );
			}
*/
			if( libewf_handle_get_compression_values(
			     export_handle->input_handle,
			     &( export_handle->compression_level ),
			     &( export_handle->compression_flags ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve compression values.",
				 function );

				return( -1 );
			}
		}
/* experimental version only
		if( export_handle->ewf_format != LIBEWF_FORMAT_V2_ENCASE7 )
		{
			if( export_handle->compression_method != LIBEWF_COMPRESSION_METHOD_DEFLATE )
			{
				export_handle->compression_method = LIBEWF_COMPRESSION_METHOD_DEFLATE;
			}
		}
		if( libewf_handle_set_compression_method(
		     export_handle->ewf_output_handle,
		     export_handle->compression_method,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set compression method.",
			 function );

			return( -1 );
		}
*/
		if( libewf_handle_set_compression_values(
		     export_handle->ewf_output_handle,
		     export_handle->compression_level,
		     export_handle->compression_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set compression values.",
			 function );

			return( -1 );
		}
		if( ( export_handle->compression_level != LIBEWF_COMPRESSION_NONE )
		 || ( ( export_handle->compression_flags & LIBEWF_COMPRESS_FLAG_USE_EMPTY_BLOCK_COMPRESSION ) != 0 ) )
		{
			export_handle->write_compressed = 1;
		}
		else
		{
			export_handle->write_compressed = 0;
		}
		if( copy_input_values != 0 )
		{
			if( libewf_handle_get_maximum_segment_size(
			     export_handle->input_handle,
			     &( export_handle->maximum_segment_size ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve maximum segment size.",
				 function );

				return( -1 );
			}
		}
		if( libewf_handle_set_maximum_segment_size(
		     export_handle->ewf_output_handle,
		     export_handle->maximum_segment_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set maximum segment size.",
			 function );

			return( -1 );
		}
		if( copy_input_values == 0 )
		{
			if( libewf_handle_set_sectors_per_chunk(
			     export_handle->ewf_output_handle,
			     export_handle->sectors_per_chunk,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set sectors per chunk in output handle.",
				 function );

				return( -1 );
			}
#if defined( HAVE_GUID_SUPPORT ) || defined( WINAPI )
			if( ( export_handle->ewf_format == LIBEWF_FORMAT_ENCASE5 )
			 || ( export_handle->ewf_format == LIBEWF_FORMAT_ENCASE6 )
			 || ( export_handle->ewf_format == LIBEWF_FORMAT_EWFX ) )
			{
				guid_type = GUID_TYPE_RANDOM;
			}
			else if( ( export_handle->ewf_format == LIBEWF_FORMAT_LINEN5 )
			      || ( export_handle->ewf_format == LIBEWF_FORMAT_LINEN6 ) )
			{
				guid_type = GUID_TYPE_TIME;
			}
			if( guid_type != 0 )
			{
				/* Sets the GUID if necessary
				 */
				if( guid_generate(
				     guid,
				     GUID_SIZE,
				     guid_type,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to generate GUID for set identifier.",
					 function );

					return( -1 );
				}
				if( libewf_handle_set_segment_file_set_identifier(
				     export_handle->ewf_output_handle,
				     guid,
				     16,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set segment file set identifier.",
					 function );

					return( -1 );
				}
			}
#endif
		}
	}
	else if( ( export_handle->output_format == EXPORT_HANDLE_OUTPUT_FORMAT_RAW )
	      && ( export_handle->use_stdout == 0 ) )
	{
		if( libsmraw_handle_set_media_size(
		     export_handle->raw_output_handle,
		     (size64_t) export_handle->export_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set media size.",
			 function );

			return( -1 );
		}
		if( libsmraw_handle_set_maximum_segment_size(
		     export_handle->raw_output_handle,
		     export_handle->maximum_segment_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set maximum segment size.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Sets the hash value in the output handle
 * Returns 1 if successful or -1 on error
 */
int export_handle_set_hash_value(
     export_handle_t *export_handle,
     char *hash_value_identifier,
     size_t hash_value_identifier_length,
     libcstring_system_character_t *hash_value,
     size_t hash_value_length,
     libcerror_error_t **error )
{
	static char *function = "export_handle_set_hash_value";
	int result            = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( export_handle->output_format == EXPORT_HANDLE_OUTPUT_FORMAT_EWF )
	{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libewf_handle_set_utf16_hash_value(
		          export_handle->ewf_output_handle,
		          (uint8_t *) hash_value_identifier,
		          hash_value_identifier_length,
		          (uint16_t *) hash_value,
		          hash_value_length,
		          error );
#else
		result = libewf_handle_set_utf8_hash_value(
		          export_handle->ewf_output_handle,
		          (uint8_t *) hash_value_identifier,
		          hash_value_identifier_length,
		          (uint8_t *) hash_value,
		          hash_value_length,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set hash value: %s.",
			 function,
			 hash_value_identifier );

			return( -1 );
		}
	}
	else if( ( export_handle->output_format == EXPORT_HANDLE_OUTPUT_FORMAT_RAW )
	      && ( export_handle->use_stdout == 0 ) )
	{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libsmraw_handle_set_utf16_integrity_hash_value(
		          export_handle->raw_output_handle,
		          (uint8_t *) hash_value_identifier,
		          hash_value_identifier_length,
		          (uint16_t *) hash_value,
		          hash_value_length,
		          error );
#else
		result = libsmraw_handle_set_utf8_integrity_hash_value(
		          export_handle->raw_output_handle,
		          (uint8_t *) hash_value_identifier,
		          hash_value_identifier_length,
		          (uint8_t *) hash_value,
		          hash_value_length,
		          error );
#endif

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set integrity hash value: %s.",
			 function,
			 hash_value_identifier );

			return( -1 );
		}
	}
	return( 1 );
}

/* Appends a read error to the output handle
 * Returns 1 if successful or -1 on error
 */
int export_handle_append_read_error(
      export_handle_t *export_handle,
      off64_t start_offset,
      size_t number_of_bytes,
      libcerror_error_t **error )
{
	static char *function      = "export_handle_append_read_error";
	uint64_t start_sector      = 0;
	uint64_t number_of_sectors = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( export_handle->bytes_per_sector == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid export handle - invalid bytes per sector value out of bounds.",
		 function );

		return( -1 );
	}
	start_sector      = start_offset / export_handle->bytes_per_sector;
	number_of_sectors = number_of_bytes / export_handle->bytes_per_sector;

	if( ( number_of_bytes % export_handle->bytes_per_sector ) != 0 )
	{
		number_of_sectors += 1;
	}
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
	if( libewf_handle_append_checksum_error(
	     export_handle->input_handle,
	     start_sector,
	     number_of_sectors,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append checksum error.",
		 function );

		return( -1 );
	}
#endif
	if( export_handle->output_format == EXPORT_HANDLE_OUTPUT_FORMAT_EWF )
	{
		if( export_handle->ewf_output_handle == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid export handle - missing ewf output handle.",
			 function );

			return( -1 );
		}
		if( libewf_handle_append_acquiry_error(
		     export_handle->ewf_output_handle,
		     start_sector,
		     number_of_sectors,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append acquiry error.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Finalizes the export handle
 * Returns the number of input bytes written or -1 on error
 */
ssize_t export_handle_finalize(
         export_handle_t *export_handle,
         libcerror_error_t **error )
{
	static char *function = "export_handle_finalize";
	ssize_t write_count   = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( export_handle->calculate_md5 != 0 )
	{
		if( export_handle_set_hash_value(
		     export_handle,
		     "MD5",
		     3,
		     export_handle->calculated_md5_hash_string,
		     32,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set hash value: MD5.",
			 function );

			return( -1 );
		}
	}
	if( export_handle->calculate_sha1 != 0 )
	{
		if( export_handle_set_hash_value(
		     export_handle,
		     "SHA1",
		     4,
		     export_handle->calculated_sha1_hash_string,
		     40,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set hash value: SHA1.",
			 function );

			return( -1 );
		}
	}
	if( export_handle->calculate_sha256 != 0 )
	{
		if( export_handle_set_hash_value(
		     export_handle,
		     "SHA256",
		     6,
		     export_handle->calculated_sha256_hash_string,
		     64,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set hash value: SHA256.",
			 function );

			return( -1 );
		}
	}
	if( export_handle->output_format == EXPORT_HANDLE_OUTPUT_FORMAT_EWF )
	{
		write_count = libewf_handle_write_finalize(
		               export_handle->ewf_output_handle,
	        	       error );

		if( write_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to finalize EWF file(s).",
			 function );

			return( -1 );
		}
	}
	return( write_count );
}

/* Exports the input
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_input(
     export_handle_t *export_handle,
     uint8_t swap_byte_pairs,
     uint8_t print_status_information,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	process_status_t *process_status                    = NULL;
	storage_media_buffer_t *storage_media_buffer        = NULL;
	uint8_t *data                                       = NULL;
	static char *function                               = "export_handle_export_input";
	size64_t export_count                               = 0;
	size64_t media_size                                 = 0;
	size_t process_buffer_size                          = 0;
	size_t data_size                                    = 0;
	size_t read_size                                    = 0;
	ssize_t read_count                                  = 0;
	ssize_t read_process_count                          = 0;
	ssize_t write_count                                 = 0;
	ssize_t write_process_count                         = 0;
	int status                                          = PROCESS_STATUS_COMPLETED;

#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
	storage_media_buffer_t *output_storage_media_buffer = NULL;
	uint8_t *input_buffer                               = NULL;
	size32_t output_chunk_size                          = 0;
	size_t write_size                                   = 0;
#endif

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( export_handle->input_chunk_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing input chunk size.",
		 function );

		return( -1 );
	}
	if( export_handle->input_chunk_size > (size32_t) INT32_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid input chunk size value exceeds maximum.",
		 function );

		return( -1 );
	}
#if !defined( HAVE_LOW_LEVEL_FUNCTIONS )
	if( export_handle->process_buffer_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid export handle - process buffer size value exceeds maximum.",
		 function );

		return( -1 );
	}
#endif
	if( libewf_handle_get_media_size(
	     export_handle->input_handle,
	     &media_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve media size.",
		 function );

		goto on_error;
	}
	if( ( export_handle->export_size > media_size )
	 || ( export_handle->export_size > (ssize64_t) INT64_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid export handle - export size value out of bounds.",
		 function );

		return( -1 );
	}
	if( export_handle->export_offset > 0 )
	{
		if( ( export_handle->export_offset >= (uint64_t) media_size )
		 || ( ( export_handle->export_size + export_handle->export_offset ) > (uint64_t) media_size ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid offset value out of bounds.",
			 function );

			goto on_error;
		}
		if( export_handle_seek_offset(
		     export_handle,
		     export_handle->export_offset,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to seek offset.",
			 function );

			goto on_error;
		}
	}
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
	if( export_handle_get_output_chunk_size(
	     export_handle,
	     &output_chunk_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the output chunk size.",
		 function );

		goto on_error;
	}
	if( output_chunk_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid output chunk size.",
		 function );

		goto on_error;
	}
#endif
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
	process_buffer_size = (size_t) export_handle->input_chunk_size;
#else
	if( export_handle->process_buffer_size == 0 )
	{
		process_buffer_size = (size_t) export_handle->input_chunk_size;
	}
	else
	{
		process_buffer_size = export_handle->process_buffer_size;
	}
#endif
	if( storage_media_buffer_initialize(
	     &storage_media_buffer,
	     process_buffer_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create storage media buffer.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
	if( storage_media_buffer_initialize(
	     &output_storage_media_buffer,
	     output_chunk_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create output storage media buffer.",
		 function );

		goto on_error;
	}
#endif
	if( export_handle_initialize_integrity_hash(
	     export_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize integrity hash(es).",
		 function );

		goto on_error;
        }
	if( process_status_initialize(
	     &process_status,
	     _LIBCSTRING_SYSTEM_STRING( "Export" ),
	     _LIBCSTRING_SYSTEM_STRING( "exported" ),
	     _LIBCSTRING_SYSTEM_STRING( "Written" ),
	     stderr,
	     print_status_information,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create process status.",
		 function );

		goto on_error;
	}
	if( process_status_start(
	     process_status,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to start process status.",
		 function );

		goto on_error;
	}
	while( export_count < (size64_t) export_handle->export_size )
	{
		read_size = process_buffer_size;

		if( ( media_size - export_count ) < read_size )
		{
			read_size = (size_t) ( media_size - export_count );
		}
		read_count = export_handle_read_buffer(
		              export_handle,
		              storage_media_buffer,
		              read_size,
		              error );

		if( read_count < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read data.",
			 function );

			goto on_error;
		}
		if( read_count == 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unexpected end of data.",
			 function );

			goto on_error;
		}
		read_process_count = export_handle_prepare_read_buffer(
		                      export_handle,
		                      storage_media_buffer,
		                      error );

		if( read_process_count < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to prepare buffer after read.",
			 function );

			goto on_error;
		}
		if( read_process_count > (ssize_t) read_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: more bytes read than requested.",
			 function );

			goto on_error;
		}
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
		/* Set the chunk data size in the compression buffer
		 */
		if( storage_media_buffer->data_in_compression_buffer == 1 )
		{
			storage_media_buffer->compression_buffer_data_size = (size_t) read_process_count;
		}
#endif
		/* Swap byte pairs
		 */
		if( swap_byte_pairs == 1 )
		{
			if( export_handle_swap_byte_pairs(
			     export_handle,
			     storage_media_buffer,
			     read_process_count,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_CONVERSION,
				 LIBCERROR_CONVERSION_ERROR_GENERIC,
				 "%s: unable to swap byte pairs.",
				 function );

				goto on_error;
			}
		}
		if( storage_media_buffer_get_data(
		     storage_media_buffer,
		     &data,
		     &data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve storage media buffer data.",
			 function );

			goto on_error;
		}
		/* Digest hashes are calcultated after swap
		 */
		if( export_handle_update_integrity_hash(
		     export_handle,
		     data,
		     read_process_count,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to update integrity hash(es).",
			 function );

			goto on_error;
		}
		export_count += read_process_count;

		while( read_process_count > 0 )
		{
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
			if( read_process_count > (ssize_t) output_chunk_size )
			{
				write_size = output_chunk_size;
			}
			else
			{
				write_size = (size_t) read_process_count;
			}
			if( ( output_storage_media_buffer->raw_buffer_data_size + write_size ) > output_chunk_size )
			{
				write_size = output_chunk_size -  output_storage_media_buffer->raw_buffer_data_size;
			}
			if( storage_media_buffer->data_in_compression_buffer == 1 )
			{
				input_buffer = storage_media_buffer->compression_buffer;
			}
			else
			{
				input_buffer = storage_media_buffer->raw_buffer;
			}
			if( memory_copy(
			     &( output_storage_media_buffer->raw_buffer[ output_storage_media_buffer->raw_buffer_data_size ] ),
			     input_buffer,
			     write_size ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy data from input buffer to output raw buffer.",
				 function );

				goto on_error;
			}
			output_storage_media_buffer->raw_buffer_data_size += write_size;

			/* Make sure the output chunk is filled upto the output chunk size
			 */
			if( ( export_count < (size64_t) export_handle->export_size )
			 && ( output_storage_media_buffer->raw_buffer_data_size < output_chunk_size ) )
			{
				continue;
			}
#endif
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
			write_process_count = export_handle_prepare_write_buffer(
			                       export_handle,
			                       output_storage_media_buffer,
			                       error );
#else
			write_process_count = export_handle_prepare_write_buffer(
			                       export_handle,
			                       storage_media_buffer,
			                       error );
#endif

			if( write_process_count < 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				"%s: unable to prepare buffer before write.",
				 function );

				goto on_error;
			}
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
			write_count = export_handle_write_buffer(
				       export_handle,
				       output_storage_media_buffer,
				       write_process_count,
				       error );
#else
			write_count = export_handle_write_buffer(
				       export_handle,
				       storage_media_buffer,
				       write_process_count,
				       error );
#endif

			if( write_count < 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write data to file.",
				 function );

				goto on_error;
			}
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
			output_storage_media_buffer->raw_buffer_data_size = 0;
#endif
			read_process_count -= write_process_count;
		}
		if( process_status_update(
		     process_status,
		     export_count,
		     export_handle->export_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to update process status.",
			 function );

			goto on_error;
		}
		if( export_handle->abort != 0 )
		{
			break;
		}
  	}
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
	if( storage_media_buffer_free(
	     &output_storage_media_buffer,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free output storage media buffer.",
		 function );

		goto on_error;
	}
#endif
	if( storage_media_buffer_free(
	     &storage_media_buffer,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free input storage media buffer.",
		 function );

		goto on_error;
	}
	if( export_handle_finalize_integrity_hash(
	     export_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to finalize integrity hash(es).",
		 function );

		goto on_error;
	}
	write_count = export_handle_finalize(
	               export_handle,
	               error );

	if( write_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to finalize.",
		 function );

		goto on_error;
	}
	if( export_handle->abort != 0 )
	{
		status = PROCESS_STATUS_ABORTED;
	}
	if( process_status_stop(
	     process_status,
	     export_count,
	     status,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to stop process status.",
		 function );

		goto on_error;
	}
	if( process_status_free(
	     &process_status,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free process status.",
		 function );

		goto on_error;
	}
	if( export_handle->abort == 0 )
	{
		if( export_handle_hash_values_fprint(
		     export_handle,
		     export_handle->notify_stream,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print export hash values.",
			 function );

			goto on_error;
		}
		if( export_handle_checksum_errors_fprint(
		     export_handle,
		     export_handle->notify_stream,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print export errors.",
			 function );

			goto on_error;
		}
		if( log_handle != NULL )
		{
			if( export_handle_hash_values_fprint(
			     export_handle,
			     log_handle->log_stream,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print export has values in log handle.",
				 function );

				goto on_error;
			}
			if( export_handle_checksum_errors_fprint(
			     export_handle,
			     log_handle->log_stream,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print export errors in log handle.",
				 function );

				goto on_error;
			}
		}
	}
	return( 1 );

on_error:
	if( process_status != NULL )
	{
		process_status_stop(
		 process_status,
		 export_count,
		 PROCESS_STATUS_FAILED,
		 NULL );
		process_status_free(
		 &process_status,
		 NULL );
	}
#if defined( HAVE_LOW_LEVEL_FUNCTIONS )
	if( output_storage_media_buffer != NULL )
	{
		storage_media_buffer_free(
		 &output_storage_media_buffer,
		 NULL );
	}
#endif
	if( storage_media_buffer != NULL )
	{
		storage_media_buffer_free(
		 &storage_media_buffer,
		 NULL );
	}
	return( -1 );
}

/* Exports the single files
 * Returns 1 if successful, 0 if not or -1 on error
 */
int export_handle_export_single_files(
     export_handle_t *export_handle,
     const libcstring_system_character_t *export_path,
     uint8_t print_status_information,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	libewf_file_entry_t *file_entry  = NULL;
	process_status_t *process_status = NULL;
	static char *function            = "export_handle_export_single_files";
	size_t export_path_size          = 0;
	int result                       = 0;
	int status                       = PROCESS_STATUS_COMPLETED;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( export_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export path.",
		 function );

		return( -1 );
	}
	export_path_size = 1 + libcstring_system_string_length(
	                        export_handle->target_path );

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libcpath_path_sanitize_wide(
	     export_handle->target_path,
	     &export_path_size,
	     error ) != 1 )
#else
	if( libcpath_path_sanitize(
	     export_handle->target_path,
	     &export_path_size,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable sanitize export path.",
		 function );

		goto on_error;
	}
	result = libewf_handle_get_root_file_entry(
	          export_handle->input_handle,
	          &file_entry,
	          error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve root file entry.",
		 function );

		goto on_error;
	}
	if( process_status_initialize(
	     &process_status,
	     _LIBCSTRING_SYSTEM_STRING( "Export" ),
	     _LIBCSTRING_SYSTEM_STRING( "exported" ),
	     _LIBCSTRING_SYSTEM_STRING( "Written" ),
	     stderr,
	     print_status_information,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create process status.",
		 function );

		goto on_error;
	}
	if( process_status_start(
	     process_status,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to start process status.",
		 function );

		goto on_error;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libcpath_path_make_directory_wide(
	     export_handle->target_path,
	     error ) != 1 )
#else
	if( libcpath_path_make_directory(
	     export_handle->target_path,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to create directory: %" PRIs_LIBCSTRING_SYSTEM "",
		 function,
		 export_handle->target_path );

		goto on_error;
	}
	log_handle_printf(
	 log_handle,
	 "Created directory: %" PRIs_LIBCSTRING_SYSTEM ".\n",
	 export_handle->target_path );

	result = export_handle_export_file_entry(
	          export_handle,
	          file_entry,
	          export_handle->target_path,
	          export_path_size,
	          export_path_size - 1,
	          log_handle,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to export root file entry.",
		 function );

		goto on_error;
	}
	if( export_handle->abort != 0 )
	{
		status = PROCESS_STATUS_ABORTED;
	}
	if( process_status_stop(
	     process_status,
	     0,
	     status,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to stop process status.",
		 function );

		goto on_error;
	}
	if( process_status_free(
	     &process_status,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free process status.",
		 function );

		goto on_error;
	}
	if( libewf_file_entry_free(
	     &file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free root file entry.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( process_status != NULL )
	{
		process_status_stop(
		 process_status,
		 0,
		 PROCESS_STATUS_FAILED,
		 NULL );
		process_status_free(
		 &process_status,
		 NULL );
	}
	if( file_entry != NULL )
	{
		libewf_file_entry_free(
		 &file_entry,
		 NULL );
	}
	return( -1 );
}

/* Exports a (single) file entry
 * Returns 1 if successful, 0 if not or -1 on error
 */
int export_handle_export_file_entry(
     export_handle_t *export_handle,
     libewf_file_entry_t *file_entry,
     const libcstring_system_character_t *export_path,
     size_t export_path_size,
     size_t file_entry_path_index,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	libcstring_system_character_t *name        = NULL;
	libcstring_system_character_t *target_path = NULL;
	static char *function                      = "export_handle_export_file_entry";
	size_t name_size                           = 0;
	size_t target_path_size                    = 0;
	uint8_t file_entry_type                    = 0;
	int result                                 = 0;
	int return_value                           = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( libewf_file_entry_get_type(
	     file_entry,
	     &file_entry_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file entry type.",
		 function );

		goto on_error;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libewf_file_entry_get_utf16_name_size(
	          file_entry,
	          &name_size,
	          error );
#else
	result = libewf_file_entry_get_utf8_name_size(
	          file_entry,
	          &name_size,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the name.",
		 function );

		goto on_error;
	}
	if( name_size > 1 )
	{
		name = libcstring_system_string_allocate(
		        name_size );

		if( name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create name.",
			 function );

			goto on_error;
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libewf_file_entry_get_utf16_name(
		          file_entry,
		          (uint16_t *) name,
		          name_size,
		          error );
#else
		result = libewf_file_entry_get_utf8_name(
		          file_entry,
		          (uint8_t *) name,
		          name_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve the name.",
			 function );

			goto on_error;
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		if( libcpath_path_sanitize_filename_wide(
		     name,
		     &name_size,
		     error ) != 1 )
#else
		if( libcpath_path_sanitize_filename(
		     name,
		     &name_size,
		     error ) != 1 )
#endif
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable sanitize name.",
			 function );

			goto on_error;
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		if( libcpath_path_join_wide(
		     &target_path,
		     &target_path_size,
		     export_path,
		     export_path_size - 1,
		     name,
		     name_size - 1,
		     error ) != 1 )
#else
		if( libcpath_path_join(
		     &target_path,
		     &target_path_size,
		     export_path,
		     export_path_size - 1,
		     name,
		     name_size - 1,
		     error ) != 1 )
#endif
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create target path.",
			 function );

			goto on_error;
		}
		memory_free(
		 name );

		name = NULL;

		if( target_path == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid target path.",
			 function );

			goto on_error;
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libcfile_file_exists_wide(
			  target_path,
			  error );
#else
		result = libcfile_file_exists(
			  target_path,
			  error );
#endif
		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_GENERIC,
			 "%s: unable to determine if %" PRIs_LIBCSTRING_SYSTEM " exists.",
			 function,
			 target_path );

			goto on_error;
		}
		else if( result != 0 )
		{
			log_handle_printf(
			 log_handle,
			 "Skipping file entry it already exists.\n" );
		}
		else if( file_entry_type == LIBEWF_FILE_ENTRY_TYPE_FILE )
		{
			if( file_entry_path_index >= target_path_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: file entry path index value out of bounds.",
				 function );

				return( -1 );
			}
			/* TODO what about NTFS streams ?
			 */
			fprintf(
			 export_handle->notify_stream,
			 "Single file: %" PRIs_LIBCSTRING_SYSTEM "\n",
			 &( target_path[ file_entry_path_index ] ) );

			return_value = export_handle_export_file_entry_data(
				        export_handle,
			                file_entry,
			                target_path,
			                error );

			if( return_value == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GENERIC,
				 "%s: unable to export file entry data.",
				 function );

				goto on_error;
			}
			if( return_value == 0 )
			{
				fprintf(
				 export_handle->notify_stream,
				 "FAILED\n" );

				if( log_handle != NULL )
				{
					log_handle_printf(
					 log_handle,
					 "FAILED\n" );
				}
			}
			fprintf(
			 export_handle->notify_stream,
			 "\n" );
		}
		else if( file_entry_type == LIBEWF_FILE_ENTRY_TYPE_DIRECTORY )
		{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
			if( libcpath_path_make_directory_wide(
			     target_path,
			     error ) != 1 )
#else
			if( libcpath_path_make_directory(
			     target_path,
			     error ) != 1 )
#endif
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to create directory: %" PRIs_LIBCSTRING_SYSTEM "",
				 function,
				 target_path );

				goto on_error;
			}
			log_handle_printf(
			 log_handle,
			 "Created directory: %" PRIs_LIBCSTRING_SYSTEM ".\n",
			 target_path );
		}
	}
	else
	{
		log_handle_printf(
		 log_handle,
		 "Skipping file entry without a name.\n" );

		target_path      = (libcstring_system_character_t *) export_path;
		target_path_size = export_path_size;
	}
	if( file_entry_type == LIBEWF_FILE_ENTRY_TYPE_DIRECTORY )
	{
		result = export_handle_export_file_entry_sub_file_entries(
		          export_handle,
		          file_entry,
		          target_path,
		          target_path_size,
		          file_entry_path_index,
		          log_handle,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to export sub file entries.",
			 function );

			goto on_error;
		}
		else if( result == 0 )
		{
			return_value = 0;
		}
	}
	if( target_path != export_path )
	{
		memory_free(
		 target_path );
	}
	return( return_value );

on_error:
	if( ( target_path != NULL )
	 && ( target_path != export_path ) )
	{
		memory_free(
		 target_path );
	}
	if( name != NULL )
	{
		memory_free(
		 name );
	}
	return( -1 );
}

/* Exports the data of a (single) file entry
 * Returns 1 if successful, 0 if not or -1 on error
 */
int export_handle_export_file_entry_data(
     export_handle_t *export_handle,
     libewf_file_entry_t *file_entry,
     const libcstring_system_character_t *export_path,
     libcerror_error_t **error )
{
	libcfile_file_t *file         = NULL;
	uint8_t *file_entry_data      = NULL;
	static char *function         = "export_handle_export_file_entry_data";
	size64_t file_entry_data_size = 0;
	size_t process_buffer_size    = EXPORT_HANDLE_BUFFER_SIZE;
	size_t read_size              = 0;
	ssize_t read_count            = 0;
	ssize_t write_count           = 0;
	int result                    = 1;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( export_handle->input_chunk_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing input chunk size.",
		 function );

		return( -1 );
	}
	if( export_handle->input_chunk_size > (size32_t) INT32_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid input chunk size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( export_handle->process_buffer_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid export handle - process buffer size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( export_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export path.",
		 function );

		return( -1 );
	}
	if( libcfile_file_initialize(
	     &file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file.",
		 function );

		goto on_error;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libcfile_file_open_wide(
	     file,
	     export_path,
	     LIBCFILE_OPEN_WRITE,
	     error ) != 1 )
#else
	if( libcfile_file_open(
	     file,
	     export_path,
	     LIBCFILE_OPEN_WRITE,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file: %" PRIs_LIBCSTRING_SYSTEM ".",
		 function,
		 export_path );

		goto on_error;
	}
	/* Export the file entry data
	 */
	if( libewf_file_entry_get_size(
	     file_entry,
	     &file_entry_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file entry data size.",
		 function );

		goto on_error;
	}
	/* If there is no file entry data an empty file is written
	 */
	if( file_entry_data_size > 0 )
	{
		if( export_handle->process_buffer_size == 0 )
		{
			process_buffer_size = export_handle->input_chunk_size;
		}
		else
		{
			process_buffer_size = export_handle->process_buffer_size;
		}
		/* This function in not necessary for normal use
		 * but it was added for testing
		 */
		if( libewf_file_entry_seek_offset(
		     file_entry,
		     0,
		     SEEK_SET,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to seek the start of the file entry data.",
			 function );

			goto on_error;
		}
		file_entry_data = (uint8_t *) memory_allocate(
		                               sizeof( uint8_t ) * process_buffer_size );

		if( file_entry_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create file entry data.",
			 function );

			goto on_error;
		}
		while( file_entry_data_size > 0 )
		{
			if( file_entry_data_size >= EXPORT_HANDLE_BUFFER_SIZE )
			{
				read_size = EXPORT_HANDLE_BUFFER_SIZE;
			}
			else
			{
				read_size = (size_t) file_entry_data_size;
			}
			read_count = libewf_file_entry_read_buffer(
			              file_entry,
			              file_entry_data,
			              read_size,
			              error );

			if( read_count == (ssize_t) -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read file entry data.",
				 function );

				goto on_error;
			}
			else if( read_count != (ssize_t) read_size )
			{
				result = 0;

				break;
			}
			file_entry_data_size -= read_size;

			write_count = libcfile_file_write_buffer(
			               file,
			               file_entry_data,
			               read_size,
			               error );

			if( write_count != (ssize_t) read_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write file entry data.",
				 function );

				goto on_error;
			}
		}
		memory_free(
		 file_entry_data );

		file_entry_data = NULL;
	}
	if( libcfile_file_close(
	     file,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close file.",
		 function );

		goto on_error;
	}
	if( libcfile_file_free(
	     &file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free file.",
		 function );

		goto on_error;
	}
	return( result );

on_error:
	if( file_entry_data != NULL )
	{
		memory_free(
		 file_entry_data );
	}
	if( file != NULL )
	{
		libcfile_file_free(
		 &file,
		 NULL );
	}
	return( -1 );
}

/* Exports a (single) file entry sub file entries
 * Returns 1 if successful, 0 if not or -1 on error
 */
int export_handle_export_file_entry_sub_file_entries(
     export_handle_t *export_handle,
     libewf_file_entry_t *file_entry,
     const libcstring_system_character_t *export_path,
     size_t export_path_size,
     size_t file_entry_path_index,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	libewf_file_entry_t *sub_file_entry = NULL;
	static char *function               = "export_handle_export_file_entry_sub_file_entries";
	int number_of_sub_file_entries      = 0;
	int result                          = 0;
	int return_value                    = 1;
	int sub_file_entry_index            = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( libewf_file_entry_get_number_of_sub_file_entries(
	     file_entry,
	     &number_of_sub_file_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sub file entries.",
		 function );

		goto on_error;
	}
	for( sub_file_entry_index = 0;
	     sub_file_entry_index < number_of_sub_file_entries;
	     sub_file_entry_index++ )
	{
		if( libewf_file_entry_get_sub_file_entry(
		     file_entry,
		     sub_file_entry_index,
		     &sub_file_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to free retrieve sub file entry: %d.",
			 function,
			 sub_file_entry_index );

			goto on_error;
		}
		result = export_handle_export_file_entry(
		          export_handle,
		          sub_file_entry,
		          export_path,
		          export_path_size,
		          file_entry_path_index,
		          log_handle,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to export sub file entry: %d.",
			 function,
			 sub_file_entry_index );

			goto on_error;
		}
		else if( result == 0 )
		{
			return_value = 0;
		}
		if( libewf_file_entry_free(
		     &sub_file_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free sub file entry: %d.",
			 function,
			 sub_file_entry_index );

			goto on_error;
		}
	}
	return( return_value );

on_error:
	if( sub_file_entry != NULL )
	{
		libewf_file_entry_free(
		 &sub_file_entry,
		 NULL );
	}
	return( -1 );
}

/* Print the hash values to a stream
 * Returns 1 if successful or -1 on error
 */
int export_handle_hash_values_fprint(
     export_handle_t *export_handle,
     FILE *stream,
     libcerror_error_t **error )
{
	static char *function = "export_handle_hash_values_fprint";

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stream.",
		 function );

		return( -1 );
	}
	if( export_handle->calculate_md5 != 0 )
	{
		if( export_handle->calculated_md5_hash_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid export handle - missing calculated MD5 hash string.",
			 function );

			return( -1 );
		}
		fprintf(
		 stream,
		 "MD5 hash calculated over data:\t\t%" PRIs_LIBCSTRING_SYSTEM "\n",
		 export_handle->calculated_md5_hash_string );
	}
	if( export_handle->calculate_sha1 != 0 )
	{
		if( export_handle->calculated_sha1_hash_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid export handle - missing calculated SHA1 hash string.",
			 function );

			return( -1 );
		}
		fprintf(
		 stream,
		 "SHA1 hash calculated over data:\t\t%" PRIs_LIBCSTRING_SYSTEM "\n",
		 export_handle->calculated_sha1_hash_string );
	}
	if( export_handle->calculate_sha256 != 0 )
	{
		if( export_handle->calculated_sha256_hash_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid export handle - missing calculated SHA256 hash string.",
			 function );

			return( -1 );
		}
		fprintf(
		 stream,
		 "SHA256 hash calculated over data:\t%" PRIs_LIBCSTRING_SYSTEM "\n",
		 export_handle->calculated_sha256_hash_string );
	}
	return( 1 );
}

/* Print the checksum errors to a stream
 * Returns 1 if successful or -1 on error
 */
int export_handle_checksum_errors_fprint(
     export_handle_t *export_handle,
     FILE *stream,
     libcerror_error_t **error )
{
	libcstring_system_character_t *filename      = NULL;
	libcstring_system_character_t *last_filename = NULL;
	static char *function                        = "export_handle_checksum_errors_fprint";
	size_t filename_size                         = 0;
	size_t last_filename_size                    = 0;
	uint64_t start_sector                        = 0;
	uint64_t last_sector                         = 0;
	uint64_t number_of_sectors                   = 0;
	uint32_t number_of_errors                    = 0;
	uint32_t error_index                         = 0;
	int result                                   = 0;
	int return_value                             = 1;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stream.",
		 function );

		return( -1 );
	}
	if( libewf_handle_get_number_of_checksum_errors(
	     export_handle->input_handle,
	     &number_of_errors,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the number of checksum errors.",
		 function );

		goto on_error;
	}
	if( number_of_errors > 0 )
	{
		fprintf(
		 stream,
		 "Read errors during export:\n" );
		fprintf(
		 stream,
		 "\ttotal number: %" PRIu32 "\n",
		 number_of_errors );
		
		for( error_index = 0;
		     error_index < number_of_errors;
		     error_index++ )
		{
			if( libewf_handle_get_checksum_error(
			     export_handle->input_handle,
			     error_index,
			     &start_sector,
			     &number_of_sectors,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve the checksum error: %" PRIu32 ".",
				 function,
				 error_index );

				start_sector      = 0;
				number_of_sectors = 0;

				return_value = -1;
			}
			last_sector = start_sector + number_of_sectors - 1;

			fprintf(
			 stream,
			 "\tat sector(s): %" PRIu64 " - %" PRIu64 " (number: %" PRIu64 ")",
			 start_sector,
			 last_sector,
			 number_of_sectors );

			fprintf(
			 stream,
			 " in segment file(s):" );

			start_sector *= export_handle->bytes_per_sector;
			last_sector  *= export_handle->bytes_per_sector;

			while( start_sector <= last_sector )
			{
				if( libewf_handle_seek_offset(
				     export_handle->input_handle,
				     (off64_t) start_sector,
				     SEEK_SET,
				     error ) == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_SEEK_FAILED,
					 "%s: unable to seek offset: %" PRIi64 ".",
					 function,
					 start_sector );

					goto on_error;
				}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
				result = libewf_handle_get_filename_size_wide(
				          export_handle->input_handle,
				          &filename_size,
				          error );
#else
				result = libewf_handle_get_filename_size(
				          export_handle->input_handle,
				          &filename_size,
				          error );
#endif

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve filename size.",
					 function );

					goto on_error;
				}
				else if( result != 0 )
				{
					filename = libcstring_system_string_allocate(
					            filename_size );


					if( filename == NULL )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_MEMORY,
						 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
						 "%s: unable to create filename.",
						 function );

						goto on_error;
					}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
					if( libewf_handle_get_filename_wide(
					     export_handle->input_handle,
					     filename,
					     filename_size,
					     error ) != 1 )
#else
					if( libewf_handle_get_filename(
					     export_handle->input_handle,
					     filename,
					     filename_size,
					     error ) != 1 )
#endif
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
						 "%s: unable to retrieve filename.",
						 function );

						goto on_error;
					}
					if( last_filename == NULL )
					{
						fprintf(
						 stream,
						 " %s",
						 filename );

						last_filename      = filename;
						last_filename_size = filename_size;
					}
					else if( ( last_filename_size != filename_size )
					      || ( memory_compare(
						    last_filename,
						    filename,
						    filename_size ) != 0 ) )
					{
						fprintf(
						 stream,
						 ", %s",
						 filename );

						memory_free(
						 last_filename );

						last_filename      = filename;
						last_filename_size = filename_size;
					}
					else
					{
						memory_free(
						 filename );
					}
				}
				start_sector += export_handle->input_chunk_size;
			}
			if( last_filename != NULL )
			{
				memory_free(
				 last_filename );

				last_filename      = NULL;
				last_filename_size = 0;
			}
			fprintf(
			 stream,
			 "\n" );
		}
		fprintf(
		 stream,
		 "\n" );
	}
	return( return_value );

on_error:
	if( last_filename != NULL )
	{
		memory_free(
		 last_filename );
	}
	if( filename != NULL )
	{
		memory_free(
		 filename );
	}
	return( -1 );
}

