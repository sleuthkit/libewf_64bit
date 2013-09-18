/*
 * Character type string functions
 *
 * Copyright (c) 2010-2013, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBSMDEV_STRING_H )
#define _LIBSMDEV_STRING_H

#include <common.h>
#include <types.h>

#include "libsmdev_libcerror.h"
#include "libsmdev_libcstring.h"

#if defined( _cplusplus )
extern "C" {
#endif

ssize_t libsmdev_string_trim_copy_from_byte_stream(
         uint8_t *string,
         size_t string_size,
         const uint8_t *byte_stream,
         size_t byte_stream_size,
         libcerror_error_t **error );

#if defined( _cplusplus )
}
#endif

#endif

