/*
 * Low level writing functions
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#if defined( TIME_WITH_SYS_TIME )
#include <sys/time.h>
#include <time.h>
#elif defined( HAVE_SYS_TIME_H )
#include <sys/time.h>
#else
#include <time.h>
#endif

#include "libewf_chunk_data.h"
#include "libewf_chunk_table.h"
#include "libewf_compression.h"
#include "libewf_definitions.h"
#include "libewf_empty_block.h"
#include "libewf_filename.h"
#include "libewf_header_values.h"
#include "libewf_libbfio.h"
#include "libewf_libcdata.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"
#include "libewf_libfvalue.h"
#include "libewf_libfcache.h"
#include "libewf_libmfdata.h"
#include "libewf_media_values.h"
#include "libewf_read_io_handle.h"
#include "libewf_section.h"
#include "libewf_segment_file.h"
#include "libewf_segment_table.h"
#include "libewf_unused.h"
#include "libewf_write_io_handle.h"

#include "ewf_checksum.h"
#include "ewf_data.h"
#include "ewf_definitions.h"
#include "ewf_section.h"
#include "ewf_table.h"
#include "ewfx_delta_chunk.h"

/* Initialize the write IO handle
 * Returns 1 if successful or -1 on error
 */
int libewf_write_io_handle_initialize(
     libewf_write_io_handle_t **write_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libewf_write_io_handle_initialize";

	if( write_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid write IO handle.",
		 function );

		return( -1 );
	}
	if( *write_io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid write IO handle value already set.",
		 function );

		return( -1 );
	}
	*write_io_handle = memory_allocate_structure(
	                    libewf_write_io_handle_t );

	if( write_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create write IO handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *write_io_handle,
	     0,
	     sizeof( libewf_write_io_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear write IO handle.",
		 function );

		goto on_error;
	}
	( *write_io_handle )->maximum_segment_file_size   = INT32_MAX;
	( *write_io_handle )->remaining_segment_file_size = LIBEWF_DEFAULT_SEGMENT_FILE_SIZE;
	( *write_io_handle )->maximum_chunks_per_section  = EWF_MAXIMUM_OFFSETS_IN_TABLE;

	/* 'Z' - 'E' + 1 = 22
	 * ( ( 22 * 26 * 26 ) + 99 ) = 14971
	 */
	( *write_io_handle )->maximum_number_of_segments = (uint16_t) 14971;

	return( 1 );

on_error:
	if( *write_io_handle != NULL )
	{
		memory_free(
		 *write_io_handle );

		*write_io_handle = NULL;
	}
	return( -1 );
}

/* Frees the write IO handle including elements
 * Returns 1 if successful or -1 on error
 */
int libewf_write_io_handle_free(
     libewf_write_io_handle_t **write_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libewf_write_io_handle_free";

	if( write_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid write IO handle.",
		 function );

		return( -1 );
	}
	if( *write_io_handle != NULL )
	{
		if( ( *write_io_handle )->data_section != NULL )
		{
			memory_free(
			 ( *write_io_handle )->data_section );
		}
		if( ( *write_io_handle )->table_offsets != NULL )
		{
			memory_free(
			 ( *write_io_handle )->table_offsets );
		}
		if( ( *write_io_handle )->compressed_zero_byte_empty_block != NULL )
		{
			memory_free(
			 ( *write_io_handle )->compressed_zero_byte_empty_block );
		}
		memory_free(
		 *write_io_handle );

		*write_io_handle = NULL;
	}
	return( 1 );
}

/* Clones the write IO handle
 * Returns 1 if successful or -1 on error
 */
int libewf_write_io_handle_clone(
     libewf_write_io_handle_t **destination_write_io_handle,
     libewf_write_io_handle_t *source_write_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libewf_write_io_handle_clone";
	size_t offsets_size   = 0;

	if( destination_write_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination write IO handle.",
		 function );

		return( -1 );
	}
	if( *destination_write_io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination write IO handle value already set.",
		 function );

		return( -1 );
	}
	if( source_write_io_handle == NULL )
	{
		*destination_write_io_handle = NULL;

		return( 1 );
	}
	*destination_write_io_handle = memory_allocate_structure(
	                                libewf_write_io_handle_t );

	if( *destination_write_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination write IO handle.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_write_io_handle,
	     source_write_io_handle,
	     sizeof( libewf_write_io_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy source to destination write IO handle.",
		 function );

		goto on_error;
	}
	( *destination_write_io_handle )->data_section            = NULL;
	( *destination_write_io_handle )->table_offsets           = NULL;
	( *destination_write_io_handle )->number_of_table_offsets = 0;

	if( source_write_io_handle->data_section != NULL )
	{
		( *destination_write_io_handle )->data_section = memory_allocate_structure(
		                                                  ewf_data_t );

		if( ( *destination_write_io_handle )->data_section == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create destination data section.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     ( *destination_write_io_handle )->data_section,
		     source_write_io_handle->data_section,
		     sizeof( ewf_data_t ) ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy source to destination data section.",
			 function );

			goto on_error;
		}
	}
	if( source_write_io_handle->table_offsets != NULL )
	{
		offsets_size = sizeof( ewf_table_offset_t ) * source_write_io_handle->number_of_table_offsets;

		( *destination_write_io_handle )->table_offsets = (ewf_table_offset_t *) memory_allocate(
		                                                                          offsets_size );

		if( ( *destination_write_io_handle )->table_offsets == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create destination table offsets.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     ( *destination_write_io_handle )->table_offsets,
		     source_write_io_handle->table_offsets,
		     offsets_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy source to destination table offsets.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( *destination_write_io_handle != NULL )
	{
		if( ( *destination_write_io_handle )->table_offsets != NULL )
		{
			memory_free(
			 ( *destination_write_io_handle )->table_offsets );
		}
		if( ( *destination_write_io_handle )->data_section != NULL )
		{
			memory_free(
			 ( *destination_write_io_handle )->data_section );
		}
		memory_free(
		 *destination_write_io_handle );

		*destination_write_io_handle = NULL;
	}
	return( -1 );
}

/* Initializes the write IO handle value to start writing
 * Returns 1 if successful or -1 on error
 */
int libewf_write_io_handle_initialize_values(
     libewf_write_io_handle_t *write_io_handle,
     libewf_io_handle_t *io_handle,
     libewf_media_values_t *media_values,
     libewf_segment_table_t *segment_table,
     libcerror_error_t **error )
{
	uint8_t *compressed_zero_byte_empty_block = NULL;
	uint8_t *zero_byte_empty_block            = NULL;
	static char *function                     = "libewf_write_io_handle_initialize_values";
	void *reallocation                        = NULL;
	int64_t required_number_of_segments       = 0;
	int result                                = 0;

	if( write_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid write IO handle.",
		 function );

		return( -1 );
	}
	if( write_io_handle->values_initialized != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: write values were initialized and cannot be initialized anymore.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.",
		 function );

		return( -1 );
	}
	if( segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	/* Determine the EWF file format
	 */
	if( io_handle->format == LIBEWF_FORMAT_LVF )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: writing format LVF currently not supported.",
		 function );

		return( -1 );
	}
	if( segment_table->maximum_segment_size == 0 )
	{
		segment_table->maximum_segment_size = write_io_handle->maximum_segment_file_size;
	}
	/* If no input write size was provided check if EWF file format allows for streaming
	 */
	if( media_values->media_size == 0 )
	{
		if( ( io_handle->format != LIBEWF_FORMAT_ENCASE2 )
		 && ( io_handle->format != LIBEWF_FORMAT_ENCASE3 )
		 && ( io_handle->format != LIBEWF_FORMAT_ENCASE4 )
		 && ( io_handle->format != LIBEWF_FORMAT_ENCASE5 )
		 && ( io_handle->format != LIBEWF_FORMAT_ENCASE6 )
		 && ( io_handle->format != LIBEWF_FORMAT_LINEN5 )
		 && ( io_handle->format != LIBEWF_FORMAT_LINEN6 )
		 && ( io_handle->format != LIBEWF_FORMAT_FTK )
		 && ( io_handle->format != LIBEWF_FORMAT_EWFX ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: EWF file format does not allow for streaming write.",
			 function );

			goto on_error;
		}
	}
	else
	{
		/* Determine the required number of segments allowed to write
		 */
		required_number_of_segments = (int64_t) media_values->media_size / (int64_t) segment_table->maximum_segment_size;

		if( required_number_of_segments > (int64_t) write_io_handle->maximum_number_of_segments )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: the maximum number of allowed segment files will be exceeded with the segment file size: %" PRIu64 ".",
			 function,
			 segment_table->maximum_segment_size );

			goto on_error;
		}
	}
	if( media_values->media_size > LIBEWF_2_TIB )
	{
		if( ( io_handle->format != LIBEWF_FORMAT_ENCASE6 )
		 && ( io_handle->format != LIBEWF_FORMAT_EWFX ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: EWF file format does not allow for a media size greater than 2 TiB.",
			 function );

			goto on_error;
		}
	}
	if( write_io_handle->compressed_zero_byte_empty_block == NULL )
	{
		zero_byte_empty_block = (uint8_t *) memory_allocate(
		                                     sizeof( uint8_t ) * (size_t) media_values->chunk_size );

		if( zero_byte_empty_block == NULL )
		{	
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create zero byte empty block.",
			 function );

			goto on_error;
		}
		if( memory_set(
		     zero_byte_empty_block,
		     0,
		     sizeof( uint8_t ) * (size_t) media_values->chunk_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear zero byte empty block.",
			 function );

			goto on_error;
		}
		write_io_handle->compressed_zero_byte_empty_block_size = 512;

		compressed_zero_byte_empty_block = (uint8_t *) memory_allocate(
				                                sizeof( uint8_t ) * write_io_handle->compressed_zero_byte_empty_block_size );

		if( compressed_zero_byte_empty_block == NULL )
		{	
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create compressed zero byte empty block.",
			 function );

			goto on_error;
		}
		result = libewf_compress(
			  compressed_zero_byte_empty_block,
			  &( write_io_handle->compressed_zero_byte_empty_block_size ),
			  zero_byte_empty_block,
			  (size_t) media_values->chunk_size,
			  io_handle->compression_level,
			  error );

		/* Check if the compressed buffer was too small
		 * and a new compressed data size buffer was passed back
		 */
		if( ( result == -1 )
		 && ( write_io_handle->compressed_zero_byte_empty_block_size > 0 ) )
		{
#if !defined( HAVE_COMPRESS_BOUND ) && !defined( WINAPI )
			/* The some version of zlib require a fairly large buffer
			 * if compressBound() was not used but 2 x 512 then assume
			 * the chunk size + 16 is sufficient
			 */
			write_io_handle->compressed_zero_byte_empty_block_size = media_values->chunk_size + 16;
#endif

			libcerror_error_free(
			 error );

			reallocation = memory_reallocate(
			                compressed_zero_byte_empty_block,
			                sizeof( uint8_t ) * write_io_handle->compressed_zero_byte_empty_block_size );

			if( reallocation == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to resize compressed zero byte empty block.",
				 function );

				return( -1 );
			}
			compressed_zero_byte_empty_block = (uint8_t *) reallocation;

			result = libewf_compress(
			          compressed_zero_byte_empty_block,
			          &( write_io_handle->compressed_zero_byte_empty_block_size ),
			          zero_byte_empty_block,
			          (size_t) media_values->chunk_size,
			          io_handle->compression_level,
			          error );
		}
		if( result != 1 )
		{
			libcerror_error_free(
			 error );

			memory_free(
			 compressed_zero_byte_empty_block );

			write_io_handle->compressed_zero_byte_empty_block_size = 0;
		}
		else
		{
			write_io_handle->compressed_zero_byte_empty_block = compressed_zero_byte_empty_block;
		}
		compressed_zero_byte_empty_block = NULL;

		memory_free(
		 zero_byte_empty_block );

		zero_byte_empty_block = NULL;
	}
	/* Flag that the write values were initialized
	 */
	write_io_handle->values_initialized = 1;

	return( 1 );

