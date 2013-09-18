/*
 * Hash values functions
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

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libewf_definitions.h"
#include "libewf_hash_values.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"
#include "libewf_libcstring.h"
#include "libewf_libfvalue.h"
#include "libewf_libuna.h"

#include "ewf_definitions.h"

/* Initializes the hash values
 * Returns 1 if successful or -1 on error
 */
int libewf_hash_values_initialize(
     libfvalue_table_t **hash_values,
     libcerror_error_t **error )
{
	static char *function = "libewf_hash_values_initialize";

	if( libfvalue_table_initialize(
	     hash_values,
	     LIBEWF_HASH_VALUES_DEFAULT_NUMBER,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create hash values.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Parses a MD5 hash for its value
 * Returns 1 if successful or -1 on error
 */
int libewf_hash_values_parse_md5_hash(
     libfvalue_table_t *hash_values,
     const uint8_t *md5_hash,
     size_t md5_hash_size,
     libcerror_error_t **error )
{
	uint8_t md5_hash_string[ 33 ];

	libfvalue_value_t *hash_value = NULL;
	static char *function         = "libewf_hash_values_parse_md5_hash";
	size_t md5_hash_index         = 0;
	size_t md5_hash_string_index  = 0;
	uint8_t md5_digit             = 0;
	int result                    = 0;

	if( md5_hash == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MD5 hash.",
		 function );

		return( -1 );
	}
	if( md5_hash_size < 16 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: MD5 hash too small.",
		 function );

		return( -1 );
	}
	result = libfvalue_table_get_value_by_identifier(
	          hash_values,
	          (uint8_t *) "MD5",
	          4,
	          &hash_value,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve if hash value: MD5.",
		 function );

		goto on_error;
	}
	else if( result == 0 )
	{
		for( md5_hash_index = 0;
		     md5_hash_index < md5_hash_size;
		     md5_hash_index++ )
		{
			md5_digit = md5_hash[ md5_hash_index ] / 16;

			if( md5_digit <= 9 )
			{
				md5_hash_string[ md5_hash_string_index++ ] = (uint8_t) '0' + md5_digit;
			}
			else
			{
				md5_hash_string[ md5_hash_string_index++ ] = (uint8_t) 'a' + ( md5_digit - 10 );
			}
			md5_digit = md5_hash[ md5_hash_index ] % 16;

			if( md5_digit <= 9 )
			{
				md5_hash_string[ md5_hash_string_index++ ] = (uint8_t) '0' + md5_digit;
			}
			else
			{
				md5_hash_string[ md5_hash_string_index++ ] = (uint8_t) 'a' + ( md5_digit - 10 );
			}
		}
		md5_hash_string[ md5_hash_string_index++ ] = 0;

		if( libfvalue_value_type_initialize(
		     &hash_value,
		     LIBFVALUE_VALUE_TYPE_STRING_UTF8,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create hash value.",
			 function );

			goto on_error;
		}
		if( libfvalue_value_set_identifier(
		     hash_value,
		     (uint8_t *) "MD5",
		     4,
		     LIBFVALUE_VALUE_IDENTIFIER_FLAG_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set hash value: MD5 identifier.",
			 function );

			goto on_error;
		}
		if( libfvalue_value_set_data(
		     hash_value,
		     (uint8_t *) md5_hash_string,
		     33,
		     LIBFVALUE_CODEPAGE_UTF8,
		     LIBFVALUE_VALUE_DATA_FLAG_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set hash value: MD5 data.",
			 function );

			goto on_error;
		}
		if( libfvalue_table_set_value(
		     hash_values,
		     hash_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set hash value: MD5 in table.",
			 function );

			goto on_error;
		}
		hash_value = NULL;
	}
	return( 1 );

on_error:
	if( hash_value != NULL )
	{
		libfvalue_value_free(
		 &hash_value,
		 NULL );
	}
	return( -1 );
}

/* Parses an SHA1 hash for its value
 * Returns 1 if successful or -1 on error
 */
