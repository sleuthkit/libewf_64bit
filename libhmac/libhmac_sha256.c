/*
 * SHA256 functions
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

#include <common.h>
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#if defined( WINAPI )
#include <wincrypt.h>

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_SHA_H )
#include <openssl/sha.h>

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
#include <openssl/evp.h>

#endif

#include "libhmac_byte_stream.h"
#include "libhmac_definitions.h"
#include "libhmac_libcerror.h"
#include "libhmac_sha256.h"

#if !defined( LIBHMAC_HAVE_SHA256_SUPPORT )

/* FIPS 180-2 based SHA-256 functions
 */

/* The first 32-bits of the fractional parts of the square roots of the first 8 primes [ 2, 19 ]
 */
uint32_t libhmac_sha256_prime_square_roots[ 8 ] = {
	0x6a09e667UL, 0xbb67ae85UL, 0x3c6ef372UL, 0xa54ff53aUL,
	0x510e527fUL, 0x9b05688cUL, 0x1f83d9abUL, 0x5be0cd19UL
};

/* The first 32-bits of the fractional parts of the cube roots of the first 64 primes [ 2, 311 ]
 */
uint32_t libhmac_sha256_prime_cube_roots[ 64 ] = {
	0x428a2f98UL, 0x71374491UL, 0xb5c0fbcfUL, 0xe9b5dba5UL,
	0x3956c25bUL, 0x59f111f1UL, 0x923f82a4UL, 0xab1c5ed5UL,
	0xd807aa98UL, 0x12835b01UL, 0x243185beUL, 0x550c7dc3UL,
	0x72be5d74UL, 0x80deb1feUL, 0x9bdc06a7UL, 0xc19bf174UL,
	0xe49b69c1UL, 0xefbe4786UL, 0x0fc19dc6UL, 0x240ca1ccUL,
	0x2de92c6fUL, 0x4a7484aaUL, 0x5cb0a9dcUL, 0x76f988daUL,
	0x983e5152UL, 0xa831c66dUL, 0xb00327c8UL, 0xbf597fc7UL,
	0xc6e00bf3UL, 0xd5a79147UL, 0x06ca6351UL, 0x14292967UL,
	0x27b70a85UL, 0x2e1b2138UL, 0x4d2c6dfcUL, 0x53380d13UL,
	0x650a7354UL, 0x766a0abbUL, 0x81c2c92eUL, 0x92722c85UL,
	0xa2bfe8a1UL, 0xa81a664bUL, 0xc24b8b70UL, 0xc76c51a3UL,
	0xd192e819UL, 0xd6990624UL, 0xf40e3585UL, 0x106aa070UL,
	0x19a4c116UL, 0x1e376c08UL, 0x2748774cUL, 0x34b0bcb5UL,
	0x391c0cb3UL, 0x4ed8aa4aUL, 0x5b9cca4fUL, 0x682e6ff3UL,
	0x748f82eeUL, 0x78a5636fUL, 0x84c87814UL, 0x8cc70208UL,
	0x90befffaUL, 0xa4506cebUL, 0xbef9a3f7UL, 0xc67178f2UL
};

#define libhmac_sha256_transform_extend_32bit_value( values_32bit, value_32bit_index, s0, s1 ) \
	s0 = byte_stream_bit_rotate_right_32bit( values_32bit[ value_32bit_index - 15 ], 7 ) \
	   ^ byte_stream_bit_rotate_right_32bit( values_32bit[ value_32bit_index - 15 ], 18 ) \
	   ^ ( values_32bit[ value_32bit_index - 15 ] >> 3 ); \
	s1 = byte_stream_bit_rotate_right_32bit( values_32bit[ value_32bit_index - 2 ], 17 ) \
	   ^ byte_stream_bit_rotate_right_32bit( values_32bit[ value_32bit_index - 2 ], 19 ) \
	   ^ ( values_32bit[ value_32bit_index - 2 ] >> 10 ); \
\
	values_32bit[ value_32bit_index ] = values_32bit[ value_32bit_index - 16 ] \
	                                  + s0 \
	                                  + values_32bit[ value_32bit_index - 7 ] \
	                                  + s1