on_error:
	if( compressed_zero_byte_empty_block != NULL )
	{
		memory_free(
		 compressed_zero_byte_empty_block );
	}
	if( zero_byte_empty_block != NULL )
	{
		memory_free(
		 zero_byte_empty_block );
	}
	return( -1 );
}

/* Initializes the write IO handle to resume writing
 * Returns 1 if successful or -1 on error
 */
int libewf_write_io_handle_initialize_resume(
     libewf_write_io_handle_t *write_io_handle,
     libewf_io_handle_t *io_handle,
     libbfio_pool_t *file_io_pool,
     libewf_media_values_t *media_values,
     libmfdata_file_list_t *segment_files_list,
     libfcache_cache_t *segment_files_cache,
     libmfdata_list_t *chunk_table_list,
     libewf_chunk_table_t *chunk_table,
     libewf_segment_table_t *segment_table,
     libcerror_error_t **error )
{
	libcdata_list_element_t *previous_section_list_element = NULL;
	libcdata_list_element_t *section_list_element          = NULL;
	libewf_section_t *previous_section                     = NULL;
	libewf_section_t *section                              = NULL;
	libewf_segment_file_t *segment_file                    = NULL;
	static char *function                                  = "libewf_write_io_handle_initialize_resume";
	uint8_t backtrace_to_last_chunks_sections              = 0;
	uint8_t reopen_segment_file                            = 0;
	int file_io_pool_entry                                 = 0;
	int number_of_chunks                                   = 0;
	int number_of_segment_files                            = 0;
	int number_of_unusable_chunks                          = 0;
	int segment_files_list_index                           = 0;
	int supported_section                                  = 0;

	if( write_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid write IO handle.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.",
		 function );

		return( -1 );
	}
	if( libmfdata_file_list_get_number_of_files(
	     segment_files_list,
	     &number_of_segment_files,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of segment files.",
		 function );

		return( -1 );
	}
	if( ( number_of_segment_files <= 0 )
	 || ( number_of_segment_files > (int) UINT16_MAX ) )
	{
		libcerror_error_set(
		 error,
	 	 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of segment files value out of bounds.",
		 function );

		return( -1 );
	}
	segment_files_list_index = number_of_segment_files - 1;

	if( libmfdata_file_list_get_file_value_by_index(
	     segment_files_list,
	     file_io_pool,
	     segment_files_cache,
	     segment_files_list_index,
	     (intptr_t **) &segment_file,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment file: %d from list.",
		 function,
		 segment_files_list_index + 1 );

		return( -1 );
	}
	if( segment_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing segment file.",
		 function );

		return( -1 );
	}
	if( segment_file->section_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid segment file - missing section list.",
		 function );

		return( -1 );
	}
	if( libcdata_list_get_last_element(
	     segment_file->section_list,
	     &section_list_element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve last element from section list.",
		 function );

		return( -1 );
	}
	if( libcdata_list_element_get_value(
	     section_list_element,
	     (intptr_t **) &section,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from section list element.",
		 function );

		return( -1 );
	}
	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing last section.",
		 function );

		return( -1 );
	}
	if( section->type_length == 4 )
	{
		if( memory_compare(
		     (void *) section->type,
		     (void *) "data",
		     4 ) == 0 )
		{
			if( segment_files_list_index == 0 )
			{
				backtrace_to_last_chunks_sections = 1;
			}
		}
		else if( memory_compare(
			  (void *) section->type,
			  (void *) "hash",
			  4 ) == 0 )
		{
			backtrace_to_last_chunks_sections = 1;
		}
	}
	else if( section->type_length == 5 )
	{
		if( memory_compare(
		     (void *) section->type,
		     (void *) "xhash",
		     6 ) == 0 )
		{
			backtrace_to_last_chunks_sections = 1;
		}
	}
	else if( section->type_length == 5 )
	{
		if( memory_compare(
		     (void *) section->type,
		     (void *) "digest",
		     7 ) == 0 )
		{
			backtrace_to_last_chunks_sections = 1;
		}
		else if( memory_compare(
			  (void *) section->type,
			  (void *) "error2",
			  7 ) == 0 )
		{
			backtrace_to_last_chunks_sections = 1;
		}
	}
	else if( section->type_length == 7 )
	{
		if( memory_compare(
		     (void *) section->type,
		     (void *) "session",
		     8 ) == 0 )
		{
			backtrace_to_last_chunks_sections = 1;
		}
	}
	if( backtrace_to_last_chunks_sections != 0 )
	{
		if( libcdata_list_element_get_previous_element(
		     section_list_element,
		     &section_list_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve previous element from section list element.",
			 function );

			return( -1 );
		}
		while( section_list_element != NULL )
		{
			if( libcdata_list_element_get_value(
			     section_list_element,
			     (intptr_t **) &section,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve value from section list element.",
				 function );

				return( -1 );
			}
			if( section == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing section.",
				 function );

				return( -1 );
			}
			if( memory_compare(
			     (void *) section->type,
			     (void *) "table",
			     5 ) == 0 )
			{
				break;
			}
			if( libcdata_list_element_get_previous_element(
			     section_list_element,
			     &section_list_element,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve previous element from section list element.",
				 function );

				return( -1 );
			}
		}
		if( section_list_element == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing last chunks section.",
			 function );

			return( -1 );
		}
	}
	if( section->type_length == 4 )
	{
		if( memory_compare(
		     (void *) section->type,
		     (void *) "data",
		     4 ) == 0 )
		{
			/* The sections containing the chunks and offsets were read entirely
			 * in the previous segment file
			 */
			supported_section                           = 1;
			reopen_segment_file                         = 1;
			write_io_handle->resume_segment_file_offset = section->end_offset;
			write_io_handle->create_chunks_section      = 1;
		}
		else if( memory_compare(
			  (void *) section->type,
			  (void *) "done",
			  4 ) == 0 )
		{
			/* The segment file was read entirely
			 */
			supported_section = 1;
		}
		else if( memory_compare(
			  (void *) section->type,
			  (void *) "next",
			  4 ) == 0 )
		{
			/* The segment file was read entirely
			 */
			supported_section = 1;
		}
	}
	else if( section->type_length == 5 )
	{
		if( memory_compare(
		     (void *) section->type,
		     (void *) "table",
		     6 ) == 0 )
		{
			/* Determine if the table section also contains chunks
			 */
			if( libcdata_list_element_get_previous_element(
			     section_list_element,
			     &previous_section_list_element,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve previous element from section list element.",
				 function );

				return( -1 );
			}
			if( libcdata_list_element_get_value(
			     previous_section_list_element,
			     (intptr_t **) &previous_section,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve value from previous section list element.",
				 function );

				return( -1 );
			}
			if( previous_section == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing previous section.",
				 function );

				return( -1 );
			}
			if( memory_compare(
			     (void *) previous_section->type,
			     (void *) "sectors",
			     8 ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported previous section: %s.",
				 function,
				 previous_section->type );

				return( -1 );
			}
			if( chunk_table->previous_last_chunk_filled > chunk_table->last_chunk_filled )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: previous last chunk offset filled cannot be greater than current.",
				 function );

				return( -1 );
			}
			number_of_unusable_chunks = chunk_table->last_chunk_filled - chunk_table->previous_last_chunk_filled;

			if( libmfdata_list_get_number_of_elements(
			     chunk_table_list,
			     &number_of_chunks,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve the number of chunks in the chunk table list.",
				 function );

				return( -1 );
			}
			if( number_of_unusable_chunks > number_of_chunks )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: number of unusable chunk cannot be greater than number of chunk.",
				 function );

				return( -1 );
			}
			/* The sections containing the chunks and offsets were read partially
			 */
			section_list_element = previous_section_list_element;
			section              = previous_section;

			if( libmfdata_list_resize(
			     chunk_table_list,
			     number_of_chunks - number_of_unusable_chunks,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
				 "%s: unable to resize chunk table list.",
				 function );

				return( -1 );
			}
			chunk_table->last_chunk_filled   = chunk_table->previous_last_chunk_filled;
			chunk_table->last_chunk_compared = chunk_table->previous_last_chunk_filled;

			supported_section                           = 1;
			reopen_segment_file                         = 1;
			write_io_handle->resume_segment_file_offset = section->start_offset;
			write_io_handle->create_chunks_section      = 1;
		}
	}
	else if( section->type_length == 6 )
	{
		if( memory_compare(
		     (void *) section->type,
		     (void *) "table2",
		     6 ) == 0 )
		{
			/* Determine if the table section also contains chunks
			 */
			if( libcdata_list_element_get_previous_element(
			     section_list_element,
			     &previous_section_list_element,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve previous element from section list element.",
				 function );

				return( -1 );
			}
			if( libcdata_list_element_get_value(
			     previous_section_list_element,
			     (intptr_t **) &previous_section,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve value from previous section list element.",
				 function );

				return( -1 );
			}
			if( previous_section == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing previous section.",
				 function );

				return( -1 );
			}
			if( memory_compare(
			     (void *) previous_section->type,
			     (void *) "table",
			     6 ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported previous section: %s.",
				 function,
				 previous_section->type );

				return( -1 );
			}
			if( libcdata_list_element_get_previous_element(
			     previous_section_list_element,
			     &previous_section_list_element,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve previous element from previous section list element.",
				 function );

				return( -1 );
			}
			if( libcdata_list_element_get_value(
			     previous_section_list_element,
			     (intptr_t **) &previous_section,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve value from second previous section list element.",
				 function );

				return( -1 );
			}
			if( previous_section == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing second previous section.",
				 function );

				return( -1 );
			}
			if( memory_compare(
			     (void *) previous_section->type,
			     (void *) "sectors",
			     8 ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported second previous section: %s.",
				 function,
				 previous_section->type );

				return( -1 );
			}
			if( chunk_table->previous_last_chunk_filled > chunk_table->last_chunk_filled )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: previous last chunk offset filled cannot be greater than current.",
				 function );

				return( -1 );
			}
			number_of_unusable_chunks = chunk_table->last_chunk_filled - chunk_table->previous_last_chunk_filled;

			if( libmfdata_list_get_number_of_elements(
			     chunk_table_list,
			     &number_of_chunks,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve the number of chunks in the chunk table list.",
				 function );

				return( -1 );
			}
			if( number_of_unusable_chunks > number_of_chunks )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: number of unusable chunks cannot be greater than number of chunks.",
				 function );

				return( -1 );
			}
			/* The sections containing the chunks and offsets were read partially
			 */
			section_list_element = previous_section_list_element;
			section              = previous_section;

			if( libmfdata_list_resize(
			     chunk_table_list,
			     number_of_chunks - number_of_unusable_chunks,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
				 "%s: unable to resize chunk table list.",
				 function );

				return( -1 );
			}
			chunk_table->last_chunk_filled   = chunk_table->previous_last_chunk_filled;
			chunk_table->last_chunk_compared = chunk_table->previous_last_chunk_filled;

			supported_section                           = 1;
			reopen_segment_file                         = 1;
			write_io_handle->resume_segment_file_offset = section->start_offset;
			write_io_handle->create_chunks_section      = 1;
		}
	}
	else if( section->type_length == 7 )
	{
		if( memory_compare(
		     (void *) section->type,
		     (void *) "sectors",
		     7 ) == 0 )
		{
			/* Uncertain if the sections containing the chunks was read entirely
			 * the offsets to the chunks are missing so the chunks need to be rewritten anyway
			 */
			supported_section                           = 1;
			reopen_segment_file                         = 1;
			write_io_handle->resume_segment_file_offset = section->start_offset;
			write_io_handle->create_chunks_section      = 1;
		}
	}
	if( supported_section == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: write resume from section: %s not supported.",
		 function,
		 (char *) section->type );

		return( -1 );
	}
	/* Set offset into media data
	 */
	io_handle->current_offset = (off64_t) chunk_table->last_chunk_compared * media_values->chunk_size;

	/* Set write IO handle values
	 */
	write_io_handle->input_write_count        = (ssize64_t) io_handle->current_offset;
	write_io_handle->number_of_chunks_written = chunk_table->last_chunk_compared;
	write_io_handle->write_finalized          = 0;

	if( reopen_segment_file != 0 )
	{
		if( write_io_handle->resume_segment_file_offset > (off64_t) segment_table->maximum_segment_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: resume segment file offset cannot be greater than segment file size.",
			 function );

			return( -1 );
		}
		write_io_handle->remaining_segment_file_size = segment_table->maximum_segment_size
		                                             - write_io_handle->resume_segment_file_offset;

		write_io_handle->number_of_chunks_written_to_segment = segment_file->number_of_chunks
		                                                     - number_of_unusable_chunks;

		if( libmfdata_file_list_get_file_by_index(
		     segment_files_list,
		     segment_files_list_index,
		     &file_io_pool_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve segment file: %d from list.",
			 function,
			 segment_files_list_index + 1 );

			return( -1 );
		}
		if( libbfio_pool_reopen(
		     file_io_pool,
		     file_io_pool_entry,
		     LIBBFIO_OPEN_READ_WRITE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to reopen segment file: %d.",
			 function,
			 segment_files_list_index + 1 );

			return( -1 );
		}
		segment_file->flags |= LIBEWF_SEGMENT_FILE_FLAG_WRITE_OPEN;
	}
	return( 1 );
}