int libewf_hash_values_parse_sha1_hash(
     libfvalue_table_t *hash_values,
     const uint8_t *sha1_hash,
     size_t sha1_hash_size,
     libcerror_error_t **error )
{
	uint8_t sha1_hash_string[ 41 ];

	libfvalue_value_t *hash_value = NULL;
	static char *function         = "libewf_hash_values_parse_sha1_hash";
	size_t sha1_hash_index        = 0;
	size_t sha1_hash_string_index = 0;
	int result                    = 0;
	uint8_t sha1_digit            = 0;

	if( hash_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hash values.",
		 function );

		return( -1 );
	}
	if( sha1_hash == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid SHA1 hash.",
		 function );

		return( -1 );
	}
	if( sha1_hash_size < 20 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: SHA1 hash too small.",
		 function );

		return( -1 );
	}
	result = libfvalue_table_get_value_by_identifier(
	          hash_values,
	          (uint8_t *) "SHA1",
	          5,
	          &hash_value,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve if hash value: SHA1.",
		 function );

		goto on_error;
	}
	else if( result == 0 )
	{
		for( sha1_hash_index = 0;
		     sha1_hash_index < sha1_hash_size;
		     sha1_hash_index++ )
		{
			sha1_digit = sha1_hash[ sha1_hash_index ] / 16;

			if( sha1_digit <= 9 )
			{
				sha1_hash_string[ sha1_hash_string_index++ ] = (uint8_t) '0' + sha1_digit;
			}
			else
			{
				sha1_hash_string[ sha1_hash_string_index++ ] = (uint8_t) 'a' + ( sha1_digit - 10 );
			}
			sha1_digit = sha1_hash[ sha1_hash_index ] % 16;

			if( sha1_digit <= 9 )
			{
				sha1_hash_string[ sha1_hash_string_index++ ] = (uint8_t) '0' + sha1_digit;
			}
			else
			{
				sha1_hash_string[ sha1_hash_string_index++ ] = (uint8_t) 'a' + ( sha1_digit - 10 );
			}
		}
		sha1_hash_string[ sha1_hash_string_index++ ] = 0;

		if( libfvalue_value_type_initialize(
		     &hash_value,
		     LIBFVALUE_VALUE_TYPE_STRING_UTF8,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create hash value.",
			 function );

			goto on_error;
		}
		if( libfvalue_value_set_identifier(
		     hash_value,
		     (uint8_t *) "SHA1",
		     5,
		     LIBFVALUE_VALUE_IDENTIFIER_FLAG_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set hash value: SHA1 identifier.",
			 function );

			goto on_error;
		}
		if( libfvalue_value_set_data(
		     hash_value,
		     (uint8_t *) sha1_hash_string,
		     41,
		     LIBFVALUE_CODEPAGE_UTF8,
		     LIBFVALUE_VALUE_DATA_FLAG_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set hash value: SHA1 data.",
			 function );

			goto on_error;
		}
		if( libfvalue_table_set_value(
		     hash_values,
		     hash_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set hash value: SHA1 in table.",
			 function );

			goto on_error;
		}
		hash_value = NULL;
	}
	return( 1 );

on_error:
	if( hash_value != NULL )
	{
		libfvalue_value_free(
		 &hash_value,
		 NULL );
	}
	return( -1 );
}

/* Parse an xhash for the hash values
 * Returns 1 if successful or -1 on error
 */
