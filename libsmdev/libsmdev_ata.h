/*
 * ATA/ATAPI functions
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

#if !defined( _LIBSMDEV_ATA_H )
#define _LIBSMDEV_ATA_H

#include <common.h>
#include <types.h>

#if defined( HAVE_CYGWIN_HDREG_H )
#include <cygwin/hdreg.h>
#endif

#if defined( HAVE_LINUX_HDREG_H )
#include <linux/hdreg.h>
#endif

#include "libsmdev_libcerror.h"
#include "libsmdev_libcfile.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HDIO_GET_IDENTITY )
int libsmdev_ata_get_device_configuration(
     libcfile_file_t *device_file,
     struct hd_driveid *device_configuration,
     libcerror_error_t **error );
#endif

#if defined( __cplusplus )
}
#endif

#endif