/* Calculates an estimate of the number of chunks that fit within a segment file
 * Returns 1 if successful or -1 on error
 */
int libewf_write_io_handle_calculate_chunks_per_segment_file(
     uint32_t *chunks_per_segment_file,
     size64_t remaining_segment_file_size,
     uint32_t maximum_chunks_per_section,
     uint32_t number_of_chunks_written_to_segment,
     uint32_t number_of_chunks_written,
     libewf_media_values_t *media_values,
     uint8_t format,
     uint8_t ewf_format,
     uint8_t unrestrict_offset_table,
     libcerror_error_t **error )
{
	static char *function                      = "libewf_write_io_handle_calculate_chunks_per_segment_file";
	int64_t calculated_chunks_per_segment_file = 0;
	int64_t maximum_chunks_per_segment_file    = 0;
	int64_t remaining_number_of_chunks         = 0;
	int64_t required_chunk_sections            = 0;

	if( chunks_per_segment_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunks per segment file.",
		 function );

		return( -1 );
	}
	if( maximum_chunks_per_section == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid maximum chunks per section.",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.",
		 function );

		return( -1 );
	}
	/* Calculate the maximum number of chunks within this segment file
	 */
	maximum_chunks_per_segment_file = remaining_segment_file_size;

	if( ewf_format == EWF_FORMAT_S01 )
	{
		/* The EWF-S01 format uses compression this will add 16 bytes on average
		 */
		maximum_chunks_per_segment_file /= media_values->chunk_size + 16;
	}
	else
	{
		maximum_chunks_per_segment_file /= media_values->chunk_size + sizeof( uint32_t );
	}
	/* Determine the number of required chunk sections
	 */
	if( unrestrict_offset_table == 0 )
	{
		required_chunk_sections = maximum_chunks_per_segment_file
		                        % maximum_chunks_per_section;
	}
	else
	{
		required_chunk_sections = 1;
	}
	calculated_chunks_per_segment_file = remaining_segment_file_size;

	if( ewf_format == EWF_FORMAT_S01 )
	{
		/* Leave space for the chunk section starts
		 */
		calculated_chunks_per_segment_file -= required_chunk_sections
		                                    * sizeof( ewf_section_start_t );

		/* Leave space for the table offsets
		 */
		calculated_chunks_per_segment_file -= maximum_chunks_per_segment_file
		                                    * sizeof( ewf_table_offset_t );
	}
	else if( format == LIBEWF_FORMAT_ENCASE1 )
	{
		/* Leave space for the chunk section starts and the offset table checksum
		 */
		calculated_chunks_per_segment_file -= required_chunk_sections
		                                    * ( sizeof( ewf_section_start_t ) + sizeof( uint32_t ) );

		/* Leave space for the table offsets
		 */
		calculated_chunks_per_segment_file -= maximum_chunks_per_segment_file
		                                    * sizeof( ewf_table_offset_t );
	}
	else
	{
		/* Leave space for the chunk, table and table2 section starts and the table and table2 offset table checksums
		 */
		calculated_chunks_per_segment_file -= required_chunk_sections
		                                    * ( ( 3 * sizeof( ewf_section_start_t ) ) + ( 2 * sizeof( uint32_t ) ) );

		/* Leave space for the table and table2 offsets
		 */
		calculated_chunks_per_segment_file -= 2 * maximum_chunks_per_segment_file
		                                    * sizeof( ewf_table_offset_t );
	}
	/* Calculate the number of chunks within this segment file
	 */
	if( ewf_format == EWF_FORMAT_S01 )
	{
		/* The EWF-S01 format uses compression this will add 16 bytes on average
		 */
		calculated_chunks_per_segment_file /= media_values->chunk_size + 16;
	}
	else
	{
		calculated_chunks_per_segment_file /= media_values->chunk_size + sizeof( uint32_t );
	}
	/* If the input size is known determine the remaining number of chunks
	 */
	if( media_values->media_size > 0 )
	{
		remaining_number_of_chunks = (int64_t) media_values->number_of_chunks
		                           - (int64_t) number_of_chunks_written;

		/* Check if less chunks remain than the number of chunks calculated
		 */
		if( remaining_number_of_chunks < calculated_chunks_per_segment_file )
		{
			calculated_chunks_per_segment_file = remaining_number_of_chunks;
		}
	}
	/* Make sure to return the total number of chunks per segment file
	 */
	if( number_of_chunks_written_to_segment > 0 )
	{
		calculated_chunks_per_segment_file += number_of_chunks_written_to_segment;
	}
	/* Fail safe segment should contain at least 1 chunk
	 */
	if( calculated_chunks_per_segment_file <= 0 )
	{
		calculated_chunks_per_segment_file = 1;
	}
	/* Fail safe no more than 2^32 values are allowed
	 */
	else if( calculated_chunks_per_segment_file > (int64_t) UINT32_MAX )
	{
		calculated_chunks_per_segment_file = UINT32_MAX;
	}
	*chunks_per_segment_file = (uint32_t) calculated_chunks_per_segment_file;

	return( 1 );
}

