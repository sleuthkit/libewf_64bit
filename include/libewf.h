/*
 * Library to access the Expert Witness Compression Format (EWF)
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

#if !defined( _LIBEWF_H )
#define _LIBEWF_H

#include <libewf/codepage.h>
#include <libewf/definitions.h>
#include <libewf/error.h>
#include <libewf/extern.h>
#include <libewf/features.h>
#include <libewf/types.h>

#include <stdio.h>

#if defined( LIBEWF_HAVE_BFIO )
#include <libbfio.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------
 * Support functions
 * ------------------------------------------------------------------------- */

/* Returns the library version
 */
LIBEWF_EXTERN \
const char *libewf_get_version(
             void );

/* Returns the access flags for reading
 */
LIBEWF_EXTERN \
int libewf_get_access_flags_read(
     void );

/* Returns the access flags for reading and writing
 */
LIBEWF_EXTERN \
int libewf_get_access_flags_read_write(
     void );

/* Returns the access flags for writing
 */
LIBEWF_EXTERN \
int libewf_get_access_flags_write(
     void );

/* Returns the access flags for resume writing
 */
LIBEWF_EXTERN \
int libewf_get_access_flags_write_resume(
     void );

/* Retrieves the narrow system string codepage
 * A value of 0 represents no codepage, UTF-8 encoding is used instead
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_get_codepage(
     int *codepage,
     libewf_error_t **error );

/* Sets the narrow system string codepage
 * A value of 0 represents no codepage, UTF-8 encoding is used instead
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_set_codepage(
     int codepage,
     libewf_error_t **error );

/* Determines if a file is an EWF file (check for the EWF file signature)
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBEWF_EXTERN \
int libewf_check_file_signature(
     const char *filename,
     libewf_error_t **error );

#if defined( LIBEWF_HAVE_WIDE_CHARACTER_TYPE )
/* Determines if a file is an EWF file (check for the EWF file signature)
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBEWF_EXTERN \
int libewf_check_file_signature_wide(
     const wchar_t *filename,
     libewf_error_t **error );
#endif

#if defined( LIBEWF_HAVE_BFIO )
/* Determines if a file is an EWF file (check for the EWF file signature) using a Basic File IO (bfio) handle
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBEWF_EXTERN \
int libewf_check_file_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libewf_error_t **error );
#endif

/* Globs the segment files according to the EWF naming schema
 * if the format is known the filename should contain the base of the filename
 * otherwise the function will try to determine the format based on the extension
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_glob(
     const char *filename,
     size_t filename_length,
     uint8_t format,
     char **filenames[],
     int *number_of_filenames,
     libewf_error_t **error );

/* Frees the globbed filenames
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_glob_free(
     char *filenames[],
     int number_of_filenames,
     libewf_error_t **error );

#if defined( LIBEWF_HAVE_WIDE_CHARACTER_TYPE )
/* Globs the segment files according to the EWF naming schema
 * if the format is known the filename should contain the base of the filename
 * otherwise the function will try to determine the format based on the extension
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_glob_wide(
     const wchar_t *filename,
     size_t filename_length,
     uint8_t format,
     wchar_t **filenames[],
     int *number_of_filenames,
     libewf_error_t **error );

/* Frees the globbed wide filenames
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_glob_wide_free(
     wchar_t *filenames[],
     int number_of_filenames,
     libewf_error_t **error );
#endif

/* -------------------------------------------------------------------------
 * Support functions - deprecated
 * ------------------------------------------------------------------------- */

/* Returns the flags for reading
 *
 * This function deprecated use libewf_get_access_flags_read instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN \
uint8_t libewf_get_flags_read(
         void );

/* Returns the flags for reading and writing
 *
 * This function deprecated use libewf_get_access_flags_read_write instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN \
uint8_t libewf_get_flags_read_write(
         void );

/* Returns the flags for writing
 *
 * This function deprecated use libewf_get_access_flags_write instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN \
uint8_t libewf_get_flags_write(
         void );

/* Returns the flags for resume writing
 *
 * This function deprecated use libewf_get_access_flags_write_resume instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN \
uint8_t libewf_get_flags_write_resume(
         void );

/* -------------------------------------------------------------------------
 * Notify functions
 * ------------------------------------------------------------------------- */

/* Sets the verbose notification
 */
LIBEWF_EXTERN \
void libewf_notify_set_verbose(
      int verbose );

/* Sets the notification stream
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_notify_set_stream(
     FILE *stream,
     libewf_error_t **error );

/* Opens the notification stream using a filename
 * The stream is opened in append mode
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_notify_stream_open(
     const char *filename,
     libewf_error_t **error );

/* Closes the notification stream if opened using a filename
 * Returns 0 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_notify_stream_close(
     libewf_error_t **error );

/* -------------------------------------------------------------------------
 * Error functions
 * ------------------------------------------------------------------------- */

/* Frees the error including elements
 */
LIBEWF_EXTERN \
void libewf_error_free(
      libewf_error_t **error );

/* Prints a descriptive string of the error to the stream
 * Returns the number of printed characters if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_error_fprint(
     libewf_error_t *error,
     FILE *stream );

/* Prints a descriptive string of the error to the string
 * The end-of-string character is not included in the return value
 * Returns the number of printed characters if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_error_sprint(
     libewf_error_t *error,
     char *string,
     size_t size );

/* Prints a backtrace of the error to the stream
 * Returns the number of printed characters if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_error_backtrace_fprint(
     libewf_error_t *error,
     FILE *stream );

/* Prints a backtrace of the error to the string
 * The end-of-string character is not included in the return value
 * Returns the number of printed characters if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_error_backtrace_sprint(
     libewf_error_t *error,
     char *string,
     size_t size );

/* -------------------------------------------------------------------------
 * File/Handle functions
 * ------------------------------------------------------------------------- */

/* Initialize the handle
 * The handle must point to a NULL pointer to be allocated
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_initialize(
     libewf_handle_t **handle,
     libewf_error_t **error );

/* Frees the handle including elements
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_free(
     libewf_handle_t **handle,
     libewf_error_t **error );

/* Clones the handle including elements
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_clone(
     libewf_handle_t **destination_handle,
     libewf_handle_t *source_handle,
     libewf_error_t **error );

/* Signals the handle to abort its current activity
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_signal_abort(
     libewf_handle_t *handle,
     libewf_error_t **error );

/* Opens a set of EWF file(s)
 * For reading files should contain all filenames that make up an EWF image
 * For writing files should contain the base of the filename, extentions like .e01 will be automatically added
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_open(
     libewf_handle_t *handle,
     char * const filenames[],
     int number_of_filenames,
     int access_flags,
     libewf_error_t **error );

#if defined( LIBEWF_HAVE_WIDE_CHARACTER_TYPE )
/* Opens a set of EWF file(s)
 * For reading files should contain all filenames that make up an EWF image
 * For writing files should contain the base of the filename, extentions like .e01 will be automatically added
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_open_wide(
     libewf_handle_t *handle,
     wchar_t * const filenames[],
     int number_of_filenames,
     int access_flags,
     libewf_error_t **error );
#endif

#if defined( LIBEWF_HAVE_BFIO )
/* Opens a set of EWF file(s) using a Basic File IO (bfio) pool
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_open_file_io_pool(
     libewf_handle_t *handle,
     libbfio_pool_t *file_io_pool,
     int access_flags,
     libewf_error_t **error );
#endif

/* Closes the EWF handle
 * Returns 0 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_close(
     libewf_handle_t *handle,
     libewf_error_t **error );

/* Prepares chunk of (media) data after reading it according to the handle settings
 * It applies decompression if necessary and validates the chunk checksum
 * This function should be used after libewf_handle_read_chunk
 * The value chunk_buffer_size contains the size of the chunk
 * Returns the resulting chunk size or -1 on error
 */
LIBEWF_EXTERN \
ssize_t libewf_handle_prepare_read_chunk(
         libewf_handle_t *handle,
         void *chunk_buffer,
         size_t chunk_buffer_size,
         void *uncompressed_chunk_buffer,
         size_t *uncompressed_chunk_buffer_size,
         int8_t is_compressed,
         uint32_t chunk_checksum,
         int8_t read_checksum,
         libewf_error_t **error );

/* Reads a chunk of (media) data from the current offset into a buffer
 * Will read until the requested size is filled or the entire chunk is read
 * The values read_checksum and chunk_checksum are used for uncompressed chunks only
 * The value chunk_checksum is set to a runtime version of the value in the checksum_buffer
 * The value read_checksum is set if the checksum has been read into checksum_buffer
 * The value chunk_buffer_size contains the size of the chunk buffer
 * Returns the number of bytes read or -1 on error
 */
LIBEWF_EXTERN \
ssize_t libewf_handle_read_chunk(
         libewf_handle_t *handle,
         void *chunk_buffer,
         size_t chunk_buffer_size,
         int8_t *is_compressed,
         void *checksum_buffer,
         uint32_t *chunk_checksum,
         int8_t *read_checksum,
         libewf_error_t **error );

/* Reads (media) data at the current offset into a buffer
 * Returns the number of bytes read or -1 on error
 */
LIBEWF_EXTERN \
ssize_t libewf_handle_read_buffer(
         libewf_handle_t *handle,
         void *buffer,
         size_t buffer_size,
         libewf_error_t **error );

/* Reads (media) data at a specific offset
 * Returns the number of bytes read or -1 on error
 */
LIBEWF_EXTERN \
ssize_t libewf_handle_read_random(
         libewf_handle_t *handle,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libewf_error_t **error );

/* Prepares a chunk of (media) data before writing according to the handle settings
 * This function should be used before libewf_handle_write_chunk
 * The chunk_buffer_size should contain the actual chunk size
 * The function sets the chunk checksum, is compressed and write checksum values
 * Returns the resulting chunk size or -1 on error
 */
LIBEWF_EXTERN \
ssize_t libewf_handle_prepare_write_chunk(
         libewf_handle_t *handle,
         void *buffer,
         size_t buffer_size,
         void *compressed_chunk_buffer,
         size_t *compressed_chunk_buffer_size,
         int8_t *is_compressed,
         uint32_t *chunk_checksum,
         int8_t *write_checksum,
         libewf_error_t **error );

/* Writes a chunk of (media) data in EWF format at the current offset
 * the necessary settings of the write values must have been made
 * chunk_buffer_size contains the size of the data within the buffer while
 * data_size contains the size of the actual input data
 * Will initialize write if necessary
 * Returns the number of input bytes written, 0 when no longer bytes can be written or -1 on error
 */
LIBEWF_EXTERN \
ssize_t libewf_handle_write_chunk(
         libewf_handle_t *handle,
         const void *chunk_buffer,
         size_t chunk_buffer_size,
         size_t data_size,
         int8_t is_compressed,
         void *checksum_buffer,
         uint32_t chunk_checksum,
         int8_t write_checksum,
         libewf_error_t **error );

