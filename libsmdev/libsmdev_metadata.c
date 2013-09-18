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

#include <common.h>
#include <memory.h>
#include <types.h>

#if defined( WINAPI )
#include <io.h>
#endif

#if defined( WINAPI )
#include <winioctl.h>

#elif defined( HAVE_CYGWIN_FS_H )
#include <cygwin/fs.h>

#elif defined( HAVE_LINUX_FS_H )
/* Required for Linux platforms that use a sizeof( u64 )
 * in linux/fs.h but have no typedef of it
 */
#if !defined( HAVE_U64 )
typedef size_t u64;
#endif

#include <linux/fs.h>

#else

#if defined( HAVE_SYS_DISK_H )
#include <sys/disk.h>
#endif

#if defined( HAVE_SYS_DISKLABEL_H )
#include <sys/disklabel.h>
#endif

#endif

#include "libsmdev_definitions.h"
#include "libsmdev_handle.h"
#include "libsmdev_libcdata.h"
#include "libsmdev_libcerror.h"
#include "libsmdev_libcfile.h"
#include "libsmdev_libcnotify.h"
#include "libsmdev_libcstring.h"
#include "libsmdev_libuna.h"
#include "libsmdev_sector_range.h"
#include "libsmdev_track_value.h"
#include "libsmdev_types.h"

#if defined( WINAPI )

#if !defined( IOCTL_DISK_GET_LENGTH_INFO )
#define IOCTL_DISK_GET_LENGTH_INFO \
	CTL_CODE( IOCTL_DISK_BASE, 0x0017, METHOD_BUFFERED, FILE_READ_ACCESS )

typedef struct
{
	LARGE_INTEGER Length;
}
GET_LENGTH_INFORMATION;

#endif /* !defined( IOCTL_DISK_GET_LENGTH_INFO ) */

#if !defined( IOCTL_DISK_GET_DRIVE_GEOMETRY_EX )
#define IOCTL_DISK_GET_DRIVE_GEOMETRY_EX \
	CTL_CODE( IOCTL_DISK_BASE, 0x0028, METHOD_BUFFERED, FILE_ANY_ACCESS )

typedef struct _DISK_GEOMETRY_EX
{
	DISK_GEOMETRY Geometry;
	LARGE_INTEGER DiskSize;
	UCHAR Data[ 1 ];
}
DISK_GEOMETRY_EX, *PDISK_GEOMETRY_EX;

#endif /* !defined( IOCTL_DISK_GET_DRIVE_GEOMETRY_EX ) */

#endif /* defined( WINAPI ) */

/* Retrieves the media size
 * Returns the 1 if succesful or -1 on error
 */
int libsmdev_handle_get_media_size(
     libsmdev_handle_t *handle,
     size64_t *media_size,
     libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmdev_handle_get_media_size";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmdev_internal_handle_t *) handle;

	if( internal_handle->device_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing device file.",
		 function );

		return( -1 );
	}
	if( media_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media size.",
		 function );

		return( -1 );
	}
	if( internal_handle->media_size_set == 0 )
	{
		if( libcfile_file_get_size(
		     internal_handle->device_file,
		     &( internal_handle->media_size ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine device file size.",
			 function );

			return( -1 );
		}
		internal_handle->media_size_set = 1;
	}
	*media_size = internal_handle->media_size;

	return( 1 );
}

/* Retrieves the number of bytes per sector
 * Returns the 1 if succesful or -1 on error
 */
