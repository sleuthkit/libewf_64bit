/*
 * Byte stream functions
 *
 * Copyright (c) 2011-2013, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBHMAC_BYTE_STREAM_H )
#define _LIBHMAC_BYTE_STREAM_H

#include <common.h>
#include <byte_stream.h>

#if defined( __cplusplus )
extern "C" {
#endif

#define libhmac_byte_stream_copy_to_16x_uint32_big_endian( byte_stream, values ) \
	values[ 0 ]   = ( byte_stream )[ 0 ]; \
	values[ 0 ] <<= 8; \
	values[ 0 ]  |= ( byte_stream )[ 1 ]; \
	values[ 0 ] <<= 8; \
	values[ 0 ]  |= ( byte_stream )[ 2 ]; \
	values[ 0 ] <<= 8; \
	values[ 0 ]  |= ( byte_stream )[ 3 ]; \
 \
	values[ 1 ]   = ( byte_stream )[ 4 ]; \
	values[ 1 ] <<= 8; \
	values[ 1 ]  |= ( byte_stream )[ 5 ]; \
	values[ 1 ] <<= 8; \
	values[ 1 ]  |= ( byte_stream )[ 6 ]; \
	values[ 1 ] <<= 8; \
	values[ 1 ]  |= ( byte_stream )[ 7 ]; \
 \
	values[ 2 ]   = ( byte_stream )[ 8 ]; \
	values[ 2 ] <<= 8; \
	values[ 2 ]  |= ( byte_stream )[ 9 ]; \
	values[ 2 ] <<= 8; \
	values[ 2 ]  |= ( byte_stream )[ 10 ]; \
	values[ 2 ] <<= 8; \
	values[ 2 ]  |= ( byte_stream )[ 11 ]; \
 \
	values[ 3 ]   = ( byte_stream )[ 12 ]; \
	values[ 3 ] <<= 8; \
	values[ 3 ]  |= ( byte_stream )[ 13 ]; \
	values[ 3 ] <<= 8; \
	values[ 3 ]  |= ( byte_stream )[ 14 ]; \
	values[ 3 ] <<= 8; \
	values[ 3 ]  |= ( byte_stream )[ 15 ]; \
 \
	values[ 4 ]   = ( byte_stream )[ 16 ]; \
	values[ 4 ] <<= 8; \
	values[ 4 ]  |= ( byte_stream )[ 17 ]; \
	values[ 4 ] <<= 8; \
	values[ 4 ]  |= ( byte_stream )[ 18 ]; \
	values[ 4 ] <<= 8; \
	values[ 4 ]  |= ( byte_stream )[ 19 ]; \
 \
	values[ 5 ]   = ( byte_stream )[ 20 ]; \
	values[ 5 ] <<= 8; \
	values[ 5 ]  |= ( byte_stream )[ 21 ]; \
	values[ 5 ] <<= 8; \
	values[ 5 ]  |= ( byte_stream )[ 22 ]; \
	values[ 5 ] <<= 8; \
	values[ 5 ]  |= ( byte_stream )[ 23 ]; \
 \
	values[ 6 ]   = ( byte_stream )[ 24 ]; \
	values[ 6 ] <<= 8; \
	values[ 6 ]  |= ( byte_stream )[ 25 ]; \
	values[ 6 ] <<= 8; \
	values[ 6 ]  |= ( byte_stream )[ 26 ]; \
	values[ 6 ] <<= 8; \
	values[ 6 ]  |= ( byte_stream )[ 27 ]; \
 \
	values[ 7 ]   = ( byte_stream )[ 28 ]; \
	values[ 7 ] <<= 8; \
	values[ 7 ]  |= ( byte_stream )[ 29 ]; \
	values[ 7 ] <<= 8; \
	values[ 7 ]  |= ( byte_stream )[ 30 ]; \
	values[ 7 ] <<= 8; \
	values[ 7 ]  |= ( byte_stream )[ 31 ]; \
 \
	values[ 8 ]   = ( byte_stream )[ 32 ]; \
	values[ 8 ] <<= 8; \
	values[ 8 ]  |= ( byte_stream )[ 33 ]; \
	values[ 8 ] <<= 8; \
	values[ 8 ]  |= ( byte_stream )[ 34 ]; \
	values[ 8 ] <<= 8; \
	values[ 8 ]  |= ( byte_stream )[ 35 ]; \
 \
	values[ 9 ]   = ( byte_stream )[ 36 ]; \
	values[ 9 ] <<= 8; \
	values[ 9 ]  |= ( byte_stream )[ 37 ]; \
	values[ 9 ] <<= 8; \
	values[ 9 ]  |= ( byte_stream )[ 38 ]; \
	values[ 9 ] <<= 8; \
	values[ 9 ]  |= ( byte_stream )[ 39 ]; \
 \
	values[ 10 ]   = ( byte_stream )[ 40 ]; \
	values[ 10 ] <<= 8; \
	values[ 10 ]  |= ( byte_stream )[ 41 ]; \
	values[ 10 ] <<= 8; \
	values[ 10 ]  |= ( byte_stream )[ 42 ]; \
	values[ 10 ] <<= 8; \
	values[ 10 ]  |= ( byte_stream )[ 43 ]; \
 \
	values[ 11 ]   = ( byte_stream )[ 44 ]; \
	values[ 11 ] <<= 8; \
	values[ 11 ]  |= ( byte_stream )[ 45 ]; \
	values[ 11 ] <<= 8; \
	values[ 11 ]  |= ( byte_stream )[ 46 ]; \
	values[ 11 ] <<= 8; \
	values[ 11 ]  |= ( byte_stream )[ 47 ]; \
 \
	values[ 12 ]   = ( byte_stream )[ 48 ]; \
	values[ 12 ] <<= 8; \
	values[ 12 ]  |= ( byte_stream )[ 49 ]; \
	values[ 12 ] <<= 8; \
	values[ 12 ]  |= ( byte_stream )[ 50 ]; \
	values[ 12 ] <<= 8; \
	values[ 12 ]  |= ( byte_stream )[ 51 ]; \
 \
	values[ 13 ]   = ( byte_stream )[ 52 ]; \
	values[ 13 ] <<= 8; \
	values[ 13 ]  |= ( byte_stream )[ 53 ]; \
	values[ 13 ] <<= 8; \
	values[ 13 ]  |= ( byte_stream )[ 54 ]; \
	values[ 13 ] <<= 8; \
	values[ 13 ]  |= ( byte_stream )[ 55 ]; \
 \
	values[ 14 ]   = ( byte_stream )[ 56 ]; \
	values[ 14 ] <<= 8; \
	values[ 14 ]  |= ( byte_stream )[ 57 ]; \
	values[ 14 ] <<= 8; \
	values[ 14 ]  |= ( byte_stream )[ 58 ]; \
	values[ 14 ] <<= 8; \
	values[ 14 ]  |= ( byte_stream )[ 59 ]; \
 \
	values[ 15 ]   = ( byte_stream )[ 60 ]; \
	values[ 15 ] <<= 8; \
	values[ 15 ]  |= ( byte_stream )[ 61 ]; \
	values[ 15 ] <<= 8; \
	values[ 15 ]  |= ( byte_stream )[ 62 ]; \
	values[ 15 ] <<= 8; \
	values[ 15 ]  |= ( byte_stream )[ 63 ];

#define libhmac_byte_stream_copy_to_16x_uint32_little_endian( byte_stream, values ) \
	values[ 0 ]   = ( byte_stream )[ 3 ]; \
	values[ 0 ] <<= 8; \
	values[ 0 ]  |= ( byte_stream )[ 2 ]; \
	values[ 0 ] <<= 8; \
	values[ 0 ]  |= ( byte_stream )[ 1 ]; \
	values[ 0 ] <<= 8; \
	values[ 0 ]  |= ( byte_stream )[ 0 ]; \
 \
	values[ 1 ]   = ( byte_stream )[ 7 ]; \
	values[ 1 ] <<= 8; \
	values[ 1 ]  |= ( byte_stream )[ 6 ]; \
	values[ 1 ] <<= 8; \
	values[ 1 ]  |= ( byte_stream )[ 5 ]; \
	values[ 1 ] <<= 8; \
	values[ 1 ]  |= ( byte_stream )[ 4 ]; \
 \
	values[ 2 ]   = ( byte_stream )[ 11 ]; \
	values[ 2 ] <<= 8; \
	values[ 2 ]  |= ( byte_stream )[ 10 ]; \
	values[ 2 ] <<= 8; \
	values[ 2 ]  |= ( byte_stream )[ 9 ]; \
	values[ 2 ] <<= 8; \
	values[ 2 ]  |= ( byte_stream )[ 8 ]; \
 \
	values[ 3 ]   = ( byte_stream )[ 15 ]; \
	values[ 3 ] <<= 8; \
	values[ 3 ]  |= ( byte_stream )[ 14 ]; \
	values[ 3 ] <<= 8; \
	values[ 3 ]  |= ( byte_stream )[ 13 ]; \
	values[ 3 ] <<= 8; \
	values[ 3 ]  |= ( byte_stream )[ 12 ]; \
 \
	values[ 4 ]   = ( byte_stream )[ 19 ]; \
	values[ 4 ] <<= 8; \
	values[ 4 ]  |= ( byte_stream )[ 18 ]; \
	values[ 4 ] <<= 8; \
	values[ 4 ]  |= ( byte_stream )[ 17 ]; \
	values[ 4 ] <<= 8; \
	values[ 4 ]  |= ( byte_stream )[ 16 ]; \
 \
	values[ 5 ]   = ( byte_stream )[ 23 ]; \
	values[ 5 ] <<= 8; \
	values[ 5 ]  |= ( byte_stream )[ 22 ]; \
	values[ 5 ] <<= 8; \
	values[ 5 ]  |= ( byte_stream )[ 21 ]; \
	values[ 5 ] <<= 8; \
	values[ 5 ]  |= ( byte_stream )[ 20 ]; \
 \
	values[ 6 ]   = ( byte_stream )[ 27 ]; \
	values[ 6 ] <<= 8; \
	values[ 6 ]  |= ( byte_stream )[ 26 ]; \
	values[ 6 ] <<= 8; \
	values[ 6 ]  |= ( byte_stream )[ 25 ]; \
	values[ 6 ] <<= 8; \
	values[ 6 ]  |= ( byte_stream )[ 24 ]; \
 \
	values[ 7 ]   = ( byte_stream )[ 31 ]; \
	values[ 7 ] <<= 8; \
	values[ 7 ]  |= ( byte_stream )[ 30 ]; \
	values[ 7 ] <<= 8; \
	values[ 7 ]  |= ( byte_stream )[ 29 ]; \
	values[ 7 ] <<= 8; \
	values[ 7 ]  |= ( byte_stream )[ 28 ]; \
 \
	values[ 8 ]   = ( byte_stream )[ 35 ]; \
	values[ 8 ] <<= 8; \
	values[ 8 ]  |= ( byte_stream )[ 34 ]; \
	values[ 8 ] <<= 8; \
	values[ 8 ]  |= ( byte_stream )[ 33 ]; \
	values[ 8 ] <<= 8; \
	values[ 8 ]  |= ( byte_stream )[ 32 ]; \
 \
	values[ 9 ]   = ( byte_stream )[ 39 ]; \
	values[ 9 ] <<= 8; \
	values[ 9 ]  |= ( byte_stream )[ 38 ]; \
	values[ 9 ] <<= 8; \
	values[ 9 ]  |= ( byte_stream )[ 37 ]; \
	values[ 9 ] <<= 8; \
	values[ 9 ]  |= ( byte_stream )[ 36 ]; \
 \
	values[ 10 ]   = ( byte_stream )[ 43 ]; \
	values[ 10 ] <<= 8; \
	values[ 10 ]  |= ( byte_stream )[ 42 ]; \
	values[ 10 ] <<= 8; \
	values[ 10 ]  |= ( byte_stream )[ 41 ]; \
	values[ 10 ] <<= 8; \
	values[ 10 ]  |= ( byte_stream )[ 40 ]; \
 \
	values[ 11 ]   = ( byte_stream )[ 47 ]; \
	values[ 11 ] <<= 8; \
	values[ 11 ]  |= ( byte_stream )[ 46 ]; \
	values[ 11 ] <<= 8; \
	values[ 11 ]  |= ( byte_stream )[ 45 ]; \
	values[ 11 ] <<= 8; \
	values[ 11 ]  |= ( byte_stream )[ 44 ]; \
 \
	values[ 12 ]   = ( byte_stream )[ 51 ]; \
	values[ 12 ] <<= 8; \
	values[ 12 ]  |= ( byte_stream )[ 50 ]; \
	values[ 12 ] <<= 8; \
	values[ 12 ]  |= ( byte_stream )[ 49 ]; \
	values[ 12 ] <<= 8; \
	values[ 12 ]  |= ( byte_stream )[ 48 ]; \
 \
	values[ 13 ]   = ( byte_stream )[ 55 ]; \
	values[ 13 ] <<= 8; \
	values[ 13 ]  |= ( byte_stream )[ 54 ]; \
	values[ 13 ] <<= 8; \
	values[ 13 ]  |= ( byte_stream )[ 53 ]; \
	values[ 13 ] <<= 8; \
	values[ 13 ]  |= ( byte_stream )[ 52 ]; \
 \
	values[ 14 ]   = ( byte_stream )[ 59 ]; \
	values[ 14 ] <<= 8; \
	values[ 14 ]  |= ( byte_stream )[ 58 ]; \
	values[ 14 ] <<= 8; \
	values[ 14 ]  |= ( byte_stream )[ 57 ]; \
	values[ 14 ] <<= 8; \
	values[ 14 ]  |= ( byte_stream )[ 56 ]; \
 \
	values[ 15 ]   = ( byte_stream )[ 63 ]; \
	values[ 15 ] <<= 8; \
	values[ 15 ]  |= ( byte_stream )[ 62 ]; \
	values[ 15 ] <<= 8; \
	values[ 15 ]  |= ( byte_stream )[ 61 ]; \
	values[ 15 ] <<= 8; \
	values[ 15 ]  |= ( byte_stream )[ 60 ];

#if defined( __cplusplus )
}
#endif

#endif