/* Writes (media) data at the current offset
 * the necessary settings of the write values must have been made
 * Will initialize write if necessary
 * Returns the number of input bytes written, 0 when no longer bytes can be written or -1 on error
 */
LIBEWF_EXTERN \
ssize_t libewf_handle_write_buffer(
         libewf_handle_t *handle,
         const void *buffer,
         size_t buffer_size,
         libewf_error_t **error );

/* Writes (media) data at a specific offset,
 * the necessary settings of the write values must have been made
 * Will initialize write if necessary
 * Returns the number of input bytes written, 0 when no longer bytes can be written or -1 on error
 */
LIBEWF_EXTERN \
ssize_t libewf_handle_write_random(
         libewf_handle_t *handle,
         const void *buffer,
         size_t buffer_size,
         off64_t offset,
         libewf_error_t **error );

/* Finalizes the write by correcting the EWF the meta data in the segment files
 * This function is required after writing from stream
 * Returns the number of input bytes written or -1 on error
 */
LIBEWF_EXTERN \
ssize_t libewf_handle_write_finalize(
         libewf_handle_t *handle,
         libewf_error_t **error );

/* Seeks a certain offset of the (media) data
 * Returns the offset if seek is successful or -1 on error
 */
LIBEWF_EXTERN \
off64_t libewf_handle_seek_offset(
         libewf_handle_t *handle,
         off64_t offset,
         int whence,
         libewf_error_t **error );

/* Retrieves the current offset of the (media) data
 * Returns the offset if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_offset(
     libewf_handle_t *handle,
     off64_t *offset,
     libewf_error_t **error );

/* Sets the maximum number of (concurrent) open file handles
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_set_maximum_number_of_open_handles(
     libewf_handle_t *handle,
     int maximum_number_of_open_handles,
     libewf_error_t **error );

/* Retrieves the segment filename size
 * The filename size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_segment_filename_size(
     libewf_handle_t *handle,
     size_t *filename_size,
     libewf_error_t **error );

/* Retrieves the segment filename
 * The filename size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_segment_filename(
     libewf_handle_t *handle,
     char *filename,
     size_t filename_size,
     libewf_error_t **error );

/* Sets the segment filename
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_set_segment_filename(
     libewf_handle_t *handle,
     const char *filename,
     size_t filename_length,
     libewf_error_t **error );

#if defined( LIBEWF_HAVE_WIDE_CHARACTER_TYPE )
/* Retrieves the segment filename size
 * The filename size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_segment_filename_size_wide(
     libewf_handle_t *handle,
     size_t *filename_size,
     libewf_error_t **error );

/* Retrieves the segment filename
 * The filename size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_segment_filename_wide(
     libewf_handle_t *handle,
     wchar_t *filename,
     size_t filename_size,
     libewf_error_t **error );

/* Sets the segment filename
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_set_segment_filename_wide(
     libewf_handle_t *handle,
     const wchar_t *filename,
     size_t filename_length,
     libewf_error_t **error );
#endif

/* Retrieves the maximum segment file size
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_maximum_segment_size(
     libewf_handle_t *handle,
     size64_t *maximum_segment_size,
     libewf_error_t **error );

/* Sets the maximum segment file size
 * A maximum segment file size of 0 represents the maximum possible size for the format
 * If the maximum segment file size is smaller than the size needed to store a single chunk
 * the size off the latter is enforced and not the maximum segment file size
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_set_maximum_segment_size(
     libewf_handle_t *handle,
     size64_t maximum_segment_size,
     libewf_error_t **error );

/* Retrieves the delta segment filename size
 * The filename size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_delta_segment_filename_size(
     libewf_handle_t *handle,
     size_t *filename_size,
     libewf_error_t **error );

/* Retrieves the delta segment filename
 * The filename size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_delta_segment_filename(
     libewf_handle_t *handle,
     char *filename,
     size_t filename_size,
     libewf_error_t **error );

/* Sets the delta segment filename
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_set_delta_segment_filename(
     libewf_handle_t *handle,
     const char *filename,
     size_t filename_length,
     libewf_error_t **error );

#if defined( LIBEWF_HAVE_WIDE_CHARACTER_TYPE )
/* Retrieves the delta segment filename size
 * The filename size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_delta_segment_filename_size_wide(
     libewf_handle_t *handle,
     size_t *filename_size,
     libewf_error_t **error );

/* Retrieves the delta segment filename
 * The filename size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_delta_segment_filename_wide(
     libewf_handle_t *handle,
     wchar_t *filename,
     size_t filename_size,
     libewf_error_t **error );

/* Sets the delta segment filename
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_set_delta_segment_filename_wide(
     libewf_handle_t *handle,
     const wchar_t *filename,
     size_t filename_length,
     libewf_error_t **error );
#endif

/* Retrieves the maximum delta segment file size
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_maximum_delta_segment_size(
     libewf_handle_t *handle,
     size64_t *maximum_delta_segment_size,
     libewf_error_t **error );

/* Sets the maximum delta segment file size
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_set_maximum_delta_segment_size(
     libewf_handle_t *handle,
     size64_t maximum_delta_segment_size,
     libewf_error_t **error );

/* Determine if the segment files are corrupted
 * Returns 1 if corrupted, 0 if not or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_segment_files_corrupted(
     libewf_handle_t *handle,
     libewf_error_t **error );

/* Retrieves the filename size of the (delta) segment file of the current chunk
 * The filename size includes the end of string character
 * Returns 1 if successful, 0 if no such filename or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_filename_size(
     libewf_handle_t *handle,
     size_t *filename_size,
     libewf_error_t **error );

/* Retrieves the filename of the (delta) segment file of the current chunk
 * The filename size should include the end of string character
 * Returns 1 if successful, 0 if no such filename or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_filename(
     libewf_handle_t *handle,
     char *filename,
     size_t filename_size,
     libewf_error_t **error );

#if defined( LIBEWF_HAVE_WIDE_CHARACTER_TYPE )
/* Retrieves the filename size of the (delta) segment file of the current chunk
 * The filename size includes the end of string character
 * Returns 1 if successful, 0 if no such filename or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_filename_size_wide(
     libewf_handle_t *handle,
     size_t *filename_size,
     libewf_error_t **error );

/* Retrieves the filename of the (delta) segment file of the current chunk
 * The filename size should include the end of string character
 * Returns 1 if successful, 0 if no such filename or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_filename_wide(
     libewf_handle_t *handle,
     wchar_t *filename,
     size_t filename_size,
     libewf_error_t **error );
#endif

#if defined( LIBEWF_HAVE_BFIO )
/* Retrieves the file IO handle of the (delta) segment file of the current chunk
 * Returns 1 if successful, 0 if no such file IO handle or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_file_io_handle(
     libewf_handle_t *handle,
     libbfio_handle_t **file_io_handle,
     libewf_error_t **error );
#endif

/* -------------------------------------------------------------------------
 * File/Handle functions - deprecated
 * ------------------------------------------------------------------------- */

/* Sets the maximum amount of (concurrent) open file handles
 * Returns 1 if successful or -1 on error
 *
 * This function deprecated use libewf_handle_set_maximum_number_of_open_handles instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN \
int libewf_handle_set_maximum_amount_of_open_handles(
     libewf_handle_t *handle,
     int maximum_amount_of_open_handles,
     libewf_error_t **error );

/* Retrieves the amount of sectors
 * Returns 1 if successful or -1 on error
 *
 * This function deprecated use libewf_handle_get_number_of_sectors instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN \
int libewf_handle_get_amount_of_sectors(
     libewf_handle_t *handle,
     uint64_t *amount_of_sectors,
     libewf_error_t **error );

/* Retrieves the amount of chunks written
 * Returns 1 if successful or -1 on error
 *
 * This function deprecated use libewf_handle_get_number_of_chunks_written instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN \
int libewf_handle_get_write_amount_of_chunks(
     libewf_handle_t *handle,
     uint32_t *amount_of_chunks,
     libewf_error_t **error );

/* Retrieves the segment file size
 * Returns 1 if successful or -1 on error
 *
 * This function deprecated use libewf_handle_get_maximum_segment_size instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN \
int libewf_handle_get_segment_file_size(
     libewf_handle_t *handle,
     size64_t *segment_file_size,
     libewf_error_t **error );

/* Sets the segment file size
 * Returns 1 if successful or -1 on error
 *
 * This function deprecated use libewf_handle_set_maximum_segment_size instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN \
int libewf_handle_set_segment_file_size(
     libewf_handle_t *handle,
     size64_t segment_file_size,
     libewf_error_t **error );

/* Retrieves the delta segment file size
 * Returns 1 if successful or -1 on error
 *
 * This function deprecated use libewf_handle_get_maximum_delta_segment_size instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN \
int libewf_handle_get_delta_segment_file_size(
     libewf_handle_t *handle,
     size64_t *delta_segment_file_size,
     libewf_error_t **error );

/* Sets the delta segment file size
 * Returns 1 if successful or -1 on error
 *
 * This function deprecated use libewf_handle_set_maximum_delta_segment_size instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN \
int libewf_handle_set_delta_segment_file_size(
     libewf_handle_t *handle,
     size64_t delta_segment_file_size,
     libewf_error_t **error );

/* -------------------------------------------------------------------------
 * Meta data functions
 * ------------------------------------------------------------------------- */