/* Calculates the number of chunks that fit within a (chunks) section
 * Returns 1 if successful or -1 on error
 */
int libewf_write_io_handle_calculate_chunks_per_section(
     uint32_t *chunks_per_section,
     uint32_t maximum_chunks_per_section,
     uint32_t number_of_chunks_written_to_segment,
     uint32_t chunks_per_segment_file,
     uint8_t unrestrict_offset_table,
     libcerror_error_t **error )
{
	static char *function               = "libewf_write_io_handle_calculate_chunks_per_section";
	uint32_t remaining_number_of_chunks = 0;

	if( chunks_per_section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunks per section.",
		 function );

		return( -1 );
	}
	if( maximum_chunks_per_section == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid maximum chunks per section.",
		 function );

		return( -1 );
	}
	if( number_of_chunks_written_to_segment > chunks_per_segment_file )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: number of chunks written to segment exceeds number of chunks per segment file.",
		 function );

		return( -1 );
	}
        remaining_number_of_chunks = chunks_per_segment_file - number_of_chunks_written_to_segment;

	if( ( unrestrict_offset_table == 0 )
	 && ( remaining_number_of_chunks > (int64_t) maximum_chunks_per_section ) )
	{
		remaining_number_of_chunks = (int64_t) maximum_chunks_per_section;
	}
	/* Fail safe no more than 2^31 values are allowed
	 */
	if( remaining_number_of_chunks > (int64_t) INT32_MAX )
	{
		remaining_number_of_chunks = INT32_MAX;
	}
	*chunks_per_section = (uint32_t) remaining_number_of_chunks;

	return( 1 );
}

/* Tests if the current segment file is full
 * Returns 1 if full, 0 if not or -1 on error
 */
int libewf_write_io_handle_test_segment_file_full(
     ssize64_t remaining_segment_file_size,
     uint32_t number_of_chunks_written_to_segment,
     libewf_media_values_t *media_values,
     ssize64_t input_write_count,
     uint32_t chunks_per_segment_file,
     uint32_t number_of_chunks_written,
     uint8_t format,
     uint8_t ewf_format,
     libcerror_error_t **error )
{
	static char *function = "libewf_write_io_handle_test_segment_file_full";

	if( media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.",
		 function );

		return( -1 );
	}
	/* Check if the maximum number of chunks has been reached
	 */
	if( ( media_values->number_of_chunks != 0 )
	 && ( media_values->number_of_chunks == number_of_chunks_written ) )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: all required chunks have been written.\n",
			 function );
		}
#endif

		return( 1 );
	}
	/* Check if the end of the input has been reached
	*/
	if( ( media_values->media_size != 0 )
	 && ( input_write_count >= (ssize64_t) media_values->media_size ) )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: all required data has been written.\n",
			 function );
		}
#endif

		return( 1 );
	}
	/* The EWF-S01 and EnCase1 format should use the pre calculated size
	 */
	if( ( ewf_format == EWF_FORMAT_S01 )
	 || ( format == LIBEWF_FORMAT_ENCASE1 ) )
	{
		if( number_of_chunks_written_to_segment >= chunks_per_segment_file )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
			 	"%s: no space left for additional chunk.\n",
				 function );
			}
#endif

			return( 1 );
		}
	}
	/* Determine if a chunk would fit in the segment file
	 */
	else if( remaining_segment_file_size < (ssize64_t) ( media_values->chunk_size + sizeof( uint32_t ) ) )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: no space left for additional chunk - file size exceeded.\n",
			 function );
		}
#endif

		return( 1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: space left for additional chunk.\n",
		 function );
	}
#endif

	return( 0 );
}

/* Tests if the current chunks section is full
 * Returns 1 if full, 0 if not or -1 on error
 */
int libewf_write_io_handle_test_chunks_section_full(
     off64_t chunks_section_offset,
     ssize64_t remaining_segment_file_size,
     libewf_media_values_t *media_values,
     ssize64_t input_write_count,
     off64_t segment_file_offset,
     uint32_t maximum_chunks_per_section,
     uint32_t number_of_chunks_written_to_section,
     uint32_t number_of_chunks_written,
     uint32_t chunks_per_section,
     uint8_t format,
     uint8_t ewf_format,
     uint8_t unrestrict_offset_table,
     libcerror_error_t **error )
{
	static char *function = "libewf_write_io_handle_test_chunks_section_full";

	if( media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.",
		 function );

		return( -1 );
	}
	if( segment_file_offset > (off64_t) INT64_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid segment file offset value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( maximum_chunks_per_section == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid maximum chunks per section.",
		 function );

		return( -1 );
	}
	/* Check if a chunks section has been opened
	 */
	if( chunks_section_offset == 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: no chunks section has been created.\n",
			 function );
		}
#endif

		return( 0 );
	}
	/* Check if the maximum number of chunks has been reached
	 */
	if( ( media_values->number_of_chunks != 0 )
	 && ( media_values->number_of_chunks == number_of_chunks_written ) )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: all required chunks have been written.\n",
			 function );
		}
#endif

		return( 1 );
	}
	/* Check if the end of the input has been reached
	*/
	if( ( media_values->media_size != 0 )
	 && ( input_write_count >= (ssize64_t) media_values->media_size ) )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: all required data has been written.\n",
			 function );
		}
#endif
		return( 1 );
	}
	/* Check if the maximum number of chunks restriction should apply
	 */
	if( ( unrestrict_offset_table == 0 )
	 && ( number_of_chunks_written_to_section >= maximum_chunks_per_section ) )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: no space left for additional chunk - maximum reached.\n",
			 function );
		}
#endif
		return( 1 );
	}
	/* Fail safe no more than 2^31 chunks are allowed
	 */
	if( number_of_chunks_written_to_section > (uint32_t) INT32_MAX )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: no space left for additional chunk - preventing chunk overflow.\n",
			 function );
		}
#endif
		return( 1 );
	}
	/* Prevent offset overflow
	 */
	if( ( segment_file_offset - chunks_section_offset ) > (off64_t) INT32_MAX )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: no space left for additional chunk - preventing offset overflow.\n",
			 function );
		}
#endif
		return( 1 );
	}
	/* The EWF-S01 and EnCase1 format do not allow for a growth of the offset table
	 */
	if( ( ewf_format == EWF_FORMAT_S01 )
	 || ( format == LIBEWF_FORMAT_ENCASE1 ) )
	{
		if( number_of_chunks_written_to_section >= chunks_per_section )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: no space left for additional chunk.\n",
				 function );
			}