#define libhmac_sha256_transform_unfolded_extend_32bit_values( values_32bit, s0, s1 ) \
	libhmac_sha256_transform_extend_32bit_value( values_32bit, 16, s0, s1 ); \
	libhmac_sha256_transform_extend_32bit_value( values_32bit, 17, s0, s1 ); \
	libhmac_sha256_transform_extend_32bit_value( values_32bit, 18, s0, s1 ); \
	libhmac_sha256_transform_extend_32bit_value( values_32bit, 19, s0, s1 ); \
	libhmac_sha256_transform_extend_32bit_value( values_32bit, 20, s0, s1 ); \
	libhmac_sha256_transform_extend_32bit_value( values_32bit, 21, s0, s1 ); \
	libhmac_sha256_transform_extend_32bit_value( values_32bit, 22, s0, s1 ); \
	libhmac_sha256_transform_extend_32bit_value( values_32bit, 23, s0, s1 ); \
	libhmac_sha256_transform_extend_32bit_value( values_32bit, 24, s0, s1 ); \
	libhmac_sha256_transform_extend_32bit_value( values_32bit, 25, s0, s1 ); \
	libhmac_sha256_transform_extend_32bit_value( values_32bit, 26, s0, s1 ); \
	libhmac_sha256_transform_extend_32bit_value( values_32bit, 27, s0, s1 ); \
	libhmac_sha256_transform_extend_32bit_value( values_32bit, 28, s0, s1 ); \
	libhmac_sha256_transform_extend_32bit_value( values_32bit, 29, s0, s1 ); \
	libhmac_sha256_transform_extend_32bit_value( values_32bit, 30, s0, s1 ); \
	libhmac_sha256_transform_extend_32bit_value( values_32bit, 31, s0, s1 ); \
	libhmac_sha256_transform_extend_32bit_value( values_32bit, 32, s0, s1 ); \
	libhmac_sha256_transform_extend_32bit_value( values_32bit, 33, s0, s1 ); \
	libhmac_sha256_transform_extend_32bit_value( values_32bit, 34, s0, s1 ); \
	libhmac_sha256_transform_extend_32bit_value( values_32bit, 35, s0, s1 ); \
	libhmac_sha256_transform_extend_32bit_value( values_32bit, 36, s0, s1 ); \
	libhmac_sha256_transform_extend_32bit_value( values_32bit, 37, s0, s1 ); \
	libhmac_sha256_transform_extend_32bit_value( values_32bit, 38, s0, s1 ); \
	libhmac_sha256_transform_extend_32bit_value( values_32bit, 39, s0, s1 ); \
	libhmac_sha256_transform_extend_32bit_value( values_32bit, 40, s0, s1 ); \
	libhmac_sha256_transform_extend_32bit_value( values_32bit, 41, s0, s1 ); \
	libhmac_sha256_transform_extend_32bit_value( values_32bit, 42, s0, s1 ); \
	libhmac_sha256_transform_extend_32bit_value( values_32bit, 43, s0, s1 ); \
	libhmac_sha256_transform_extend_32bit_value( values_32bit, 44, s0, s1 ); \
	libhmac_sha256_transform_extend_32bit_value( values_32bit, 45, s0, s1 ); \
	libhmac_sha256_transform_extend_32bit_value( values_32bit, 46, s0, s1 ); \
	libhmac_sha256_transform_extend_32bit_value( values_32bit, 47, s0, s1 ); \
	libhmac_sha256_transform_extend_32bit_value( values_32bit, 48, s0, s1 ); \
	libhmac_sha256_transform_extend_32bit_value( values_32bit, 49, s0, s1 ); \
	libhmac_sha256_transform_extend_32bit_value( values_32bit, 50, s0, s1 ); \
	libhmac_sha256_transform_extend_32bit_value( values_32bit, 51, s0, s1 ); \
	libhmac_sha256_transform_extend_32bit_value( values_32bit, 52, s0, s1 ); \
	libhmac_sha256_transform_extend_32bit_value( values_32bit, 53, s0, s1 ); \
	libhmac_sha256_transform_extend_32bit_value( values_32bit, 54, s0, s1 ); \
	libhmac_sha256_transform_extend_32bit_value( values_32bit, 55, s0, s1 ); \
	libhmac_sha256_transform_extend_32bit_value( values_32bit, 56, s0, s1 ); \
	libhmac_sha256_transform_extend_32bit_value( values_32bit, 57, s0, s1 ); \
	libhmac_sha256_transform_extend_32bit_value( values_32bit, 58, s0, s1 ); \
	libhmac_sha256_transform_extend_32bit_value( values_32bit, 59, s0, s1 ); \
	libhmac_sha256_transform_extend_32bit_value( values_32bit, 60, s0, s1 ); \
	libhmac_sha256_transform_extend_32bit_value( values_32bit, 61, s0, s1 ); \
	libhmac_sha256_transform_extend_32bit_value( values_32bit, 62, s0, s1 ); \
	libhmac_sha256_transform_extend_32bit_value( values_32bit, 63, s0, s1 );

#define libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, value_32bit_index, hash_values, hash_value_index0, hash_value_index1, hash_value_index2, hash_value_index3, hash_value_index4, hash_value_index5, hash_value_index6, hash_value_index7, s0, s1, t1, t2 ) \
	s0 = byte_stream_bit_rotate_right_32bit( hash_values[ hash_value_index0 ], 2 ) \
	   ^ byte_stream_bit_rotate_right_32bit( hash_values[ hash_value_index0 ], 13 ) \
	   ^ byte_stream_bit_rotate_right_32bit( hash_values[ hash_value_index0 ], 22 ); \
	s1 = byte_stream_bit_rotate_right_32bit( hash_values[ hash_value_index4 ], 6 ) \
	   ^ byte_stream_bit_rotate_right_32bit( hash_values[ hash_value_index4 ], 11 ) \
	   ^ byte_stream_bit_rotate_right_32bit( hash_values[ hash_value_index4 ], 25 ); \
\
	t1  = hash_values[ hash_value_index7 ]; \
	t1 += s1; \
	t1 += ( hash_values[ hash_value_index4 ] & hash_values[ hash_value_index5 ] ) \
	    ^ ( ~( hash_values[ hash_value_index4 ] ) & hash_values[ hash_value_index6 ] ); \
	t1 += libhmac_sha256_prime_cube_roots[ value_32bit_index ]; \
	t1 += values_32bit[ value_32bit_index ]; \
	t2  = s0; \
	t2 += ( hash_values[ hash_value_index0 ] & hash_values[ hash_value_index1 ] ) \
	    ^ ( hash_values[ hash_value_index0 ] & hash_values[ hash_value_index2 ] ) \
	    ^ ( hash_values[ hash_value_index1 ] & hash_values[ hash_value_index2 ] ); \
\
	hash_values[ hash_value_index3 ] += t1; \
	hash_values[ hash_value_index7 ]  = t1 + t2;