/* Retrieves the number of sectors per chunk
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_sectors_per_chunk(
     libewf_handle_t *handle,
     uint32_t *sectors_per_chunk,
     libewf_error_t **error );

/* Sets the number of sectors per chunk
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_set_sectors_per_chunk(
     libewf_handle_t *handle,
     uint32_t sectors_per_chunk,
     libewf_error_t **error );

/* Retrieves the number of bytes per sector
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_bytes_per_sector(
     libewf_handle_t *handle,
     uint32_t *bytes_per_sector,
     libewf_error_t **error );

/* Sets the number of bytes per sector
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_set_bytes_per_sector(
     libewf_handle_t *handle,
     uint32_t bytes_per_sector,
     libewf_error_t **error );

/* Retrieves the number of sectors
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_number_of_sectors(
     libewf_handle_t *handle,
     uint64_t *number_of_sectors,
     libewf_error_t **error );

/* Retrieves the chunk size
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_chunk_size(
     libewf_handle_t *handle,
     size32_t *chunk_size,
     libewf_error_t **error );

/* Retrieves the error granularity
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_error_granularity(
     libewf_handle_t *handle,
     uint32_t *error_granularity,
     libewf_error_t **error );

/* Sets the error granularity
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_set_error_granularity(
     libewf_handle_t *handle,
     uint32_t error_granularity,
     libewf_error_t **error );

/* Retrieves the compression values
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_compression_values(
     libewf_handle_t *handle,
     int8_t *compression_level,
     uint8_t *compression_flags,
     libewf_error_t **error );

/* Sets the compression values
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_set_compression_values(
     libewf_handle_t *handle,
     int8_t compression_level,
     uint8_t compression_flags,
     libewf_error_t **error );

/* Retrieves the size of the contained (media) data
 * This function will compensate for a media_size that is not a multitude of bytes_per_sector
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_media_size(
     libewf_handle_t *handle,
     size64_t *media_size,
     libewf_error_t **error );

/* Sets the media size
 * The media_size is stored as number_of_sectors x bytes_per_sector
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_set_media_size(
     libewf_handle_t *handle,
     size64_t media_size,
     libewf_error_t **error );

/* Retrieves the media type value
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_media_type(
     libewf_handle_t *handle,
     uint8_t *media_type,
     libewf_error_t **error );

/* Sets the media type
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_set_media_type(
     libewf_handle_t *handle,
     uint8_t media_type,
     libewf_error_t **error );

/* Retrieves the media flags
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_media_flags(
     libewf_handle_t *handle,
     uint8_t *media_flags,
     libewf_error_t **error );

/* Sets the media flags
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_set_media_flags(
     libewf_handle_t *handle,
     uint8_t media_flags,
     libewf_error_t **error );

/* Retrieves the format type value
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_format(
     libewf_handle_t *handle,
     uint8_t *format,
     libewf_error_t **error );

/* Sets the output format
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_set_format(
     libewf_handle_t *handle,
     uint8_t format,
     libewf_error_t **error );

/* Retrieves the segment file set identifier
 * The identifier is a GUID and is 16 bytes of size
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_segment_file_set_identifier(
     libewf_handle_t *handle,
     uint8_t *set_identifier,
     size_t size,
     libewf_error_t **error );

/* Sets the segment file set identifier
 * The identifier is a GUID and is 16 bytes of size
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_set_segment_file_set_identifier(
     libewf_handle_t *handle,
     const uint8_t *set_identifier,
     size_t size,
     libewf_error_t **error );

/* Retrieves the MD5 hash
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_md5_hash(
     libewf_handle_t *handle,
     uint8_t *md5_hash,
     size_t size,
     libewf_error_t **error );

/* Sets the MD5 hash
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_set_md5_hash(
     libewf_handle_t *handle,
     uint8_t *md5_hash,
     size_t size,
     libewf_error_t **error );

/* Retrieves the SHA1 hash
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_sha1_hash(
     libewf_handle_t *handle,
     uint8_t *sha1_hash,
     size_t size,
     libewf_error_t **error );

/* Sets the SHA1 hash
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_set_sha1_hash(
     libewf_handle_t *handle,
     uint8_t *sha1_hash,
     size_t size,
     libewf_error_t **error );

/* Retrieves the number of chunks written
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_number_of_chunks_written(
     libewf_handle_t *handle,
     uint32_t *number_of_chunks,
     libewf_error_t **error );

/* Sets the read zero chunk on error
 * The chunk is not zeroed if read raw is used
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_set_read_zero_chunk_on_error(
     libewf_handle_t *handle,
     uint8_t zero_on_error,
     libewf_error_t **error );

/* Copies the media values from the source to the destination handle
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_copy_media_values(
     libewf_handle_t *destination_handle,
     libewf_handle_t *source_handle,
     libewf_error_t **error );

/* Retrieves the number of acquiry errors
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_number_of_acquiry_errors(
     libewf_handle_t *handle,
     uint32_t *number_of_errors,
     libewf_error_t **error );

/* Retrieves an acquiry error
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_acquiry_error(
     libewf_handle_t *handle,
     uint32_t index,
     uint64_t *start_sector,
     uint64_t *number_of_sectors,
     libewf_error_t **error );

/* Appends an acquiry error
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_append_acquiry_error(
     libewf_handle_t *handle,
     uint64_t start_sector,
     uint64_t number_of_sectors,
     libewf_error_t **error );

/* Retrieves the number of checksum errors
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_number_of_checksum_errors(
     libewf_handle_t *handle,
     uint32_t *number_of_errors,
     libewf_error_t **error );

/* Retrieves a checksum error
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_checksum_error(
     libewf_handle_t *handle,
     uint32_t index,
     uint64_t *start_sector,
     uint64_t *number_of_sectors,
     libewf_error_t **error );

/* Appends a checksum error
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_append_checksum_error(
     libewf_handle_t *handle,
     uint64_t start_sector,
     uint64_t number_of_sectors,
     libewf_error_t **error );

/* Retrieves the number of sessions
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_number_of_sessions(
     libewf_handle_t *handle,
     uint32_t *number_of_sessions,
     libewf_error_t **error );

/* Retrieves a session
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_session(
     libewf_handle_t *handle,
     uint32_t index,
     uint64_t *start_sector,
     uint64_t *number_of_sectors,
     libewf_error_t **error );

/* Appends a session
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_append_session(
     libewf_handle_t *handle,
     uint64_t start_sector,
     uint64_t number_of_sectors,
     libewf_error_t **error );

/* Retrieves the number of tracks
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_number_of_tracks(
     libewf_handle_t *handle,
     uint32_t *number_of_tracks,
     libewf_error_t **error );

/* Retrieves a track
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_track(
     libewf_handle_t *handle,
     uint32_t index,
     uint64_t *start_sector,
     uint64_t *number_of_sectors,
     libewf_error_t **error );

/* Appends a track
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_append_track(
     libewf_handle_t *handle,
     uint64_t start_sector,
     uint64_t number_of_sectors,
     libewf_error_t **error );

/* Retrieves the header codepage
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_header_codepage(
     libewf_handle_t *handle,
     int *header_codepage,
     libewf_error_t **error );

/* Sets the header codepage
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_set_header_codepage(
     libewf_handle_t *handle,
     int header_codepage,
     libewf_error_t **error );

/* Retrieves the header values date format
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_header_values_date_format(
     libewf_handle_t *handle,
     int *date_format,
     libewf_error_t **error );

/* Sets the header values date format
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_set_header_values_date_format(
     libewf_handle_t *handle,
     int date_format,
     libewf_error_t **error );

/* Retrieves the number of header values
 * Returns 1 if successful, 0 if no header values are present or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_number_of_header_values(
     libewf_handle_t *handle,
     uint32_t *number_of_values,
     libewf_error_t **error );

/* Retrieves the size of the header value identifier of a specific index
 * The identifier size includes the end of string character
 * Returns 1 if successful, 0 if no header values are present or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_header_value_identifier_size(
     libewf_handle_t *handle,
     uint32_t index,
     size_t *identifier_size,
     libewf_error_t **error );

/* Retrieves the header value identifier of a specific index
 * The identifier size should include the end of string character
 * Returns 1 if successful, 0 if no header values are present or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_header_value_identifier(
     libewf_handle_t *handle,
     uint32_t index,
     uint8_t *identifier,
     size_t identifier_size,
     libewf_error_t **error );

/* Retrieves the size of the UTF-8 encoded header value of an identifier
 * The string size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_utf8_header_value_size(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *utf8_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-8 encoded header value of an identifier
 * The string size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_utf8_header_value(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libewf_error_t **error );

/* Retrieves the header value case number
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
#define libewf_handle_get_utf8_header_value_case_number( handle, value, value_size, error ) \
        libewf_handle_get_utf8_header_value( handle, (uint8_t *) "case_number", 11, value, value_size, error )

/* Retrieves the header value description
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
#define libewf_handle_get_utf8_header_value_description( handle, value, value_size, error ) \
        libewf_handle_get_utf8_header_value( handle, (uint8_t *) "description", 11, value, value_size, error )

/* Retrieves the header value examiner name
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
#define libewf_handle_get_utf8_header_value_examiner_name( handle, value, value_size, error ) \
        libewf_handle_get_utf8_header_value( handle, (uint8_t *) "examiner_name", 13, value, value_size, error )

/* Retrieves the header value evidence number
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
#define libewf_handle_get_utf8_header_value_evidence_number( handle, value, value_size, error ) \
        libewf_handle_get_utf8_header_value( handle, (uint8_t *) "evidence_number", 15, value, value_size, error )

/* Retrieves the header value notes
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
#define libewf_handle_get_utf8_header_value_notes( handle, value, value_size, error ) \
        libewf_handle_get_utf8_header_value( handle, (uint8_t *) "notes", 5, value, value_size, error )

/* Retrieves the header value acquiry date
 * The string is encoded according to the date format
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
#define libewf_handle_get_utf8_header_value_acquiry_date( handle, value, value_size, error ) \
        libewf_handle_get_utf8_header_value( handle, (uint8_t *) "acquiry_date", 12, value, value_size, error )

/* Retrieves the header value system date
 * The string is encoded according to the date format
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
#define libewf_handle_get_utf8_header_value_system_date( handle, value, value_size, error ) \
        libewf_handle_get_utf8_header_value( handle, (uint8_t *) "system_date", 11, value, value_size, error )

/* Retrieves the header value acquiry operating system
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
#define libewf_handle_get_utf8_header_value_acquiry_operating_system( handle, value, value_size, error ) \
        libewf_handle_get_utf8_header_value( handle, (uint8_t *) "acquiry_operating_system", 24, value, value_size, error )

/* Retrieves the header value acquiry software version
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
#define libewf_handle_get_utf8_header_value_acquiry_software_version( handle, value, value_size, error ) \
        libewf_handle_get_utf8_header_value( handle, (uint8_t *) "acquiry_software_version", 24, value, value_size, error )

/* Retrieves the header value password
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
#define libewf_handle_get_utf8_header_value_password( handle, value, value_size, error ) \
        libewf_handle_get_utf8_header_value( handle, (uint8_t *) "password", 8, value, value_size, error )

/* Retrieves the header value compression level
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
#define libewf_handle_get_utf8_header_value_compression_level( handle, value, value_size, error ) \
        libewf_handle_get_utf8_header_value( handle, (uint8_t *) "compression_level", 17, value, value_size, error )

/* Retrieves the header value model
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
#define libewf_handle_get_utf8_header_value_model( handle, value, value_size, error ) \
        libewf_handle_get_utf8_header_value( handle, (uint8_t *) "model", 5, value, value_size, error )

/* Retrieves the header value serial number
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
#define libewf_handle_get_utf8_header_value_serial_number( handle, value, value_size, error ) \
        libewf_handle_get_utf8_header_value( handle, (uint8_t *) "serial_number", 13, value, value_size, error )

/* Sets the UTF-8 encoded header value specified by the identifier
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_set_utf8_header_value(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libewf_error_t **error );

/* Sets the header value case number
 * Returns 1 if successful or -1 on error
 */
#define libewf_handle_set_utf8_header_value_case_number( handle, value, value_length, error ) \
        libewf_handle_set_utf8_header_value( handle, (uint8_t *) "case_number", 11, value, value_length, error )

/* Sets the header value description
 * Returns 1 if successful or -1 on error
 */
#define libewf_handle_set_utf8_header_value_description( handle, value, value_length, error ) \
        libewf_handle_set_utf8_header_value( handle, (uint8_t *) "description", 11, value, value_length, error )

/* Sets the header value examiner name
 * Returns 1 if successful or -1 on error
 */