int libsmdev_handle_get_bytes_per_sector(
     libsmdev_handle_t *handle,
     uint32_t *bytes_per_sector,
     libcerror_error_t **error )
{
#if defined( WINAPI )
	DISK_GEOMETRY disk_geometry;
	DISK_GEOMETRY_EX disk_geometry_extended;
#endif

	libsmdev_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmdev_handle_get_bytes_per_sector";
	ssize_t read_count                          = 0;

#if defined( WINAPI )
	uint32_t error_code                         = 0;
#endif

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmdev_internal_handle_t *) handle;

	if( internal_handle->device_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid device handle - missing device file.",
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
	if( internal_handle->bytes_per_sector_set == 0 )
	{
#if defined( WINAPI )
		read_count = libcfile_file_io_control_read_with_error_code(
		              internal_handle->device_file,
		              IOCTL_DISK_GET_DRIVE_GEOMETRY_EX,
		              NULL,
		              0,
		              (uint8_t *) &disk_geometry_extended,
		              sizeof( DISK_GEOMETRY_EX ),
		              &error_code,
		              error );

		if( read_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_IOCTL_FAILED,
			 "%s: unable to query device for: IOCTL_DISK_GET_DRIVE_GEOMETRY_EX.",
			 function );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				if( ( error != NULL )
				 && ( *error != NULL ) )
				{
					libcnotify_print_error_backtrace(
					 *error );
				}
			}
#endif
			libcerror_error_free(
			 error );

			if( error_code == ERROR_NOT_SUPPORTED )
			{
				/* A floppy device does not support IOCTL_DISK_GET_DRIVE_GEOMETRY_EX
				 */
				read_count = libcfile_file_io_control_read(
				              internal_handle->device_file,
				              IOCTL_DISK_GET_DRIVE_GEOMETRY,
				              NULL,
				              0,
				              (uint8_t *) &disk_geometry,
				              sizeof( DISK_GEOMETRY ),
				              error );

				if( read_count == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_IOCTL_FAILED,
					 "%s: unable to query device for: IOCTL_DISK_GET_DRIVE_GEOMETRY.",
					 function );

#if defined( HAVE_DEBUG_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						if( ( error != NULL )
						 && ( *error != NULL ) )
						{
							libcnotify_print_error_backtrace(
							 *error );
						}
					}
#endif
					libcerror_error_free(
					 error );
				}
				else
				{
					internal_handle->bytes_per_sector     = (uint32_t) disk_geometry.BytesPerSector;
					internal_handle->bytes_per_sector_set = 1;
				}
			}
		}
		else
		{
			internal_handle->bytes_per_sector     = (uint32_t) disk_geometry_extended.Geometry.BytesPerSector;
			internal_handle->bytes_per_sector_set = 1;
		}
#elif defined( BLKSSZGET )
		read_count = libcfile_file_io_control_read(
		              internal_handle->device_file,
		              BLKSSZGET,
		              NULL,
		              0,
		              (uint8_t *) &( internal_handle->bytes_per_sector ),
		              4,
		              error );

		if( read_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_IOCTL_FAILED,
			 "%s: unable to query device for: BLKSSZGET.",
			 function );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				if( ( error != NULL )
				 && ( *error != NULL ) )
				{
					libcnotify_print_error_backtrace(
					 *error );
				}
			}
#endif
			libcerror_error_free(
			 error );
		}
		else
		{
			internal_handle->bytes_per_sector_set = 1;
		}
#elif defined( DKIOCGETBLOCKCOUNT )
		read_count = libcfile_file_io_control_read(
		              internal_handle->device_file,
		              DKIOCGETBLOCKSIZE,
		              NULL,
		              0,
		              (uint8_t *) &( internal_handle->bytes_per_sector ),
		              4,
		              error );

		if( read_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_IOCTL_FAILED,
			 "%s: unable to query device for: DKIOCGETBLOCKSIZE.",
			 function );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				if( ( error != NULL )
				 && ( *error != NULL ) )
				{
					libcnotify_print_error_backtrace(
					 *error );
				}
			}
#endif
			libcerror_error_free(
			 error );
		}
		else
		{
			internal_handle->bytes_per_sector_set = 1;
		}
#endif
	}
	if( internal_handle->bytes_per_sector_set == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported platform.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: bytes per sector: %" PRIu32 "\n",
		 function,
		 internal_handle->bytes_per_sector );
	}
#endif
	*bytes_per_sector = internal_handle->bytes_per_sector;

	return( 1 );
}

/* Retrieves the media type
 * Returns 1 if successful or -1 on error
 */