#define libhmac_sha256_transform_unfolded_calculate_hash_values( values_32bit, hash_values, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 0, hash_values, 0, 1, 2, 3, 4, 5, 6, 7, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 1, hash_values, 7, 0, 1, 2, 3, 4, 5, 6, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 2, hash_values, 6, 7, 0, 1, 2, 3, 4, 5, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 3, hash_values, 5, 6, 7, 0, 1, 2, 3, 4, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 4, hash_values, 4, 5, 6, 7, 0, 1, 2, 3, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 5, hash_values, 3, 4, 5, 6, 7, 0, 1, 2, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 6, hash_values, 2, 3, 4, 5, 6, 7, 0, 1, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 7, hash_values, 1, 2, 3, 4, 5, 6, 7, 0, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 8, hash_values, 0, 1, 2, 3, 4, 5, 6, 7, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 9, hash_values, 7, 0, 1, 2, 3, 4, 5, 6, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 10, hash_values, 6, 7, 0, 1, 2, 3, 4, 5, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 11, hash_values, 5, 6, 7, 0, 1, 2, 3, 4, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 12, hash_values, 4, 5, 6, 7, 0, 1, 2, 3, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 13, hash_values, 3, 4, 5, 6, 7, 0, 1, 2, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 14, hash_values, 2, 3, 4, 5, 6, 7, 0, 1, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 15, hash_values, 1, 2, 3, 4, 5, 6, 7, 0, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 16, hash_values, 0, 1, 2, 3, 4, 5, 6, 7, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 17, hash_values, 7, 0, 1, 2, 3, 4, 5, 6, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 18, hash_values, 6, 7, 0, 1, 2, 3, 4, 5, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 19, hash_values, 5, 6, 7, 0, 1, 2, 3, 4, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 20, hash_values, 4, 5, 6, 7, 0, 1, 2, 3, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 21, hash_values, 3, 4, 5, 6, 7, 0, 1, 2, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 22, hash_values, 2, 3, 4, 5, 6, 7, 0, 1, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 23, hash_values, 1, 2, 3, 4, 5, 6, 7, 0, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 24, hash_values, 0, 1, 2, 3, 4, 5, 6, 7, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 25, hash_values, 7, 0, 1, 2, 3, 4, 5, 6, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 26, hash_values, 6, 7, 0, 1, 2, 3, 4, 5, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 27, hash_values, 5, 6, 7, 0, 1, 2, 3, 4, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 28, hash_values, 4, 5, 6, 7, 0, 1, 2, 3, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 29, hash_values, 3, 4, 5, 6, 7, 0, 1, 2, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 30, hash_values, 2, 3, 4, 5, 6, 7, 0, 1, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 31, hash_values, 1, 2, 3, 4, 5, 6, 7, 0, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 32, hash_values, 0, 1, 2, 3, 4, 5, 6, 7, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 33, hash_values, 7, 0, 1, 2, 3, 4, 5, 6, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 34, hash_values, 6, 7, 0, 1, 2, 3, 4, 5, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 35, hash_values, 5, 6, 7, 0, 1, 2, 3, 4, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 36, hash_values, 4, 5, 6, 7, 0, 1, 2, 3, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 37, hash_values, 3, 4, 5, 6, 7, 0, 1, 2, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 38, hash_values, 2, 3, 4, 5, 6, 7, 0, 1, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 39, hash_values, 1, 2, 3, 4, 5, 6, 7, 0, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 40, hash_values, 0, 1, 2, 3, 4, 5, 6, 7, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 41, hash_values, 7, 0, 1, 2, 3, 4, 5, 6, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 42, hash_values, 6, 7, 0, 1, 2, 3, 4, 5, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 43, hash_values, 5, 6, 7, 0, 1, 2, 3, 4, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 44, hash_values, 4, 5, 6, 7, 0, 1, 2, 3, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 45, hash_values, 3, 4, 5, 6, 7, 0, 1, 2, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 46, hash_values, 2, 3, 4, 5, 6, 7, 0, 1, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 47, hash_values, 1, 2, 3, 4, 5, 6, 7, 0, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 48, hash_values, 0, 1, 2, 3, 4, 5, 6, 7, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 49, hash_values, 7, 0, 1, 2, 3, 4, 5, 6, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 50, hash_values, 6, 7, 0, 1, 2, 3, 4, 5, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 51, hash_values, 5, 6, 7, 0, 1, 2, 3, 4, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 52, hash_values, 4, 5, 6, 7, 0, 1, 2, 3, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 53, hash_values, 3, 4, 5, 6, 7, 0, 1, 2, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 54, hash_values, 2, 3, 4, 5, 6, 7, 0, 1, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 55, hash_values, 1, 2, 3, 4, 5, 6, 7, 0, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 56, hash_values, 0, 1, 2, 3, 4, 5, 6, 7, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 57, hash_values, 7, 0, 1, 2, 3, 4, 5, 6, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 58, hash_values, 6, 7, 0, 1, 2, 3, 4, 5, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 59, hash_values, 5, 6, 7, 0, 1, 2, 3, 4, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 60, hash_values, 4, 5, 6, 7, 0, 1, 2, 3, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 61, hash_values, 3, 4, 5, 6, 7, 0, 1, 2, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 62, hash_values, 2, 3, 4, 5, 6, 7, 0, 1, s0, s1, t1, t2 ) \
	libhmac_sha256_transform_unfolded_calculate_hash_value( values_32bit, 63, hash_values, 1, 2, 3, 4, 5, 6, 7, 0, s0, s1, t1, t2 )

/* Calculates the SHA256 of 64 byte sized blocks of data in a buffer
 * Returns the number of bytes used if successful or -1 on error
 */