#define libewf_handle_set_utf8_header_value_examiner_name( handle, value, value_length, error ) \
        libewf_handle_set_utf8_header_value( handle, (uint8_t *) "examiner_name", 13, value, value_length, error )

/* Sets the header value evidence number
 * Returns 1 if successful or -1 on error
 */
#define libewf_handle_set_utf8_header_value_evidence_number( handle, value, value_length, error ) \
        libewf_handle_set_utf8_header_value( handle, (uint8_t *) "evidence_number", 15, value, value_length, error )

/* Sets the header value notes
 * Returns 1 if successful or -1 on error
 */
#define libewf_handle_set_utf8_header_value_notes( handle, value, value_length, error ) \
        libewf_handle_set_utf8_header_value( handle, (uint8_t *) "notes", 5, value, value_length, error )

/* Sets the header value acquiry date
 * Returns 1 if successful or -1 on error
 */
#define libewf_handle_set_utf8_header_value_acquiry_date( handle, value, value_length, error ) \
        libewf_handle_set_utf8_header_value( handle, (uint8_t *) "acquiry_date", 12, value, value_length, error )

/* Sets the header value system date
 * Returns 1 if successful or -1 on error
 */
#define libewf_handle_set_utf8_header_value_system_date( handle, value, value_length, error ) \
        libewf_handle_set_utf8_header_value( handle, (uint8_t *) "system_date", 11, value, value_length, error )

/* Sets the header value acquiry operating system
 * Returns 1 if successful or -1 on error
 */
#define libewf_handle_set_utf8_header_value_acquiry_operating_system( handle, value, value_length, error ) \
        libewf_handle_set_utf8_header_value( handle, (uint8_t *) "acquiry_operating_system", 24, value, value_length, error )

/* Sets the header value acquiry software version
 * Returns 1 if successful or -1 on error
 */
#define libewf_handle_set_utf8_header_value_acquiry_software_version( handle, value, value_length, error ) \
        libewf_handle_set_utf8_header_value( handle, (uint8_t *) "acquiry_software_version", 24, value, value_length, error )

/* Sets the header value password
 * Returns 1 if successful or -1 on error
 */
#define libewf_handle_set_utf8_header_value_password( handle, value, value_length, error ) \
        libewf_handle_set_utf8_header_value( handle, (uint8_t *) "password", 8, value, value_length, error )

/* Sets the header value compression level
 * Returns 1 if successful or -1 on error
 */
#define libewf_handle_set_utf8_header_value_compression_level( handle, value, value_length, error ) \
        libewf_handle_set_utf8_header_value( handle, (uint8_t *) "compression_level", 17, value, value_length, error )

/* Sets the header value model
 * Returns 1 if successful or -1 on error
 */
#define libewf_handle_set_utf8_header_value_model( handle, value, value_length, error ) \
        libewf_handle_set_utf8_header_value( handle, (uint8_t *) "model", 5, value, value_length, error )

/* Sets the header value serial number
 * Returns 1 if successful or -1 on error
 */
#define libewf_handle_set_utf8_header_value_serial_number( handle, value, value_length, error ) \
        libewf_handle_set_utf8_header_value( handle, (uint8_t *) "serial_number", 13, value, value_length, error )

/* Retrieves the size of the UTF-16 encoded header value of an identifier
 * The string size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_utf16_header_value_size(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *utf16_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-16 encoded header value of an identifier
 * The string size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_utf16_header_value(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libewf_error_t **error );

/* Sets the UTF-16 encoded header value specified by the identifier
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_set_utf16_header_value(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libewf_error_t **error );

/* Copies the header values from the source to the destination handle
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_copy_header_values(
     libewf_handle_t *destination_handle,
     libewf_handle_t *source_handle,
     libewf_error_t **error );

/* Retrieves the number of hash values
 * Returns 1 if successful, 0 if no hash values are present or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_number_of_hash_values(
     libewf_handle_t *handle,
     uint32_t *number_of_values,
     libewf_error_t **error );

/* Retrieves the size of the hash value identifier of a specific index
 * The identifier size includes the end of string character
 * Returns 1 if successful, 0 if no hash values are present or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_hash_value_identifier_size(
     libewf_handle_t *handle,
     uint32_t index,
     size_t *identifier_size,
     libewf_error_t **error );

/* Retrieves the hash value identifier of a specific index
 * The identifier size should include the end of string character
 * Returns 1 if successful, 0 if no hash values are present or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_hash_value_identifier(
     libewf_handle_t *handle,
     uint32_t index,
     uint8_t *identifier,
     size_t identifier_size,
     libewf_error_t **error );

/* Retrieves the size of the UTF-8 encoded hash value of an identifier
 * The string size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_utf8_hash_value_size(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *utf8_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-8 encoded hash value of an identifier
 * The string size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_utf8_hash_value(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libewf_error_t **error );

/* Retrieves the hash value MD5
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
#define libewf_handle_get_utf8_hash_value_md5( handle, value, value_size, error ) \
        libewf_handle_get_utf8_hash_value( handle, (uint8_t *) "MD5", 3, value, value_size, error )

/* Retrieves the hash value SHA1
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
#define libewf_handle_get_utf8_hash_value_sha1( handle, value, value_size, error ) \
        libewf_handle_get_utf8_hash_value( handle, (uint8_t *) "SHA1", 4, value, value_size, error )

/* Sets the UTF-8 hash value specified by the identifier
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_set_utf8_hash_value(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libewf_error_t **error );

/* Sets the hash value MD5
 * Returns 1 if successful or -1 on error
 */
#define libewf_handle_set_utf8_hash_value_md5( handle, value, value_length, error ) \
        libewf_handle_set_utf8_hash_value( handle, (uint8_t *) "MD5", 3, value, value_length, error )

/* Sets the hash value SHA1
 * Returns 1 if successful or -1 on error
 */
#define libewf_handle_set_utf8_hash_value_sha1( handle, value, value_length, error ) \
        libewf_handle_set_utf8_hash_value( handle, (uint8_t *) "SHA1", 4, value, value_length, error )

/* Retrieves the size of the UTF-16 encoded hash value of an identifier
 * The string size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_utf16_hash_value_size(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *utf16_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-16 encoded hash value of an identifier
 * The string size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_utf16_hash_value(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libewf_error_t **error );

/* Sets the UTF-16 hash value specified by the identifier
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_set_utf16_hash_value(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libewf_error_t **error );

/* Retrieves the root (single) file entry
 * Returns 1 if successful, 0 if no file entries are present or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_get_root_file_entry(
     libewf_handle_t *handle,
     libewf_file_entry_t **root_file_entry,
     libewf_error_t **error );

/* Retrieves the (single) file entry for the specific UTF-8 encoded path
 * The path separator is the \ character
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_get_file_entry_by_utf8_path(
     libewf_handle_t *handle,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libewf_file_entry_t **file_entry,
     libewf_error_t **error );

/* Retrieves the (single) file entry for the specific UTF-16 encoded path
 * The path separator is the \ character
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_get_file_entry_by_utf16_path(
     libewf_handle_t *handle,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libewf_file_entry_t **file_entry,
     libewf_error_t **error );

/* -------------------------------------------------------------------------
 * Meta data functions - deprecated
 * ------------------------------------------------------------------------- */

/* Sets the read wipe chunk on error
 * The chunk is not wiped if read raw is used
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_set_read_zero_chunk_on_error instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN \
int libewf_handle_set_read_wipe_chunk_on_error(
     libewf_handle_t *handle,
     uint8_t wipe_on_error,
     libewf_error_t **error );

/* Retrieves the amount of acquiry errors
 * Returns 1 if successful or -1 on error
 *
 * This function deprecated use libewf_handle_get_number_of_acquiry_errors instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN \
int libewf_handle_get_amount_of_acquiry_errors(
     libewf_handle_t *handle,
     uint32_t *amount_of_errors,
     libewf_error_t **error );

/* Add an acquiry error
 * Returns 1 if successful or -1 on error
 *
 * This function deprecated use libewf_handle_append_acquiry_error instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN \
int libewf_handle_add_acquiry_error(
     libewf_handle_t *handle,
     uint64_t start_sector,
     uint64_t number_of_sectors,
     libewf_error_t **error );

/* Retrieves the number of checksum errors
 * Returns 1 if successful or -1 on error
 *
 * This function deprecated use libewf_handle_get_number_of_checksum_errors instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN \
int libewf_handle_get_number_of_crc_errors(
     libewf_handle_t *handle,
     uint32_t *number_of_errors,
     libewf_error_t **error );

/* Retrieves the amount of checksum errors
 * Returns 1 if successful or -1 on error
 *
 * This function deprecated use libewf_handle_get_number_of_checksum_errors instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN \
int libewf_handle_get_amount_of_crc_errors(
     libewf_handle_t *handle,
     uint32_t *amount_of_errors,
     libewf_error_t **error );

/* Retrieves a checksum error
 * Returns 1 if successful or -1 on error
 *
 * This function deprecated use libewf_handle_get_checksum_error instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN \
int libewf_handle_get_crc_error(
     libewf_handle_t *handle,
     uint32_t index,
     uint64_t *start_sector,
     uint64_t *number_of_sectors,
     libewf_error_t **error );

/* Add a checksum error
 * Returns 1 if successful or -1 on error
 *
 * This function deprecated use libewf_handle_append_checksum_error instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN \
int libewf_handle_add_crc_error(
     libewf_handle_t *handle,
     uint64_t start_sector,
     uint64_t number_of_sectors,
     libewf_error_t **error );

/* Retrieves the amount of sessions
 * Returns 1 if successful or -1 on error
 *
 * This function deprecated use libewf_handle_get_number_of_sessions instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN \
int libewf_handle_get_amount_of_sessions(
     libewf_handle_t *handle,
     uint32_t *amount_of_sessions,
     libewf_error_t **error );

/* Add a session
 * Returns 1 if successful or -1 on error
 *
 * This function deprecated use libewf_handle_append_session instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN \
int libewf_handle_add_session(
     libewf_handle_t *handle,
     uint64_t start_sector,
     uint64_t number_of_sectors,
     libewf_error_t **error );

/* Retrieves the amount of header values
 * Returns 1 if successful, 0 if no header values are present or -1 on error
 *
 * This function deprecated use libewf_handle_get_number_of_header_values instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN \
int libewf_handle_get_amount_of_header_values(
     libewf_handle_t *handle,
     uint32_t *amount_of_values,
     libewf_error_t **error );

/* Retrieves the size of the UTF-8 encoded header value of an identifier
 * The value size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 *
 * This function deprecated use libewf_handle_get_utf8_header_value_size instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN \
int libewf_handle_get_header_value_size(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *value_size,
     libewf_error_t **error );

/* Retrieves the UTF-8 encoded header value of an identifier
 * The value size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 *
 * This function deprecated use libewf_handle_get_utf8_header_value instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN \
int libewf_handle_get_header_value(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint8_t *value,
     size_t value_size,
     libewf_error_t **error );

/* Retrieves the header value case number
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
#define libewf_handle_get_header_value_case_number( handle, value, value_size, error ) \
        libewf_handle_get_header_value( handle, (uint8_t *) "case_number", 11, value, value_size, error )

/* Retrieves the header value description
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
#define libewf_handle_get_header_value_description( handle, value, value_size, error ) \
        libewf_handle_get_header_value( handle, (uint8_t *) "description", 11, value, value_size, error )

/* Retrieves the header value examiner name
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
#define libewf_handle_get_header_value_examiner_name( handle, value, value_size, error ) \
        libewf_handle_get_header_value( handle, (uint8_t *) "examiner_name", 13, value, value_size, error )

/* Retrieves the header value evidence number
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
#define libewf_handle_get_header_value_evidence_number( handle, value, value_size, error ) \
        libewf_handle_get_header_value( handle, (uint8_t *) "evidence_number", 15, value, value_size, error )

/* Retrieves the header value notes
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
#define libewf_handle_get_header_value_notes( handle, value, value_size, error ) \
        libewf_handle_get_header_value( handle, (uint8_t *) "notes", 5, value, value_size, error )

/* Retrieves the header value acquiry date
 * The string is encoded according to the date format
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
#define libewf_handle_get_header_value_acquiry_date( handle, value, value_size, error ) \
        libewf_handle_get_header_value( handle, (uint8_t *) "acquiry_date", 12, value, value_size, error )

/* Retrieves the header value system date
 * The string is encoded according to the date format
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
#define libewf_handle_get_header_value_system_date( handle, value, value_size, error ) \
        libewf_handle_get_header_value( handle, (uint8_t *) "system_date", 11, value, value_size, error )

/* Retrieves the header value acquiry operating system
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
#define libewf_handle_get_header_value_acquiry_operating_system( handle, value, value_size, error ) \
        libewf_handle_get_header_value( handle, (uint8_t *) "acquiry_operating_system", 24, value, value_size, error )

/* Retrieves the header value acquiry software version
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
#define libewf_handle_get_header_value_acquiry_software_version( handle, value, value_size, error ) \
        libewf_handle_get_header_value( handle, (uint8_t *) "acquiry_software_version", 24, value, value_size, error )

/* Retrieves the header value password
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
#define libewf_handle_get_header_value_password( handle, value, value_size, error ) \
        libewf_handle_get_header_value( handle, (uint8_t *) "password", 8, value, value_size, error )

/* Retrieves the header value compression type
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
#define libewf_handle_get_header_value_compression_type( handle, value, value_size, error ) \
        libewf_handle_get_header_value( handle, (uint8_t *) "compression_type", 16, value, value_size, error )

/* Retrieves the header value model
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
#define libewf_handle_get_header_value_model( handle, value, value_size, error ) \
        libewf_handle_get_header_value( handle, (uint8_t *) "model", 5, value, value_size, error )

/* Retrieves the header value serial number
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
#define libewf_handle_get_header_value_serial_number( handle, value, value_size, error ) \
        libewf_handle_get_header_value( handle, (uint8_t *) "serial_number", 13, value, value_size, error )

/* Retrieves the header value compression type
 * Returns 1 if successful, 0 if value not present or -1 on error
 *
 * This function deprecated use libewf_handle_get_utf8_header_value_compression_level instead
 */
