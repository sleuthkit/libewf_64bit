/*
 * File entry functions
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

#if !defined( _LIBEWF_INTERNAL_FILE_ENTRY_H )
#define _LIBEWF_INTERNAL_FILE_ENTRY_H

#include <common.h>
#include <types.h>

#include "libewf_extern.h"
#include "libewf_handle.h"
#include "libewf_libcdata.h"
#include "libewf_libcerror.h"
#include "libewf_single_file_entry.h"
#include "libewf_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_internal_file_entry libewf_internal_file_entry_t;

struct libewf_internal_file_entry
{
	/* The internal EWF handle
	 */
	libewf_internal_handle_t *internal_handle;

	/* The file entry tree node
	 */
	libcdata_tree_node_t *file_entry_tree_node;

	/* The file entry flags
	 */
	uint8_t flags;

	/* The offset
	 */
	off64_t offset;
};

int libewf_file_entry_initialize(
     libewf_file_entry_t **file_entry,
     libewf_internal_handle_t *internal_handle,
     libcdata_tree_node_t *file_entry_tree_node,
     uint8_t flags,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_file_entry_free(
     libewf_file_entry_t **file_entry,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_file_entry_get_type(
     libewf_file_entry_t *file_entry,
     uint8_t *type,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_file_entry_get_flags(
     libewf_file_entry_t *file_entry,
     uint32_t *flags,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_file_entry_get_media_data_offset(
     libewf_file_entry_t *file_entry,
     off64_t *media_data_offset,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_file_entry_get_media_data_size(
     libewf_file_entry_t *file_entry,
     size64_t *media_data_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_file_entry_get_duplicate_media_data_offset(
     libewf_file_entry_t *file_entry,
     off64_t *duplicate_media_data_offset,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_file_entry_get_utf8_name_size(
     libewf_file_entry_t *file_entry,
     size_t *utf8_name_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_file_entry_get_utf8_name(
     libewf_file_entry_t *file_entry,
     uint8_t *utf8_name,
     size_t utf8_name_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_file_entry_get_utf16_name_size(
     libewf_file_entry_t *file_entry,
     size_t *utf16_name_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_file_entry_get_utf16_name(
     libewf_file_entry_t *file_entry,
     uint16_t *utf16_name,
     size_t utf16_name_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_file_entry_get_size(
     libewf_file_entry_t *file_entry,
     size64_t *size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_file_entry_get_creation_time(
     libewf_file_entry_t *file_entry,
     uint32_t *creation_time,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_file_entry_get_modification_time(
     libewf_file_entry_t *file_entry,
     uint32_t *modification_time,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_file_entry_get_access_time(
     libewf_file_entry_t *file_entry,
     uint32_t *access_time,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_file_entry_get_entry_modification_time(
     libewf_file_entry_t *file_entry,
     uint32_t *entry_modification_time,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_file_entry_get_utf8_hash_value_md5(
     libewf_file_entry_t *file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_file_entry_get_utf16_hash_value_md5(
     libewf_file_entry_t *file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_file_entry_get_utf8_hash_value_sha1(
     libewf_file_entry_t *file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_file_entry_get_utf16_hash_value_sha1(
     libewf_file_entry_t *file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBEWF_EXTERN \
ssize_t libewf_file_entry_read_buffer(
         libewf_file_entry_t *file_entry,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error );

LIBEWF_EXTERN \
ssize_t libewf_file_entry_read_random(
         libewf_file_entry_t *file_entry,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error );

LIBEWF_EXTERN \
off64_t libewf_file_entry_seek_offset(
         libewf_file_entry_t *file_entry,
         off64_t offset,
         int whence,
         libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_file_entry_get_offset(
     libewf_file_entry_t *file_entry,
     off64_t *offset,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_file_entry_get_number_of_sub_file_entries(
     libewf_file_entry_t *file_entry,
     int *number_of_sub_entries,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_file_entry_get_sub_file_entry(
     libewf_file_entry_t *file_entry,
     int sub_file_entry_index,
     libewf_file_entry_t **sub_file_entry,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_file_entry_get_sub_file_entry_by_utf8_name(
     libewf_file_entry_t *file_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libewf_file_entry_t **sub_file_entry,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_file_entry_get_sub_file_entry_by_utf8_path(
     libewf_file_entry_t *file_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libewf_file_entry_t **sub_file_entry,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_file_entry_get_sub_file_entry_by_utf16_name(
     libewf_file_entry_t *file_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libewf_file_entry_t **sub_file_entry,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_file_entry_get_sub_file_entry_by_utf16_path(
     libewf_file_entry_t *file_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libewf_file_entry_t **sub_file_entry,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