ssize_t libhmac_sha256_transform(
         libhmac_internal_sha256_context_t *internal_context,
         const uint8_t *buffer,
         size_t size,
         libcerror_error_t **error )
{
	uint32_t hash_values[ 8 ];
	uint32_t values_32bit[ 64 ];

	static char *function     = "libhmac_sha256_transform";
	uint32_t s0               = 0;
	uint32_t s1               = 0;
	uint32_t t1               = 0;
	uint32_t t2               = 0;
	size_t buffer_offset      = 0;

#if !defined( LIBHMAC_UNFOLLED_LOOPS )
	uint8_t hash_values_index = 0;
	uint8_t value_32bit_index = 0;
#endif

	if( internal_context == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid internal context.",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.",
		 function );

		return( -1 );
	}
	while( size >= LIBHMAC_SHA256_BLOCK_SIZE )
	{
		if( memory_copy(
		     hash_values,
		     internal_context->hash_values,
		     sizeof( uint32_t ) * 8 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy hash values.",
			 function );

			goto on_error;
		}
#if defined( LIBHMAC_UNFOLLED_LOOPS )
		/* Break the block into 16 x 32-bit values
		 */
		libhmac_byte_stream_copy_to_16x_uint32_big_endian(
		 &( buffer[ buffer_offset ] ),
		 values_32bit );

		buffer_offset += LIBHMAC_SHA256_BLOCK_SIZE;

		/* Extend to 64 x 32-bit values
		 */
		libhmac_sha256_transform_unfolded_extend_32bit_values(
		 values_32bit,
		 s0,
		 s1 );

		/* Calculate the hash values for the 32-bit values
		 */
		libhmac_sha256_transform_unfolded_calculate_hash_values(
		 values_32bit,
		 hash_values,
		 s0,
		 s1,
		 t1,
		 t2 );

		/* Update the hash values in the context
		 */
		internal_context->hash_values[ 0 ] += hash_values[ 0 ];
		internal_context->hash_values[ 1 ] += hash_values[ 1 ];
		internal_context->hash_values[ 2 ] += hash_values[ 2 ];
		internal_context->hash_values[ 3 ] += hash_values[ 3 ];
		internal_context->hash_values[ 4 ] += hash_values[ 4 ];
		internal_context->hash_values[ 5 ] += hash_values[ 5 ];
		internal_context->hash_values[ 6 ] += hash_values[ 6 ];
		internal_context->hash_values[ 7 ] += hash_values[ 7 ];

#else
		/* Break the block into 16 x 32-bit values
		 */
		for( value_32bit_index = 0;
		     value_32bit_index < 16;
		     value_32bit_index++ )
		{
			byte_stream_copy_to_uint32_big_endian(
			 &( buffer[ buffer_offset ] ),
			 values_32bit[ value_32bit_index ] );

			buffer_offset += sizeof( uint32_t );
		}
		/* Extend to 64 x 32-bit values
		 */
		for( value_32bit_index = 16;
		     value_32bit_index < 64;
		     value_32bit_index++ )
		{
			libhmac_sha256_transform_extend_32bit_value(
			 values_32bit,
			 value_32bit_index,
			 s0,
			 s1 );
		}
		/* Calculate the hash values for the 32-bit values
		 */
		for( value_32bit_index = 0;
		     value_32bit_index < 64;
		     value_32bit_index++ )
		{
			s0 = byte_stream_bit_rotate_right_32bit( hash_values[ 0 ], 2 )
			   ^ byte_stream_bit_rotate_right_32bit( hash_values[ 0 ], 13 )
			   ^ byte_stream_bit_rotate_right_32bit( hash_values[ 0 ], 22 );
			s1 = byte_stream_bit_rotate_right_32bit( hash_values[ 4 ], 6 )
			   ^ byte_stream_bit_rotate_right_32bit( hash_values[ 4 ], 11 )
			   ^ byte_stream_bit_rotate_right_32bit( hash_values[ 4 ], 25 );

			t1  = hash_values[ 7 ];
			t1 += s1;
			t1 += ( hash_values[ 4 ] & hash_values[ 5 ] )
			    ^ ( ~( hash_values[ 4 ] ) & hash_values[ 6 ] );
			t1 += libhmac_sha256_prime_cube_roots[ value_32bit_index ];
			t1 += values_32bit[ value_32bit_index ];
			t2  = s0;
			t2 += ( hash_values[ 0 ] & hash_values[ 1 ] )
			    ^ ( hash_values[ 0 ] & hash_values[ 2 ] )
			    ^ ( hash_values[ 1 ] & hash_values[ 2 ] );

			hash_values[ 7 ] = hash_values[ 6 ];
			hash_values[ 6 ] = hash_values[ 5 ];
			hash_values[ 5 ] = hash_values[ 4 ];
			hash_values[ 4 ] = hash_values[ 3 ] + t1;
			hash_values[ 3 ] = hash_values[ 2 ];
			hash_values[ 2 ] = hash_values[ 1 ];
			hash_values[ 1 ] = hash_values[ 0 ];
			hash_values[ 0 ] = t1 + t2;
		}
		/* Update the hash values in the context
		 */
		for( hash_values_index = 0;
		     hash_values_index < 8;
		     hash_values_index++ )
		{
			internal_context->hash_values[ hash_values_index ] += hash_values[ hash_values_index ];
		}
#endif /* defined( LIBHMAC_UNFOLLED_LOOPS ) */

		size -= LIBHMAC_SHA256_BLOCK_SIZE;
	}
	/* Prevent sensitive data from leaking
	 */
	if( memory_set(
	     hash_values,
	     0,
	     sizeof( uint32_t ) * 8 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear hash values.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     values_32bit,
	     0,
	     sizeof( uint32_t ) * 64 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear 32-bit values.",
		 function );

		goto on_error;
	}
	return( (ssize_t) buffer_offset );