#define libewf_handle_get_utf8_header_value_compression_type( handle, value, value_size, error ) \
        libewf_handle_get_utf8_header_value( handle, (uint8_t *) "compression_type", 16, value, value_size, error )

/* Sets the UTF-8 encoded header value specified by the identifier
 * Returns 1 if successful or -1 on error
 *
 * This function deprecated use libewf_handle_set_utf8_header_value instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN \
int libewf_handle_set_header_value(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint8_t *value,
     size_t value_length,
     libewf_error_t **error );

/* Sets the header value case number
 * Returns 1 if successful or -1 on error
 */
#define libewf_handle_set_header_value_case_number( handle, value, value_length, error ) \
        libewf_handle_set_header_value( handle, (uint8_t *) "case_number", 11, value, value_length, error )

/* Sets the header value description
 * Returns 1 if successful or -1 on error
 */
#define libewf_handle_set_header_value_description( handle, value, value_length, error ) \
        libewf_handle_set_header_value( handle, (uint8_t *) "description", 11, value, value_length, error )

/* Sets the header value examiner name
 * Returns 1 if successful or -1 on error
 */
#define libewf_handle_set_header_value_examiner_name( handle, value, value_length, error ) \
        libewf_handle_set_header_value( handle, (uint8_t *) "examiner_name", 13, value, value_length, error )

/* Sets the header value evidence number
 * Returns 1 if successful or -1 on error
 */
#define libewf_handle_set_header_value_evidence_number( handle, value, value_length, error ) \
        libewf_handle_set_header_value( handle, (uint8_t *) "evidence_number", 15, value, value_length, error )

/* Sets the header value notes
 * Returns 1 if successful or -1 on error
 */
#define libewf_handle_set_header_value_notes( handle, value, value_length, error ) \
        libewf_handle_set_header_value( handle, (uint8_t *) "notes", 5, value, value_length, error )

/* Sets the header value acquiry date
 * Returns 1 if successful or -1 on error
 */
#define libewf_handle_set_header_value_acquiry_date( handle, value, value_length, error ) \
        libewf_handle_set_header_value( handle, (uint8_t *) "acquiry_date", 12, value, value_length, error )

/* Sets the header value system date
 * Returns 1 if successful or -1 on error
 */
#define libewf_handle_set_header_value_system_date( handle, value, value_length, error ) \
        libewf_handle_set_header_value( handle, (uint8_t *) "system_date", 11, value, value_length, error )

/* Sets the header value acquiry operating system
 * Returns 1 if successful or -1 on error
 */
#define libewf_handle_set_header_value_acquiry_operating_system( handle, value, value_length, error ) \
        libewf_handle_set_header_value( handle, (uint8_t *) "acquiry_operating_system", 24, value, value_length, error )

/* Sets the header value acquiry software version
 * Returns 1 if successful or -1 on error
 */
#define libewf_handle_set_header_value_acquiry_software_version( handle, value, value_length, error ) \
        libewf_handle_set_header_value( handle, (uint8_t *) "acquiry_software_version", 24, value, value_length, error )

/* Sets the header value password
 * Returns 1 if successful or -1 on error
 */
#define libewf_handle_set_header_value_password( handle, value, value_length, error ) \
        libewf_handle_set_header_value( handle, (uint8_t *) "password", 8, value, value_length, error )

/* Sets the header value compression type
 * Returns 1 if successful or -1 on error
 */
#define libewf_handle_set_header_value_compression_type( handle, value, value_length, error ) \
        libewf_handle_set_header_value( handle, (uint8_t *) "compression_type", 16, value, value_length, error )

/* Sets the header value model
 * Returns 1 if successful or -1 on error
 */
#define libewf_handle_set_header_value_model( handle, value, value_length, error ) \
        libewf_handle_set_header_value( handle, (uint8_t *) "model", 5, value, value_length, error )

/* Sets the header value serial number
 * Returns 1 if successful or -1 on error
 */
#define libewf_handle_set_header_value_serial_number( handle, value, value_length, error ) \
        libewf_handle_set_header_value( handle, (uint8_t *) "serial_number", 13, value, value_length, error )

/* Sets the header value compression type
 * Returns 1 if successful or -1 on error
 *
 * This function deprecated use libewf_handle_set_utf8_header_value_compression_level instead
 */
#define libewf_handle_set_utf8_header_value_compression_type( handle, value, value_length, error ) \
        libewf_handle_set_utf8_header_value( handle, (uint8_t *) "compression_type", 16, value, value_length, error )

/* Retrieves the amount of hash values
 * Returns 1 if successful, 0 if no hash values are present or -1 on error
 *
 * This function deprecated use libewf_handle_get_number_of_hash_values instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN \
int libewf_handle_get_amount_of_hash_values(
     libewf_handle_t *handle,
     uint32_t *amount_of_values,
     libewf_error_t **error );

/* Retrieves the size of the UTF-8 encoded hash value of an identifier
 * The value size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 *
 * This function deprecated use libewf_handle_get_utf8_hash_value_size instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN \
int libewf_handle_get_hash_value_size(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *value_size,
     libewf_error_t **error );

/* Retrieves the UTF-8 encoded hash value of an identifier
 * The value size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 *
 * This function deprecated use libewf_handle_get_utf8_hash_value instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN \
int libewf_handle_get_hash_value(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint8_t *value,
     size_t value_size,
     libewf_error_t **error );

/* Retrieves the hash value MD5
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
#define libewf_handle_get_hash_value_md5( handle, value, value_size, error ) \
        libewf_handle_get_hash_value( handle, "MD5", 3, value, value_size, error )

/* Retrieves the hash value SHA1
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
#define libewf_handle_get_hash_value_sha1( handle, value, value_size, error ) \
        libewf_handle_get_hash_value( handle, "SHA1", 4, value, value_size, error )

/* Sets the UTF-8 hash value specified by the identifier
 * Returns 1 if successful or -1 on error
 *
 * This function deprecated use libewf_handle_set_utf8_hash_value instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN \
int libewf_handle_set_hash_value(
     libewf_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint8_t *value,
     size_t value_length,
     libewf_error_t **error );

/* Sets the hash value MD5
 * Returns 1 if successful or -1 on error
 */
#define libewf_handle_set_hash_value_md5( handle, value, value_length, error ) \
        libewf_handle_set_hash_value( handle, "MD5", 3, value, value_length, error )

/* Sets the hash value SHA1
 * Returns 1 if successful or -1 on error
 */
#define libewf_handle_set_hash_value_sha1( handle, value, value_length, error ) \
        libewf_handle_set_hash_value( handle, "SHA1", 4, value, value_length, error )

/* -------------------------------------------------------------------------
 * Single file entry functions
 * ------------------------------------------------------------------------- */