int libewf_hash_values_parse_xhash(
     libfvalue_table_t *hash_values,
     const uint8_t *xhash,
     size_t xhash_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_hash_values_parse_xhash";

	if( libfvalue_table_copy_from_utf8_xml_string(
	     hash_values,
	     xhash,
	     xhash_size,
	     (uint8_t *) "xhash",
	     5,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy UTF-8 string to hash values table.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Generate an xhash
 * Sets xhash and xhash size
 * Returns 1 if successful or -1 on error
 */
int libewf_hash_values_generate_xhash(
     libfvalue_table_t *hash_values,
     uint8_t **xhash,
     size_t *xhash_size,
     libcerror_error_t **error )
{
	libfvalue_value_t *hash_value     = NULL;
	uint8_t *identifier               = NULL;
	char *xml_head                    = NULL;
	char *xml_xhash_close_tag         = NULL;
	char *xml_xhash_open_tag          = NULL;
	static char *function             = "libewf_hash_values_generate_xhash";
	size_t xhash_index                = 0;
	size_t identifier_size            = 0;
	size_t value_string_size          = 0;
	size_t xml_head_length            = 0;
	size_t xml_xhash_close_tag_length = 0;
	size_t xml_xhash_open_tag_length  = 0;
	int hash_value_index              = 0;
	int number_of_hash_values         = 0;
	int result                        = 0;

	if( xhash == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid xhash.",
		 function );

		return( -1 );
	}
	if( *xhash != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: xhash already created.",
		 function );

		return( -1 );
	}
	if( xhash_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid xhash size.",
		 function );

		return( -1 );
	}
	if( libfvalue_table_get_number_of_values(
	     hash_values,
	     &number_of_hash_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of hash values.",
		 function );

		goto on_error;
	}
	xml_head = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";

	xml_head_length = libcstring_narrow_string_length(
	                   xml_head );

	xml_xhash_open_tag = "<xhash>\n";

	xml_xhash_open_tag_length = libcstring_narrow_string_length(
	                             xml_xhash_open_tag );

	xml_xhash_close_tag = "</xhash>\n\n";

	xml_xhash_close_tag_length = libcstring_narrow_string_length(
	                              xml_xhash_close_tag );

	/* Reserve space for the UTF-8 byte order mark and the XML skeleton data
	 */
	*xhash_size = 3 + xml_head_length + xml_xhash_open_tag_length + xml_xhash_close_tag_length;

	for( hash_value_index = 0;
	     hash_value_index < number_of_hash_values;
	     hash_value_index++ )
	{
		if( libfvalue_table_get_value_by_index(
		     hash_values,
		     hash_value_index,
		     &hash_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve hash value: %d.",
			 function,
			 hash_value_index );

			goto on_error;
		}
		if( libfvalue_value_get_identifier(
		     hash_value,
		     &identifier,
		     &identifier_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve identifier of hash value: %d.",
			 function,
			 hash_value_index );

			goto on_error;
		}
		if( ( identifier == NULL )
		 || ( identifier_size == 0 ) )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: missing identifier for hash value: %d.\n",
				 function,
				 hash_value_index );
			}
#endif
			continue;
		}
		result = libfvalue_value_get_utf8_string_size(
		          hash_value,
		          0,
		          &value_string_size,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve string size of hash value: %s.",
			 function,
			 (char *) identifier );

			goto on_error;
		}
		if( ( result != 0 )
		 && ( value_string_size > 1 ) )
		{
			/* Reserve space for a leading tab, <identifier>value</identifier> and a newline
			 */
			*xhash_size += 7 + ( 2 * ( identifier_size - 1 ) ) + ( value_string_size - 1 );
		}
	}
	/* Reserve space for the end-of-string character
	 */
	*xhash_size += 1;

	*xhash = (uint8_t *) memory_allocate(
                              sizeof( uint8_t ) * *xhash_size );

	if( *xhash == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: unable to create xhash.",
		 function );

		goto on_error;
	}
	( *xhash )[ xhash_index++ ] = 0xef;
	( *xhash )[ xhash_index++ ] = 0xbb;
	( *xhash )[ xhash_index++ ] = 0xbf;

	if( libcstring_narrow_string_copy(
	     (char *) &( ( *xhash )[ xhash_index ] ),
	     xml_head,
	     xml_head_length ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy XML head string.",
		 function );

		goto on_error;
	}
	xhash_index += xml_head_length;

	if( libcstring_narrow_string_copy(
	     (char *) &( ( *xhash )[ xhash_index ] ),
	     xml_xhash_open_tag,
	     xml_xhash_open_tag_length ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy xhash open tag string.",
		 function );

		goto on_error;
	}
	xhash_index += xml_xhash_open_tag_length;

	for( hash_value_index = 0;
	     hash_value_index < number_of_hash_values;
	     hash_value_index++ )
	{
		if( libfvalue_table_get_value_by_index(
		     hash_values,
		     hash_value_index,
		     &hash_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve hash value: %d.",
			 function,
			 hash_value_index );

			goto on_error;
		}
		if( libfvalue_value_get_identifier(
		     hash_value,
		     &identifier,
		     &identifier_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve identifier of hash value: %d.",
			 function,
			 hash_value_index );

			goto on_error;
		}
		if( ( identifier == NULL )
		 || ( identifier_size == 0 ) )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: missing identifier for hash value: %d.\n",
				 function,
				 hash_value_index );
			}