on_error:
	memory_set(
	 values_32bit,
	 0,
	 sizeof( uint32_t ) * 64 );

	memory_set(
	 hash_values,
	 0,
	 sizeof( uint32_t ) * 8 );

	return( -1 );
}

#endif /* !defined( LIBHMAC_HAVE_SHA256_SUPPORT ) */

/* Initializes the SHA256 context
 * The context must point to a NULL pointer to be allocated
 * Returns 1 if successful or -1 on error
 */
int libhmac_sha256_initialize(
     libhmac_sha256_context_t **context,
     libcerror_error_t **error )
{
	libhmac_internal_sha256_context_t *internal_context = NULL;
	static char *function                               = "libhmac_sha256_initialize";

	if( context == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid context.",
		 function );

		return( -1 );
	}
	if( *context != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid context value already set.",
		 function );

		return( -1 );
	}
	internal_context = memory_allocate_structure(
	                    libhmac_internal_sha256_context_t );

	if( internal_context == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create context.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_context,
	     0,
	     sizeof( libhmac_internal_sha256_context_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear context.",
		 function );

		memory_free(
		 internal_context );

		return( -1 );
	}
#if defined( WINAPI ) && defined( CALG_SHA_256 ) && ( WINVER >= 0x0600 )
	/* Request the AES crypt provider, fail back to the RSA crypt provider
	*/
	if( CryptAcquireContext(
	     &( internal_context->crypt_provider ),
	     NULL,
	     NULL,
	     PROV_RSA_AES,
	     CRYPT_VERIFYCONTEXT ) == 0 )
	{
		if( CryptAcquireContext(
		     &( internal_context->crypt_provider ),
		     NULL,
		     NULL,
		     PROV_RSA_FULL,
		     CRYPT_VERIFYCONTEXT ) == 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create AES or RSA crypt provider.",
			 function );

			goto on_error;
		}
	}
	if( internal_context->crypt_provider == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: unable to create crypt provider.",
		 function );

		goto on_error;
	}
	if( CryptCreateHash(
	     internal_context->crypt_provider,
	     CALG_SHA_256,
	     0,
	     0,
	     &( internal_context->hash ) ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create hash object.",
		 function );

		CryptReleaseContext(
		 internal_context->crypt_provider,
		 0 );

		goto on_error;
	}

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_SHA_H ) && defined( SHA256_DIGEST_LENGTH )
	if( SHA256_Init(
	     &( internal_context->sha256_context ) ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize context.",
		 function );

		goto on_error;
	}

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H ) && defined( HAVE_EVP_SHA256 )
	EVP_MD_CTX_init(
	 &( internal_context->evp_md_context ) );

	if( EVP_DigestInit_ex(
	     &( internal_context->evp_md_context ),
	     EVP_sha256(),
	     NULL ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize context.",
		 function );

		EVP_MD_CTX_cleanup(
		 &( internal_context->evp_md_context ) );

		goto on_error;
	}

#else
	if( memory_copy(
	     internal_context->hash_values,
	     libhmac_sha256_prime_square_roots,
	     sizeof( uint32_t ) * 8 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy initial hash values.",
		 function );

		return( -1 );
	}
#endif
	*context = (libhmac_sha256_context_t *) internal_context;

	return( 1 );

on_error:
	if( internal_context != NULL )
	{
		memory_free(
		 internal_context );
	}
	return( -1 );
}

/* Free the SHA256 context and its values
 * Returns 1 if successful or -1 on error
 */
int libhmac_sha256_free(
     libhmac_sha256_context_t **context,
     libcerror_error_t **error )
{
	libhmac_internal_sha256_context_t *internal_context = NULL;
	static char *function                               = "libhmac_sha256_free";

	if( context == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid context.",
		 function );

		return( -1 );
	}
	if( *context != NULL )
	{
		internal_context = (libhmac_internal_sha256_context_t *) *context;
		*context         = NULL;

#if defined( WINAPI ) && defined( CALG_SHA_256 ) && ( WINVER >= 0x0600 )
		if( internal_context->crypt_provider != 0 )
		{
			CryptReleaseContext(
			 internal_context->crypt_provider,
			 0 );
		}
		if( internal_context->hash != 0 )
		{
			CryptDestroyHash(
			 internal_context->hash );
		}

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_SHA_H ) && defined( SHA256_DIGEST_LENGTH )
		/* No additional clean up necessary
		 */

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H ) && defined( HAVE_EVP_SHA256 )
		if( EVP_MD_CTX_cleanup(
		     &( internal_context->evp_md_context ) ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to clean up context.",
			 function );
		}

#else
		/* No additional clean up necessary
		 */
#endif
		memory_free(
		 internal_context );
	}
	return( 1 );
}

/* Updates the SHA256 context
 * Returns 1 if successful or -1 on error
 */
int libhmac_sha256_update(
     libhmac_sha256_context_t *context,
     const uint8_t *buffer,
     size_t size,
     libcerror_error_t **error )
{
	libhmac_internal_sha256_context_t *internal_context = NULL;
	static char *function                               = "libhmac_sha256_update";

#if !defined( LIBHMAC_HAVE_SHA256_SUPPORT )
	size_t buffer_offset                                = 0;
	size_t remaining_block_size                         = 0;
	ssize_t process_count                               = 0;
#endif

	if( context == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid context.",
		 function );

		return( -1 );
	}
	internal_context = (libhmac_internal_sha256_context_t *) context;

