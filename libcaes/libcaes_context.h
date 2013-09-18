/*
 * AES context functions
 *
 * Copyright (C) 2011-2013, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBCAES_CONTEXT_H )
#define _LIBCAES_CONTEXT_H

#include <common.h>
#include <types.h>

#if defined( WINAPI )
#include <wincrypt.h>

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_AES_H )
#include <openssl/aes.h>

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
#include <openssl/evp.h>

#endif

#include "libcaes_extern.h"
#include "libcaes_definitions.h"
#include "libcaes_libcerror.h"
#include "libcaes_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libcaes_internal_context libcaes_internal_context_t;

struct libcaes_internal_context
{
#if defined( WINAPI ) && ( WINVER >= 0x0600 )
	HCRYPTPROV crypt_provider;
        HCRYPTKEY key;

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_AES_H )
        AES_KEY key;

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
	EVP_CIPHER_CTX evp_context;
        uint8_t key[ 32 ];
	size_t bit_size;

#else
	/* The number of round keys
	 */
	uint8_t number_of_round_keys;

	/* The round keys
	 */
	uint32_t *round_keys;

	/* The round keys data
	 */
	uint32_t round_keys_data[ 68 ];

#endif
};

LIBCAES_EXTERN \
int libcaes_context_initialize(
     libcaes_context_t **context,
     libcerror_error_t **error );

LIBCAES_EXTERN \
int libcaes_context_free(
     libcaes_context_t **context,
     libcerror_error_t **error );

#ifdef __cplusplus
}
#endif

#endif