#endif
			continue;
		}
		result = libfvalue_value_get_utf8_string_size(
		          hash_value,
		          0,
		          &value_string_size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve string size of hash value: %s.",
			 function,
			 (char *) identifier );

			goto on_error;
		}
		if( ( result != 0 )
		 && ( value_string_size > 1 ) )
		{
			( *xhash )[ xhash_index++ ] = (uint8_t) '\t';
			( *xhash )[ xhash_index++ ] = (uint8_t) '<';

			if( libcstring_narrow_string_copy(
			     (char *) &( ( *xhash )[ xhash_index ] ),
			     (char *) identifier,
			     identifier_size - 1 ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy %s open tag string.",
				 function,
				 (char *) identifier );

				goto on_error;
			}
			xhash_index += identifier_size - 1;

			( *xhash )[ xhash_index++ ] = (uint8_t) '>';

			if( libfvalue_value_copy_to_utf8_string_with_index(
			     hash_value,
			     0,
			     *xhash,
			     *xhash_size,
			     &xhash_index,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy string of hash value: %s.",
				 function,
				 (char *) identifier );

				goto on_error;
			}
			( *xhash )[ xhash_index - 1 ] = (uint8_t) '<';
			( *xhash )[ xhash_index++   ] = (uint8_t) '/';

			if( libcstring_narrow_string_copy(
			     (char *) &( ( *xhash )[ xhash_index ] ),
			     (char *) identifier,
			     identifier_size - 1 ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy %s close tag string.",
				 function,
				 (char *) identifier );

				goto on_error;
			}
			xhash_index += identifier_size - 1;

			( *xhash )[ xhash_index++ ] = (uint8_t) '>';
			( *xhash )[ xhash_index++ ] = (uint8_t) '\n';
		}
	}
	if( libcstring_narrow_string_copy(
	     (char *) &( ( *xhash )[ xhash_index ] ),
	     xml_xhash_close_tag,
	     xml_xhash_close_tag_length ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy xhash close tag string.",
		 function );

		goto on_error;
	}
	xhash_index += xml_xhash_close_tag_length;

	/* Make sure the string is terminated
	 */
	( *xhash )[ xhash_index ] = 0;

	return( 1 );

on_error:
	if( *xhash != NULL )
	{
		memory_free(
		 *xhash );

		*xhash = NULL;
	}
	*xhash_size = 0;

	return( -1 );
}

/* Generate a MD5 hash
 * Returns 1 if successful or -1 on error
 */