#if defined( WINAPI ) && defined( CALG_SHA_256 ) && ( WINVER >= 0x0600 )
	if( internal_context->hash == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid context - missing hash.",
		 function );

		return( -1 );
	}
#endif
	if( buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( size == 0 )
	{
		return( 1 );
	}
#if defined( WINAPI ) && defined( CALG_SHA_256 ) && ( WINVER >= 0x0600 )
#if ( SIZEOF_SIZE_T == 8 ) || defined( _WIN64 )
	if( size > (size_t) UINT32_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.",
		 function );

		return( -1 );
	}
#endif
	if( CryptHashData(
	     internal_context->hash,
	     (BYTE *) buffer,
	     (DWORD) size,
	     0 ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to update hash.",
		 function );

		return( -1 );
	}

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_SHA_H ) && defined( SHA256_DIGEST_LENGTH )
	if( size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( SHA256_Update(
	     &( internal_context->sha256_context ),
	     (const void *) buffer,
	     size ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to update context.",
		 function );

		return( -1 );
	}

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H ) && defined( HAVE_EVP_SHA256 )
	if( size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( EVP_DigestUpdate(
	     &( internal_context->evp_md_context ),
	     (const void *) buffer,
	     size ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to update context.",
		 function );

		return( -1 );
	}

#else

	if( size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( internal_context->block_offset > 0 )
	{
		remaining_block_size = LIBHMAC_SHA256_BLOCK_SIZE - internal_context->block_offset;

		if( remaining_block_size > size )
		{
			remaining_block_size = size;
		}
		if( memory_copy(
		     &( internal_context->block[ internal_context->block_offset ] ),
		     buffer,
		     remaining_block_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy data to context block.",
			 function );

			return( -1 );
		}
		internal_context->block_offset += remaining_block_size;

		if( internal_context->block_offset < LIBHMAC_SHA256_BLOCK_SIZE )
		{
			return( 1 );
		}
		buffer_offset += remaining_block_size;
		size          -= remaining_block_size;

		process_count = libhmac_sha256_transform(
		                 internal_context,
		                 internal_context->block,
		                 LIBHMAC_SHA256_BLOCK_SIZE,
		                 error );

		if( process_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to transform context block.",
			 function );

			return( -1 );
		}
		internal_context->hash_count  += process_count;
		internal_context->block_offset = 0;
	}
	if( size > 0 )
	{
		process_count = libhmac_sha256_transform(
		                 internal_context,
		                 &( buffer[ buffer_offset ] ),
		                 size,
		                 error );

		if( process_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to transform buffer.",
			 function );

			return( -1 );
		}
		internal_context->hash_count += process_count;

		buffer_offset += process_count;
		size          -= process_count;
	}
	if( size > 0 )
	{
		if( size >= LIBHMAC_SHA256_BLOCK_SIZE )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid size value out of bounds.",
			 function );

			return( -1 );
		}
		if( memory_copy(
		     internal_context->block,
		     &( buffer[ buffer_offset ] ),
		     size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy remaining data to context block.",
			 function );

			return( -1 );
		}
		internal_context->block_offset = size;
	}
#endif
	return( 1 );
}

/* Finalizes the SHA256 context
 * Returns 1 if successful or -1 on error
 */
int libhmac_sha256_finalize(
     libhmac_sha256_context_t *context,
     uint8_t *hash,
     size_t hash_size,
     libcerror_error_t **error )
{
	libhmac_internal_sha256_context_t *internal_context = NULL;
	static char *function                               = "libhmac_sha256_finalize";

#if defined( WINAPI ) && defined( CALG_SHA_256 ) && ( WINVER >= 0x0600 )
	DWORD safe_hash_size                                = 0;

#elif defined( HAVE_LIBCRYPTO ) && !defined( HAVE_OPENSSL_SHA_H ) && defined( HAVE_OPENSSL_EVP_H ) && defined( HAVE_EVP_SHA256 )
	unsigned int safe_hash_size                         = 0;

#elif !defined( LIBHMAC_HAVE_SHA256_SUPPORT )
	uint64_t bit_size                                   = 0;
	size_t block_size                                   = 0;
	size_t number_of_blocks                             = 0;
	ssize_t process_count                               = 0;

#if !defined( LIBHMAC_UNFOLLED_LOOPS )
	size_t hash_index                                   = 0;
	int hash_values_index                               = 0;
#endif
#endif
	if( context == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid context.",
		 function );

		return( -1 );
	}
	internal_context = (libhmac_internal_sha256_context_t *) context;

#if defined( WINAPI ) && defined( CALG_SHA_256 ) && ( WINVER >= 0x0600 )
	if( internal_context->hash == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid context - missing hash.",
		 function );

		return( -1 );
	}
#endif
	if( hash == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hash.",
		 function );

		return( -1 );
	}
	if( hash_size < (size_t) LIBHMAC_SHA256_HASH_SIZE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid hash value too small.",
		 function );

		return( -1 );
	}
#if defined( WINAPI ) && defined( CALG_SHA_256 ) && ( WINVER >= 0x0600 )
#if ( SIZEOF_SIZE_T == 8 ) || defined( _WIN64 )
	if( hash_size > (size_t) UINT32_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid hash size value exceeds maximum.",
		 function );

		return( -1 );
	}
