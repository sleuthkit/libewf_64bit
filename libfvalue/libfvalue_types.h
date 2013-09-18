/*
 * The internal type definitions
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

#if !defined( _LIBFVALUE_TYPES_H )
#define _LIBFVALUE_TYPES_H

#include <common.h>
#include <types.h>

/* External value type definition hides internal structure
 */
#if defined( HAVE_DEBUG_OUTPUT ) && !defined( WINAPI )
typedef struct libfvalue_data_handle {}		libfvalue_data_handle_t;
typedef struct libfvalue_split_utf8_string {}	libfvalue_split_utf8_string_t;
typedef struct libfvalue_split_utf16_string {}	libfvalue_split_utf16_string_t;
typedef struct libfvalue_table {}		libfvalue_table_t;
typedef struct libfvalue_value {}		libfvalue_value_t;

#else
typedef intptr_t libfvalue_data_handle_t;
typedef intptr_t libfvalue_split_utf8_string_t;
typedef intptr_t libfvalue_split_utf16_string_t;
typedef intptr_t libfvalue_table_t;
typedef intptr_t libfvalue_value_t;

#endif

#endif

