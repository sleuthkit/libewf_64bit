/*
 * Legacy functions
 *
 * Copyright (c) 2009-2013, Joachim Metz <joachim.metz@gmail.com>
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

#include "libbfio_definitions.h"
#include "libbfio_handle.h"
#include "libbfio_legacy.h"
#include "libbfio_libcdata.h"
#include "libbfio_libcerror.h"
#include "libbfio_pool.h"

#if !defined( HAVE_LOCAL_LIBBFIO )

/* Reads a buffer from the handle
 * Returns the number of bytes read if successful, or -1 on error
 */
ssize_t libbfio_handle_read(
         libbfio_handle_t *handle,
         uint8_t *buffer,
         size_t size,
         libcerror_error_t **error )
{
	libbfio_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libbfio_handle_read";
	ssize_t read_count                         = 0;
	int is_open                                = 0;

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
	internal_handle = (libbfio_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->read == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing read function.",
		 function );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( internal_handle->open_on_demand != 0 )
	{
		if( internal_handle->is_open == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid handle - missing is open function.",
			 function );

			return( -1 );
		}
		if( internal_handle->open == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid handle - missing open function.",
			 function );

			return( -1 );
		}
		if( internal_handle->seek_offset == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid handle - missing seek offset function.",
			 function );

			return( -1 );
		}
		is_open = internal_handle->is_open(
			   internal_handle->io_handle,
		           error );

		if( is_open == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to determine if handle is open.",
			 function );

			return( -1 );
		}
		else if( is_open == 0 )
		{
			if( internal_handle->open(
			     internal_handle->io_handle,
			     internal_handle->access_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_OPEN_FAILED,
				 "%s: unable to open handle on demand.",
				 function );

				return( -1 );
			}
			if( internal_handle->seek_offset(
			     internal_handle->io_handle,
			     internal_handle->offset,
			     SEEK_SET,
			     error ) == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_SEEK_FAILED,
				 "%s: unable to find current offset: %" PRIi64 " in handle.",
				 function,
				 internal_handle->offset );

				return( -1 );
			}
		}
	}
	read_count = internal_handle->read(
	              internal_handle->io_handle,
	              buffer,
	              size,
	              error );

	if( read_count < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read from handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->track_offsets_read != 0 )
	{
		if( libcdata_range_list_append_range(
		     internal_handle->offsets_read,
		     (uint64_t) internal_handle->offset,
		     (uint64_t) read_count,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append offset range to offsets read table.",
			 function );

			return( -1 );
		}
	}
	internal_handle->offset += (off64_t) read_count;

	if( internal_handle->open_on_demand != 0 )
	{
		if( internal_handle->close == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid handle - missing close function.",
			 function );

			return( -1 );
		}
		if( internal_handle->close(
		     internal_handle->io_handle,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to close handle on demand.",
			 function );

			return( -1 );
		}
	}
	return( read_count );
}

/* Writes a buffer to the handle
 * Returns the number of bytes written if successful, or -1 on error
 */
ssize_t libbfio_handle_write(
         libbfio_handle_t *handle,
         const uint8_t *buffer,
         size_t size,
         libcerror_error_t **error )
{
	libbfio_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libbfio_handle_write";
	ssize_t write_count                        = 0;

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
	internal_handle = (libbfio_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->write == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing write function.",
		 function );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.",
		 function );

		return( -1 );
	}
	write_count = internal_handle->write(
	               internal_handle->io_handle,
	               buffer,
	               size,
	               error );

	if( write_count < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write to handle.",
		 function );

		return( -1 );
	}
	internal_handle->offset += (off64_t) write_count;

	if( (size64_t) internal_handle->offset > internal_handle->size )
	{
		internal_handle->size = (size64_t) internal_handle->offset;
	}
	return( write_count );
}

/* Reads a buffer from a handle in the pool
 * Returns the number of bytes read or -1 on error
 */