#endif
	safe_hash_size = (DWORD) hash_size;

	if( CryptGetHashParam(
	     internal_context->hash,
	     HP_HASHVAL,
	     (BYTE *) hash,
	     &safe_hash_size,
	     0 ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to finalize hash.",
		 function );

		return( -1 );
	}

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_SHA_H ) && defined( SHA256_DIGEST_LENGTH )
	if( hash_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid hash size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( hash_size < (size_t) LIBHMAC_SHA256_HASH_SIZE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid hash size value too small.",
		 function );

		return( -1 );
	}
	if( SHA256_Final(
	     hash,
	     &( internal_context->sha256_context ) ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to finalize context.",
		 function );

		return( -1 );
	}

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H ) && defined( HAVE_EVP_SHA256 )
	if( hash_size > (size_t) UINT_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid hash size value exceeds maximum.",
		 function );

		return( -1 );
	}
	safe_hash_size = (unsigned int) hash_size;

	if( EVP_DigestFinal_ex(
	     &( internal_context->evp_md_context ),
	     (unsigned char *) hash,
	     &safe_hash_size ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to finalize context.",
		 function );

		return( -1 );
	}

#else
	if( hash_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid hash size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( hash_size < (size_t) LIBHMAC_SHA256_HASH_SIZE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid hash size value too small.",
		 function );

		return( -1 );
	}
	/* Add padding with a size of 56 mod 64
	 */
	number_of_blocks = 1;

	if( internal_context->block_offset > 55 )
	{
		number_of_blocks += 1;
	}
	block_size = number_of_blocks * LIBHMAC_SHA256_BLOCK_SIZE;

	if( memory_set(
	     &( internal_context->block[ internal_context->block_offset ] ),
	     0,
	     block_size - internal_context->block_offset ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear context block.",
		 function );

		return( -1 );
	}
	/* The first byte of the padding contains 0x80
	 */
	internal_context->block[ internal_context->block_offset ] = 0x80;

	bit_size = ( internal_context->hash_count + internal_context->block_offset ) * 8;

	byte_stream_copy_from_uint64_big_endian(
	 &( internal_context->block[ block_size - 8 ] ),
	 bit_size );

	process_count = libhmac_sha256_transform(
	                 internal_context,
	                 internal_context->block,
	                 block_size,
	                 error );

	if( process_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to transform context block.",
		 function );

		return( -1 );
	}
#if !defined( LIBHMAC_UNFOLLED_LOOPS )
	for( hash_values_index = 0;
	     hash_values_index < 8;
	     hash_values_index++ )
	{
		byte_stream_copy_from_uint32_big_endian(
		 &( hash[ hash_index ] ),
		 internal_context->hash_values[ hash_values_index ] );

		hash_index += sizeof( uint32_t );
	}
#else
	byte_stream_copy_from_uint32_big_endian(
	 &( hash[ 0 ] ),
	 internal_context->hash_values[ 0 ] );

	byte_stream_copy_from_uint32_big_endian(
	 &( hash[ 4 ] ),
	 internal_context->hash_values[ 1 ] );

	byte_stream_copy_from_uint32_big_endian(
	 &( hash[ 8 ] ),
	 internal_context->hash_values[ 2 ] );

	byte_stream_copy_from_uint32_big_endian(
	 &( hash[ 12 ] ),
	 internal_context->hash_values[ 3 ] );

	byte_stream_copy_from_uint32_big_endian(
	 &( hash[ 16 ] ),
	 internal_context->hash_values[ 4 ] );

	byte_stream_copy_from_uint32_big_endian(
	 &( hash[ 20 ] ),
	 internal_context->hash_values[ 5 ] );

	byte_stream_copy_from_uint32_big_endian(
	 &( hash[ 24 ] ),
	 internal_context->hash_values[ 6 ] );

	byte_stream_copy_from_uint32_big_endian(
	 &( hash[ 28 ] ),
	 internal_context->hash_values[ 7 ] );

