/*
 * Date and time functions
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

#if !defined( _LIBEWF_DATE_TIME_H )
#define _LIBEWF_DATE_TIME_H

#include <common.h>
#include <types.h>

#include "libewf_libcerror.h"

#if defined( TIME_WITH_SYS_TIME )
#include <sys/time.h>
#include <time.h>
#elif defined( HAVE_SYS_TIME_H )
#include <sys/time.h>
#else
#include <time.h>
#endif

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( WINAPI )
#define libewf_date_time_mktime( time_elements ) \
	mktime( time_elements )

#elif defined( HAVE_MKTIME )
#define libewf_date_time_mktime( time_elements ) \
	mktime( time_elements )

#else
#error Missing mktime function
#endif

int libewf_date_time_localtime(
     const time_t *timestamp,
     struct tm *time_elements,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