int libewf_hash_values_generate_md5_hash(
     libfvalue_table_t *hash_values,
     uint8_t *md5_hash,
     size_t md5_hash_size,
     uint8_t *md5_hash_set,
     libcerror_error_t **error )
{
	uint8_t md5_hash_string[ 33 ];

	libfvalue_value_t *hash_value = NULL;
	static char *function         = "libewf_hash_values_generate_md5_hash";
	size_t md5_hash_index         = 0;
	size_t md5_hash_string_index  = 0;
	uint8_t md5_digit             = 0;
	int result                    = 0;

	if( hash_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hash values.",
		 function );

		return( -1 );
	}
	if( md5_hash == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MD5 hash.",
		 function );

		return( -1 );
	}
	if( md5_hash_size < 16 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: MD5 hash too small.",
		 function );

		return( -1 );
	}
	if( md5_hash_set == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MD5 hash set.",
		 function );

		return( -1 );
	}
	result = libfvalue_table_get_value_by_identifier(
	          hash_values,
	          (uint8_t *) "MD5",
	          4,
	          &hash_value,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve if hash value: MD5.",
		 function );

		return( -1 );
	}
	/* No need to generate the MD5 hash
	 */
	else if( result == 0 )
	{
		*md5_hash_set = 0;

		return( 1 );
	}
	if( libfvalue_value_copy_to_utf8_string(
	     hash_value,
	     0,
	     md5_hash_string,
	     33,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy hash value: MD5 to UTF-8 string.",
		 function );

		return( -1 );
	}
	while( md5_hash_string_index < 32 )
	{
		if( ( md5_hash_string[ md5_hash_string_index ] >= (uint8_t) '0' )
		 && ( md5_hash_string[ md5_hash_string_index ] <= (uint8_t) '9' ) )
		{
			md5_digit = (uint8_t) ( md5_hash_string[ md5_hash_string_index ] - (uint8_t) '0' );
		}
		else if( ( md5_hash_string[ md5_hash_string_index ] >= (uint8_t) 'A' )
		      && ( md5_hash_string[ md5_hash_string_index ] <= (uint8_t) 'F' ) )
		{
			md5_digit = 10 + (uint8_t) ( md5_hash_string[ md5_hash_string_index ] - (uint8_t) 'A' );
		}
		else if( ( md5_hash_string[ md5_hash_string_index ] >= (uint8_t) 'a' )
		      && ( md5_hash_string[ md5_hash_string_index ] <= (uint8_t) 'f' ) )
		{
			md5_digit = 10 + (uint8_t) ( md5_hash_string[ md5_hash_string_index ] - (uint8_t) 'a' );
		}
		else if( md5_hash_string[ md5_hash_string_index ] == 0 )
		{
			md5_digit = 0;
		}
		else
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid hexadecimal digit: 0x%02" PRIx8 " in MD5 string.",
			 function,
			 md5_hash_string[ md5_hash_string_index ] );

			return( -1 );
		}
		md5_hash_string_index++;

		md5_digit <<= 4;

		if( ( md5_hash_string[ md5_hash_string_index ] >= (uint8_t) '0' )
		 && ( md5_hash_string[ md5_hash_string_index ] <= (uint8_t) '9' ) )
		{
			md5_digit += (uint8_t) ( md5_hash_string[ md5_hash_string_index ] - (uint8_t) '0' );
		}
		else if( ( md5_hash_string[ md5_hash_string_index ] >= (uint8_t) 'A' )
		      && ( md5_hash_string[ md5_hash_string_index ] <= (uint8_t) 'F' ) )
		{
			md5_digit += 10 + (uint8_t) ( md5_hash_string[ md5_hash_string_index ] - (uint8_t) 'A' );
		}
		else if( ( md5_hash_string[ md5_hash_string_index ] >= (uint8_t) 'a' )
		      && ( md5_hash_string[ md5_hash_string_index ] <= (uint8_t) 'f' ) )
		{
			md5_digit += 10 + (uint8_t) ( md5_hash_string[ md5_hash_string_index ] - (uint8_t) 'a' );
		}
		else if( md5_hash_string[ md5_hash_string_index ] != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid hexadecimal digit: 0x%02" PRIx8 " in MD5 string.",
			 function,
			 md5_hash_string[ md5_hash_string_index ] );

			return( -1 );
		}
		md5_hash_string_index++;

		md5_hash[ md5_hash_index++ ] = md5_digit;
	}
	*md5_hash_set = 1;

	return( 1 );
}

/* Generate an SHA1 hash
 * Returns 1 if successful or -1 on error
 */
