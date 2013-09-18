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

#if !defined( _LIBHMAC_SHA256_H )
#define _LIBHMAC_SHA256_H

#include <common.h>
#include <types.h>

#if defined( WINAPI )
#include <wincrypt.h>

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_SHA_H )
#include <openssl/sha.h>

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
#include <openssl/evp.h>

#endif

#include "libhmac_extern.h"
#include "libhmac_libcerror.h"
#include "libhmac_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

/* Make sure the WINAPI version is Vista or later otherwise
 * a cross compilation will contain broken SHA-256 support
 */
#if defined( WINAPI ) && defined( CALG_SHA_256 ) && ( WINVER >= 0x0600 )
#define LIBHMAC_HAVE_SHA256_SUPPORT

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_SHA_H ) && defined( SHA256_DIGEST_LENGTH )
#define LIBHMAC_HAVE_SHA256_SUPPORT

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H ) && defined( HAVE_EVP_SHA256 )
#define LIBHMAC_HAVE_SHA256_SUPPORT

#endif

#if !defined( LIBHMAC_HAVE_SHA256_SUPPORT )
#define LIBHMAC_SHA256_BLOCK_SIZE		64
#endif

typedef struct libhmac_internal_sha256_context libhmac_internal_sha256_context_t;

struct libhmac_internal_sha256_context
{
#if defined( WINAPI ) && ( WINVER >= 0x0600 ) && defined( CALG_SHA_256 )
	HCRYPTPROV crypt_provider;
	HCRYPTHASH hash;

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_SHA_H ) && defined( SHA256_DIGEST_LENGTH )
	SHA256_CTX sha256_context;

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H ) && defined( HAVE_EVP_SHA256 )
	EVP_MD_CTX evp_md_context;

#else
	/* The number of bytes hashed
	 */
	uint64_t hash_count;

	/* The 32-bit hash values
	 */
	uint32_t hash_values[ 8 ];

	/* The block offset
	 */
	size_t block_offset;

	/* The (data) block
	 */
	uint8_t block[ 128 ];
#endif
};

#if !defined( LIBHMAC_HAVE_SHA256_SUPPORT )
ssize_t libhmac_sha256_transform(
         libhmac_internal_sha256_context_t *internal_context,
         const uint8_t *buffer,
         size_t size,
         libcerror_error_t **error );
#endif

LIBHMAC_EXTERN \
int libhmac_sha256_initialize(
     libhmac_sha256_context_t **context,
     libcerror_error_t **error );

LIBHMAC_EXTERN \
int libhmac_sha256_free(
     libhmac_sha256_context_t **context,
     libcerror_error_t **error );

LIBHMAC_EXTERN \
int libhmac_sha256_update(
     libhmac_sha256_context_t *context,
     const uint8_t *buffer,
     size_t size,
     libcerror_error_t **error );

LIBHMAC_EXTERN \
int libhmac_sha256_finalize(
     libhmac_sha256_context_t *context,
     uint8_t *hash,
     size_t hash_size,
     libcerror_error_t **error );

LIBHMAC_EXTERN \
int libhmac_sha256_calculate(
     const uint8_t *buffer,
     size_t size,
     uint8_t *hash,
     size_t hash_size,
     libcerror_error_t **error );

LIBHMAC_EXTERN \
int libhmac_sha256_calculate_hmac(
     const uint8_t *key,
     size_t key_size,
     const uint8_t *buffer,
     size_t size,
     uint8_t *hmac,
     size_t hmac_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