/* Frees a file entry
 * REturns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_free(
     libewf_file_entry_t **file_entry,
     libewf_error_t **error );

/* Retrieves the type
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_type(
     libewf_file_entry_t *file_entry,
     uint8_t *type,
     libewf_error_t **error );

/* Retrieves the flags
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_flags(
     libewf_file_entry_t *file_entry,
     uint32_t *flags,
     libewf_error_t **error );

/* Retrieves the media data offset
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_media_data_offset(
     libewf_file_entry_t *file_entry,
     off64_t *media_data_offset,
     libewf_error_t **error );

/* Retrieves the media data size
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_media_data_size(
     libewf_file_entry_t *file_entry,
     size64_t *media_data_size,
     libewf_error_t **error );

/* Retrieves the duplicate media data offset
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_duplicate_media_data_offset(
     libewf_file_entry_t *file_entry,
     off64_t *duplicate_media_data_offset,
     libewf_error_t **error );

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_utf8_name_size(
     libewf_file_entry_t *file_entry,
     size_t *utf8_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-8 encoded name value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_utf8_name(
     libewf_file_entry_t *file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libewf_error_t **error );

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_utf16_name_size(
     libewf_file_entry_t *file_entry,
     size_t *utf16_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-16 encoded name value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_utf16_name(
     libewf_file_entry_t *file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libewf_error_t **error );

/* Retrieves the size
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_size(
     libewf_file_entry_t *file_entry,
     size64_t *size,
     libewf_error_t **error );

/* Retrieves the creation date and time
 * The date and time is formatted as a POSIX timestamp
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_creation_time(
     libewf_file_entry_t *file_entry,
     uint32_t *creation_time,
     libewf_error_t **error );

/* Retrieves the modification date and time
 * The date and time is formatted as a POSIX timestamp
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_modification_time(
     libewf_file_entry_t *file_entry,
     uint32_t *modification_time,
     libewf_error_t **error );

/* Retrieves the access date and time
 * The date and time is formatted as a POSIX timestamp
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_access_time(
     libewf_file_entry_t *file_entry,
     uint32_t *access_time,
     libewf_error_t **error );

/* Retrieves the entry modification date and time
 * The date and time is formatted as a POSIX timestamp
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_entry_modification_time(
     libewf_file_entry_t *file_entry,
     uint32_t *entry_modification_time,
     libewf_error_t **error );

/* Retrieves the UTF-8 encoded MD5 hash value
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_utf8_hash_value_md5(
     libewf_file_entry_t *file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libewf_error_t **error );

/* Retrieves the UTF-16 encoded MD5 hash value
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_utf16_hash_value_md5(
     libewf_file_entry_t *file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libewf_error_t **error );

/* Reads data at the current offset
 * Returns the number of bytes read or -1 on error
 */
LIBEWF_EXTERN \
ssize_t libewf_file_entry_read_buffer(
         libewf_file_entry_t *file_entry,
         void *buffer,
         size_t buffer_size,
         libewf_error_t **error );

/* Reads data at a specific offset
 * Returns the number of bytes read or -1 on error
 */
LIBEWF_EXTERN \
ssize_t libewf_file_entry_read_random(
         libewf_file_entry_t *file_entry,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libewf_error_t **error );

/* Seeks a certain offset of the data
 * Returns the offset if seek is successful or -1 on error
 */
LIBEWF_EXTERN \
off64_t libewf_file_entry_seek_offset(
         libewf_file_entry_t *file_entry,
         off64_t offset,
         int whence,
         libewf_error_t **error );

/* Retrieves the current offset of the data
 * Returns the offset if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_offset(
     libewf_file_entry_t *file_entry,
     off64_t *offset,
     libewf_error_t **error );

/* Retrieves the number of sub file entries
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_number_of_sub_file_entries(
     libewf_file_entry_t *file_entry,
     int *number_of_sub_file_entries,
     libewf_error_t **error );

/* Retrieves the sub file entry for the specific index
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_sub_file_entry(
     libewf_file_entry_t *file_entry,
     int sub_file_entry_index,
     libewf_file_entry_t **sub_file_entry,
     libewf_error_t **error );

/* Retrieves the sub file entry for the specific UTF-8 encoded name
 * Returns 1 if successful, 0 if no such sub file entry or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_sub_file_entry_by_utf8_name(
     libewf_file_entry_t *file_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libewf_file_entry_t **sub_file_entry,
     libewf_error_t **error );

/* Retrieves the sub file entry for the specific UTF-8 encoded path
 * The path separator is the \ character
 * Returns 1 if successful, 0 if no such sub file entry or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_sub_file_entry_by_utf8_path(
     libewf_file_entry_t *file_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libewf_file_entry_t **sub_file_entry,
     libewf_error_t **error );

/* Retrieves the sub file entry for the specific UTF-16 encoded name
 * Returns 1 if successful, 0 if no such sub file entry or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_sub_file_entry_by_utf16_name(
     libewf_file_entry_t *file_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libewf_file_entry_t **sub_file_entry,
     libewf_error_t **error );

/* Retrieves the sub file entry for the specific UTF-16 encoded path
 * The path separator is the \ character
 * Returns 1 if successful, 0 if no such sub file entry or -1 on error
 */
LIBEWF_EXTERN \
int libewf_file_entry_get_sub_file_entry_by_utf16_path(
     libewf_file_entry_t *file_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libewf_file_entry_t **sub_file_entry,
     libewf_error_t **error );

/* -------------------------------------------------------------------------
 * Single file entry functions - deprecated
 * ------------------------------------------------------------------------- */

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 *
 * This function deprecated use libewf_file_entry_get_utf8_name_size instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN \
int libewf_file_entry_get_name_size(
     libewf_file_entry_t *file_entry,
     size_t *name_size,
     libewf_error_t **error );

/* Retrieves the UTF-8 encoded name value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 *
 * This function deprecated use libewf_file_entry_get_utf8_name instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN \
int libewf_file_entry_get_name(
     libewf_file_entry_t *file_entry,
     uint8_t *name,
     size_t name_size,
     libewf_error_t **error );

/* Retrieves the amount of sub file entries
 * Returns 1 if successful or -1 on error
 *
 * This function deprecated use libewf_file_entry_get_number_of_sub_file_entries instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN \
int libewf_file_entry_get_amount_of_sub_file_entries(
     libewf_file_entry_t *file_entry,
     int *amount_of_sub_file_entries,
     libewf_error_t **error );

/* -------------------------------------------------------------------------
 * APIv1 functions - deprecated
 * ------------------------------------------------------------------------- */

#if 0 || defined( HAVE_V1_API )

/* -------------------------------------------------------------------------
 * Notify functions
 * ------------------------------------------------------------------------- */

/* Sets the notify values
 *
 * This function is deprecated use libewf_notify_set_verbose and libewf_notify_set_stream instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN void libewf_set_notify_values(
                    FILE *stream,
                    uint8_t verbose );

/* -------------------------------------------------------------------------
 * File/Handle functions
 * ------------------------------------------------------------------------- */

/* Signals the libewf handle to abort its current activity
 * Returns 1 if successful or -1 on error
 *
 * This function deprecated use libewf_handle_signal_abort instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_signal_abort(
                   libewf_handle_t *handle );

/* Opens a set of EWF file(s)
 * For reading files should contain all filenames that make up an EWF image
 * For writing files should contain the base of the filename, extentions like .e01 will be automatically added
 * Returns a pointer to the new instance of handle, NULL on error
 *
 * This function deprecated use libewf_handle_open instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN libewf_handle_t *libewf_open(
                                char * const filenames[],
                                int amount_of_filenames,
                                uint8_t flags );

#if defined( LIBEWF_HAVE_WIDE_CHARACTER_TYPE )
/* Opens a set of EWF file(s)
 * For reading files should contain all filenames that make up an EWF image
 * For writing files should contain the base of the filename, extentions like .e01 will be automatically added
 * Returns a pointer to the new instance of handle, NULL on error
 *
 * This function deprecated use libewf_handle_open_wide instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN libewf_handle_t *libewf_open_wide(
                                wchar_t * const filenames[],
                                int amount_of_filenames,
                                uint8_t flags );
#endif

/* Closes the EWF handle and frees memory used within the handle
 * Returns 0 if successful or -1 on error
 *
 * This function deprecated use libewf_handle_close instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_close(
                   libewf_handle_t *handle );

/* Seeks a certain offset of the (media) data
 * Returns the offset if seek is successful or -1 on error
 *
 * This function is deprecated use libewf_handle_seek_offset instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN off64_t libewf_seek_offset(
                       libewf_handle_t *handle,
                       off64_t offset );

/* Retrieves the current offset of the (media) data
 * Returns the offset if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_get_offset instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN off64_t libewf_get_offset(
                       libewf_handle_t *handle );

/* Prepares a buffer with chunk data after reading it according to the handle settings
 * intended for raw read
 * The buffer size cannot be larger than the chunk size
 * Returns the resulting chunk size or -1 on error
 *
 * This function is deprecated use libewf_handle_prepare_read_chunk instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN ssize_t libewf_raw_read_prepare_buffer(
                       libewf_handle_t *handle,
                       void *buffer,
                       size_t buffer_size,
                       void *uncompressed_buffer,
                       size_t *uncompressed_buffer_size,
                       int8_t is_compressed,
                       uint32_t chunk_checksum,
                       int8_t read_checksum );

/* Reads 'raw' data from the current offset into a buffer
 * size contains the size of the buffer
 * The function sets the chunk checksum, is compressed and read checksum values
 * Returns the amount of bytes read or -1 on error
 *
 * This function is deprecated use libewf_handle_read_chunk instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN ssize_t libewf_raw_read_buffer(
                       libewf_handle_t *handle,
                       void *buffer,
                       size_t buffer_size,
                       int8_t *is_compressed,
                       uint32_t *chunk_checksum,
                       int8_t *read_checksum );

/* Reads data from the current offset into a buffer
 * Returns the amount of bytes read or -1 on error
 *
 * This function is deprecated use libewf_handle_read_buffer instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN ssize_t libewf_read_buffer(
                       libewf_handle_t *handle,
                       void *buffer,
                       size_t buffer_size );

/* Reads (media) data at a specific offset
 * Returns the amount of bytes read or -1 on error
 *
 * This function is deprecated use libewf_handle_read_random instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN ssize_t libewf_read_random(
                       libewf_handle_t *handle,
                       void *buffer,
                       size_t buffer_size,
                       off64_t offset );

/* Prepares a buffer with chunk data before writing according to the handle settings
 * intended for raw write
 * The buffer size cannot be larger than the chunk size
 * The function sets the chunk checksum, is compressed and write checksum values
 * Returns the resulting chunk size or -1 on error
 *
 * This function is deprecated use libewf_handle_prepare_write_chunk instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN ssize_t libewf_raw_write_prepare_buffer(
                       libewf_handle_t *handle,
                       void *buffer,
                       size_t buffer_size,
                       void *compressed_buffer,
                       size_t *compressed_buffer_size,
                       int8_t *is_compressed,
                       uint32_t *chunk_checksum,
                       int8_t *write_checksum );

/* Writes 'raw' data in EWF format at the current offset
 * the necessary settings of the write values must have been made
 * size contains the size of the data within the buffer while
 * data size contains the size of the actual input data
 * Will initialize write if necessary
 * Returns the amount of input bytes written, 0 when no longer bytes can be written or -1 on error
 *
 * This function is deprecated use libewf_handle_write_chunk instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN ssize_t libewf_raw_write_buffer(
                       libewf_handle_t *handle,
                       void *buffer,
                       size_t buffer_size,
                       size_t data_size,
                       int8_t is_compressed,
                       uint32_t chunk_checksum,
                       int8_t write_checksum );

/* Writes data in EWF format at the current offset
 * the necessary settings of the write values must have been made
 * Will initialize write if necessary
 * Returns the amount of input bytes written, 0 when no longer bytes can be written or -1 on error
 *
 * This function is deprecated use libewf_handle_write_buffer instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN ssize_t libewf_write_buffer(
                       libewf_handle_t *handle,
                       void *buffer,
                       size_t buffer_size );

/* Writes data in EWF format at a specific offset,
 * the necessary settings of the write values must have been made
 * Will initialize write if necessary
 * Returns the amount of input bytes written, 0 when no longer bytes can be written or -1 on error
 *
 * This function is deprecated use libewf_handle_write_random instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN ssize_t libewf_write_random(
                       libewf_handle_t *handle,
                       void *buffer,
                       size_t buffer_size,
                       off64_t offset );

/* Finalizes the write by correcting the EWF the meta data in the segment files
 * This function is required after writing from stream
 * Returns the amount of input bytes written or -1 on error
 *
 * This function is deprecated use libewf_handle_write_finalize instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN ssize_t libewf_write_finalize(
                       libewf_handle_t *handle );

/* Retrieves the segment filename
 * The filename size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 *
 * This function is deprecated use libewf_handle_get_segment_filename instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_get_segment_filename(
                   libewf_handle_t *handle,
                   char *filename,
                   size_t filename_size );

/* Sets the segment filename
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_set_segment_filename instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_set_segment_filename(
                   libewf_handle_t *handle,
                   const char *filename,
                   size_t filename_length );

#if defined( LIBEWF_HAVE_WIDE_CHARACTER_TYPE )
/* Retrieves the segment filename
 * The filename size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 *
 * This function is deprecated use libewf_handle_get_segment_filename_wide instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_get_segment_filename_wide(
                   libewf_handle_t *handle,
                   wchar_t *filename,
                   size_t filename_size );

/* Sets the segment filename
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_set_segment_filename_wide instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_set_segment_filename_wide(
                   libewf_handle_t *handle,
                   const wchar_t *filename,
                   size_t filename_length );
#endif

/* Retrieves the segment file size
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_get_maximum_segment_size instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_get_segment_file_size(
                   libewf_handle_t *handle,
                   size64_t *segment_file_size );

/* Sets the segment file size
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_set_maximum_segment_size instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_set_segment_file_size(
                   libewf_handle_t *handle,
                   size64_t segment_file_size );

/* Retrieves the delta segment filename
 * The filename size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 *
 * This function is deprecated use libewf_handle_get_delta_segment_filename instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_get_delta_segment_filename(
                   libewf_handle_t *handle,
                   char *filename,
                   size_t filename_size );

/* Sets the delta segment filename
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_set_delta_segment_filename instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_set_delta_segment_filename(
                   libewf_handle_t *handle,
                   const char *filename,
                   size_t filename_length );

#if defined( LIBEWF_HAVE_WIDE_CHARACTER_TYPE )
/* Retrieves the delta segment filename
 * The filename size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 *
 * This function is deprecated use libewf_handle_get_delta_segment_filename_wide instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_get_delta_segment_filename_wide(
                   libewf_handle_t *handle,
                   wchar_t *filename,
                   size_t filename_size );

/* Sets the delta segment filename
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_set_delta_segment_filename_wide instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_set_delta_segment_filename_wide(
                   libewf_handle_t *handle,
                   const wchar_t *filename,
                   size_t filename_length );
#endif

/* Retrieves the delta segment file size
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_get_maximum_delta_segment_size instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_get_delta_segment_file_size(
                   libewf_handle_t *handle,
                   size64_t *delta_segment_file_size );

/* Sets the delta segment file size
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_set_maximum_delta_segment_size instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_set_delta_segment_file_size(
                   libewf_handle_t *handle,
                   size64_t delta_segment_file_size );

/* -------------------------------------------------------------------------
 * Meta data functions
 * ------------------------------------------------------------------------- */

