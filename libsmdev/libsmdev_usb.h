/*
 * USB functions
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

#if !defined( _LIBSMDEV_USB_H )
#define _LIBSMDEV_USB_H

#include <common.h>
#include <types.h>

#include "libsmdev_libcerror.h"
#include "libsmdev_libcfile.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_LINUX_USB_CH9_H )

int libsmdev_usb_ioctl(
     libcfile_file_t *device_file,
     int interface_number,
     int request,
     void *request_data,
     libcerror_error_t **error );

int libsmdev_usb_control_command(
     libcfile_file_t *device_file,
     uint8_t request_type,
     uint8_t request,
     uint16_t value,
     uint16_t index,
     uint8_t *buffer,
     size_t buffer_size,
     libcerror_error_t **error );

int libsmdev_usb_test(
     libcfile_file_t *device_file,
     libcerror_error_t **error );

#endif

#if defined( __cplusplus )
}
#endif

#endif

