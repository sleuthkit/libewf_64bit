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

#if !defined( _LIBSMRAW_INTERNAL_HANDLE_H )
#define _LIBSMRAW_INTERNAL_HANDLE_H

#include <common.h>
#include <types.h>

#include "libsmraw_extern.h"
#include "libsmraw_information_file.h"
#include "libsmraw_libbfio.h"
#include "libsmraw_libcerror.h"
#include "libsmraw_libcstring.h"
#include "libsmraw_libfvalue.h"
#include "libsmraw_libmfdata.h"
#include "libsmraw_types.h"

#if defined( _MSC_VER ) || defined( __BORLANDC__ ) || defined( __MINGW32_VERSION ) || defined( __MINGW64_VERSION_MAJOR )

/* This inclusion is needed otherwise some linkers
 * mess up exporting the legacy and metadata functions
 */
#include "libsmraw_legacy.h"
#include "libsmraw_metadata.h"
#endif

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libsmraw_internal_handle libsmraw_internal_handle_t;

struct libsmraw_internal_handle
{
        /* The basename
	 */
        libcstring_system_character_t *basename;

        /* The size of the basename
	 */
        size_t basename_size;

	/* The total number of segments
	 */
	int total_number_of_segments;

	/* The segment (file) table
	 */
	libmfdata_segment_table_t *segment_table;

	/* The pool of file IO handles
	 */
	libbfio_pool_t *file_io_pool;

	/* Value to indicate if the file IO pool was created inside the library
	 */
	uint8_t file_io_pool_created_in_library;

	/* The maximum number of open handles in the pool
	 */
	int maximum_number_of_open_handles;

	/* A value to indicate if the read values have been initialized
	 */
	uint8_t read_values_initialized;

	/* A value to indicate if the write values have been initialized
	 */
	uint8_t write_values_initialized;

	/* A value to indicate if the information file
	 * needs to be written on close
	 */
	uint8_t write_information_on_close;

	/* The information file
	 */
	libsmraw_information_file_t *information_file;

	/* The media size
	 */
        size64_t media_size;

	/* The media values table
	 */
	libfvalue_table_t *media_values;

	/* The information values table
	 */
	libfvalue_table_t *information_values;

	/* The integrity hash values table
	 */
	libfvalue_table_t *integrity_hash_values;

	/* Value to indicate if abort was signalled
	 */
	int abort;
};

LIBSMRAW_EXTERN \
int libsmraw_handle_initialize(
     libsmraw_handle_t **handle,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_free(
     libsmraw_handle_t **handle,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_signal_abort(
     libsmraw_handle_t *handle,
     libcerror_error_t **error );

int libsmraw_internal_handle_initialize_write_values(
     libsmraw_internal_handle_t *raw_io_handle,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_open(
     libsmraw_handle_t *handle,
     char * const filenames[],
     int number_of_filenames,
     int access_flags,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )
LIBSMRAW_EXTERN \
int libsmraw_handle_open_wide(
     libsmraw_handle_t *handle,
     wchar_t * const filenames[],
     int number_of_filenames,
     int access_flags,
     libcerror_error_t **error );
#endif

LIBSMRAW_EXTERN \
int libsmraw_handle_open_file_io_pool(
     libsmraw_handle_t *handle,
     libbfio_pool_t *file_io_pool,
     int access_flags,
     libcerror_error_t **error );

int libsmraw_handle_read_information_file(
     libsmraw_internal_handle_t *internal_handle,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_close(
     libsmraw_handle_t *handle,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
ssize_t libsmraw_handle_read_buffer(
         libsmraw_handle_t *handle,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error );

LIBSMRAW_EXTERN \
ssize_t libsmraw_handle_read_random(
         libsmraw_handle_t *handle,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error );

LIBSMRAW_EXTERN \
ssize_t libsmraw_handle_write_buffer(
         libsmraw_handle_t *handle,
         const void *buffer,
         size_t buffer_size,
         libcerror_error_t **error );

LIBSMRAW_EXTERN \
ssize_t libsmraw_handle_write_random(
         libsmraw_handle_t *handle,
         const void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error );

LIBSMRAW_EXTERN \
off64_t libsmraw_handle_seek_offset(
         libsmraw_handle_t *handle,
         off64_t offset,
         int whence,
         libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_offset(
     libsmraw_handle_t *handle,
     off64_t *offset,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_set_maximum_number_of_open_handles(
     libsmraw_handle_t *handle,
     int maximum_number_of_open_handles,
     libcerror_error_t **error );

int libsmraw_handle_set_segment_name(
     intptr_t *io_handle,
     libbfio_handle_t *file_io_handle,
     int segment_index,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_segment_filename_size(
     libsmraw_handle_t *handle,
     size_t *filename_size,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_segment_filename(
     libsmraw_handle_t *handle,
     char *filename,
     size_t filename_size,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_set_segment_filename(
     libsmraw_handle_t *handle,
     const char *filename,
     size_t filename_length,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )
LIBSMRAW_EXTERN \
int libsmraw_handle_get_segment_filename_size_wide(
     libsmraw_handle_t *handle,
     size_t *filename_size,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_segment_filename_wide(
     libsmraw_handle_t *handle,
     wchar_t *filename,
     size_t filename_size,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_set_segment_filename_wide(
     libsmraw_handle_t *handle,
     const wchar_t *filename,
     size_t filename_length,
     libcerror_error_t **error );
#endif

LIBSMRAW_EXTERN \
int libsmraw_handle_get_maximum_segment_size(
     libsmraw_handle_t *handle,
     size64_t *maximum_segment_size,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_set_maximum_segment_size(
     libsmraw_handle_t *handle,
     size64_t maximum_segment_size,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_filename_size(
     libsmraw_handle_t *handle,
     size_t *filename_size,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_filename(
     libsmraw_handle_t *handle,
     char *filename,
     size_t filename_size,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )
LIBSMRAW_EXTERN \
int libsmraw_handle_get_filename_size_wide(
     libsmraw_handle_t *handle,
     size_t *filename_size,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_filename_wide(
     libsmraw_handle_t *handle,
     wchar_t *filename,
     size_t filename_size,
     libcerror_error_t **error );
#endif

LIBSMRAW_EXTERN \
int libsmraw_handle_get_file_io_handle(
     libsmraw_handle_t *handle,
     libbfio_handle_t **file_io_handle,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