int libewf_hash_values_generate_sha1_hash(
     libfvalue_table_t *hash_values,
     uint8_t *sha1_hash,
     size_t sha1_hash_size,
     uint8_t *sha1_hash_set,
     libcerror_error_t **error )
{
	uint8_t sha1_hash_string[ 41 ];

	libfvalue_value_t *hash_value = NULL;
	static char *function         = "libewf_hash_values_generate_sha1_hash";
	size_t sha1_hash_index        = 0;
	size_t sha1_hash_string_index = 0;
	uint8_t sha1_digit            = 0;
	int result                    = 0;

	if( hash_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hash values.",
		 function );

		return( -1 );
	}
	if( sha1_hash == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid SHA1 hash.",
		 function );

		return( -1 );
	}
	if( sha1_hash_size < 20 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: SHA1 hash too small.",
		 function );

		return( -1 );
	}
	if( sha1_hash_set == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid SHA1 hash set.",
		 function );

		return( -1 );
	}
	result = libfvalue_table_get_value_by_identifier(
	          hash_values,
	          (uint8_t *) "SHA1",
	          5,
	          &hash_value,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve if hash value: SHA1.",
		 function );

		return( -1 );
	}
	/* No need to generate the SHA1 hash
	 */
	else if( result == 0 )
	{
		*sha1_hash_set = 0;

		return( 1 );
	}
	if( libfvalue_value_copy_to_utf8_string(
	     hash_value,
	     0,
	     sha1_hash_string,
	     41,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy hash value: SHA1 to UTF-8 string.",
		 function );

		return( -1 );
	}
	while( sha1_hash_string_index < 40 )
	{
		if( ( sha1_hash_string[ sha1_hash_string_index ] >= (uint8_t) '0' )
		 && ( sha1_hash_string[ sha1_hash_string_index ] <= (uint8_t) '9' ) )
		{
			sha1_digit = (uint8_t) ( sha1_hash_string[ sha1_hash_string_index ] - (uint8_t) '0' );
		}
		else if( ( sha1_hash_string[ sha1_hash_string_index ] >= (uint8_t) 'A' )
		      && ( sha1_hash_string[ sha1_hash_string_index ] <= (uint8_t) 'F' ) )
		{
			sha1_digit = 10 + (uint8_t) ( sha1_hash_string[ sha1_hash_string_index ] - (uint8_t) 'A' );
		}
		else if( ( sha1_hash_string[ sha1_hash_string_index ] >= (uint8_t) 'a' )
		      && ( sha1_hash_string[ sha1_hash_string_index ] <= (uint8_t) 'f' ) )
		{
			sha1_digit = 10 + (uint8_t) ( sha1_hash_string[ sha1_hash_string_index ] - (uint8_t) 'a' );
		}
		else if( sha1_hash_string[ sha1_hash_string_index ] == 0 )
		{
			sha1_digit = 0;
		}
		else
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid hexadecimal digit: 0x%02" PRIx8 " in SHA1 string.",
			 function,
			 sha1_hash_string[ sha1_hash_string_index ] );

			return( -1 );
		}
		sha1_hash_string_index++;

		sha1_digit <<= 4;

		if( ( sha1_hash_string[ sha1_hash_string_index ] >= (uint8_t) '0' )
		 && ( sha1_hash_string[ sha1_hash_string_index ] <= (uint8_t) '9' ) )
		{
			sha1_digit += (uint8_t) ( sha1_hash_string[ sha1_hash_string_index ] - (uint8_t) '0' );
		}
		else if( ( sha1_hash_string[ sha1_hash_string_index ] >= (uint8_t) 'A' )
		      && ( sha1_hash_string[ sha1_hash_string_index ] <= (uint8_t) 'F' ) )
		{
			sha1_digit += 10 + (uint8_t) ( sha1_hash_string[ sha1_hash_string_index ] - (uint8_t) 'A' );
		}
		else if( ( sha1_hash_string[ sha1_hash_string_index ] >= (uint8_t) 'a' )
		      && ( sha1_hash_string[ sha1_hash_string_index ] <= (uint8_t) 'f' ) )
		{
			sha1_digit += 10 + (uint8_t) ( sha1_hash_string[ sha1_hash_string_index ] - (uint8_t) 'a' );
		}
		else if( sha1_hash_string[ sha1_hash_string_index ] != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid hexadecimal digit: 0x%02" PRIx8 " in SHA1 string.",
			 function,
			 sha1_hash_string[ sha1_hash_string_index ] );

			return( -1 );
		}
		sha1_hash_string_index++;

		sha1_hash[ sha1_hash_index++ ] = sha1_digit;
	}
	*sha1_hash_set = 1;

	return( 1 );
}

