/*
 * Optical disk functions
 *
 * Copyright (c) 2010-2013, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined( _LIBSMDEV_OPTICAL_DISK_H )
#define _LIBSMDEV_OPTICAL_DISK_H

#include <common.h>
#include <types.h>

#include "libsmdev_handle.h"
#include "libsmdev_libcerror.h"
#include "libsmdev_libcfile.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_LINUX_CDROM_H )

int libsmdev_optical_disc_get_table_of_contents(
     libcfile_file_t *device_file,
     libsmdev_internal_handle_t *internal_handle,
     libcerror_error_t **error );

int libsmdev_optical_disc_get_table_of_contents_scsi(
     libcfile_file_t *device_file,
     libsmdev_internal_handle_t *internal_handle,
     libcerror_error_t **error );

int libsmdev_optical_disc_get_table_of_contents_ioctl(
     libcfile_file_t *device_file,
     libsmdev_internal_handle_t *internal_handle,
     libcerror_error_t **error );

#endif

#if defined( __cplusplus )
}
#endif

#endif

