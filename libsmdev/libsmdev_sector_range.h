/*
 * Sector range functions
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

#if !defined( _LIBSMDEV_SECTOR_RANGE_H )
#define _LIBSMDEV_SECTOR_RANGE_H

#include <common.h>
#include <types.h>

#include "libsmdev_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libsmdev_sector_range libsmdev_sector_range_t;

struct libsmdev_sector_range
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
};

int libsmdev_sector_range_initialize(
     libsmdev_sector_range_t **sector_range,
     libcerror_error_t **error );

int libsmdev_sector_range_free(
     libsmdev_sector_range_t **sector_range,
     libcerror_error_t **error );

int libsmdev_sector_range_clone(
     libsmdev_sector_range_t **destination_sector_range,
     libsmdev_sector_range_t *source_sector_range,
     libcerror_error_t **error );

int libsmdev_sector_range_get(
     libsmdev_sector_range_t *sector_range,
     uint64_t *start_sector,
     uint64_t *number_of_sectors,
     libcerror_error_t **error );

int libsmdev_sector_range_set(
     libsmdev_sector_range_t *sector_range,
     uint64_t start_sector,
     uint64_t number_of_sectors,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

