/*
 * EWF hash section
 *
 * Copyright (c) 2006-2013, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _EWF_HASH_H )
#define _EWF_HASH_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct ewf_hash ewf_hash_t;

struct ewf_hash
{
	/* The MD5 hash of the aquired data
	 * consists of 16 bytes
	 */
	uint8_t md5_hash[ 16 ];

	/* Unknown
	 */
	uint8_t unknown1[ 16 ];

	/* The section checksum of all (previous) hash data
	 * consists of 4 bytes (32 bits)
	 * starts with offset 76
	 */
	uint8_t checksum[ 4 ];
};

#if defined( __cplusplus )
}
#endif

#endif

