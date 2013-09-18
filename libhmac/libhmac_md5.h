/*
 * MD5 functions
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

#if !defined( _LIBHMAC_MD5_H )
#define _LIBHMAC_MD5_H

#include <common.h>
#include <types.h>

#if defined( WINAPI )
#include <wincrypt.h>

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_MD5_H )
#include <openssl/md5.h>

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
#include <openssl/evp.h>

#endif

#include "libhmac_extern.h"
#include "libhmac_libcerror.h"
#include "libhmac_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( WINAPI ) && defined( CALG_MD5 )
#define LIBHMAC_HAVE_MD5_SUPPORT

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_MD5_H ) && defined( MD5_DIGEST_LENGTH )
#define LIBHMAC_HAVE_MD5_SUPPORT

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H ) && defined( HAVE_EVP_MD5 )
#define LIBHMAC_HAVE_MD5_SUPPORT

#endif

#if !defined( LIBHMAC_HAVE_MD5_SUPPORT )
#define LIBHMAC_MD5_BLOCK_SIZE		64
#endif

typedef struct libhmac_internal_md5_context libhmac_internal_md5_context_t;

struct libhmac_internal_md5_context
{
#if defined( WINAPI ) && defined( CALG_MD5 )
	HCRYPTPROV crypt_provider;
	HCRYPTHASH hash;

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_MD5_H ) && defined( MD5_DIGEST_LENGTH )
	MD5_CTX md5_context;

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H ) && defined( HAVE_EVP_MD5 )
	EVP_MD_CTX evp_md_context;

#else
	/* The number of bytes hashed
	 */
	uint64_t hash_count;

	/* The hash values
	 */
	uint32_t hash_values[ 4 ];

	/* The block offset
	 */
	size_t block_offset;

	/* The (data) block
	 */
	uint8_t block[ 128 ];
#endif
};

#if !defined( LIBHMAC_HAVE_MD5_SUPPORT )
ssize_t libhmac_md5_transform(
         libhmac_internal_md5_context_t *internal_context,
         const uint8_t *buffer,
         size_t size,
         libcerror_error_t **error );
#endif

LIBHMAC_EXTERN \
int libhmac_md5_initialize(
     libhmac_md5_context_t **context,
     libcerror_error_t **error );

LIBHMAC_EXTERN \
int libhmac_md5_free(
     libhmac_md5_context_t **context,
     libcerror_error_t **error );

LIBHMAC_EXTERN \
int libhmac_md5_update(
     libhmac_md5_context_t *context,
     const uint8_t *buffer,
     size_t size,
     libcerror_error_t **error );

LIBHMAC_EXTERN \
int libhmac_md5_finalize(
     libhmac_md5_context_t *context,
     uint8_t *hash,
     size_t hash_size,
     libcerror_error_t **error );

LIBHMAC_EXTERN \
int libhmac_md5_calculate(
     const uint8_t *buffer,
     size_t size,
     uint8_t *hash,
     size_t hash_size,
     libcerror_error_t **error );

LIBHMAC_EXTERN \
int libhmac_md5_calculate_hmac(
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

