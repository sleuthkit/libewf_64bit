/*
 * The internal libodraw header
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

#if !defined( _EWFTOOLS_LIBODRAW_H )
#define _EWFTOOLS_LIBODRAW_H

#include <common.h>

#if defined( HAVE_LOCAL_LIBODRAW )
#include <libodraw_data_file.h>
#include <libodraw_definitions.h>
#include <libodraw_handle.h>
#include <libodraw_metadata.h>
#include <libodraw_support.h>
#include <libodraw_types.h>

#elif defined( HAVE_LIBODRAW_H )
#include <libodraw.h>

#endif

#endif

