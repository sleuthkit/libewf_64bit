/*
 * The internal definitions
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

#if !defined( _LIBCLOCALE_INTERNAL_DEFINITIONS_H )
#define _LIBCLOCALE_INTERNAL_DEFINITIONS_H

#include <common.h>

/* Define HAVE_LOCAL_LIBCLOCALE for local use of libclocale
 */
#if !defined( HAVE_LOCAL_LIBCLOCALE )
#include <libclocale/definitions.h>

/* The definitions in <libclocale/definitions.h> are copied here
 * for local use of libclocale
 */
#else

#define LIBCLOCALE_VERSION			20130103

/* The libclocale version string
 */
#define LIBCLOCALE_VERSION_STRING		"20130103"

#endif

#endif

