/*
 * Meta data functions
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

#if !defined( _LIBSMDEV_METADATA_H )
#define _LIBSMDEV_METADATA_H

#include <common.h>
#include <types.h>

#include "libsmdev_extern.h"
#include "libsmdev_libcerror.h"
#include "libsmdev_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

LIBSMDEV_EXTERN \
int libsmdev_handle_get_media_size(
     libsmdev_handle_t *handle,
     size64_t *media_size,
     libcerror_error_t **error );

LIBSMDEV_EXTERN \
int libsmdev_handle_get_bytes_per_sector(
     libsmdev_handle_t *handle,
     uint32_t *bytes_per_sector,
     libcerror_error_t **error );

LIBSMDEV_EXTERN \
int libsmdev_handle_get_media_type(
     libsmdev_handle_t *handle,
     uint8_t *media_type,
     libcerror_error_t **error );

LIBSMDEV_EXTERN \
int libsmdev_handle_get_bus_type(
     libsmdev_handle_t *handle,
     uint8_t *bus_type,
     libcerror_error_t **error );

LIBSMDEV_EXTERN \
int libsmdev_handle_get_utf8_information_value(
     libsmdev_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBSMDEV_EXTERN \
int libsmdev_handle_get_utf16_information_value(
     libsmdev_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBSMDEV_EXTERN \
int libsmdev_handle_get_number_of_sessions(
     libsmdev_handle_t *handle,
     int *number_of_sessions,
     libcerror_error_t **error );

LIBSMDEV_EXTERN \
int libsmdev_handle_get_session(
     libsmdev_handle_t *handle,
     int index,
     uint64_t *start_sector,
     uint64_t *number_of_sectors,
     libcerror_error_t **error );

LIBSMDEV_EXTERN \
int libsmdev_handle_get_number_of_tracks(
     libsmdev_handle_t *handle,
     int *number_of_tracks,
     libcerror_error_t **error );

LIBSMDEV_EXTERN \
int libsmdev_handle_get_track(
     libsmdev_handle_t *handle,
     int index,
     uint64_t *start_sector,
     uint64_t *number_of_sectors,
     uint8_t *type,
     libcerror_error_t **error );

LIBSMDEV_EXTERN \
int libsmdev_handle_get_number_of_error_retries(
     libsmdev_handle_t *handle,
     uint8_t *number_of_error_retries,
     libcerror_error_t **error );

LIBSMDEV_EXTERN \
int libsmdev_handle_set_number_of_error_retries(
     libsmdev_handle_t *handle,
     uint8_t number_of_error_retries,
     libcerror_error_t **error );

LIBSMDEV_EXTERN \
int libsmdev_handle_get_error_granularity(
     libsmdev_handle_t *handle,
     size_t *error_granularity,
     libcerror_error_t **error );

LIBSMDEV_EXTERN \
int libsmdev_handle_set_error_granularity(
     libsmdev_handle_t *handle,
     size_t error_granularity,
     libcerror_error_t **error );

LIBSMDEV_EXTERN \
int libsmdev_handle_get_error_flags(
     libsmdev_handle_t *handle,
     uint8_t *error_flags,
     libcerror_error_t **error );

LIBSMDEV_EXTERN \
int libsmdev_handle_set_error_flags(
     libsmdev_handle_t *handle,
     uint8_t error_flags,
     libcerror_error_t **error );

LIBSMDEV_EXTERN \
int libsmdev_handle_get_number_of_errors(
     libsmdev_handle_t *handle,
     int *number_of_errors,
     libcerror_error_t **error );

LIBSMDEV_EXTERN \
int libsmdev_handle_get_error(
     libsmdev_handle_t *handle,
     int index,
     off64_t *offset,
     size64_t *size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

