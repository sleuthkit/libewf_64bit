/*
 * Debug functions
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
#include <types.h>

#include "libsmdev_debug.h"
#include "libsmdev_libcnotify.h"

#if defined( HAVE_DEBUG_OUTPUT )

/* Prints a description of the device type
 */
const char *libsmdev_debug_print_device_type(
             uint8_t device_type )
{
	switch( device_type )
	{
		case 0x00:
			return(  "Direct access" );

		case 0x01:
			return( "Sequential access" );

		case 0x02:
			return( "Printer" );

		case 0x03:
			return( "Processor" );

		case 0x04:
			return( "Write-once" );

		case 0x05:
			return( "Optical disk (CD/DVD/BD)" );

		case 0x06:
			return( "Scanner" );

		case 0x07:
			return( "Optical memory" );

		case 0x08:
			return( "Medium changer" );

		case 0x09:
			return( "Communications" );

		case 0x0a:
		case 0x0b:
			return( "Graphic arts pre-press" );

		case 0x0c:
			return( "Storage array controller" );

		case 0x0d:
			return( "Enclosure services" );

		case 0x0e:
			return( "Simplified direct-access" );

		case 0x0f:
			return( "Optical card reader/writer" );

		case 0x10:
			return( "Bridging expander" );

		case 0x11:
			return( "Object-based Storage" );

		case 0x12:
			return( "Automation/Drive Interface" );

		case 0x13:
		case 0x14:
		case 0x15:
		case 0x16:
		case 0x17:
		case 0x18:
		case 0x1a:
		case 0x1b:
		case 0x1c:
		case 0x1d:
			 return( "Reserved" );

		case 0x1e:
			return( "Well known logical unit" );

		default:
			break;
	}
	return( "Unknown" );
}

#endif /* defined( HAVE_DEBUG_OUTPUT ) */

