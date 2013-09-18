/*
 * Information file functions
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

#if !defined( _LIBSMRAW_INFORMATION_FILE_H )
#define _LIBSMRAW_INFORMATION_FILE_H

#include <common.h>
#include <file_stream.h>
#include <types.h>

#include "libsmraw_libcerror.h"
#include "libsmraw_libcstring.h"
#include "libsmraw_libfvalue.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libsmraw_information_file libsmraw_information_file_t;

struct libsmraw_information_file
{
	/* The filename
	 */
	libcstring_system_character_t *name;

	/* The size of the filename
	 */
	size_t name_size;

	/* The file stream
	 */
	FILE *file_stream;
};

int libsmraw_information_file_initialize(
     libsmraw_information_file_t **information_file,
     libcerror_error_t **error );

int libsmraw_information_file_free(
     libsmraw_information_file_t **information_file,
     libcerror_error_t **error );

int libsmraw_information_file_set_name(
     libsmraw_information_file_t *information_file,
     const libcstring_system_character_t *name,
     size_t name_length,
     libcerror_error_t **error );

int libsmraw_information_file_open(
     libsmraw_information_file_t *information_file,
     const libcstring_system_character_t *mode,
     libcerror_error_t **error );

int libsmraw_information_file_close(
     libsmraw_information_file_t *information_file,
     libcerror_error_t **error );

int libsmraw_information_file_read_section(
     libsmraw_information_file_t *information_file,
     const uint8_t *section_identifier,
     size_t section_identifier_length,
     libfvalue_table_t *values_table,
     libcerror_error_t **error );

int libsmraw_information_file_write_section(
     libsmraw_information_file_t *information_file,
     const uint8_t *section_identifier,
     size_t section_identifier_length,
     libfvalue_table_t *values_table,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