int libsmdev_handle_get_media_type(
     libsmdev_handle_t *handle,
     uint8_t *media_type,
     libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmdev_handle_get_media_type";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmdev_internal_handle_t *) handle;

	if( internal_handle->device_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid device handle - missing device file.",
		 function );

		return( -1 );
	}
	if( media_type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media type.",
		 function );

		return( -1 );
	}
	if( internal_handle->media_information_set == 0 )
	{
		if( libsmdev_internal_handle_determine_media_information(
		     internal_handle,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine media information.",
			 function );

			return( -1 );
		}
	}
	if( internal_handle->device_type == 0x05 )
	{
		*media_type = LIBSMDEV_MEDIA_TYPE_OPTICAL;
	}
	else if( internal_handle->removable != 0 )
	{
		*media_type = LIBSMDEV_MEDIA_TYPE_REMOVABLE;
	}
	else
	{
		*media_type = LIBSMDEV_MEDIA_TYPE_FIXED;
	}
	return( 1 );
}

/* Retrieves the bus type
 * Returns 1 if successful or -1 on error
 */
int libsmdev_handle_get_bus_type(
     libsmdev_handle_t *handle,
     uint8_t *bus_type,
     libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmdev_handle_get_bus_type";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmdev_internal_handle_t *) handle;

	if( internal_handle->device_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid device handle - missing device file.",
		 function );

		return( -1 );
	}
	if( bus_type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid bus type.",
		 function );

		return( -1 );
	}
	if( internal_handle->media_information_set == 0 )
	{
		if( libsmdev_internal_handle_determine_media_information(
		     internal_handle,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine media information.",
			 function );

			return( -1 );
		}
	}
	*bus_type = internal_handle->bus_type;

	return( 1 );
}

/* Retrieves an UTF-8 encoded information value for the specific identifier
 * The value size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libsmdev_handle_get_utf8_information_value(
     libsmdev_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	uint8_t *information_value                  = NULL;
	static char *function                       = "libsmdev_handle_get_utf8_information_value";
	size_t information_value_size               = 0;
	size_t string_index                         = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmdev_internal_handle_t *) handle;

	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
	if( internal_handle->media_information_set == 0 )
	{
		if( libsmdev_internal_handle_determine_media_information(
		     internal_handle,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine media information.",
			 function );

			return( -1 );
		}
	}
	if( identifier_length == 5 )
	{
		if( libcstring_narrow_string_compare(
		     "model",
		     (char *) identifier,
		     identifier_length ) == 0 )
		{
			information_value = (uint8_t *) internal_handle->model;
		}
	}
	else if( identifier_length == 6 )
	{
		if( libcstring_narrow_string_compare(
		     "vendor",
		     (char *) identifier,
		     identifier_length ) == 0 )
		{
			information_value = (uint8_t *) internal_handle->vendor;
		}
	}
	else if( identifier_length == 13 )
	{
		if( libcstring_narrow_string_compare(
		     "serial_number",
		     (char *) identifier,
		     identifier_length ) == 0 )
		{
			information_value = (uint8_t *) internal_handle->serial_number;
		}
	}
	if( information_value == NULL )
	{
		return( 0 );
	}
	if( information_value[ 0 ] == 0 )
	{
		return( 0 );
	}
	/* Determine the header value size
	 */
	information_value_size = 1 + libcstring_narrow_string_length(
	                              (char *) information_value );

	if( utf8_string_size < information_value_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-8 string too small.",
		 function );

		return( -1 );
	}
	for( string_index = 0;
	     string_index < information_value_size - 1;
	     string_index++ )
	{
		utf8_string[ string_index ] = (uint8_t) information_value[ string_index ];
	}
	utf8_string[ information_value_size - 1 ] = 0;

	return( 1 );
}