#endif

			return( 1 );
		}
	}
	/* Determine if a chunk would fit in the segment file
	 */
	else if( remaining_segment_file_size < (ssize64_t) ( media_values->chunk_size + sizeof( uint32_t ) ) )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
		 	"%s: no space left for additional chunk - file size exceeded.\n",
			 function );
		}
#endif

		return( 1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
	 	"%s: space left for additional chunk.\n",
		 function );
	}
#endif

	return( 0 );
}

/* Creates a new segment file and opens it for writing
 * The necessary sections at the start of the segment file are written
 * Returns 1 if successful or -1 on error
 */
int libewf_write_io_handle_create_segment_file(
     libewf_io_handle_t *io_handle,
     libbfio_pool_t *file_io_pool,
     libmfdata_file_list_t *segment_files_list,
     libfcache_cache_t *segment_files_cache,
     libewf_segment_table_t *segment_table,
     uint16_t segment_number,
     uint16_t maximum_number_of_segments,
     uint8_t segment_file_type,
     libewf_segment_file_t **segment_file,
     int *segment_files_list_index,
     int *file_io_pool_entry,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle        = NULL;
	libcstring_system_character_t *filename = NULL;
	static char *function                   = "libewf_write_io_handle_create_segment_file";
	size_t filename_size                    = 0;
	int bfio_access_flags                   = 0;

	if( segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( segment_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	if( *segment_file != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid segment file - value already set.",
		 function );

		return( -1 );
	}
	if( segment_files_list_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment files list index.",
		 function );

		return( -1 );
	}
	if( file_io_pool_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO pool entry.",
		 function );

		return( -1 );
	}
	if( libewf_filename_create(
	     &filename,
	     &filename_size,
	     segment_table->basename,
	     segment_table->basename_size - 1,
	     segment_number,
	     maximum_number_of_segments,
	     segment_file_type,
	     io_handle->format,
	     io_handle->ewf_format,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create segment file: %" PRIu16 " filename.",
		 function,
		 segment_number );

		goto on_error;
	}
	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing filename.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: creating segment file: %" PRIu16 " with filename: %" PRIs_LIBCSTRING_SYSTEM ".\n",
		 function,
		 segment_number,
		 filename );
	}
#endif
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

		goto on_error;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libbfio_file_set_name_wide(
	     file_io_handle,
	     filename,
	     filename_size,
	     error ) != 1 )
#else
	if( libbfio_file_set_name(
	     file_io_handle,
	     filename,
	     filename_size,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set filename in file IO handle.",
		 function );

		goto on_error;
	}
	memory_free(
	 filename );

	filename = NULL;

	if( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_DWF )
	{
		bfio_access_flags = LIBBFIO_OPEN_READ_WRITE_TRUNCATE;
	}
	else
	{
		bfio_access_flags = LIBBFIO_OPEN_WRITE_TRUNCATE;
	}
	if( libbfio_pool_append_handle(
	     file_io_pool,
	     file_io_pool_entry,
	     file_io_handle,
	     bfio_access_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append file IO handle to pool.",
		 function );

		goto on_error;
	}
	file_io_handle = NULL;

	if( libbfio_pool_open(
	     file_io_pool,
	     *file_io_pool_entry,
	     bfio_access_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file IO pool entry: %d.",
		 function,
		 *file_io_pool_entry );

		goto on_error;
	}
	if( libewf_segment_file_initialize(
	     segment_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create segment file.",
		 function );

		goto on_error;
	}
	( *segment_file )->type           = segment_file_type;
	( *segment_file )->segment_number = segment_number;
	( *segment_file )->flags         |= LIBEWF_SEGMENT_FILE_FLAG_WRITE_OPEN;

	if( libmfdata_file_list_append_file(
	     segment_files_list,
	     segment_files_list_index,
	     *file_io_pool_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set segment file: %" PRIu16 " in list.",
		 function,
		 segment_number );

		goto on_error;
	}
	if( libmfdata_file_list_set_file_value_by_index(
	     segment_files_list,
	     segment_files_cache,
	     *segment_files_list_index,
	     (intptr_t *) *segment_file,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libewf_segment_file_free,
	     LIBMFDATA_FILE_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set segment file: %" PRIu16 " value in list.",
		 function,
		 segment_number );

		*segment_file = NULL;

		goto on_error;
	}
	return( 1 );

on_error:
	if( *segment_file != NULL )
	{
		libewf_segment_file_free(
		 segment_file,
		 NULL );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	if( filename != NULL )
	{
		memory_free(
		 filename );
	}
	return( -1 );

}

/* Writes a new chunk of data in EWF format at the current offset
 * The necessary settings of the write values must have been made
 * Returns the number of bytes written, 0 when no longer bytes can be written or -1 on error
 */
ssize_t libewf_write_io_handle_write_new_chunk(
         libewf_write_io_handle_t *write_io_handle,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         libewf_media_values_t *media_values,
         libmfdata_file_list_t *segment_files_list,
         libfcache_cache_t *segment_files_cache,
         libewf_segment_table_t *segment_table,
         libmfdata_list_t *chunk_table_list,
         libfvalue_table_t *header_values,
         libfvalue_table_t *hash_values,
         libewf_header_sections_t *header_sections,
         libewf_hash_sections_t *hash_sections,
         libcdata_array_t *sessions,
         libcdata_array_t *tracks,
         libcdata_range_list_t *acquiry_errors,
         int chunk_index,
         uint8_t *chunk_buffer,
         size_t chunk_buffer_size,
         size_t chunk_data_size,
         int8_t is_compressed,
         uint8_t *checksum_buffer,
         uint32_t chunk_checksum,
         int8_t write_checksum,
         libcerror_error_t **error )
{
	libewf_segment_file_t *segment_file = NULL;
	void *reallocation                  = NULL;
	static char *function               = "libewf_write_io_handle_write_new_chunk";
	off64_t segment_file_offset         = 0;
	ssize_t total_write_count           = 0;
	ssize_t write_count                 = 0;
	int chunk_exists                    = 0;
	int file_io_pool_entry              = -1;
	int insufficient_output_space       = 0;
	int number_of_chunks                = 0;
	int number_of_segment_files         = 0;
	int result                          = 0;
	int segment_files_list_index        = 0;

	if( write_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid write IO handle.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.",
		 function );

		return( -1 );
	}
	if( segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	chunk_exists = libmfdata_list_is_set(
			chunk_table_list,
			chunk_index,
			error );

	if( chunk_exists == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if the chunk: %d exists in the chunk table.",
		 function,
		 chunk_index );

		return( -1 );
	}
	else if( chunk_exists != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid chunk: %d already exists.",
		 function,
		 chunk_index );

		return( -1 );
	}
	/* Check if the write was already finalized
	 */
	if( write_io_handle->write_finalized != 0 )
	{
		return( 0 );
	}
	if( libmfdata_list_get_number_of_elements(
	     chunk_table_list,
	     &number_of_chunks,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the number of chunks in the chunk table.",
		 function );

		return( -1 );
	}
	/* Allocate the necessary number of chunk offsets
	 * this reduces the number of reallocations
	 */
	if( (uint32_t) number_of_chunks < media_values->number_of_chunks )
        {
		if( libmfdata_list_resize(
		     chunk_table_list,
		     (int) media_values->number_of_chunks,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
			 "%s: unable to resize chunk table.",
			 function );

			return( -1 );
		}
	}
	if( chunk_buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk buffer.",
		 function );

		return( -1 );
	}
	if( ( chunk_buffer_size == 0 )
	 || ( chunk_buffer_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid chunk size value out of bounds.",
		 function );

		return( -1 );
	}
	/* Check if the number of bytes as specified have been written
	 */
	if( ( media_values->media_size != 0 )
	 && ( write_io_handle->input_write_count >= (ssize64_t) media_values->media_size ) )
	{
		return( 0 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
	 	"%s: writing chunk: %d with size: %" PRIzd " (data size: %" PRIzd ").\n",
		 function,
		 chunk_index,
		 chunk_buffer_size,
		 chunk_data_size );
	}
#endif
	if( libmfdata_file_list_get_number_of_files(
	     segment_files_list,
	     &number_of_segment_files,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of segment files.",
		 function );

		return( -1 );
	}
	if( ( number_of_segment_files < 0 )
	 || ( number_of_segment_files > (int) UINT16_MAX ) )
	{
		libcerror_error_set(
		 error,
	 	 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of segment files value out of bounds.",
		 function );

		return( -1 );
	}
	if( number_of_segment_files != 0 )
	{
		segment_files_list_index = number_of_segment_files - 1;

		if( libmfdata_file_list_get_file_value_by_index(
		     segment_files_list,
		     file_io_pool,
		     segment_files_cache,
		     segment_files_list_index,
		     (intptr_t **) &segment_file,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve segment file: %" PRIu16 " from list.",
			 function,
			 segment_files_list_index + 1 );

			return( -1 );
		}
		if( segment_file == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing segment file: %" PRIu16 ".",
			 function,
			 segment_files_list_index + 1 );

			return( -1 );
		}
		if( ( segment_file->flags & LIBEWF_SEGMENT_FILE_FLAG_WRITE_OPEN ) == 0 )
		{
			segment_file = NULL;

			segment_files_list_index += 1;
		}
	}
	if( segment_file == NULL )
	{
		if( header_sections == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid header sections.",
			 function );

			return( -1 );
		}
		if( ( header_sections->header == NULL )
		 && ( header_sections->header2 == NULL )
		 && ( header_sections->xheader == NULL ) )
		{
			if( libewf_header_sections_generate(
			     header_sections,
			     header_values,
			     time( NULL ),
			     io_handle->compression_level,
			     io_handle->format,
			     io_handle->header_codepage,
			     error ) == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create header(s).",
				 function );

				return( -1 );
			}
		}
		write_io_handle->create_chunks_section               = 1;
		write_io_handle->chunks_per_section                  = 0;
		write_io_handle->number_of_chunks_written_to_segment = 0;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: creating segment file with segment number: %" PRIu16 ".\n",
			 function,
			 segment_files_list_index + 1 );
		}
