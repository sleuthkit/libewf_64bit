/*
 * Error functions
 *
 * Copyright (c) 2010-2012, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBSMRAW_INTERNAL_ERROR_H )
#define _LIBSMRAW_INTERNAL_ERROR_H

#include <common.h>
#include <file_stream.h>
#include <types.h>

#if !defined( HAVE_LOCAL_LIBSMRAW )
#include <libsmraw/error.h>
#endif

#include "libsmraw_extern.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if !defined( HAVE_LOCAL_LIBSMRAW )

LIBSMRAW_EXTERN \
void libsmraw_error_free(
      libsmraw_error_t **error );

LIBSMRAW_EXTERN \
int libsmraw_error_fprint(
     libsmraw_error_t *error,
     FILE *stream );

LIBSMRAW_EXTERN \
int libsmraw_error_sprint(
     libsmraw_error_t *error,
     char *string,
     size_t size );

LIBSMRAW_EXTERN \
int libsmraw_error_backtrace_fprint(
     libsmraw_error_t *error,
     FILE *stream );

LIBSMRAW_EXTERN \
int libsmraw_error_backtrace_sprint(
     libsmraw_error_t *error,
     char *string,
     size_t size );

#endif

#if defined( __cplusplus )
}
#endif

#endif

