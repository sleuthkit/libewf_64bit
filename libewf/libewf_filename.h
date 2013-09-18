/*
 * Filename functions
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

#if !defined( _LIBEWF_FILENAME_H )
#define _LIBEWF_FILENAME_H

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libewf_libcstring.h"
#include "libewf_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libewf_filename_set_extension(
     char *extension,
     uint16_t segment_number,
     uint16_t maximum_number_of_segments,
     uint8_t segment_file_type,
     uint8_t format,
     uint8_t ewf_format,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )
int libewf_filename_set_extension_wide(
     wchar_t *extension,
     uint16_t segment_number,
     uint16_t maximum_number_of_segments,
     uint8_t segment_file_type,
     uint8_t format,
     uint8_t ewf_format,
     libcerror_error_t **error );
#endif

int libewf_filename_create(
     libcstring_system_character_t **filename,
     size_t *filename_size,
     libcstring_system_character_t *basename,
     size_t basename_length,
     uint16_t segment_number,
     uint16_t maximum_number_of_segments,
     uint8_t segment_file_type,
     uint8_t format,
     uint8_t ewf_format,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