#endif
		if( libewf_write_io_handle_create_segment_file(
		     io_handle,
		     file_io_pool,
		     segment_files_list,
		     segment_files_cache,
		     segment_table,
		     (uint16_t) ( segment_files_list_index + 1 ),
		     write_io_handle->maximum_number_of_segments,
		     LIBEWF_SEGMENT_FILE_TYPE_EWF,
		     &segment_file,
		     &segment_files_list_index,
		     &file_io_pool_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to create segment file: %" PRIu16 ".",
			 function,
			 segment_files_list_index + 1 );

			return( -1 );
		}
		/* Reserve space for the done or next section
		 */
		write_io_handle->remaining_segment_file_size = segment_table->maximum_segment_size
		                                             - sizeof( ewf_section_start_t );

		/* Write the start of the segment file
		 * like the file header, the header, volume and/or data section, etc.
		 */
		write_count = libewf_segment_file_write_start(
		               segment_file,
		               io_handle,
		               file_io_pool,
		               file_io_pool_entry,
		               media_values,
		               header_sections,
		               &( write_io_handle->data_section ),
		               error );

		if( write_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write segment file start.",
			 function );

			return( -1 );
		}
		total_write_count                            += write_count;
		write_io_handle->remaining_segment_file_size -= write_count;

		/* Determine the number of chunks per segment file
		 */
		if( segment_file->number_of_chunks == 0 )
		{
			if( libewf_write_io_handle_calculate_chunks_per_segment_file(
			     &( write_io_handle->chunks_per_segment_file ),
			     write_io_handle->remaining_segment_file_size,
			     write_io_handle->maximum_chunks_per_section,
			     write_io_handle->number_of_chunks_written_to_segment,
			     write_io_handle->number_of_chunks_written,
			     media_values,
			     io_handle->format,
			     io_handle->ewf_format,
			     write_io_handle->unrestrict_offset_table,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine the number of chunks per segment file.",
				 function );

				return( -1 );
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: calculated number of chunks per segment file: %" PRIu32 ".\n",
				 function,
				 write_io_handle->chunks_per_segment_file );
			}
#endif
		}
		else
		{
			write_io_handle->chunks_per_segment_file = segment_file->number_of_chunks;
		}
	}
	else
	{
		if( libmfdata_file_list_get_file_by_index(
		     segment_files_list,
		     segment_files_list_index,
		     &file_io_pool_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve segment file: %d from list.",
			 function,
			 segment_files_list_index + 1 );

			return( -1 );
		}
	}
	/* Set segment file to the correct offset if write is resumed
	 */
	if( write_io_handle->resume_segment_file_offset > 0 )
	{
		if( libbfio_pool_seek_offset(
		     file_io_pool,
		     file_io_pool_entry,
		     write_io_handle->resume_segment_file_offset,
		     SEEK_SET,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to seek resume segment file offset: %" PRIi64 " in segment file: %d.",
			 function,
			 write_io_handle->resume_segment_file_offset,
			 segment_files_list_index + 1 );

			return( -1 );
		}
		write_io_handle->resume_segment_file_offset = 0;
	}
	/* Check if a chunk section should be created
	 */
	if( write_io_handle->create_chunks_section == 1 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
		 	"%s: creating chunks section.\n",
			 function );
		}
#endif
		write_io_handle->create_chunks_section               = 0;
		write_io_handle->number_of_chunks_written_to_section = 0;
		write_io_handle->chunks_section_write_count          = 0;

		if( io_handle->ewf_format == EWF_FORMAT_S01 )
		{
			/* Leave space for the chunk section start
			 */
			write_io_handle->remaining_segment_file_size -= sizeof( ewf_section_start_t );
		}
		else if( io_handle->format == LIBEWF_FORMAT_ENCASE1 )
		{
			/* Leave space for the chunk section start and the offset table checksum
			 */
			write_io_handle->remaining_segment_file_size -= sizeof( ewf_section_start_t ) + sizeof( uint32_t );
		}
		else
		{
			/* Leave space for the chunk, table and table2 section starts and the table and table2 offset table checksums
			 */
			write_io_handle->remaining_segment_file_size -= ( 3 * sizeof( ewf_section_start_t ) ) + ( 2 * sizeof( uint32_t ) );
		}
		if( libbfio_pool_get_offset(
		     file_io_pool,
		     file_io_pool_entry,
		     &( write_io_handle->chunks_section_offset ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve current offset in segment file.",
			 function );

			return( -1 );
		}
		/* Recalculate the number of chunks per segment file for a better segment file fill when compression is used
		 */
		if( segment_file->number_of_chunks == 0 )
		{
			if( libewf_write_io_handle_calculate_chunks_per_segment_file(
			     &( write_io_handle->chunks_per_segment_file ),
			     write_io_handle->remaining_segment_file_size,
			     write_io_handle->maximum_chunks_per_section,
			     write_io_handle->number_of_chunks_written_to_segment,
			     write_io_handle->number_of_chunks_written,
			     media_values,
			     io_handle->format,
			     io_handle->ewf_format,
			     write_io_handle->unrestrict_offset_table,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine the number of chunks per segment file.",
				 function );

				return( -1 );
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: calculated number of chunks per segment file: %" PRIu32 ".\n",
				 function,
				 write_io_handle->chunks_per_segment_file );
			}
#endif
		}
		else
		{
			write_io_handle->chunks_per_segment_file = segment_file->number_of_chunks;
		}
		if( libewf_write_io_handle_calculate_chunks_per_section(
		     &( write_io_handle->chunks_per_section ),
		     write_io_handle->maximum_chunks_per_section,
		     write_io_handle->number_of_chunks_written_to_segment,
		     write_io_handle->chunks_per_segment_file,
		     write_io_handle->unrestrict_offset_table,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine the number of chunks per chunks section.",
			 function );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
		 	"%s: calculated number of chunks per section: %" PRIu32 ".\n",
			 function,
			 write_io_handle->chunks_per_section );
		}
#endif
		if( write_io_handle->number_of_table_offsets < write_io_handle->chunks_per_section )
		{
			reallocation = memory_reallocate(
			                write_io_handle->table_offsets,
			                sizeof( ewf_table_offset_t ) * write_io_handle->chunks_per_section );

			if( reallocation == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create table offsets.",
				 function );

				return( -1 );
			}
			write_io_handle->table_offsets           = (ewf_table_offset_t *) reallocation;
			write_io_handle->number_of_table_offsets = write_io_handle->chunks_per_section;
		}
		/* Write the section start of the chunks section
		 */
		write_count = libewf_segment_file_write_chunks_section_start(
		               segment_file,
		               io_handle,
		               file_io_pool,
		               file_io_pool_entry,
		               write_io_handle->chunks_section_offset,
		               chunk_table_list,
		               write_io_handle->table_offsets,
		               write_io_handle->number_of_table_offsets,
		               write_io_handle->number_of_chunks_written,
		               write_io_handle->chunks_per_section,
		               error );

		if( write_count == -1 )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: unable to write section start for chunks.\n",
				 function );
			}
#endif

			return( -1 );
		}
		total_write_count                            += write_count;
		write_io_handle->remaining_segment_file_size -= write_count;
	}
	if( libbfio_pool_get_offset(
	     file_io_pool,
	     file_io_pool_entry,
	     &segment_file_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve current offset in segment file.",
		 function );

		return( -1 );
	}
	/* Write the chunk data
	 */
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
	 	"%s: writing %" PRIzd " bytes to segment file: %d.\n",
		 function,
		 chunk_buffer_size,
		 segment_files_list_index + 1 );
	}