#endif /* !defined( LIBHMAC_UNFOLLED_LOOPS ) */

	/* Prevent sensitive data from leaking
	 */
	if( memory_set(
	     internal_context,
	     0,
	     sizeof( libhmac_internal_sha256_context_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear context.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Calculates the SHA256 of the buffer
 * Returns 1 if successful or -1 on error
 */
int libhmac_sha256_calculate(
     const uint8_t *buffer,
     size_t size,
     uint8_t *hash,
     size_t hash_size,
     libcerror_error_t **error )
{
	libhmac_sha256_context_t *context = NULL;
	static char *function             = "libhmac_sha256_calculate";

	if( libhmac_sha256_initialize(
	     &context,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable initialize context.",
		 function );

		goto on_error;
	}
	if( libhmac_sha256_update(
	     context,
	     buffer,
	     size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable update context.",
		 function );

		goto on_error;
	}
	if( libhmac_sha256_finalize(
	     context,
	     hash,
	     hash_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable finalize context.",
		 function );

		goto on_error;
	}
	if( libhmac_sha256_free(
	     &context,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable finalize context.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( context != NULL )
	{
		libhmac_sha256_free(
		 &context,
		 NULL );
	}
	return( -1 );
}

/* Calculates the SHA256 HMAC of the buffer
 * HMAC is defined in RFC 2104
 * Returns 1 if successful or -1 on error
 */
int libhmac_sha256_calculate_hmac(
     const uint8_t *key,
     size_t key_size,
     const uint8_t *buffer,
     size_t size,
     uint8_t *hmac,
     size_t hmac_size,
     libcerror_error_t **error )
{
	uint8_t key_hash[ LIBHMAC_SHA256_HASH_SIZE ];

	libhmac_sha256_context_t *context = NULL;
	uint8_t *key_data                 = NULL;
	uint8_t *inner_padding            = NULL;
	uint8_t *outer_padding            = NULL;
	static char *function             = "libhmac_sha256_calculate_hmac";
	size_t block_index                = 0;
	size_t block_size                 = 64;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	if( key_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid key size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( hmac_size < (size_t) LIBHMAC_SHA256_HASH_SIZE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid HMAC size value too small.",
		 function );

		return( -1 );
	}
	key_data = (uint8_t *) memory_allocate(
	                        sizeof( uint8_t ) * block_size );

	if( key_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create key data.",
		 function );

		goto on_error;
	}
	if( key_size <= block_size )
	{
		if( memory_copy(
		     key_data,
		     key,
		     key_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy key data.",
			 function );

			goto on_error;
		}
		if( memory_set(
		     &( key_data[ key_size ] ),
		     0,
		     block_size - key_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear key remaining data.",
			 function );

			goto on_error;
		}
	}
	else
	{
		if( libhmac_sha256_initialize(
		     &context,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable initialize context.",
			 function );

			goto on_error;
		}
		if( libhmac_sha256_update(
		     context,
		     key,
		     key_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable update context.",
			 function );

			goto on_error;
		}
		if( libhmac_sha256_finalize(
		     context,
		     key_hash,
		     LIBHMAC_SHA256_HASH_SIZE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable finalize context.",
			 function );

			goto on_error;
		}
		if( libhmac_sha256_free(
		     &context,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable finalize context.",
			 function );

			goto on_error;
		}
		if( block_size <= LIBHMAC_SHA256_HASH_SIZE )
		{
			if( memory_copy(
			     key_data,
			     key_hash,
			     LIBHMAC_SHA256_HASH_SIZE ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy key hash data.",
				 function );

				goto on_error;
			}
			if( memory_set(
			     &( key_data[ LIBHMAC_SHA256_HASH_SIZE ] ),
			     0,
			     block_size - LIBHMAC_SHA256_HASH_SIZE ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to clear remaining key data.",
				 function );

				goto on_error;
			}
		}
		else
		{
			if( memory_copy(
			     key_data,
			     key_hash,
			     block_size ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy key hash data.",
				 function );

				goto on_error;
			}
		}
	}
	inner_padding = (uint8_t *) memory_allocate(
	                             sizeof( uint8_t ) * block_size );

	if( inner_padding == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create inner padding.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     inner_padding,
	     0x36,
	     block_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to set inner padding.",
		 function );

		goto on_error;
	}
	outer_padding = (uint8_t *) memory_allocate(
	                             sizeof( uint8_t ) * block_size );

	if( outer_padding == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create outer padding.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     outer_padding,
	     0x5c,
	     block_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to set outer padding.",
		 function );

		goto on_error;
	}
	for( block_index = 0;
	     block_index < block_size;
	     block_index++ )
	{
		inner_padding[ block_index ] ^= key_data[ block_index ];
		outer_padding[ block_index ] ^= key_data[ block_index ];
	}
	if( libhmac_sha256_initialize(
	     &context,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable initialize context.",
		 function );

		goto on_error;
	}
	if( libhmac_sha256_update(
	     context,
	     inner_padding,
	     block_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable update context.",
		 function );

		goto on_error;
	}
	if( libhmac_sha256_update(
	     context,
	     buffer,
	     size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable update context.",
		 function );

		goto on_error;
	}
	if( libhmac_sha256_finalize(
	     context,
	     hmac,
	     hmac_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable finalize context.",
		 function );

		goto on_error;
	}
	if( libhmac_sha256_free(
	     &context,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable finalize context.",
		 function );

		goto on_error;
	}
	if( libhmac_sha256_initialize(
	     &context,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable initialize context.",
		 function );

		goto on_error;
	}
	if( libhmac_sha256_update(
	     context,
	     outer_padding,
	     block_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable update context.",
		 function );

		goto on_error;
	}
	if( libhmac_sha256_update(
	     context,
	     hmac,
	     LIBHMAC_SHA256_HASH_SIZE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable update context.",
		 function );

		goto on_error;
	}
	if( libhmac_sha256_finalize(
	     context,
	     hmac,
	     hmac_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable finalize context.",
		 function );

		goto on_error;
	}
	if( libhmac_sha256_free(
	     &context,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable finalize context.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     outer_padding,
	     0,
	     block_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear outer padding.",
		 function );

		goto on_error;
	}
	memory_free(
	 outer_padding );

	outer_padding = NULL;

	if( memory_set(
	     inner_padding,
	     0,
	     block_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear inner padding.",
		 function );

		goto on_error;
	}
	memory_free(
	 inner_padding );

	inner_padding = NULL;

	if( memory_set(
	     key_data,
	     0,
	     block_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear key data.",
		 function );

		goto on_error;
	}
	memory_free(
	 key_data );

	key_data = NULL;

	return( 1 );

on_error:
	if( context != NULL )
	{
		libhmac_sha256_free(
		 &context,
		 NULL );
	}
	if( outer_padding != NULL )
	{
		memory_set(
        	 outer_padding,
	         0,
        	 block_size );

		memory_free(
		 outer_padding );
	}
	if( inner_padding != NULL )
	{
		memory_set(
        	 inner_padding,
	         0,
        	 block_size );

		memory_free(
		 inner_padding );
	}
	if( key_data != NULL )
	{
		memory_set(
        	 key_data,
	         0,
        	 block_size );

		memory_free(
		 key_data );
	}
	return( -1 );
}

