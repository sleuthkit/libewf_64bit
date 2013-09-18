/*
 * AES encryption functions
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

#include <common.h>
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#if defined( WINAPI )
#include <wincrypt.h>

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_AES_H )
#include <openssl/sha.h>

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
#include <openssl/evp.h>

#endif

#include "libcaes_context.h"
#include "libcaes_crypt.h"
#include "libcaes_definitions.h"
#include "libcaes_key.h"
#include "libcaes_libcerror.h"
#include "libcaes_libcstring.h"
#include "libcaes_types.h"

#if !defined( LIBCAES_HAVE_AES_SUPPORT )
static int libcaes_tables_initialized = 0;
#endif

/* Initializes the AES context
 * The context must point to a NULL pointer to be allocated
 * Returns 1 if successful or -1 on error
 */
int libcaes_context_initialize(
     libcaes_context_t **context,
     libcerror_error_t **error )
{
	libcaes_internal_context_t *internal_context = NULL;
	static char *function                        = "libcaes_context_initialize";

#if defined( WINAPI ) && ( WINVER >= 0x0600 )
	DWORD error_code                             = 0;
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
	if( *context == NULL )
	{
		internal_context = memory_allocate_structure(
		                    libcaes_internal_context_t );

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
		     sizeof( libcaes_internal_context_t ) ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear context.",
			 function );

			goto on_error;
		}
#if defined( WINAPI ) && ( WINVER >= 0x0600 )
		/* Request the AES crypt provider, fail back to the RSA crypt provider
		*/
		if( CryptAcquireContext(
		     &( internal_context->crypt_provider ),
		     NULL,
		     MS_ENH_RSA_AES_PROV,
		     PROV_RSA_AES,
		     0 ) == 0 )
		{
/* TODO fallback for XP
			if( CryptAcquireContext(
			     &( internal_context->crypt_provider ),
			     NULL,
			     MS_ENH_RSA_AES_PROV_XP,
			     PROV_RSA_AES,
			     CRYPT_NEWKEYSET ) == 0 )
*/
			error_code = GetLastError();

			libcerror_system_set_error(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 error_code,
			 "%s: unable to create AES crypt provider.",
			 function );

			return( -1 );
		}
		if( internal_context->crypt_provider == 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: unable to create AES crypt provider.",
			 function );

			return( -1 );
		}

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_AES_H )
		/* No additional initialization necessary */

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
		EVP_CIPHER_CTX_init(
		 &( internal_context->evp_context ) );

		if( EVP_CIPHER_CTX_set_padding(
		     &( internal_context->evp_context ),
		     1 ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set padding in context.",
			 function );

			goto on_error;
		}

#else
		if( libcaes_tables_initialized == 0 )
		{
			if( libcaes_initialize_tables(
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to initialize tables.",
				 function );

				goto on_error;
			}
			libcaes_tables_initialized = 1;
		}
#endif
		*context = (libcaes_context_t *) internal_context;
	}
	return( 1 );

on_error:
	if( internal_context != NULL )
	{
		memory_free(
		 internal_context );
	}
	return( -1 );
}

/* Frees an AES context
 * Returns 1 if successful or -1 on error
 */
int libcaes_context_free(
     libcaes_context_t **context,
     libcerror_error_t **error )
{
	libcaes_internal_context_t *internal_context = NULL;
	static char *function                        = "libcaes_context_free";

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
		internal_context = (libcaes_internal_context_t *) *context;
		*context         = NULL;

#if defined( WINAPI ) && ( WINVER >= 0x0600 )
		if( internal_context->key != 0 )
		{
			CryptDestroyKey(
			 internal_context->key );
		}
		if( internal_context->crypt_provider != 0 )
		{
			CryptReleaseContext(
			 internal_context->crypt_provider,
			 0 );
		}

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_AES_H )
		/* No additional clean up necessary */

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
		EVP_CIPHER_CTX_cleanup(
		 &( internal_context->evp_context ) );

#else
		/* No additional clean up necessary */
#endif
		memory_free(
		 internal_context );
	}
	return( 1 );
}