/* Retrieves the amount of sectors per chunk
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_get_sectors_per_chunk instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_get_sectors_per_chunk(
                   libewf_handle_t *handle,
                   uint32_t *sectors_per_chunk );

/* Sets the amount of sectors per chunk
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_set_sectors_per_chunk instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_set_sectors_per_chunk(
                   libewf_handle_t *handle,
                   uint32_t sectors_per_chunk );

/* Retrieves the amount of bytes per sector
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_get_bytes_per_sector instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_get_bytes_per_sector(
                   libewf_handle_t *handle,
                   uint32_t *bytes_per_sector );

/* Sets the amount of bytes per sector
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_set_bytes_per_sector instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_set_bytes_per_sector(
                   libewf_handle_t *handle,
                   uint32_t bytes_per_sector );

/* Retrieves the amount of sectors
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_get_number_of_sectors instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_get_amount_of_sectors(
                   libewf_handle_t *handle,
                   uint32_t *amount_of_sectors );

/* Retrieves the chunk size
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_get_chunk_size instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_get_chunk_size(
                   libewf_handle_t *handle,
                   size32_t *chunk_size );

/* Retrieves the error granularity
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_get_error_granularity instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_get_error_granularity(
                   libewf_handle_t *handle,
                   uint32_t *error_granularity );

/* Sets the error granularity
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_set_error_granularity instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_set_error_granularity(
                   libewf_handle_t *handle,
                   uint32_t error_granularity );

/* Retrieves the compression values
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_get_compression_values instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_get_compression_values(
                   libewf_handle_t *handle,
                   int8_t *compression_level,
                   uint8_t *compress_empty_block );

/* Sets the compression values
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_set_compression_values instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_set_compression_values(
                   libewf_handle_t *handle,
                   int8_t compression_level,
                   uint8_t compress_empty_block );

/* Retrieves the size of the contained (media) data
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_get_media_size instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_get_media_size(
                   libewf_handle_t *handle,
                   size64_t *media_size );

/* Sets the media size
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_set_media_size instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_set_media_size(
                   libewf_handle_t *handle,
                   size64_t media_size );

/* Retrieves the media type value
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_get_media_type instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_get_media_type(
                   libewf_handle_t *handle,
                   uint8_t *media_type );

/* Sets the media type
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_set_media_type instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_set_media_type(
                   libewf_handle_t *handle,
                   uint8_t media_type );

/* Retrieves the media flags
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_get_media_flags instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_get_media_flags(
                   libewf_handle_t *handle,
                   uint8_t *media_flags );

/* Sets the media flags
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_set_media_flags instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_set_media_flags(
                   libewf_handle_t *handle,
                   uint8_t media_flags );

/* Retrieves the volume type value
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_get_media_flags instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_get_volume_type(
                   libewf_handle_t *handle,
                   uint8_t *volume_type );

/* Sets the volume type
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_set_media_flags instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_set_volume_type(
                   libewf_handle_t *handle,
                   uint8_t volume_type );

/* Retrieves the format type value
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_get_format instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_get_format(
                   libewf_handle_t *handle,
                   uint8_t *format );

/* Sets the output format
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_set_format instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_set_format(
                   libewf_handle_t *handle,
                   uint8_t format );

/* Retrieves the GUID
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_get_guid instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_get_guid(
                   libewf_handle_t *handle,
                   uint8_t *guid,
                   size_t size );

/* Sets the GUID
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_set_guid instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_set_guid(
                   libewf_handle_t *handle,
                   uint8_t *guid,
                   size_t size );

/* Retrieves the GUID
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_get_segment_file_set_identifier instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN \
int libewf_handle_get_guid(
     libewf_handle_t *handle,
     uint8_t *guid,
     size_t size,
     libewf_error_t **error );

/* Sets the GUID
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_set_segment_file_set_identifier instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN \
int libewf_handle_set_guid(
     libewf_handle_t *handle,
     uint8_t *guid,
     size_t size,
     libewf_error_t **error );

/* Retrieves the MD5 hash
 * Returns 1 if successful, 0 if value not present or -1 on error
 *
 * This function is deprecated use libewf_handle_get_md5_hash instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_get_md5_hash(
                   libewf_handle_t *handle,
                   uint8_t *md5_hash,
                   size_t size );

/* Sets the MD5 hash
 * Returns 1 if successful, 0 if value not present or -1 on error
 *
 * This function is deprecated use libewf_handle_set_md5_hash instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_set_md5_hash(
                   libewf_handle_t *handle,
                   uint8_t *md5_hash,
                   size_t size );

/* Retrieves the SHA1 hash
 * Returns 1 if successful, 0 if value not present or -1 on error
 *
 * This function is deprecated use libewf_handle_get_sha1_hash instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_get_sha1_hash(
                   libewf_handle_t *handle,
                   uint8_t *sha1_hash,
                   size_t size );

/* Sets the SHA1 hash
 * Returns 1 if successful, 0 if value not present or -1 on error
 *
 * This function is deprecated use libewf_handle_set_sha1_hash instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_set_sha1_hash(
                   libewf_handle_t *handle,
                   uint8_t *sha1_hash,
                   size_t size );

/* Retrieves the amount of chunks written
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_get_write_amount_of_chunks instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_get_write_amount_of_chunks(
                   libewf_handle_t *handle,
                   uint32_t *amount_of_chunks );

/* Sets the read wipe chunk on error
 * The chunk is not wiped if read raw is used
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_set_read_zero_chunk_on_error instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_set_read_wipe_chunk_on_error(
                   libewf_handle_t *handle,
                   uint8_t wipe_on_error );

/* Copies the media values from the source to the destination handle
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_copy_media_values instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_copy_media_values(
                   libewf_handle_t *destination_handle,
                   libewf_handle_t *source_handle );

/* Retrieves the amount of acquiry errors
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_get_number_of_acquiry_errors instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_get_amount_of_acquiry_errors(
                   libewf_handle_t *handle,
                   uint32_t *amount_of_errors );

/* Retrieves an acquiry error
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_get_acquiry_error instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_get_acquiry_error(
                   libewf_handle_t *handle,
                   uint32_t index,
                   off64_t *start_sector,
                   uint32_t *amount_of_sectors );

/* Add an acquiry error
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_append_acquiry_error instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_add_acquiry_error(
                   libewf_handle_t *handle,
                   off64_t start_sector,
                   uint32_t amount_of_sectors );

/* Retrieves the amount of checksum errors
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_get_number_of_checksum_errors instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_get_amount_of_crc_errors(
                   libewf_handle_t *handle,
                   uint32_t *amount_of_errors );

/* Retrieves a checksum error
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_get_checksum_error instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_get_crc_error(
                   libewf_handle_t *handle,
                   uint32_t index,
                   off64_t *start_sector,
                   uint32_t *amount_of_sectors );

/* Add a checksum error
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_append_checksum_error instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_add_crc_error(
                   libewf_handle_t *handle,
                   off64_t start_sector,
                   uint32_t amount_of_sectors );

/* Retrieves the amount of sessions
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_get_number_of_sessions instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_get_amount_of_sessions(
                   libewf_handle_t *handle,
                   uint32_t *amount_of_sessions );

/* Retrieves a session
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_get_session instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_get_session(
                   libewf_handle_t *handle,
                   uint32_t index,
                   off64_t *start_sector,
                   uint32_t *amount_of_sectors );

/* Add a session
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_append_session instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_add_session(
                   libewf_handle_t *handle,
                   off64_t start_sector,
                   uint32_t amount_of_sectors );

/* Retrieves the header codepage
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_get_header_codepage instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_get_header_codepage(
                   libewf_handle_t *handle,
                   int *header_codepage );

/* Sets the header codepage
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_set_header_codepage instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_set_header_codepage(
                   libewf_handle_t *handle,
                   int header_codepage );

/* Retrieves the amount of header values
 * Returns 1 if successful, 0 if no header values are present or -1 on error
 *
 * This function is deprecated use libewf_handle_get_number_of_header_values instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_get_amount_of_header_values(
                   libewf_handle_t *handle,
                   uint32_t *amount_of_values );

/* Retrieves the header value identifier size specified by its index
 * The identifier size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 *
 * This function is deprecated use libewf_handle_get_header_value_identifier_size instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_get_header_value_identifier_size(
                   libewf_handle_t *handle,
                   uint32_t index,
                   size_t *identifier_size );

/* Retrieves the header value identifier specified by its index
 * The strings are encoded in UTF-8
 * The identifier size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 *
 * This function is deprecated use libewf_handle_get_header_value_identifier instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_get_header_value_identifier(
                   libewf_handle_t *handle,
                   uint32_t index,
                   char *identifier,
                   size_t identifier_size );

/* Retrieves the header value size specified by the identifier
 * The value size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 *
 * This function is deprecated use libewf_handle_get_header_value_size instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_get_header_value_size(
                   libewf_handle_t *handle,
                   const char *identifier,
                   size_t *value_size );

/* Retrieves the header value specified by the identifier
 * The strings are encoded in UTF-8
 * The value size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 *
 * This function is deprecated use libewf_handle_get_header_value instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_get_header_value(
                   libewf_handle_t *handle,
                   const char *identifier,
                   char *value,
                   size_t value_size );

/* Retrieves the header value case number
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
#define libewf_get_header_value_case_number( handle, value, value_size ) \
        libewf_get_header_value( handle, "case_number", value, value_size )

/* Retrieves the header value description
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
#define libewf_get_header_value_description( handle, value, value_size ) \
        libewf_get_header_value( handle, "description", value, value_size )

/* Retrieves the header value examiner name
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
#define libewf_get_header_value_examiner_name( handle, value, value_size ) \
        libewf_get_header_value( handle, "examiner_name", value, value_size )

/* Retrieves the header value evidence number
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
#define libewf_get_header_value_evidence_number( handle, value, value_size ) \
        libewf_get_header_value( handle, "evidence_number", value, value_size )

/* Retrieves the header value notes
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
#define libewf_get_header_value_notes( handle, value, value_size ) \
        libewf_get_header_value( handle, "notes", value, value_size )

/* Retrieves the header value acquiry date
 * The string is encoded according to the date format
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
#define libewf_get_header_value_acquiry_date( handle, value, value_size ) \
        libewf_get_header_value( handle, "acquiry_date", value, value_size )

/* Retrieves the header value system date
 * The string is encoded according to the date format
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
#define libewf_get_header_value_system_date( handle, value, value_size ) \
        libewf_get_header_value( handle, "system_date", value, value_size )

/* Retrieves the header value acquiry operating system
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
#define libewf_get_header_value_acquiry_operating_system( handle, value, value_size ) \
        libewf_get_header_value( handle, "acquiry_operating_system", value, value_size )

/* Retrieves the header value acquiry software version
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
#define libewf_get_header_value_acquiry_software_version( handle, value, value_size ) \
        libewf_get_header_value( handle, "acquiry_software_version", value, value_size )

/* Retrieves the header value password
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
#define libewf_get_header_value_password( handle, value, value_size ) \
        libewf_get_header_value( handle, "password", value, value_size )

/* Retrieves the header value compression type
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
#define libewf_get_header_value_compression_type( handle, value, value_size ) \
        libewf_get_header_value( handle, "compression_type", value, value_size )

/* Retrieves the header value model
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
#define libewf_get_header_value_model( handle, value, value_size ) \
        libewf_get_header_value( handle, "model", value, value_size )

/* Retrieves the header value serial number
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
#define libewf_get_header_value_serial_number( handle, value, value_size ) \
        libewf_get_header_value( handle, "serial_number", value, value_size )

/* Sets the header value specified by the identifier
 * The strings are encoded in UTF-8
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_set_header_value instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_set_header_value(
                   libewf_handle_t *handle,
                   const char *identifier,
                   const char *value,
                   size_t value_length );

/* Sets the header value case number
 * Returns 1 if successful or -1 on error
 */