/* Retrieves an UTF-16 encoded information value for the specific identifier
 * The value size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libsmdev_handle_get_utf16_information_value(
     libsmdev_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	uint8_t *information_value                  = NULL;
	static char *function                       = "libsmdev_handle_get_utf16_information_value";
	size_t information_value_size               = 0;
	size_t string_index                         = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmdev_internal_handle_t *) handle;

	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
	if( internal_handle->media_information_set == 0 )
	{
		if( libsmdev_internal_handle_determine_media_information(
		     internal_handle,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine media information.",
			 function );

			return( -1 );
		}
	}
	if( identifier_length == 5 )
	{
		if( libcstring_narrow_string_compare(
		     "model",
		     (char *) identifier,
		     identifier_length ) == 0 )
		{
			information_value = (uint8_t *) internal_handle->model;
		}
	}
	else if( identifier_length == 6 )
	{
		if( libcstring_narrow_string_compare(
		     "vendor",
		     (char *) identifier,
		     identifier_length ) == 0 )
		{
			information_value = (uint8_t *) internal_handle->vendor;
		}
	}
	else if( identifier_length == 13 )
	{
		if( libcstring_narrow_string_compare(
		     "serial_number",
		     (char *) identifier,
		     identifier_length ) == 0 )
		{
			information_value = (uint8_t *) internal_handle->serial_number;
		}
	}
	if( information_value == NULL )
	{
		return( 0 );
	}
	if( information_value[ 0 ] == 0 )
	{
		return( 0 );
	}
	/* Determine the header value size
	 */
	information_value_size = 1 + libcstring_narrow_string_length(
	                              (char *) information_value );

	if( utf16_string_size < information_value_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-16 string too small.",
		 function );

		return( -1 );
	}
	for( string_index = 0;
	     string_index < information_value_size - 1;
	     string_index++ )
	{
		utf16_string[ string_index ] = (uint16_t) information_value[ string_index ];
	}
	utf16_string[ information_value_size - 1 ] = 0;

	return( 1 );
}

/* Retrieves the number of sessions
 * Returns 1 if successful or -1 on error
 */
