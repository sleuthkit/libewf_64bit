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

#include <common.h>
#include <memory.h>
#include <types.h>

/* Needed for _IOW amd _IOWR */
#if defined( HAVE_SYS_IOCTL_H )
#include <sys/ioctl.h>
#endif

#if defined( HAVE_LINUX_USBDEVICE_FS_H )
#include <linux/usbdevice_fs.h>
#endif

#if defined( HAVE_LINUX_USB_CH9_H )
#include <linux/usb/ch9.h>
#endif

#include "libsmdev_libcerror.h"
#include "libsmdev_libcfile.h"
#include "libsmdev_libcnotify.h"
#include "libsmdev_libcstring.h"
#include "libsmdev_scsi.h"
#include "libsmdev_usb.h"

/* Timeout in milli seconds: 1 second
 */
#define LIBSMDEV_USB_CONTROL_COMMAND_TIMEOUT	1000

#if defined( HAVE_LINUX_USB_CH9_H )

#if defined( USBDEVFS_IOCTL )

/* Sends an USB ioctl to the file descriptor
 * Returns 1 if successful or -1 on error
 */
int libsmdev_usb_ioctl(
     libcfile_file_t *device_file,
     int interface_number,
     int request,
     void *request_data,
     libcerror_error_t **error )
{
	struct usbdevfs_ioctl ioctl_request;

	static char *function = "libsmdev_usb_ioctl";
	ssize_t read_count    = 0;

	ioctl_request.ifno       = interface_number;
	ioctl_request.ioctl_code = request;
	ioctl_request.data       = request_data;

	read_count = libcfile_file_io_control_read(
	              device_file,
	              USBDEVFS_IOCTL,
	              NULL,
	              0,
	              (uint8_t *) &ioctl_request,
	              sizeof( struct usbdevfs_ioctl ),
	              error );

	if( read_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_IOCTL_FAILED,
		 "%s: unable to query device for: USBDEVFS_IOCTL.",
		 function );

		return( -1 );
	}
	return( 1 );
}

#endif /* defined( USBDEVFS_IOCTL ) */

#if defined( USBDEVFS_CONTROL )

/* Sends a USB control command to the file descriptor
 * Returns 1 if successful or -1 on error
 */
int libsmdev_usb_control_command(
     libcfile_file_t *device_file,
     uint8_t request_type,
     uint8_t request,
     uint16_t value,
     uint16_t index,
     uint8_t *buffer,
     size_t buffer_size,
     libcerror_error_t **error )
{
	struct usbdevfs_ctrltransfer control_request;

	static char *function = "libsmdev_usb_control_command";
	ssize_t read_count    = 0;

	if( buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( buffer_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid buffer size value exceeds maximum.",
		 function );

		return( -1 );
	}
	control_request.bRequestType = request_type;
	control_request.bRequest     = request;
	control_request.wValue       = value;
	control_request.wIndex       = index;
	control_request.wLength      = buffer_size;
	control_request.timeout      = LIBSMDEV_USB_CONTROL_COMMAND_TIMEOUT;
	control_request.data         = buffer;

	read_count = libcfile_file_io_control_read(
	              device_file,
	              USBDEVFS_CONTROL,
	              NULL,
	              0,
	              (uint8_t *) &control_request,
	              sizeof( struct usbdevfs_ctrltransfer ),
	              error );

	if( read_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_IOCTL_FAILED,
		 "%s: unable to query device for: USBDEVFS_CONTROL.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_print_data(
		 buffer,
		 buffer_size,
		 0 );
	}
#endif
	return( 1 );
}

#endif /* defined( USBDEVFS_CONTROL ) */

#if defined( USBDEVFS_CONNECTINFO )

/* Sends a USB ioctl to the file descriptor
 * Returns 1 if successful or -1 on error
 */
int libsmdev_usb_test(
     libcfile_file_t *device_file,
     libcerror_error_t **error )
{
	struct usbdevfs_connectinfo connection_information;

	static char *function = "libsmdev_usb_test";
	ssize_t read_count    = 0;

	read_count = libcfile_file_io_control_read(
	              device_file,
	              USBDEVFS_CONNECTINFO,
	              NULL,
	              0,
	              (uint8_t *) &connection_information,
	              sizeof( struct usbdevfs_connectinfo ),
	              error );

	if( read_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_IOCTL_FAILED,
		 "%s: unable to query device for: USBDEVFS_CONNECTINFO.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_print_data(
		 (uint8_t *) &connection_information,
		 sizeof( struct usbdevfs_connectinfo ),
		 0 );
	}
#endif
	return( 1 );
}

#endif /* #if defined( USBDEVFS_CONNECTINFO ) */

#ifdef TODO_TEST

/* Sends a USB control command to the file descriptor
 * Returns 1 if successful or -1 on error
 */
int libsmdev_usb_test(
     libcfile_file_t *device_file,
     libcerror_error_t **error )
{
	uint8_t buffer[ 255 ];

	struct usb_device_descriptor device_descriptor;

	static char *function = "libsmdev_usb_test";

	if( libsmdev_usb_control_command(
	     device_file,
	     USB_DIR_IN | USB_TYPE_STANDARD | USB_RECIP_DEVICE,
	     USB_REQ_GET_DESCRIPTOR,
	     USB_DT_DEVICE << 8,
	     0,
	     buffer,
	     255,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_IOCTL_FAILED,
		 "%s: unable to send control command.",
		 function );

		return( -1 );
	}
	return( 1 );
}

#endif /* TODO_TEST */

#endif

