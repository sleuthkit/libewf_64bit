/*
 * Track value functions
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

#if !defined( _LIBSMDEV_TRACK_VALUE_H )
#define _LIBSMDEV_TRACK_VALUE_H

#include <common.h>
#include <types.h>

#include "libsmdev_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libsmdev_track_value libsmdev_track_value_t;

struct libsmdev_track_value
{
	/* The start sector
	 */
	uint64_t start_sector;

	/* The end sector
	 */
	uint64_t end_sector;

	/* The number of sectors
	 */
	uint64_t number_of_sectors;

	/* The bytes per sector
	 */
	uint32_t bytes_per_sector;

	/* The type
	 */
	uint8_t type;
};

int libsmdev_track_value_initialize(
     libsmdev_track_value_t **track_value,
     libcerror_error_t **error );

int libsmdev_track_value_free(
     libsmdev_track_value_t **track_value,
     libcerror_error_t **error );

int libsmdev_track_value_get(
     libsmdev_track_value_t *track_value,
     uint64_t *start_sector,
     uint64_t *number_of_sectors,
     uint8_t *type,
     libcerror_error_t **error );

int libsmdev_track_value_set(
     libsmdev_track_value_t *track_value,
     uint64_t start_sector,
     uint64_t number_of_sectors,
     uint8_t type,
     libcerror_error_t **error );

int libsmdev_track_value_get_bytes_per_sector(
     libsmdev_track_value_t *track_value,
     uint32_t *bytes_per_sector,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