#endif
	write_count = libewf_segment_file_write_chunk(
		       segment_file,
		       io_handle,
		       file_io_pool,
		       file_io_pool_entry,
		       chunk_table_list,
		       chunk_index,
		       chunk_buffer,
		       chunk_buffer_size,
		       chunk_data_size,
		       is_compressed,
		       checksum_buffer,
		       &chunk_checksum,
		       write_checksum,
	               error );

	if( write_count <= -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write chunk data.",
		 function );

		if( ( error != NULL )
		 && ( libcerror_error_matches(
		       *error,
		       LIBCERROR_ERROR_DOMAIN_OUTPUT,
		       LIBCERROR_OUTPUT_ERROR_INSUFFICIENT_SPACE ) == 0 ) )
		{
			return( -1 );
		}
		insufficient_output_space = 1;
	}
	else
	{
		insufficient_output_space = 0;
	}
	if( insufficient_output_space == 0 )
	{
		total_write_count                                    += write_count;
		write_io_handle->input_write_count                   += chunk_data_size;
		write_io_handle->chunks_section_write_count          += write_count;
		write_io_handle->remaining_segment_file_size         -= write_count;
		write_io_handle->number_of_chunks_written_to_segment += 1;
		write_io_handle->number_of_chunks_written_to_section += 1;
		write_io_handle->number_of_chunks_written            += 1;

		if( ( io_handle->ewf_format == EWF_FORMAT_S01 )
		 || ( io_handle->format == LIBEWF_FORMAT_ENCASE1 ) )
		{
			/* Leave space for the chunk offset in the offset table
			 */
			write_io_handle->remaining_segment_file_size -= 2 * sizeof( ewf_table_offset_t );
		}
		else
		{
			/* Leave space for the chunk offset in the table and table2 sections
			 */
			write_io_handle->remaining_segment_file_size -= 2 * sizeof( ewf_table_offset_t );
		}
		if( libbfio_pool_get_offset(
		     file_io_pool,
		     file_io_pool_entry,
		     &segment_file_offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve current offset in segment file.",
			 function );

			return( -1 );
		}
		/* Check if the current chunks section is full, if so close the current section
		 */
		result = libewf_write_io_handle_test_chunks_section_full(
			  write_io_handle->chunks_section_offset,
			  write_io_handle->remaining_segment_file_size,
			  media_values,
			  write_io_handle->input_write_count,
			  segment_file_offset,
			  write_io_handle->maximum_chunks_per_section,
			  write_io_handle->number_of_chunks_written_to_section,
			  write_io_handle->number_of_chunks_written,
			  write_io_handle->chunks_per_section,
			  io_handle->format,
			  io_handle->ewf_format,
			  write_io_handle->unrestrict_offset_table,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if chunks section is full.",
			 function );

			return( -1 );
		}
	}
	else
	{
		if( libbfio_pool_seek_offset(
		     file_io_pool,
		     file_io_pool_entry,
		     segment_file_offset,
		     SEEK_SET,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_SEEK_FAILED,
			 "%s: cannot find offset: %" PRIi64 ".",
			 function,
			 segment_file_offset );

			return( -1 );
		}
		result = 0;
	}
	if( ( insufficient_output_space == 1 )
	 || ( result == 1 ) )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: closing chunks section number of bytes written: %" PRIi64 ".\n",
			 function,
			 write_io_handle->chunks_section_write_count );
		}
#endif
		if( write_io_handle->number_of_table_offsets < write_io_handle->number_of_chunks_written_to_section )
		{
			reallocation = memory_reallocate(
			                write_io_handle->table_offsets,
			                sizeof( ewf_table_offset_t ) * write_io_handle->number_of_chunks_written_to_section );

			if( reallocation == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create table offsets.",
				 function );

				return( -1 );
			}
			write_io_handle->table_offsets           = (ewf_table_offset_t *) reallocation;
			write_io_handle->number_of_table_offsets = write_io_handle->number_of_chunks_written_to_section;
		}

		/* Correct the offset, size in the chunks section
		 */
		write_count = libewf_segment_file_write_chunks_section_correction(
		               segment_file,
		               io_handle,
		               file_io_pool,
		               file_io_pool_entry,
		               segment_file_offset,
		               chunk_table_list,
		               write_io_handle->table_offsets,
		               write_io_handle->number_of_table_offsets,
		               write_io_handle->chunks_section_offset,
		               (size64_t) write_io_handle->chunks_section_write_count,
		               write_io_handle->number_of_chunks_written,
		               write_io_handle->number_of_chunks_written_to_section,
		               error );

		if( write_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to correct chunks section.",
			 function );

			return( -1 );
		}
		segment_file_offset                   += write_count;
		total_write_count                     += write_count;
		write_io_handle->create_chunks_section = 1;
		write_io_handle->chunks_section_offset = 0;

		if( insufficient_output_space == 0 )
		{
			/* Check if the current segment file is full, if so close the current segment file
			 */
			result = libewf_write_io_handle_test_segment_file_full(
				  write_io_handle->remaining_segment_file_size,
				  write_io_handle->number_of_chunks_written_to_segment,
				  media_values,
				  write_io_handle->input_write_count,
				  write_io_handle->chunks_per_segment_file,
				  write_io_handle->number_of_chunks_written,
				  io_handle->format,
				  io_handle->ewf_format,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine if segment file is full.",
				 function );

				return( -1 );
			}
		}
		else
		{
			result = 0;
		}
		if( ( insufficient_output_space == 1 )
		 || ( result == 1 ) )
		{
			/* Check if this is not the last segment file
			 */
			if( ( media_values->media_size == 0 )
			 || ( write_io_handle->input_write_count < (ssize64_t) media_values->media_size ) )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
				 	"%s: closing segment file with segment number: %d.\n",
					 function,
					 segment_files_list_index + 1 );
				}
#endif
				/* Finish and close the segment file
				 */
				write_count = libewf_segment_file_write_close(
					       segment_file,
					       io_handle,
					       file_io_pool,
					       file_io_pool_entry,
					       segment_file_offset,
					       write_io_handle->number_of_chunks_written_to_segment,
					       0,
					       hash_sections,
					       hash_values,
					       media_values,
					       sessions,
					       tracks,
					       acquiry_errors,
					       &( write_io_handle->data_section ),
				               error );

				if( write_count == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_WRITE_FAILED,
					 "%s: unable to close segment file.",
					 function );

					return( -1 );
				}
				segment_file_offset += write_count;
				total_write_count   += write_count;
			}
		}
	}
	if( insufficient_output_space == 1 )
	{
		return( -1 );
	}
	return( total_write_count );
}

/* Writes an existing chunk of data in EWF format at the current offset
 * The necessary settings of the write values must have been made
 * Returns the number of data bytes written, 0 when no longer bytes can be written or -1 on error
 */
ssize_t libewf_write_io_handle_write_existing_chunk(
         libewf_write_io_handle_t *write_io_handle,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         libewf_media_values_t *media_values,
         libmfdata_file_list_t *delta_segment_files_list,
         libfcache_cache_t *segment_files_cache,
         libewf_segment_table_t *delta_segment_table,
         libmfdata_list_t *chunk_table_list,
         libewf_header_sections_t *header_sections,
         int chunk_index,
         uint8_t *chunk_buffer,
         size_t chunk_buffer_size,
         size_t chunk_data_size LIBEWF_ATTRIBUTE_UNUSED,
         int8_t is_compressed,
         uint8_t *checksum_buffer,
         uint32_t chunk_checksum,
         int8_t write_checksum,
         libcerror_error_t **error )
{
	libcdata_list_element_t *last_list_element = NULL;
	libewf_section_t *last_section             = NULL;
	libewf_segment_file_t *segment_file        = NULL;
	static char *function                      = "libewf_write_io_handle_write_existing_chunk";
	off64_t existing_chunk_offset              = 0;
	off64_t segment_file_offset                = 0;
	size64_t existing_chunk_size               = 0;
	size64_t required_segment_file_size        = 0;
	ssize_t total_write_count                  = 0;
	ssize_t write_count                        = 0;
	uint32_t existing_chunk_flags              = 0;
	uint8_t no_section_append                  = 0;
	int file_io_pool_entry                     = -1;
	int number_of_segment_files                = 0;
	int segment_files_list_index               = 0;

	LIBEWF_UNREFERENCED_PARAMETER( chunk_data_size )

	if( write_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid write IO handle.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.",
		 function );

		return( -1 );
	}
	if( delta_segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid delta segment table.",
		 function );

		return( -1 );
	}
	if( chunk_buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk buffer.",
		 function );

		return( -1 );
	}
	if( ( chunk_buffer_size == 0 )
	 || ( chunk_buffer_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid chunk size value out of bounds.",
		 function );

		return( -1 );
	}
	if( is_compressed != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_CONFLICTING_VALUE,
		 "%s: chunk compression cannot be used.",
		 function );

		return( -1 );
	}
	if( libmfdata_list_get_data_range_by_index(
	     chunk_table_list,
	     chunk_index,
	     &file_io_pool_entry,
	     &existing_chunk_offset,
	     &existing_chunk_size,
	     &existing_chunk_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data range of chunk: %d.",
		 function,
		 chunk_index );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: writing delta chunk: %d with size: %" PRIzd ".\n",
		 function,
		 chunk_index,
		 chunk_buffer_size );
	}
