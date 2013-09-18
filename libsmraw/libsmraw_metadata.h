/*
 * Meta data functions
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

#if !defined( _LIBSMRAW_METADATA_H )
#define _LIBSMRAW_METADATA_H

#include <common.h>
#include <types.h>

#include "libsmraw_extern.h"
#include "libsmraw_libcerror.h"
#include "libsmraw_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

LIBSMRAW_EXTERN \
int libsmraw_handle_get_media_size(
     libsmraw_handle_t *handle,
     size64_t *media_size,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_set_media_size(
     libsmraw_handle_t *handle,
     size64_t media_size,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_bytes_per_sector(
     libsmraw_handle_t *handle,
     uint32_t *bytes_per_sector,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_set_bytes_per_sector(
     libsmraw_handle_t *handle,
     uint32_t bytes_per_sector,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_media_type(
     libsmraw_handle_t *handle,
     uint8_t *media_type,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_set_media_type(
     libsmraw_handle_t *handle,
     uint8_t media_type,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_media_flags(
     libsmraw_handle_t *handle,
     uint8_t *media_flags,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_set_media_flags(
     libsmraw_handle_t *handle,
     uint8_t media_flags,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_number_of_information_values(
     libsmraw_handle_t *handle,
     int *number_of_information_values,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_information_value_identifier_size(
     libsmraw_handle_t *handle,
     int information_value_index,
     size_t *identifier_size,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_information_value_identifier(
     libsmraw_handle_t *handle,
     int information_value_index,
     uint8_t *identifier,
     size_t identifier_size,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_utf8_information_value_size(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_utf8_information_value(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_set_utf8_information_value(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_utf16_information_value_size(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_utf16_information_value(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_set_utf16_information_value(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_number_of_integrity_hash_values(
     libsmraw_handle_t *handle,
     int *number_of_integrity_hash_values,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_integrity_hash_value_identifier_size(
     libsmraw_handle_t *handle,
     int integrity_hash_value_index,
     size_t *identifier_size,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_integrity_hash_value_identifier(
     libsmraw_handle_t *handle,
     int integrity_hash_value_index,
     uint8_t *identifier,
     size_t identifier_size,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_utf8_integrity_hash_value_size(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_utf8_integrity_hash_value(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_set_utf8_integrity_hash_value(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_utf16_integrity_hash_value_size(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_get_utf16_integrity_hash_value(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_handle_set_utf16_integrity_hash_value(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