#define libewf_set_header_value_case_number( handle, value, value_length ) \
        libewf_set_header_value( handle, "case_number", value, value_length )

/* Sets the header value description
 * Returns 1 if successful or -1 on error
 */
#define libewf_set_header_value_description( handle, value, value_length ) \
        libewf_set_header_value( handle, "description", value, value_length )

/* Sets the header value examiner name
 * Returns 1 if successful or -1 on error
 */
#define libewf_set_header_value_examiner_name( handle, value, value_length ) \
        libewf_set_header_value( handle, "examiner_name", value, value_length )

/* Sets the header value evidence number
 * Returns 1 if successful or -1 on error
 */
#define libewf_set_header_value_evidence_number( handle, value, value_length ) \
        libewf_set_header_value( handle, "evidence_number", value, value_length )

/* Sets the header value notes
 * Returns 1 if successful or -1 on error
 */
#define libewf_set_header_value_notes( handle, value, value_length ) \
        libewf_set_header_value( handle, "notes", value, value_length )

/* Sets the header value acquiry date
 * Returns 1 if successful or -1 on error
 */
#define libewf_set_header_value_acquiry_date( handle, value, value_length ) \
        libewf_set_header_value( handle, "acquiry_date", value, value_length )

/* Sets the header value system date
 * Returns 1 if successful or -1 on error
 */
#define libewf_set_header_value_system_date( handle, value, value_length ) \
        libewf_set_header_value( handle, "system_date", value, value_length )

/* Sets the header value acquiry operating system
 * Returns 1 if successful or -1 on error
 */
#define libewf_set_header_value_acquiry_operating_system( handle, value, value_length ) \
        libewf_set_header_value( handle, "acquiry_operating_system", value, value_length )

/* Sets the header value acquiry software version
 * Returns 1 if successful or -1 on error
 */
#define libewf_set_header_value_acquiry_software_version( handle, value, value_length ) \
        libewf_set_header_value( handle, "acquiry_software_version", value, value_length )

/* Sets the header value password
 * Returns 1 if successful or -1 on error
 */
#define libewf_set_header_value_password( handle, value, value_length ) \
        libewf_set_header_value( handle, "password", value, value_length )

/* Sets the header value compression type
 * Returns 1 if successful or -1 on error
 */
#define libewf_set_header_value_compression_type( handle, value, value_length ) \
        libewf_set_header_value( handle, "compression_type", value, value_length )

/* Sets the header value model
 * Returns 1 if successful or -1 on error
 */
#define libewf_set_header_value_model( handle, value, value_length ) \
        libewf_set_header_value( handle, "model", value, value_length )

/* Sets the header value serial number
 * Returns 1 if successful or -1 on error
 */
#define libewf_set_header_value_serial_number( handle, value, value_length ) \
        libewf_set_header_value( handle, "serial_number", value, value_length )

/* Parses the header values from the xheader, header2 or header section
 * Will parse the first available header in order mentioned above
 * Returns 1 if successful, 0 if already parsed or -1 on error
 *
 * This function is deprecated and no longer required
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_parse_header_values(
                   libewf_handle_t *handle,
                   uint8_t date_format );

/* Copies the header values from the source to the destination handle
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_copy_header_values instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_copy_header_values(
                   libewf_handle_t *destination_handle,
                   libewf_handle_t *source_handle );

/* Retrieves the amount of hash values
 * Returns 1 if successful, 0 if no hash values are present or -1 on error
 *
 * This function is deprecated use libewf_handle_get_number_of_hash_values instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_get_amount_of_hash_values(
                   libewf_handle_t *handle,
                   uint32_t *amount_of_values );

/* Retrieves the hash value identifier size specified by its index
 * The identifier size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 *
 * This function is deprecated use libewf_handle_get_hash_value_identifier_size instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_get_hash_value_identifier_size(
                   libewf_handle_t *handle,
                   uint32_t index,
                   size_t *identifier_size );

/* Retrieves the hash value identifier specified by its index
 * The strings are encoded in UTF-8
 * The identifier size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 *
 * This function is deprecated use libewf_handle_get_hash_value_identifier instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_get_hash_value_identifier(
                   libewf_handle_t *handle,
                   uint32_t index,
                   char *identifier,
                   size_t identifier_size );

/* Retrieves the hash value size specified by the identifier
 * The value size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 *
 * This function is deprecated use libewf_handle_get_hash_value_size instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_get_hash_value_size(
                   libewf_handle_t *handle,
                   const char *identifier,
                   size_t *value_size );

/* Retrieves the hash value specified by the identifier
 * The strings are encoded in UTF-8
 * The value size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 *
 * This function is deprecated use libewf_handle_get_hash_value instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_get_hash_value(
                   libewf_handle_t *handle,
                   const char *identifier,
                   char *value,
                   size_t value_size );

/* Retrieves the hash value MD5
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
#define libewf_get_hash_value_md5( handle, value, value_size ) \
        libewf_get_hash_value( handle, "MD5", value, value_size )

/* Retrieves the hash value SHA1
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
#define libewf_get_hash_value_sha1( handle, value, value_size ) \
        libewf_get_hash_value( handle, "SHA1", value, value_size )

/* Sets the hash value specified by the identifier
 * The strings are encoded in UTF-8
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libewf_handle_set_hash_value instead
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_set_hash_value(
                   libewf_handle_t *handle,
                   const char *identifier,
                   const char *value,
                   size_t value_length );

/* Sets the hash value MD5
 * Returns 1 if successful or -1 on error
 */
#define libewf_set_hash_value_md5( handle, value, value_length ) \
        libewf_set_hash_value( handle, "MD5", value, value_length )

/* Sets the hash value SHA1
 * Returns 1 if successful or -1 on error
 */
#define libewf_set_hash_value_sha1( handle, value, value_length ) \
        libewf_set_hash_value( handle, "SHA1", value, value_length )

/* Parses the hash values from the xhash section
 * Returns 1 if successful, 0 if already parsed or -1 on error
 *
 * This function is deprecated and no longer required
 */
LIBEWF_DEPRECATED \
LIBEWF_EXTERN int libewf_parse_hash_values(
                   libewf_handle_t *handle );

#endif /* defined( HAVE_V1_API ) */

#ifdef __cplusplus
}
#endif

#endif /* !defined( _LIBEWF_H ) */

