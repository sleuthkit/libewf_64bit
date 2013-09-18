/*
 * File functions
 *
 * Copyright (c) 2008-2013, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBCFILE_INTERNAL_FILE_H )
#define _LIBCFILE_INTERNAL_FILE_H

#include <common.h>
#include <types.h>

#include "libcfile_extern.h"
#include "libcfile_libcerror.h"
#include "libcfile_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libcfile_internal_file libcfile_internal_file_t;

struct libcfile_internal_file
{
#if defined( WINAPI ) && !defined( USE_CRT_FUNCTIONS )
	/* The (file) handle
	 */
	HANDLE handle;

	/* Value to indicate the filename indicate this is a device file
	 */
	uint8_t is_device_filename;
#else
	/* The (file) descriptor
	 */
	int descriptor;
#endif
};

LIBCFILE_EXTERN \
int libcfile_file_initialize(
     libcfile_file_t **file,
     libcerror_error_t **error );

LIBCFILE_EXTERN \
int libcfile_file_free(
     libcfile_file_t **file,
     libcerror_error_t **error );

LIBCFILE_EXTERN \
int libcfile_file_open(
     libcfile_file_t *file,
     const char *filename,
     int access_flags,
     libcerror_error_t **error );

LIBCFILE_EXTERN \
int libcfile_file_open_with_error_code(
     libcfile_file_t *file,
     const char *filename,
     int access_flags,
     uint32_t *error_code,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

LIBCFILE_EXTERN \
int libcfile_file_open_wide(
     libcfile_file_t *file,
     const wchar_t *filename,
     int access_flags,
     libcerror_error_t **error );

LIBCFILE_EXTERN \
int libcfile_file_open_wide_with_error_code(
     libcfile_file_t *file,
     const wchar_t *filename,
     int access_flags,
     uint32_t *error_code,
     libcerror_error_t **error );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

LIBCFILE_EXTERN \
int libcfile_file_close(
     libcfile_file_t *file,
     libcerror_error_t **error );

#if defined( WINAPI ) && ( WINVER <= 0x0500 ) && !defined( USE_CRT_FUNCTIONS )
/* TODO implement */
BOOL libcfile_ReadFile(
      HANDLE file_handle,
      LPVOID buffer,
      DWORD read_size,
      DWORD *read_count,
      LPOVERLAPPED overlapped );
#endif

LIBCFILE_EXTERN \
ssize_t libcfile_file_read_buffer(
         libcfile_file_t *file,
         uint8_t *buffer,
         size_t size,
         libcerror_error_t **error );

LIBCFILE_EXTERN \
ssize_t libcfile_file_read_buffer_with_error_code(
         libcfile_file_t *file,
         uint8_t *buffer,
         size_t size,
         uint32_t *error_code,
         libcerror_error_t **error );

#if defined( WINAPI ) && ( WINVER <= 0x0500 ) && !defined( USE_CRT_FUNCTIONS )
/* TODO implement */
BOOL libcfile_WriteFile(
      HANDLE file_handle,
      LPVOID buffer,
      DWORD write_size,
      DWORD *write_count,
      LPOVERLAPPED overlapped );
#endif

LIBCFILE_EXTERN \
ssize_t libcfile_file_write_buffer(
         libcfile_file_t *file,
         const uint8_t *buffer,
         size_t size,
         libcerror_error_t **error );

LIBCFILE_EXTERN \
ssize_t libcfile_file_write_buffer_with_error_code(
         libcfile_file_t *file,
         const uint8_t *buffer,
         size_t size,
         uint32_t *error_code,
         libcerror_error_t **error );

#if defined( WINAPI ) && ( WINVER <= 0x0500 ) && !defined( USE_CRT_FUNCTIONS )
BOOL libcfile_SetFilePointerEx(
      HANDLE file_handle,
      LARGE_INTEGER distance_to_move_large_integer,
      LARGE_INTEGER *new_file_pointer_large_integer,
      DWORD move_method );
#endif

LIBCFILE_EXTERN \
off64_t libcfile_file_seek_offset(
         libcfile_file_t *file,
         off64_t offset,
         int whence,
         libcerror_error_t **error );

#if defined( WINAPI ) && ( WINVER <= 0x0500 ) && !defined( USE_CRT_FUNCTIONS )
BOOL libcfile_SetEndOfFile(
      HANDLE file_handle );
#endif

LIBCFILE_EXTERN \
int libcfile_file_resize(
     libcfile_file_t *file,
     size64_t size,
     libcerror_error_t **error );

LIBCFILE_EXTERN \
int libcfile_file_is_open(
     libcfile_file_t *file,
     libcerror_error_t **error );

#if defined( WINAPI ) && ( WINVER <= 0x0500 ) && !defined( USE_CRT_FUNCTIONS )
BOOL libcfile_GetFileSizeEx(
      HANDLE file_handle,
      LARGE_INTEGER *file_size_large_integer );
#endif

LIBCFILE_EXTERN \
int libcfile_file_get_offset(
     libcfile_file_t *file,
     off64_t *offset,
     libcerror_error_t **error );

LIBCFILE_EXTERN \
int libcfile_file_get_size(
     libcfile_file_t *file,
     size64_t *size,
     libcerror_error_t **error );

LIBCFILE_EXTERN \
int libcfile_file_is_device(
     libcfile_file_t *file,
     libcerror_error_t **error );

LIBCFILE_EXTERN \
ssize_t libcfile_file_io_control_read(
         libcfile_file_t *file,
         uint32_t control_code,
         uint8_t *control_data,
         size_t control_data_size,
         uint8_t *data,
         size_t data_size,
         libcerror_error_t **error );

LIBCFILE_EXTERN \
ssize_t libcfile_file_io_control_read_with_error_code(
         libcfile_file_t *file,
         uint32_t control_code,
         uint8_t *control_data,
         size_t control_data_size,
         uint8_t *data,
         size_t data_size,
         uint32_t *error_code,
         libcerror_error_t **error );

LIBCFILE_EXTERN \
int libcfile_file_set_access_behavior(
     libcfile_file_t *file,
     int access_behavior,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

