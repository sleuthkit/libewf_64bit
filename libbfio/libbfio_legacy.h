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

#if !defined( _LIBBFIO_LEGACY_H )
#define _LIBBFIO_LEGACY_H

#include <common.h>
#include <types.h>

#include "libbfio_extern.h"
#include "libbfio_libcerror.h"
#include "libbfio_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if !defined( HAVE_LOCAL_LIBBFIO )

LIBBFIO_EXTERN \
ssize_t libbfio_handle_read(
         libbfio_handle_t *handle,
         uint8_t *buffer,
         size_t size,
         libcerror_error_t **error );

LIBBFIO_EXTERN \
ssize_t libbfio_handle_write(
         libbfio_handle_t *handle,
         const uint8_t *buffer,
         size_t size,
         libcerror_error_t **error );

LIBBFIO_EXTERN \
ssize_t libbfio_pool_read(
         libbfio_pool_t *pool,
         int entry,
         uint8_t *buffer,
         size_t size,
         libcerror_error_t **error );

LIBBFIO_EXTERN \
ssize_t libbfio_pool_write(
         libbfio_pool_t *pool,
         int entry,
         const uint8_t *buffer,
         size_t size,
         libcerror_error_t **error );

#endif /* !defined( HAVE_LOCAL_LIBBFIO ) */

#if defined( __cplusplus )
}
#endif

#endif