ssize_t libbfio_pool_read(
         libbfio_pool_t *pool,
         int entry,
         uint8_t *buffer,
         size_t size,
         libcerror_error_t **error )
{
	libbfio_internal_pool_t *internal_pool = NULL;
	static char *function                  = "libbfio_pool_read";
	ssize_t read_count                     = 0;
	int access_flags                       = 0;
	int is_open                            = 0;

	if( pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid pool.",
		 function );

		return( -1 );
	}
	internal_pool = (libbfio_internal_pool_t *) pool;

	if( internal_pool->handles == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid pool - missing handles.",
		 function );

		return( -1 );
	}
	if( ( entry < 0 )
	 || ( entry >= internal_pool->number_of_handles ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid entry.",
		 function );

		return( -1 );
	}
	/* Make sure the handle is open
	 */
	is_open = libbfio_handle_is_open(
	           internal_pool->handles[ entry ],
	           error );

	if( is_open == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if entry: %d is open.",
		 function,
	         entry );

		return( -1 );
	}
	else if( is_open == 0 )
	{
		if( libbfio_handle_get_access_flags(
		     internal_pool->handles[ entry ],
		     &access_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve access flags.",
			 function );

			return( -1 );
		}
		if( libbfio_pool_open_handle(
		     internal_pool,
		     internal_pool->handles[ entry ],
		     access_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to open entry: %d.",
			 function,
			 entry );

			return( -1 );
		}
	}
	if( internal_pool->maximum_number_of_open_handles != LIBBFIO_POOL_UNLIMITED_NUMBER_OF_OPEN_HANDLES )
	{
		if( libbfio_pool_move_handle_to_front_of_last_used_list(
		     internal_pool,
		     internal_pool->handles[ entry ],
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to move handle to front of last used list.",
			 function );

			return( -1 );
		}
	}
	read_count = libbfio_handle_read_buffer(
	              internal_pool->handles[ entry ],
	              buffer,
	              size,
	              error );

	if( read_count < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read from entry: %d.",
		 function,
		 entry );

		return( -1 );
	}
	return( read_count );
}

/* Writes a buffer to a handle in the pool
 * Returns the number of bytes written or -1 on error
 */
ssize_t libbfio_pool_write(
         libbfio_pool_t *pool,
         int entry,
         const uint8_t *buffer,
         size_t size,
         libcerror_error_t **error )
{
	libbfio_internal_pool_t *internal_pool = NULL;
	static char *function                  = "libbfio_pool_write";
	ssize_t write_count                    = 0;
	int access_flags                       = 0;
	int is_open                            = 0;

	if( pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid pool.",
		 function );

		return( -1 );
	}
	internal_pool = (libbfio_internal_pool_t *) pool;

	if( internal_pool->handles == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid pool - missing handles.",
		 function );

		return( -1 );
	}
	if( ( entry < 0 )
	 || ( entry >= internal_pool->number_of_handles ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid entry.",
		 function );

		return( -1 );
	}
	/* Make sure the handle is open
	 */
	is_open = libbfio_handle_is_open(
	           internal_pool->handles[ entry ],
	           error );

	if( is_open == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if entry: %d is open.",
		 function,
	         entry );

		return( -1 );
	}
	else if( is_open == 0 )
	{
		if( libbfio_handle_get_access_flags(
		     internal_pool->handles[ entry ],
		     &access_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve access flags.",
			 function );

			return( -1 );
		}
		if( libbfio_pool_open_handle(
		     internal_pool,
		     internal_pool->handles[ entry ],
		     access_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to open entry: %d.",
			 function,
			 entry );

			return( -1 );
		}
	}
	if( internal_pool->maximum_number_of_open_handles != LIBBFIO_POOL_UNLIMITED_NUMBER_OF_OPEN_HANDLES )
	{
		if( libbfio_pool_move_handle_to_front_of_last_used_list(
		     internal_pool,
		     internal_pool->handles[ entry ],
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to move handle to front of last used list.",
			 function );

			return( -1 );
		}
	}
	write_count = libbfio_handle_write_buffer(
	               internal_pool->handles[ entry ],
	               buffer,
	               size,
	               error );

	if( write_count < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write to entry: %d.",
		 function,
		 entry );

		return( -1 );
	}
	return( write_count );
}

#endif /* !defined( HAVE_LOCAL_LIBBFIO ) */

