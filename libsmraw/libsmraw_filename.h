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

#if !defined( _LIBSMRAW_FILENAME_H )
#define _LIBSMRAW_FILENAME_H

#include <common.h>
#include <types.h>

#include "libsmraw_libcerror.h"
#include "libsmraw_libcstring.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libsmraw_filename_create(
     libcstring_system_character_t **filename,
     size_t *filename_size,
     libcstring_system_character_t *basename,
     size_t basename_size,
     int total_number_of_segments,
     int current_file_io_pool_entry,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

