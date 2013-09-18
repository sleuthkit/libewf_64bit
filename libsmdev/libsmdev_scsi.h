/*
 * SCSI functions
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

#if !defined( _LIBSMDEV_SCSI_H )
#define _LIBSMDEV_SCSI_H

#include <common.h>
#include <types.h>

#include "libsmdev_libcerror.h"
#include "libsmdev_libcfile.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_SCSI_SG_H )

enum LIBSMDEV_SCSI_OPERATION_CODES
{
	LIBSMDEV_SCSI_OPERATION_CODE_INQUIRY				= 0x12,

	LIBSMDEV_SCSI_OPERATION_CODE_READ_TOC				= 0x43,

	LIBSMDEV_SCSI_OPERATION_CODE_READ_DISK_INFORMATION		= 0x51,
	LIBSMDEV_SCSI_OPERATION_CODE_READ_TRACK_INFORMATION		= 0x52,
};

/* SCSI device types:
 * 0x00      - direct-access device (e.g., magnetic disk)
 * 0x01      - sequential-access device (e.g., magnetic tape)
 * 0x02      - printer device
 * 0x03      - processor device
 * 0x04      - write-once device
 * 0x05      - optical disk (CDROM) device
 * 0x06      - scanner device
 * 0x07      - optical memory device (e.g., some optical disks)
 * 0x08      - medium Changer (e.g. jukeboxes)
 * 0x09      - communications device
 * 0x0a-0x0b - defined by ASC IT8 (Graphic arts pre-press devices)
 * 0x0c      - Storage array controller device (e.g., RAID)
 * 0x0d      - Enclosure services device
 * 0x0e      - Simplified direct-access device (e.g., magnetic disk)
 * 0x0f      - Optical card reader/writer device
 * 0x10      - Reserved for bridging expanders
 * 0x11      - Object-based Storage Device
 * 0x12      - Automation/Drive Interface
 * 0x13-0x1d - reserved
 * 0x1e      - Well known logical unit
 * 0x1f      - unknown or no device type
 */

/* The SCSI inquiry command descriptor block (CDB)
 */
typedef struct libsmdev_scsi_inquiry_cdb libsmdev_scsi_inquiry_cdb_t;

struct libsmdev_scsi_inquiry_cdb
{
	/* The operation code
	 */
	uint8_t operation_code;

	/* The LUN and reserved bits
	 * Bits:
	 * 0 - 4 reserved
	 * 5 - 7 LUN
	 */
	uint8_t lun;

	/* Reserved
	 */
	uint8_t reserved1;

	/* Reserved
	 */
	uint8_t reserved2;

	/* The size of the receive buffer (allocation length)
	 */
	uint8_t receive_size;

	/* The control byte
	 */
	uint8_t control;
};

/* The SCSI table of contents (TOC) command descriptor block (CDB)
 * format definitions
 */
enum LIBSMDEV_SCSI_TOC_CDB_FORMATS
{
	LIBSMDEV_SCSI_TOC_CDB_FORMAT_FORMATTED_TOC			= 0x00,
	LIBSMDEV_SCSI_TOC_CDB_FORMAT_RAW_TOC				= 0x02,
};

/* The SCSI read table of contents (TOC) command descriptor block (CDB)
 */
typedef struct libsmdev_scsi_read_toc_cdb libsmdev_scsi_read_toc_cdb_t;

struct libsmdev_scsi_read_toc_cdb
{
	/* The operation code
	 */
	uint8_t operation_code;

	/* The MSF bit and reserved bits
	 * Bits:
	 * 0     reserved
	 * 1     MSF
	 * 2 - 7 reserved
	 */
	uint8_t msf;

	/* The format and reserved bits
	 * Bits:
	 * 0 - 3 format
	 * 4 - 7 reserved
	 */
	uint8_t format;

	/* Reserved
	 */
	uint8_t reserved[ 3 ];

	/* TOC entry index
	 */
	uint8_t entry_index;

	/* The size of the receive buffer (allocation length)
	 * Contains a 16-bit big-endian value
	 */
	uint8_t receive_size[ 2 ];

	/* The control byte
	 */
	uint8_t control;
};

/* The SCSI read disc information command descriptor block (CDB)
 */
typedef struct libsmdev_scsi_read_disc_information_cdb libsmdev_scsi_read_disc_information_cdb_t;

