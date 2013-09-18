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

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libsmdev_definitions.h"
#include "libsmdev_libcerror.h"
#include "libsmdev_track_value.h"

/* Initialize a track value
 * Make sure the value is pointing to is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libsmdev_track_value_initialize(
     libsmdev_track_value_t **track_value,
     libcerror_error_t **error )
{
	static char *function = "libsmdev_track_value_initialize";

	if( track_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid track value.",
		 function );

		return( -1 );
	}
	if( *track_value != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid track value value already set.",
		 function );

		return( -1 );
	}
	*track_value = memory_allocate_structure(
	                libsmdev_track_value_t );

	if( *track_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create track value.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *track_value,
	     0,
	     sizeof( libsmdev_track_value_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear track value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *track_value != NULL )
	{
		memory_free(
		 *track_value );

		*track_value = NULL;
	}
	return( -1 );
}

/* Frees a track value
 * Returns 1 if successful or -1 on error
 */
int libsmdev_track_value_free(
     libsmdev_track_value_t **track_value,
     libcerror_error_t **error )
{
	static char *function = "libsmdev_track_value_free";

	if( track_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid track value.",
		 function );

		return( -1 );
	}
	if( *track_value != NULL )
	{
		memory_free(
		 *track_value );

		*track_value = NULL;
	}
	return( 1 );
}

/* Retrieves a track value
 * Returns 1 if successful or -1 on error
 */
int libsmdev_track_value_get(
     libsmdev_track_value_t *track_value,
     uint64_t *start_sector,
     uint64_t *number_of_sectors,
     uint8_t *type,
     libcerror_error_t **error )
{
	static char *function = "libsmdev_track_value_get";

	if( track_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid track value.",
		 function );

		return( -1 );
	}
	if( start_sector == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid start sector.",
		 function );

		return( -1 );
	}
	if( number_of_sectors == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of sectors.",
		 function );

		return( -1 );
	}
	if( type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid type.",
		 function );

		return( -1 );
	}
	*start_sector      = track_value->start_sector;
	*number_of_sectors = track_value->number_of_sectors;
	*type              = track_value->type;

	return( 1 );
}

/* Sets a track value
 * Returns 1 if successful or -1 on error
 */
int libsmdev_track_value_set(
     libsmdev_track_value_t *track_value,
     uint64_t start_sector,
     uint64_t number_of_sectors,
     uint8_t type,
     libcerror_error_t **error )
{
	static char *function = "libsmdev_track_value_set";

	if( track_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid track value.",
		 function );

		return( -1 );
	}
	if( start_sector > (uint64_t) INT64_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid start sector value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( number_of_sectors > (uint64_t) INT64_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid number of sectors value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( type != LIBSMDEV_TRACK_TYPE_UNKNOWN )
	 && ( type != LIBSMDEV_TRACK_TYPE_AUDIO )
	 && ( type != LIBSMDEV_TRACK_TYPE_CDG )
	 && ( type != LIBSMDEV_TRACK_TYPE_MODE1_2048 )
	 && ( type != LIBSMDEV_TRACK_TYPE_MODE1_2352 )
	 && ( type != LIBSMDEV_TRACK_TYPE_MODE2_2048 )
	 && ( type != LIBSMDEV_TRACK_TYPE_MODE2_2324 )
	 && ( type != LIBSMDEV_TRACK_TYPE_MODE2_2336 )
	 && ( type != LIBSMDEV_TRACK_TYPE_MODE2_2352 )
	 && ( type != LIBSMDEV_TRACK_TYPE_CDI_2336 )
	 && ( type != LIBSMDEV_TRACK_TYPE_CDI_2352 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported type.",
		 function );

		return( -1 );
	}
	switch( type )
	{
		case LIBSMDEV_TRACK_TYPE_MODE1_2048:
		case LIBSMDEV_TRACK_TYPE_MODE2_2048:
			track_value->bytes_per_sector = 2048;

			break;

		case LIBSMDEV_TRACK_TYPE_MODE2_2324:
			track_value->bytes_per_sector = 2324;

			break;

		case LIBSMDEV_TRACK_TYPE_MODE2_2336:
		case LIBSMDEV_TRACK_TYPE_CDI_2336:
			track_value->bytes_per_sector = 2336;

			break;

		case LIBSMDEV_TRACK_TYPE_AUDIO:
		case LIBSMDEV_TRACK_TYPE_MODE1_2352:
		case LIBSMDEV_TRACK_TYPE_MODE2_2352:
		case LIBSMDEV_TRACK_TYPE_CDI_2352:
			track_value->bytes_per_sector = 2352;

			break;

		case LIBSMDEV_TRACK_TYPE_CDG:
			track_value->bytes_per_sector = 2448;

			break;
	}
	track_value->start_sector      = start_sector;
	track_value->end_sector        = start_sector + number_of_sectors;
	track_value->number_of_sectors = number_of_sectors;
	track_value->type              = type;

	return( 1 );
}

/* Retrieves the bytes per sector
 * Returns 1 if successful or -1 on error
 */
int libsmdev_track_value_get_bytes_per_sector(
     libsmdev_track_value_t *track_value,
     uint32_t *bytes_per_sector,
     libcerror_error_t **error )
{
	static char *function = "libsmdev_track_value_get_bytes_per_sector";

	if( track_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid track value.",
		 function );

		return( -1 );
	}
	if( bytes_per_sector == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid bytes per sector.",
		 function );

		return( -1 );
	}
	*bytes_per_sector = track_value->bytes_per_sector;

	return( 1 );
}