#endif
	if( ( existing_chunk_flags & LIBEWF_RANGE_FLAG_IS_DELTA ) == 0 )
	{
		if( libmfdata_file_list_get_number_of_files(
		     delta_segment_files_list,
		     &number_of_segment_files,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of delta segment files.",
			 function );

			return( -1 );
		}
		if( ( number_of_segment_files < 0 )
		 || ( number_of_segment_files > (int) UINT16_MAX ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid number of segment files value out of bounds.",
			 function );

			return( -1 );
		}
		if( number_of_segment_files != 0 )
		{
			segment_files_list_index = number_of_segment_files - 1;

			/* Check if a new delta segment file should be created
			 */
			if( libmfdata_file_list_get_file_by_index(
			     delta_segment_files_list,
			     segment_files_list_index,
			     &file_io_pool_entry,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve delta segment file: %d from list.",
				 function,
				 segment_files_list_index + 1 );

				return( -1 );
			}
			if( libmfdata_file_list_get_file_value_by_index(
			     delta_segment_files_list,
			     file_io_pool,
			     segment_files_cache,
			     segment_files_list_index,
			     (intptr_t **) &segment_file,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve delta segment file: %d value from list.",
				 function,
				 segment_files_list_index + 1 );

				return( -1 );
			}
			if( segment_file == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing delta segment file: %d.",
				 function,
				 segment_files_list_index + 1 );

				return( -1 );
			}
			if( segment_file->section_list == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid segment file - missing section list.",
				 function );

				return( -1 );
			}
			if( libcdata_list_get_last_element(
			     segment_file->section_list,
			     &last_list_element,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve last element from section list.",
				 function );

				return( -1 );
			}
			if( libcdata_list_element_get_value(
			     last_list_element,
			     (intptr_t **) &last_section,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve value from last section list element.",
				 function );

				return( -1 );
			}
			if( last_section == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing last section.",
				 function );

				return( -1 );
			}
			if( libbfio_pool_get_offset(
			     file_io_pool,
			     file_io_pool_entry,
			     &segment_file_offset,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve current offset in segment file.",
				 function );

				return( -1 );
			}
			/* Make sure the current segment file offset points to the start of the last section
			 */
			if( segment_file_offset != last_section->start_offset )
			{
				if( libbfio_pool_seek_offset(
				     file_io_pool,
				     file_io_pool_entry,
				     last_section->start_offset,
				     SEEK_SET,
				     error ) == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_SEEK_FAILED,
					 "%s: unable to seek offset: %" PRIi64 " in delta segment file: %d.",
					 function,
					 last_section->start_offset,
					 segment_files_list_index + 1 );

					return( -1 );
				}
				segment_file_offset = last_section->start_offset;
			}
			required_segment_file_size = (size64_t) last_section->start_offset
					           + chunk_buffer_size
					           + sizeof( uint32_t )
					           + sizeof( ewf_section_start_t );

			/* Check if chunk fits in exisiting delta segment file
			 */
			if( required_segment_file_size > delta_segment_table->maximum_segment_size )
			{
				/* Make sure to write a next section in the the previous delta segment file
				 * The segment file offset is updated by the function
				 */
				write_count = libewf_segment_file_write_last_section(
					       segment_file,
					       io_handle,
					       file_io_pool,
					       file_io_pool_entry,
				               segment_file_offset,
					       0,
					       error );

				if( write_count == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_WRITE_FAILED,
					 "%s: unable to write last section.",
					 function );

					return( -1 );
				}
				segment_file_offset += write_count;
				total_write_count   += write_count;

				segment_file = NULL;
			}
			else
			{
				if( libcdata_list_remove_element(
				     segment_file->section_list,
				     last_list_element,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_REMOVE_FAILED,
					 "%s: unable to remove last section from list.",
					 function );

					return( -1 );
				}
				if( libcdata_list_element_free(
				     &last_list_element,
				     (int (*)(intptr_t **, libcerror_error_t **)) &libewf_section_free,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free last list element.",
					 function );

					return( -1 );
				}
			}
		}
		if( segment_file == NULL )
		{
			if( libewf_write_io_handle_create_segment_file(
			     io_handle,
			     file_io_pool,
			     delta_segment_files_list,
			     segment_files_cache,
			     delta_segment_table,
			     (uint16_t) ( segment_files_list_index + 1 ),
			     write_io_handle->maximum_number_of_segments,
			     LIBEWF_SEGMENT_FILE_TYPE_DWF,
			     &segment_file,
			     &segment_files_list_index,
			     &file_io_pool_entry,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_OPEN_FAILED,
				 "%s: unable to create delta segment file: %d.",
				 function,
				 segment_files_list_index + 1 );

				return( -1 );
			}
			/* Write the start of the segment file
			 * like the file header, the header, volume and/or data section, etc.
			 */
			write_count = libewf_segment_file_write_start(
				       segment_file,
				       io_handle,
				       file_io_pool,
				       file_io_pool_entry,
				       media_values,
				       header_sections,
			               &( write_io_handle->data_section ),
			               error );

			if( write_count == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write segment file start.",
				 function );

				return( -1 );
			}
			segment_file_offset = write_count;
			total_write_count  += write_count;
		}
	}
	else
	{
		if( libmfdata_file_list_get_file_value_by_index(
		     delta_segment_files_list,
		     file_io_pool,
		     segment_files_cache,
		     segment_files_list_index,
		     (intptr_t **) &segment_file,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve delta segment file: %d value from list.",
			 function,
			 segment_files_list_index + 1 );

			return( -1 );
		}
		if( segment_file == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing segment file: %d.",
			 function,
			 segment_files_list_index + 1 );

			return( -1 );
		}
		segment_file_offset = existing_chunk_offset
		                    - sizeof( ewf_section_start_t )
		                    - sizeof( ewfx_delta_chunk_header_t );

		if( libbfio_pool_seek_offset(
		     file_io_pool,
		     file_io_pool_entry,
		     segment_file_offset,
		     SEEK_SET,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to seek chunk offset: %" PRIi64 ".",
			 function,
			 segment_file_offset );

			return( -1 );
		}
		no_section_append = 1;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: writing delta chunk section at offset: %" PRIi64 ".\n",
		 function,
		 segment_file_offset );
	}
#endif
	write_count = libewf_segment_file_write_delta_chunk(
		       segment_file,
		       file_io_pool,
		       file_io_pool_entry,
		       segment_file_offset,
		       chunk_table_list,
		       chunk_index,
		       chunk_buffer,
		       chunk_buffer_size,
		       checksum_buffer,
		       &chunk_checksum,
	               write_checksum,
	               no_section_append,
	               error );

	if( write_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write delta chunk section.",
		 function );

		return( -1 );
	}
	segment_file_offset += write_count;
	total_write_count   += write_count;

	if( no_section_append == 0 )
	{
		write_count = libewf_segment_file_write_last_section(
			       segment_file,
			       io_handle,
			       file_io_pool,
			       file_io_pool_entry,
			       segment_file_offset,
			       1,
		               error );

		if( write_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write last section.",
			 function );

			return( -1 );
		}
		segment_file_offset += write_count;
		total_write_count   += write_count;
	}
	return( total_write_count );
}

/* Corrects sections after streamed write
 * Returns 1 if successful or -1 on error
 */
int libewf_write_io_handle_finalize_write_sections_corrections(
     libewf_io_handle_t *io_handle,
     libbfio_pool_t *file_io_pool,
     uint32_t number_of_chunks_written_to_last_segment,
     libewf_media_values_t *media_values,
     libmfdata_file_list_t *segment_files_list,
     libfcache_cache_t *segment_files_cache,
     libfvalue_table_t *hash_values,
     libewf_hash_sections_t *hash_sections,
     libcdata_array_t *sessions,
     libcdata_array_t *tracks,
     libcdata_range_list_t *acquiry_errors,
     ewf_data_t **cached_data_section,
     libcerror_error_t **error )
{
	libewf_segment_file_t *segment_file = NULL;
	static char *function               = "libewf_write_io_handle_finalize_write_sections_corrections";
	int file_io_pool_entry              = 0;
	int number_of_segment_files         = 0;
	int last_segment_file               = 0;
	int segment_files_list_index        = 0;

	if( libmfdata_file_list_get_number_of_files(
	     segment_files_list,
	     &number_of_segment_files,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of segment files.",
		 function );

		return( -1 );
	}
	if( ( number_of_segment_files < 0 )
	 || ( number_of_segment_files > (int) UINT16_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of segment files value out of bounds.",
		 function );

		return( -1 );
	}
	for( segment_files_list_index = 0;
	     segment_files_list_index < number_of_segment_files;
	     segment_files_list_index++ )
	{
		if( segment_files_list_index == ( number_of_segment_files - 1 ) )
		{
			last_segment_file = 1;
		}
		segment_file = NULL;

		if( libmfdata_file_list_get_file_by_index(
		     segment_files_list,
		     segment_files_list_index,
		     &file_io_pool_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve segment file: %d from list.",
			 function,
			 segment_files_list_index + 1 );

			return( -1 );
		}
		if( libmfdata_file_list_get_file_value_by_index(
		     segment_files_list,
		     file_io_pool,
		     segment_files_cache,
		     segment_files_list_index,
		     (intptr_t **) &segment_file,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve segment file: %d from list.",
			 function,
			 segment_files_list_index + 1 );

			return( -1 );
		}
		if( segment_file == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing segment file: %d.",
			 function,
			 segment_files_list_index + 1 );

			return( -1 );
		}
		if( libewf_segment_file_write_sections_correction(
		     segment_file,
		     io_handle,
		     file_io_pool,
		     file_io_pool_entry,
		     number_of_chunks_written_to_last_segment,
		     last_segment_file,
		     media_values,
		     hash_values,
		     hash_sections,
		     sessions,
		     tracks,
		     acquiry_errors,
		     cached_data_section,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write sections correction to segment file: %d.",
			 function,
			 segment_files_list_index + 1 );

			return( -1 );
		}
	}
	return( 1 );
}