struct libsmdev_scsi_read_disc_information_cdb
{
	/* The operation code
	 */
	uint8_t operation_code;

	/* The data type and reserved bits
	 * Bits:
	 * 0 - 3 data type
	 * 4 - 7 reserved
	 */
	uint8_t data_type;

	/* Reserved
	 */
	uint8_t reserved[ 5 ];

	/* The size of the receive buffer (allocation length)
	 * Contains a 16-bit big-endian value
	 */
	uint8_t receive_size[ 2 ];

	/* The control byte
	 */
	uint8_t control;
};

/* The SCSI track information command descriptor block (CDB)
 * address type definitions
 */
enum LIBSMDEV_SCSI_TRACK_INFORMATION_ADDRESS_TYPES
{
	LIBSMDEV_SCSI_TRACK_INFORMATION_ADDRESS_TYPE_LBA		= 0x00,
	LIBSMDEV_SCSI_TRACK_INFORMATION_ADDRESS_TYPE_TRACK_NUMBER	= 0x01,
	LIBSMDEV_SCSI_TRACK_INFORMATION_ADDRESS_TYPE_SESSION_NUMBER	= 0x02,
};

/* The SCSI read track information command descriptor block (CDB)
 */
typedef struct libsmdev_scsi_read_track_information_cdb libsmdev_scsi_read_track_information_cdb_t;

struct libsmdev_scsi_read_track_information_cdb
{
	/* The operation code
	 */
	uint8_t operation_code;

	/* The address type, flag and reserved bits
	 * Bits:
	 * 0 - 1 address type
	 * 2     open flag
	 * 3 - 7 reserved
	 */
	uint8_t address_type;

	/* The (track) offset
	 * Contains a 32-bit big-endian value
	 */
	uint8_t offset[ 4 ];

	/* Reserved
	 */
	uint8_t reserved;

	/* The size of the receive buffer (allocation length)
	 * Contains a 16-bit big-endian value
	 */
	uint8_t receive_size[ 2 ];

	/* The control byte
	 */
	uint8_t control;
};

/* The SCSI ioctrl header
 */
typedef struct libsmdev_scsi_ioctrl_header libsmdev_scsi_ioctrl_header_t;

struct libsmdev_scsi_ioctrl_header
{
	/* The size of the request (input data)
	 */
	unsigned int request_size;

	/* The size out the response (output data)
	 */
	unsigned int response_size;

	/* The request/response data
	 */
};

#define LIBSMDEV_SCSI_RESPONSE_SIZE_INQUIRY	96
#define LIBSMDEV_SCSI_SENSE_SIZE		32

int libsmdev_scsi_command(
     libcfile_file_t *device_file,
     uint8_t *command,
     size_t command_size,
     uint8_t *response,
     size_t response_size,
     uint8_t *sense,
     size_t sense_size,
     libcerror_error_t **error );

int libsmdev_scsi_ioctrl(
     libcfile_file_t *device_file,
     void *request_data,
     size_t request_data_size,
     libcerror_error_t **error );

ssize_t libsmdev_scsi_inquiry(
         libcfile_file_t *device_file,
         uint8_t inquiry_vital_product_data,
         uint8_t code_page,
         uint8_t *response,
         size_t response_size,
         libcerror_error_t **error );

ssize_t libsmdev_scsi_read_toc(
         libcfile_file_t *device_file,
         uint8_t format,
         uint8_t *response,
         size_t response_size,
         libcerror_error_t **error );

ssize_t libsmdev_scsi_read_disc_information(
         libcfile_file_t *device_file,
         uint8_t *response,
         size_t response_size,
         libcerror_error_t **error );

ssize_t libsmdev_scsi_read_track_information(
         libcfile_file_t *device_file,
         uint32_t offset,
         uint8_t *response,
         size_t response_size,
         libcerror_error_t **error );

int libsmdev_scsi_get_identier(
     libcfile_file_t *device_file,
     libcerror_error_t **error );

int libsmdev_scsi_get_bus_type(
     libcfile_file_t *device_file,
     uint8_t *bus_type,
     libcerror_error_t **error );

int libsmdev_scsi_get_pci_bus_address(
     libcfile_file_t *device_file,
     uint8_t *pci_bus_address,
     size_t pci_bus_address_size,
     libcerror_error_t **error );

#endif

#if defined( __cplusplus )
}
#endif

#endif