int libsmdev_handle_get_number_of_sessions(
     libsmdev_handle_t *handle,
     int *number_of_sessions,
     libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmdev_handle_get_number_of_sessions";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmdev_internal_handle_t *) handle;

	if( libcdata_array_get_number_of_entries(
	     internal_handle->sessions_array,
	     number_of_sessions,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries in sessions array.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a session
 * Returns 1 if successful or -1 on error
 */
int libsmdev_handle_get_session(
     libsmdev_handle_t *handle,
     int index,
     uint64_t *start_sector,
     uint64_t *number_of_sectors,
     libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	libsmdev_sector_range_t *sector_range       = NULL;
	static char *function                       = "libsmdev_handle_get_session";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmdev_internal_handle_t *) handle;

	if( libcdata_array_get_entry_by_index(
	     internal_handle->sessions_array,
	     index,
	     (intptr_t **) &sector_range,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve session sector range: %d from array.",
		 function,
		 index );

		return( -1 );
	}
	if( libsmdev_sector_range_get(
	     sector_range,
	     start_sector,
	     number_of_sectors,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sector range.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the number of tracks
 * Returns 1 if successful or -1 on error
 */
int libsmdev_handle_get_number_of_tracks(
     libsmdev_handle_t *handle,
     int *number_of_tracks,
     libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmdev_handle_get_number_of_tracks";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmdev_internal_handle_t *) handle;

	if( libcdata_array_get_number_of_entries(
	     internal_handle->tracks_array,
	     number_of_tracks,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries in tracks array.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a track
 * Returns 1 if successful or -1 on error
 */
int libsmdev_handle_get_track(
     libsmdev_handle_t *handle,
     int index,
     uint64_t *start_sector,
     uint64_t *number_of_sectors,
     uint8_t *type,
     libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	libsmdev_track_value_t *track_value         = NULL;
	static char *function                       = "libsmdev_handle_get_track";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmdev_internal_handle_t *) handle;

	if( libcdata_array_get_entry_by_index(
	     internal_handle->tracks_array,
	     index,
	     (intptr_t **) &track_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve track value: %d from array.",
		 function,
		 index );

		return( -1 );
	}
	if( libsmdev_track_value_get(
	     track_value,
	     start_sector,
	     number_of_sectors,
	     type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve track value.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the number of read/write error retries
 * Returns the 1 if succesful or -1 on error
 */
int libsmdev_handle_get_number_of_error_retries(
     libsmdev_handle_t *handle,
     uint8_t *number_of_error_retries,
     libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmdev_handle_get_number_of_error_retries";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmdev_internal_handle_t *) handle;

	if( number_of_error_retries == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of error retries.",
		 function );

		return( -1 );
	}
	*number_of_error_retries = internal_handle->number_of_error_retries;

	return( 1 );
}

/* Sets the number of read/write error retries
 * Returns the 1 if succesful or -1 on error
 */
int libsmdev_handle_set_number_of_error_retries(
     libsmdev_handle_t *handle,
     uint8_t number_of_error_retries,
     libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmdev_handle_set_number_of_error_retries";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmdev_internal_handle_t *) handle;

	internal_handle->number_of_error_retries = number_of_error_retries;

	return( 1 );
}

/* Retrieves the read/write error granularity
 * A value of 0 represents an error granularity of the entire buffer being read/written
 * Returns the 1 if succesful or -1 on error
 */
int libsmdev_handle_get_error_granularity(
     libsmdev_handle_t *handle,
     size_t *error_granularity,
     libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmdev_handle_get_error_granularity";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmdev_internal_handle_t *) handle;

	if( error_granularity == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid read granularity.",
		 function );

		return( -1 );
	}
	*error_granularity = internal_handle->error_granularity;

	return( 1 );
}

/* Sets the read/write error granularity
 * A value of 0 represents an error granularity of the entire buffer being read/written
 * Returns the 1 if succesful or -1 on error
 */
int libsmdev_handle_set_error_granularity(
     libsmdev_handle_t *handle,
     size_t error_granularity,
     libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmdev_handle_set_error_granularity";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmdev_internal_handle_t *) handle;

	if( error_granularity > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid error granularity value exceeds maximum.",
		 function );

		return( -1 );
	}
	internal_handle->error_granularity = error_granularity;

	return( 1 );
}

/* Retrieves the read/write error flags
 * Returns the 1 if succesful or -1 on error
 */
int libsmdev_handle_get_error_flags(
     libsmdev_handle_t *handle,
     uint8_t *error_flags,
     libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmdev_handle_get_error_flags";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmdev_internal_handle_t *) handle;

	if( error_flags == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid error flags.",
		 function );

		return( -1 );
	}
	*error_flags = internal_handle->error_flags;

	return( 1 );
}

/* Sets the the read/write error flags
 * Returns the 1 if succesful or -1 on error
 */
int libsmdev_handle_set_error_flags(
     libsmdev_handle_t *handle,
     uint8_t error_flags,
     libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmdev_handle_set_error_flags";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmdev_internal_handle_t *) handle;

	if( ( error_flags & ~( LIBSMDEV_ERROR_FLAG_ZERO_ON_ERROR ) ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported error flags.",
		 function );

		return( -1 );
	}
	internal_handle->error_flags = error_flags;

	return( 1 );
}

/* Retrieves the number of read/write errors
 * Returns 1 if successful or -1 on error
 */
int libsmdev_handle_get_number_of_errors(
     libsmdev_handle_t *handle,
     int *number_of_errors,
     libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmdev_handle_get_number_of_errors";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmdev_internal_handle_t *) handle;

	if( libcdata_range_list_get_number_of_elements(
	     internal_handle->errors_range_list,
	     number_of_errors,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of elements in errors range list.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a read/write error
 * Returns 1 if successful or -1 on error
 */
int libsmdev_handle_get_error(
     libsmdev_handle_t *handle,
     int index,
     off64_t *offset,
     size64_t *size,
     libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmdev_handle_get_error";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmdev_internal_handle_t *) handle;

	if( libcdata_range_list_get_range(
	     internal_handle->errors_range_list,
	     index,
	     (uint64_t *) offset,
	     (uint64_t *) size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve error: %d from errors range list.",
		 function,
		 index );

		return( -1 );
	}
	return( 1 );
}

