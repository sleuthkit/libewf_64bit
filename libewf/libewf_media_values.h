/*
 * Media values functions
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

#if !defined( _LIBEWF_MEDIA_VALUES_H )
#define _LIBEWF_MEDIA_VALUES_H

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libewf_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_media_values libewf_media_values_t;

/* Additional subhandle for media specific parameters
 */
struct libewf_media_values
{
	/* The media size
	 */
	size64_t media_size;

	/* The size of an individual chunk
	 */
	size32_t chunk_size;

	/* The number of sectors per chunk
	 */
	uint32_t sectors_per_chunk;

	/* The number of bytes per sector
	 */
	uint32_t bytes_per_sector;

        /* The number of chunks
         */
        uint32_t number_of_chunks;

        /* The number of sectors
         */
        uint64_t number_of_sectors;

        /* The number of sectors to use as error granularity
         */
        uint32_t error_granularity;

	/* The media type
	 */
	uint8_t media_type;

	/* The media flags
	 */
	uint8_t media_flags;

        /* The GUID of the acquiry system
         */
        uint8_t guid[ 16 ];
};

int libewf_media_values_initialize(
     libewf_media_values_t **media_values,
     libcerror_error_t **error );

int libewf_media_values_free(
     libewf_media_values_t **media_values,
     libcerror_error_t **error );

int libewf_media_values_clone(
     libewf_media_values_t **destination_media_values,
     libewf_media_values_t *source_media_values,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

