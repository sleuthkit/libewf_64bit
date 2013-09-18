/*
 * Configuration file for Microsoft Visual Studio C++ compiler
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

#if !defined( _CONFIG_MSC_H )
#define _CONFIG_MSC_H

/* Define to the address where bug reports for this package should be sent.
 */
#define PACKAGE_BUGREPORT "joachim.metz@gmail.com"

/* Define the size of the wide character for WINAPI
 */
#if !defined( SIZEOF_WCHAR_T )
#define SIZEOF_WCHAR_T          2
#endif

/* Use the C Runtime (CRT) functions instead of the WINAPI functions
#define USE_CRT_FUNCTIONS	1
 */

/* Enable verbose output
#define HAVE_VERBOSE_OUTPUT	1
 */

/* Enable debug output
#define HAVE_DEBUG_OUTPUT	1
 */

/* Disable warning C4127: conditional expression is constant
 */
#pragma warning( disable : 4127 )

/* Disable warning C4201: nonstandard extension used : nameless struct/union
 */
#pragma warning( disable : 4201 )

/* Enable the v1 API
#define HAVE_V1_API		1
 */

#endif

