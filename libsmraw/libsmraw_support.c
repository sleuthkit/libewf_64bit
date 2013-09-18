/*
 * Support functions
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

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libsmraw_definitions.h"
#include "libsmraw_libbfio.h"
#include "libsmraw_libcerror.h"
#include "libsmraw_libclocale.h"
#include "libsmraw_libcstring.h"
#include "libsmraw_support.h"

#if !defined( HAVE_LOCAL_LIBSMRAW )

/* Returns the library version
 */
const char *libsmraw_get_version(
             void )
{
	return( (const char *) LIBSMRAW_VERSION_STRING );
}

/* Returns the access flags for reading
 */
int libsmraw_get_access_flags_read(
     void )
{
	return( (int) LIBSMRAW_ACCESS_FLAG_READ );
}

/* Returns the access flags for reading and writing
 */
int libsmraw_get_access_flags_read_write(
     void )
{
	return( (int) ( LIBSMRAW_ACCESS_FLAG_READ | LIBSMRAW_ACCESS_FLAG_WRITE ) );
}

/* Returns the access flags for writing
 */
int libsmraw_get_access_flags_write(
     void )
{
	return( (int) LIBSMRAW_ACCESS_FLAG_WRITE );
}

/* Retrieves the narrow system string codepage
 * A value of 0 represents no codepage, UTF-8 encoding is used instead
 * Returns 1 if successful or -1 on error
 */
int libsmraw_get_codepage(
     int *codepage,
     libcerror_error_t **error )
{
	static char *function = "libsmraw_get_codepage";

	if( libclocale_codepage_get(
	     codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve codepage.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the narrow system string codepage
 * A value of 0 represents no codepage, UTF-8 encoding is used instead
 * Returns 1 if successful or -1 on error
 */
int libsmraw_set_codepage(
     int codepage,
     libcerror_error_t **error )
{
	static char *function = "libsmraw_set_codepage";

	if( libclocale_codepage_set(
	     codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set codepage.",
		 function );

		return( -1 );
	}
	return( 1 );
}

#endif /* !defined( HAVE_LOCAL_LIBSMRAW ) */

/* Appends a globbed filename to the filenames
 * Returns 1 if successful or -1 on error
 */
int libsmraw_glob_append_segment_file(
     char **filenames[],
     int *number_of_filenames,
     const char *segment_filename,
     libcerror_error_t **error )
{
	static char *function = "libsmraw_glob_append_segment_file";
	void *reallocation    = NULL;

	if( filenames == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filenames.",
		 function );

		return( -1 );
	}
	if( number_of_filenames == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of filenames.",
		 function );

		return( -1 );
	}
	if( segment_filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment filename.",
		 function );

		return( -1 );
	}
	reallocation = memory_reallocate(
			*filenames,
			sizeof( char * ) * ( *number_of_filenames + 1 ) );

	if( reallocation == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable reallocate filenames.",
		 function );

		return( -1 );
	}
	*filenames = reallocation;

	( *filenames )[ *number_of_filenames ] = (char *) segment_filename;

	*number_of_filenames += 1;

	return( 1 );
}

/* Determines if a segment file exists
 * The segment filename is made up from the prefix and the suffix
 * Returns 1 if segment file exists, 0 if not or -1 on error
 */
int libsmraw_glob_exists_segment_file(
     libbfio_handle_t *file_io_handle,
     const char *prefix,
     size_t prefix_length,
     const char *suffix,
     size_t suffix_length,
     char **segment_filename,
     size_t *segment_filename_size,
     libcerror_error_t **error )
{
	static char *function = "libsmraw_glob_exists_segment_file";
	int result            = 0;

	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( prefix == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid prefix.",
		 function );

		return( -1 );
	}
	if( prefix_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid prefix length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( suffix != NULL )
	 && ( suffix_length > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid suffix length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( segment_filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment filename.",
		 function );

		return( -1 );
	}
	if( segment_filename_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment filename size.",
		 function );

		return( -1 );
	}
	/* The segment filename consists of the prefix, an end of string character
	 * and (if provided) the suffix
	 */
	*segment_filename_size = prefix_length + 1;

	if( suffix != NULL )
	{
		*segment_filename_size += suffix_length;
	}
	*segment_filename = (char *) memory_allocate(
	                              sizeof( char ) * *segment_filename_size );

	if( *segment_filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create segment filename.",
		 function );

		goto on_error;
	}
	if( libcstring_narrow_string_copy(
	     *segment_filename,
	     prefix,
	     prefix_length ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set prefix in segment filename.",
		 function );

		goto on_error;
	}
	( *segment_filename )[ prefix_length ] = 0;

	if( suffix != NULL )
	{
		if( libcstring_narrow_string_copy(
		     &( ( *segment_filename )[ prefix_length ] ),
		     suffix,
		     suffix_length ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set suffix in segment filename.",
			 function );

			goto on_error;
		}
		( *segment_filename )[ prefix_length + suffix_length ] = 0;
	}
	if( libbfio_file_set_name(
	     file_io_handle,
	     *segment_filename,
	     *segment_filename_size - 1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set name in file IO handle.",
		 function );

		goto on_error;
	}
	result = libbfio_handle_exists(
		  file_io_handle,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_GENERIC,
		 "%s: unable to determine if file: %s exists.",
		 function,
		 *segment_filename );

		goto on_error;
	}
	return( result );

on_error:
	if( *segment_filename != NULL )
	{
		memory_free(
		 *segment_filename );

		*segment_filename = NULL;
	}
	*segment_filename_size = 0;

	return( -1 );
}

/* Determines the naming schema
 * Returns 1 if successful, 0 if no naming schema could be determined or -1 on error
 */
int libsmraw_glob_determine_naming_schema(
     const char *suffix,
     size_t suffix_length,
     uint8_t *naming_schema,
     libcerror_error_t **error )
{
	static char *function = "libsmraw_glob_determine_naming_schema";
	size_t suffix_index   = 0;

	if( suffix == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid suffix.",
		 function );

		return( -1 );
	}
	if( suffix_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid suffix length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( naming_schema == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid naming schema.",
		 function );

		return( -1 );
	}
	*naming_schema = LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_UNKNOWN;

	/* Determine naming schema
	 */
	if( suffix_length == 2 )
	{
		if( ( ( suffix[ 0 ] == 'd' )
		   || ( suffix[ 0 ] == 'D' ) )
		 && ( ( suffix[ 1 ] == 'd' )
		   || ( suffix[ 1 ] == 'D' ) ) )
		{
			*naming_schema = LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_SINGLE;
		}
	}
	else if( suffix_length == 3 )
	{
		if( ( ( suffix[ 0 ] == 'd' )
		   || ( suffix[ 0 ] == 'D' )
		   || ( suffix[ 0 ] == 'i' )
		   || ( suffix[ 0 ] == 'I' ) )
		 && ( ( suffix[ 1 ] == 'm' )
		   || ( suffix[ 1 ] == 'M' ) )
		 && ( ( suffix[ 2 ] == 'g' )
		   || ( suffix[ 2 ] == 'G' ) ) )
		{
			*naming_schema = LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_SINGLE;
		}
		else if( ( ( suffix[ 0 ] == 'r' )
		        || ( suffix[ 0 ] == 'R' ) )
		      && ( ( suffix[ 1 ] == 'a' )
		        || ( suffix[ 1 ] == 'A' ) )
		      && ( ( suffix[ 2 ] == 'w' )
		        || ( suffix[ 2 ] == 'W' ) ) )
		{
			*naming_schema = LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_SINGLE;
		}
	}
	else if( suffix_length >= 4 )
	{
		for( suffix_index = 0;
		     suffix_index < suffix_length;
		     suffix_index++ )
		{
			if( ( suffix[ suffix_index ] >= '0' )
			 && ( suffix[ suffix_index ] <= '9' ) )
			{
				*naming_schema = LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_X_OF_N;
			}
			else if( ( suffix_index > 0 )
			      && ( ( suffix[ suffix_index ] == 'o' )
			        || ( suffix[ suffix_index ] == 'O' ) ) )
			{
				*naming_schema = LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_X_OF_N;

				break;
			}
			else
			{
				*naming_schema = LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_UNKNOWN;

				break;
			}
		}
		if( *naming_schema == LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_X_OF_N )
		{
			*naming_schema = LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_UNKNOWN;

			if( suffix_index < suffix_length )
			{
				if( ( suffix[ suffix_index ] == 'o' )
				 || ( suffix[ suffix_index ] == 'O' ) )
				{
					*naming_schema = LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_X_OF_N;

					suffix_index++;
				}
			}
		}
		if( *naming_schema == LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_X_OF_N )
		{
			*naming_schema = LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_UNKNOWN;

			if( suffix_index < suffix_length )
			{
				if( ( suffix[ suffix_index ] == 'f' )
				 || ( suffix[ suffix_index ] == 'F' ) )
				{
					*naming_schema = LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_X_OF_N;

					suffix_index++;
				}
			}
		}
		if( *naming_schema == LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_X_OF_N )
		{
			while( suffix_index < suffix_length )
			{
				if( ( suffix[ suffix_index ] >= '0' )
				 && ( suffix[ suffix_index ] <= '9' ) )
				{
					*naming_schema = LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_X_OF_N;
				}
				else
				{
					*naming_schema = LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_UNKNOWN;

					break;
				}
				suffix_index++;
			}
		}
	}
	if( *naming_schema == LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_UNKNOWN )
	{
		for( suffix_index = 0;
		     suffix_index < suffix_length;
		     suffix_index++ )
		{
			if( suffix[ suffix_index ] == 'a' )
			{
				*naming_schema = LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_SPLIT;
			}
			else if( ( suffix[ suffix_index ] == '0' )
			      && ( suffix_index < ( suffix_length - 1 ) ) )
			{
				*naming_schema = LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_NUMERIC;
			}
			else if( ( ( suffix[ suffix_index ] == '0' )
			        || ( suffix[ suffix_index ] == '1' ) )
			      && ( suffix_index == ( suffix_length - 1 ) ) )
			{
				*naming_schema = LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_NUMERIC;
			}
			else
			{
				*naming_schema = LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_UNKNOWN;

				break;
			}
		}
	}
	if( *naming_schema == LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_UNKNOWN )
	{
		return( 0 );
	}
	return( 1 );
}

/* Globs the files according to common (split) RAW naming schemas
 * Returns 1 if successful or -1 on error
 */
int libsmraw_glob(
     const char *filename,
     size_t filename_length,
     char **filenames[],
     int *number_of_filenames,
     libcerror_error_t **error )
{
	char *suffixes[ ] = {
		".raw",
		".dmg",
		".img",
		".dd",
		".000",
		".001",
		".00",
		".01",
		".0",
		".1",
		"aa",
		"00",
		"aaa",
		"000",
		NULL
	};

	libbfio_handle_t *file_io_handle    = NULL;
	char *identifier                    = NULL;
	char *segment_filename              = NULL;
	char *segment_filename_copy         = NULL;
	char *suffix                        = NULL;
	static char *function               = "libsmraw_glob";
	size_t identifier_divider           = 0;
	size_t identifier_expansion_length  = 2;
	size_t identifier_index             = 0;
	size_t identifier_length            = 0;
	size_t segment_filename_copy_index  = 0;
	size_t segment_filename_copy_length = 0;
	size_t segment_filename_copy_size   = 0;
	size_t segment_filename_size        = 0;
	size_t suffix_length                = 0;
	uint8_t allow_identifier_expansion  = 0;
	uint8_t expand_identifier           = 0;
	uint8_t naming_schema               = 0;
	int maximum_segment_file_identifier = 0;
	int result                          = 0;
	int segment_file_identifier         = 0;
	int suffix_index                    = 0;

	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( filenames == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filenames.",
		 function );

		return( -1 );
	}
	if( number_of_filenames == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of filenames.",
		 function );

		return( -1 );
	}
	*number_of_filenames = 0;

	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO handle.",
		 function );

		goto on_error;
	}
	/* Test if the full filename was provided
	 */
	result = libsmraw_glob_exists_segment_file(
		  file_io_handle,
		  filename,
		  filename_length,
		  NULL,
		  0,
		  &segment_filename,
		  &segment_filename_size,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_GENERIC,
		 "%s: unable to determine if file: %s exists.",
		 function,
		 filename );

		goto on_error;
	}
	/* Check if there is a segment file with a known suffix
	 */
	else if( result == 0 )
	{
		suffix_index = 0;

		while( result == 0 )
		{
			if( segment_filename != NULL )
			{
				memory_free(
				 segment_filename );

				segment_filename = NULL;
			}
			suffix = suffixes[ suffix_index ];

			if( suffix == NULL )
			{
				break;
			}
			suffix_length = libcstring_narrow_string_length(
			                 suffix );

			result = libsmraw_glob_exists_segment_file(
				  file_io_handle,
				  filename,
				  filename_length,
				  suffix,
				  suffix_length,
				  &segment_filename,
				  &segment_filename_size,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_GENERIC,
				 "%s: unable to determine if segment file with suffix: %s exists.",
				 function,
				 suffix );

				goto on_error;
			}
			suffix_index++;
		}
	}
	if( result == 0 )
	{
		if( segment_filename != NULL )
		{
			memory_free(
			 segment_filename );
		}
		return( 1 );
	}
	if( libsmraw_glob_append_segment_file(
	     filenames,
	     number_of_filenames,
	     segment_filename,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append segment filename.",
		 function );

		goto on_error;
	}
	/* Make a copy of the segment filename to tamper with
	 */
	segment_filename_copy = libcstring_narrow_string_allocate(
	                         segment_filename_size + identifier_expansion_length );

	if( segment_filename_copy == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create segment filename copy.",
		 function );

		goto on_error;
	}
	if( libcstring_narrow_string_copy(
	     segment_filename_copy,
	     segment_filename,
	     segment_filename_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set segment filename copy.",
		 function );

		goto on_error;
	}
	segment_filename_copy[ segment_filename_size - 1 ] = 0;
	segment_filename_copy_size                         = segment_filename_size;

	/* Determine the naming schema
	 */
	if( suffix == NULL )
	{
		/* Determine if the segment filename contains a suffix which contains a segment identifier
		 */
		segment_filename_copy_index = segment_filename_copy_size - 1;

		while( segment_filename_copy_index > 0 )
		{
			if( segment_filename_copy[ segment_filename_copy_index ] == '.' )
			{
				identifier        = &( segment_filename_copy[ segment_filename_copy_index + 1 ] );
				identifier_length = ( segment_filename_copy_size - 1 ) - ( segment_filename_copy_index + 1 );

				break;
			}
			else if( ( segment_filename_copy[ segment_filename_copy_index ] == '/' )
			      || ( segment_filename_copy[ segment_filename_copy_index ] == '\\' ) )
			{
				break;
			}
			segment_filename_copy_index--;
		}
		if( identifier != NULL )
		{
			/* Check if the extension contains a known naming schema
			 */
			result = libsmraw_glob_determine_naming_schema(
			          identifier,
			          identifier_length,
			          &naming_schema,
			          error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine naming schema.",
				 function );

				goto on_error;
			}
		}
		if( naming_schema == LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_UNKNOWN )
		{
			if( identifier == NULL )
			{
				segment_filename_copy_index = segment_filename_copy_size - 1;
			}
			/* Determine if the segment filename contains a segment identifier
			 */
			while( segment_filename_copy_index > 0 )
			{
				if( naming_schema == LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_NUMERIC )
				{
					if( segment_filename_copy[ segment_filename_copy_index ] != '0' )
					{
						identifier = &( segment_filename_copy[ segment_filename_copy_index + 1 ] );

						break;
					}
					identifier_length++;
				}
				else if( naming_schema == LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_SPLIT )
				{
					if( segment_filename_copy[ segment_filename_copy_index ] != 'a' )
					{
						identifier = &( segment_filename_copy[ segment_filename_copy_index + 1 ] );

						break;
					}
					identifier_length++;
				}
				else
				{
					if( ( segment_filename_copy[ segment_filename_copy_index ] == '0' )
					 || ( segment_filename_copy[ segment_filename_copy_index ] == '1' ) )
					{
						naming_schema     = LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_NUMERIC;
						identifier_length = 1;
					}
					else if( segment_filename_copy[ segment_filename_copy_index ] == 'a' )
					{
						naming_schema     = LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_SPLIT;
						identifier_length = 1;
					}
					else if( ( segment_filename_copy[ segment_filename_copy_index ] == '/' )
					      || ( segment_filename_copy[ segment_filename_copy_index ] == '\\' ) )
					{
						break;
					}
				}
				segment_filename_copy_index--;
			}
		}
	}
	else
	{
		segment_filename_copy_index = ( segment_filename_copy_size - 1 ) - suffix_length;

		identifier        = &( segment_filename_copy[ segment_filename_copy_index ] );
		identifier_length = suffix_length;

		/* Ignore the extension dot
		 */
		if( identifier[ 0 ] == '.' )
		{
			identifier        += 1;
			identifier_length -= 1;
		}
		result = libsmraw_glob_determine_naming_schema(
		          identifier,
		          identifier_length,
		          &naming_schema,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine naming schema.",
			 function );

			goto on_error;
		}
	}
	if( naming_schema == LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_UNKNOWN )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid naming schema missing value.",
		 function );

		goto on_error;
	}
	if( naming_schema != LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_SINGLE )
	{
		if( naming_schema == LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_NUMERIC )
		{
			maximum_segment_file_identifier = 1;

			for( identifier_index = 0;
			     identifier_index < identifier_length;
			     identifier_index++ )
			{
				maximum_segment_file_identifier *= 10;
			}
			maximum_segment_file_identifier -= 1;
		}
		else if( naming_schema == LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_SPLIT )
		{
			maximum_segment_file_identifier = 1;

			for( identifier_index = 0;
			     identifier_index < identifier_length;
			     identifier_index++ )
			{
				maximum_segment_file_identifier *= 26;
			}
			maximum_segment_file_identifier -= 1;
		}
		else if( naming_schema == LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_X_OF_N )
		{
			for( identifier_index = identifier_length - 1;
			     identifier_index > 0;
			     identifier_index-- )
			{
				if( ( identifier[ identifier_index ] < '0' )
				 || ( identifier[ identifier_index ] > '9' ) )
				{
					break;
				}
			}
			identifier_index++;

			while( identifier_index < identifier_length )
			{
				maximum_segment_file_identifier *= 10;
				maximum_segment_file_identifier += (int) ( identifier[ identifier_index ] - '0' );

				identifier_index++;
			}
			for( identifier_index = 0;
			     identifier_index < identifier_length;
			     identifier_index++ )
			{
				if( ( identifier[ identifier_index ] < '0' )
				 || ( identifier[ identifier_index ] > '9' ) )
				{
					break;
				}
			}
			identifier_length = identifier_index;
		}
		if( ( naming_schema == LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_NUMERIC )
		 || ( naming_schema == LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_X_OF_N ) )
		{
			segment_file_identifier = (int) ( identifier[ identifier_length - 1 ] - '0' );

			if( identifier_length == 1 )
			{
				allow_identifier_expansion = 1;
			}
		}
		segment_filename_copy_length = segment_filename_copy_size - 1;

		while( *number_of_filenames < INT_MAX )
		{
			if( ( segment_file_identifier + 1 ) > maximum_segment_file_identifier )
			{
				if( ( allow_identifier_expansion == 0 )
				 || ( identifier_expansion_length == 0 ) )
				{
					break;
				}
				else if( naming_schema == LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_X_OF_N )
				{
					break;
				}
			}
			segment_file_identifier++;

			if( expand_identifier != 0 )
			{
				if( naming_schema == LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_NUMERIC )
				{
					maximum_segment_file_identifier *= 10;
				}
				segment_filename_copy_index = segment_filename_copy_length;

				while( segment_filename_copy_index > 0 )
				{
					if( &( segment_filename_copy[ segment_filename_copy_index ] ) == &( identifier[ identifier_length - 1 ] ) )
					{
						break;
					}
					segment_filename_copy[ segment_filename_copy_index + 1 ] = segment_filename_copy[ segment_filename_copy_index ];

					segment_filename_copy_index--;
				}
				identifier_expansion_length  -= 1;
				identifier_length            += 1;
				segment_filename_copy_length += 1;

				expand_identifier = 0;
			}
			identifier_divider = 1;

			if( ( naming_schema == LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_NUMERIC )
			 || ( naming_schema == LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_X_OF_N ) )
			{
				if( allow_identifier_expansion != 0 )
				{
					expand_identifier = 1;
				}
				for( identifier_index = 0;
				     identifier_index < identifier_length;
				     identifier_index++ )
				{
					identifier[ identifier_length - identifier_index - 1 ] = '0' + (char) ( ( segment_file_identifier / identifier_divider ) % 10 );

					if( identifier[ identifier_length - identifier_index - 1 ] != '9' )
					{
						expand_identifier = 0;
					}
					identifier_divider *= 10;
				}
			}
			else if( naming_schema == LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_SPLIT )
			{
				for( identifier_index = 0;
				     identifier_index < identifier_length;
				     identifier_index++ )
				{
					identifier[ identifier_length - identifier_index - 1 ] = 'a' + (char) ( ( segment_file_identifier / identifier_divider ) % 26 );

					identifier_divider *= 26;
				}
			}
			segment_filename      = NULL;
			segment_filename_size = 0;

			result = libsmraw_glob_exists_segment_file(
				  file_io_handle,
				  segment_filename_copy,
				  segment_filename_copy_length,
				  NULL,
				  0,
				  &segment_filename,
				  &segment_filename_size,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_GENERIC,
				 "%s: unable to determine if segment file: %s exists.",
				 function,
				 segment_filename_copy );

				goto on_error;
			}
			else if( result == 0 )
			{
				memory_free(
				 segment_filename );

				segment_file_identifier--;

				break;
			}
			if( libsmraw_glob_append_segment_file(
			     filenames,
			     number_of_filenames,
			     segment_filename,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append segment filename.",
				 function );

				memory_free(
				 segment_filename );

				goto on_error;
			}
		}
	}
	if( ( naming_schema == LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_X_OF_N )
	 && ( segment_file_identifier != maximum_segment_file_identifier ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing segment filename(s).",
		 function );

		goto on_error;
	}
	memory_free(
	 segment_filename_copy );

	segment_filename_copy = NULL;

	if( libbfio_handle_free(
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to free file IO handle.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( segment_filename_copy != NULL )
	{
		memory_free(
		 segment_filename_copy );
	}
	if( *filenames != NULL )
	{
		libsmraw_glob_free(
		 *filenames,
		 *number_of_filenames,
		 NULL );

		*filenames = NULL;
	}
	else if( segment_filename != NULL )
	{
		memory_free(
		 segment_filename );
	}
	*number_of_filenames = 0;

	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( -1 );
}

/* Frees the globbed filenames
 * Returns 1 if successful or -1 on error
 */
int libsmraw_glob_free(
     char *filenames[],
     int number_of_filenames,
     libcerror_error_t **error )
{
	static char *function = "libsmraw_glob_free";
	int filename_iterator = 0;

	if( filenames == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filenames.",
		 function );

		return( -1 );
	}
	if( number_of_filenames < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid number of filenames value less than zero.",
		 function );

		return( -1 );
	}
	for( filename_iterator = 0;
	     filename_iterator < number_of_filenames;
	     filename_iterator++ )
	{
		memory_free(
		 filenames[ filename_iterator ] );
	}
	memory_free(
	 filenames );

	return( 1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Appends a globbed filename to the filenames
 * Returns 1 if successful or -1 on error
 */
int libsmraw_glob_wide_append_segment_file(
     wchar_t **filenames[],
     int *number_of_filenames,
     const wchar_t *segment_filename,
     libcerror_error_t **error )
{
	static char *function = "libsmraw_glob_wide_append_segment_file";
	void *reallocation    = NULL;

	if( filenames == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filenames.",
		 function );

		return( -1 );
	}
	if( number_of_filenames == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of filenames.",
		 function );

		return( -1 );
	}
	if( segment_filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment filename.",
		 function );

		return( -1 );
	}
	reallocation = memory_reallocate(
			*filenames,
			sizeof( wchar_t * ) * ( *number_of_filenames + 1 ) );

	if( reallocation == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable reallocate filenames.",
		 function );

		return( -1 );
	}
	*filenames = reallocation;

	( *filenames )[ *number_of_filenames ] = (wchar_t *) segment_filename;

	*number_of_filenames += 1;

	return( 1 );
}

/* Determines if a segment file exists
 * The segment filename is made up from the prefix and the suffix
 * Returns 1 if segment file exists, 0 if not or -1 on error
 */
int libsmraw_glob_wide_exists_segment_file(
     libbfio_handle_t *file_io_handle,
     const wchar_t *prefix,
     size_t prefix_length,
     const wchar_t *suffix,
     size_t suffix_length,
     wchar_t **segment_filename,
     size_t *segment_filename_size,
     libcerror_error_t **error )
{
	static char *function = "libsmraw_glob_wide_exists_segment_file";
	int result            = 0;

	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( prefix == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid prefix.",
		 function );

		return( -1 );
	}
	if( prefix_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid prefix length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( suffix != NULL )
	 && ( suffix_length > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid suffix length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( segment_filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment filename.",
		 function );

		return( -1 );
	}
	if( segment_filename_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment filename size.",
		 function );

		return( -1 );
	}
	/* The segment filename consists of the prefix, an end of string character
	 * and (if provided) the suffix
	 */
	*segment_filename_size = prefix_length + 1;

	if( suffix != NULL )
	{
		*segment_filename_size += suffix_length;
	}
	*segment_filename = (wchar_t *) memory_allocate(
	                                 sizeof( wchar_t ) * *segment_filename_size );

	if( *segment_filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create segment filename.",
		 function );

		return( -1 );
	}
	if( libcstring_wide_string_copy(
	     *segment_filename,
	     prefix,
	     prefix_length ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set prefix in segment filename.",
		 function );

		memory_free(
		 *segment_filename );

		*segment_filename      = NULL;
		*segment_filename_size = 0;

		return( -1 );
	}
	( *segment_filename )[ prefix_length ] = 0;

	if( suffix != NULL )
	{
		if( libcstring_wide_string_copy(
		     &( ( *segment_filename )[ prefix_length ] ),
		     suffix,
		     suffix_length ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set suffix in segment filename.",
			 function );

			memory_free(
			 *segment_filename );

			*segment_filename      = NULL;
			*segment_filename_size = 0;

			return( -1 );
		}
		( *segment_filename )[ prefix_length + suffix_length ] = 0;
	}
	if( libbfio_file_set_name_wide(
	     file_io_handle,
	     *segment_filename,
	     *segment_filename_size - 1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set name in file IO handle.",
		 function );

		memory_free(
		 *segment_filename );

		*segment_filename      = NULL;
		*segment_filename_size = 0;

		return( -1 );
	}
	result = libbfio_handle_exists(
		  file_io_handle,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_GENERIC,
		 "%s: unable to determine if file: %ls exists.",
		 function,
		 *segment_filename );

		memory_free(
		 *segment_filename );

		*segment_filename      = NULL;
		*segment_filename_size = 0;

		return( -1 );
	}
	return( result );
}

/* Determines the naming schema
 * Returns 1 if successful, 0 if no naming schema could be determined or -1 on error
 */
int libsmraw_glob_wide_determine_naming_schema(
     const wchar_t *suffix,
     size_t suffix_length,
     uint8_t *naming_schema,
     libcerror_error_t **error )
{
	static char *function = "libsmraw_glob_wide_determine_naming_schema";
	size_t suffix_index   = 0;

	if( suffix == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid suffix.",
		 function );

		return( -1 );
	}
	if( suffix_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid suffix length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( naming_schema == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid naming schema.",
		 function );

		return( -1 );
	}
	*naming_schema = LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_UNKNOWN;

	/* Determine naming schema
	 */
	if( suffix_length == 2 )
	{
		if( ( ( suffix[ 0 ] == (wchar_t) 'd' )
		   || ( suffix[ 0 ] == (wchar_t) 'D' ) )
		 && ( ( suffix[ 1 ] == (wchar_t) 'd' )
		   || ( suffix[ 1 ] == (wchar_t) 'D' ) ) )
		{
			*naming_schema = LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_SINGLE;
		}
	}
	else if( suffix_length == 3 )
	{
		if( ( ( suffix[ 0 ] == (wchar_t) 'r' )
		   || ( suffix[ 0 ] == (wchar_t) 'R' ) )
		 && ( ( suffix[ 1 ] == (wchar_t) 'a' )
		   || ( suffix[ 1 ] == (wchar_t) 'A' ) )
		 && ( ( suffix[ 2 ] == (wchar_t) 'w' )
		   || ( suffix[ 2 ] == (wchar_t) 'W' ) ) )
		{
			*naming_schema = LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_SINGLE;
		}
	}
	else if( suffix_length >= 4 )
	{
		for( suffix_index = 0;
		     suffix_index < suffix_length;
		     suffix_index++ )
		{
			if( ( suffix[ suffix_index ] >= (wchar_t) '0' )
			 && ( suffix[ suffix_index ] <= (wchar_t) '9' ) )
			{
				*naming_schema = LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_X_OF_N;
			}
			else if( ( suffix_index > 0 )
			      && ( ( suffix[ suffix_index ] == (wchar_t) 'o' )
			        || ( suffix[ suffix_index ] == (wchar_t) 'O' ) ) )
			{
				*naming_schema = LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_X_OF_N;

				break;
			}
			else
			{
				*naming_schema = LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_UNKNOWN;

				break;
			}
		}
		if( *naming_schema == LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_X_OF_N )
		{
			*naming_schema = LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_UNKNOWN;

			if( suffix_index < suffix_length )
			{
				if( ( suffix[ suffix_index ] == (wchar_t) 'o' )
				 || ( suffix[ suffix_index ] == (wchar_t) 'O' ) )
				{
					*naming_schema = LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_X_OF_N;

					suffix_index++;
				}
			}
		}
		if( *naming_schema == LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_X_OF_N )
		{
			*naming_schema = LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_UNKNOWN;

			if( suffix_index < suffix_length )
			{
				if( ( suffix[ suffix_index ] == (wchar_t) 'f' )
				 || ( suffix[ suffix_index ] == (wchar_t) 'F' ) )
				{
					*naming_schema = LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_X_OF_N;

					suffix_index++;
				}
			}
		}
		if( *naming_schema == LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_X_OF_N )
		{
			while( suffix_index < suffix_length )
			{
				if( ( suffix[ suffix_index ] >= (wchar_t) '0' )
				 && ( suffix[ suffix_index ] <= (wchar_t) '9' ) )
				{
					*naming_schema = LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_X_OF_N;
				}
				else
				{
					*naming_schema = LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_UNKNOWN;

					break;
				}
				suffix_index++;
			}
		}
	}
	if( *naming_schema == LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_UNKNOWN )
	{
		for( suffix_index = 0;
		     suffix_index < suffix_length;
		     suffix_index++ )
		{
			if( suffix[ suffix_index ] == (wchar_t) 'a' )
			{
				*naming_schema = LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_SPLIT;
			}
			else if( ( suffix[ suffix_index ] == (wchar_t) '0' )
			      && ( suffix_index < ( suffix_length - 1 ) ) )
			{
				*naming_schema = LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_NUMERIC;
			}
			else if( ( ( suffix[ suffix_index ] == (wchar_t) '0' )
			        || ( suffix[ suffix_index ] == (wchar_t) '1' ) )
			      && ( suffix_index == ( suffix_length - 1 ) ) )
			{
				*naming_schema = LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_NUMERIC;
			}
			else
			{
				*naming_schema = LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_UNKNOWN;

				break;
			}
		}
	}
	if( *naming_schema == LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_UNKNOWN )
	{
		return( 0 );
	}
	return( 1 );
}

/* Globs the files according to common (split) RAW naming schemas
 * Returns 1 if successful or -1 on error
 */
int libsmraw_glob_wide(
     const wchar_t *filename,
     size_t filename_length,
     wchar_t **filenames[],
     int *number_of_filenames,
     libcerror_error_t **error )
{
	wchar_t *suffixes[ ] = {
		L".raw",
		L".dmg",
		L".img",
		L".dd",
		L".000",
		L".001",
		L".00",
		L".01",
		L".0",
		L".1",
		L"aa",
		L"00",
		L"aaa",
		L"000",
		NULL
	};

	libbfio_handle_t *file_io_handle    = NULL;
	wchar_t *identifier                 = NULL;
	wchar_t *segment_filename           = NULL;
	wchar_t *segment_filename_copy      = NULL;
	wchar_t *suffix                     = NULL;
	static char *function               = "libsmraw_glob_wide";
	size_t identifier_divider           = 0;
	size_t identifier_expansion_length  = 2;
	size_t identifier_index             = 0;
	size_t identifier_length            = 0;
	size_t segment_filename_copy_index  = 0;
	size_t segment_filename_copy_length = 0;
	size_t segment_filename_copy_size   = 0;
	size_t segment_filename_size        = 0;
	size_t suffix_length                = 0;
	uint8_t allow_identifier_expansion  = 0;
	uint8_t expand_identifier           = 0;
	uint8_t naming_schema               = 0;
	int maximum_segment_file_identifier = 0;
	int result                          = 0;
	int segment_file_identifier         = 0;
	int suffix_index                    = 0;

	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( filenames == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filenames.",
		 function );

		return( -1 );
	}
	if( number_of_filenames == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of filenames.",
		 function );

		return( -1 );
	}
	*number_of_filenames = 0;

	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO handle.",
		 function );

		goto on_error;
	}
	/* Test if the full filename was provided
	 */
	result = libsmraw_glob_wide_exists_segment_file(
		  file_io_handle,
		  filename,
		  filename_length,
		  NULL,
		  0,
		  &segment_filename,
		  &segment_filename_size,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_GENERIC,
		 "%s: unable to determine if file: %ls exists.",
		 function,
		 filename );

		goto on_error;
	}
	/* Check if there is a segment file with a known suffix
	 */
	else if( result == 0 )
	{
		suffix_index = 0;

		while( result == 0 )
		{
			if( segment_filename != NULL )
			{
				memory_free(
				 segment_filename );

				segment_filename = NULL;
			}
			suffix = suffixes[ suffix_index ];

			if( suffix == NULL )
			{
				break;
			}
			suffix_length = libcstring_wide_string_length(
			                 suffix );

			result = libsmraw_glob_wide_exists_segment_file(
				  file_io_handle,
				  filename,
				  filename_length,
				  suffix,
				  suffix_length,
				  &segment_filename,
				  &segment_filename_size,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_GENERIC,
				 "%s: unable to determine if segment file with suffix: %ls exists.",
				 function,
				 suffix );

				goto on_error;
			}
			suffix_index++;
		}
	}
	if( result == 0 )
	{
		if( segment_filename != NULL )
		{
			memory_free(
			 segment_filename );
		}
		return( 1 );
	}
	if( libsmraw_glob_wide_append_segment_file(
	     filenames,
	     number_of_filenames,
	     segment_filename,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append segment filename.",
		 function );

		goto on_error;
	}
	/* Make a copy of the segment filename to tamper with
	 */
	segment_filename_copy = libcstring_wide_string_allocate(
	                         segment_filename_size + identifier_expansion_length );

	if( segment_filename_copy == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create segment filename copy.",
		 function );

		goto on_error;
	}
	if( libcstring_wide_string_copy(
	     segment_filename_copy,
	     segment_filename,
	     segment_filename_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set segment filename copy.",
		 function );

		goto on_error;
	}
	segment_filename_copy[ segment_filename_size - 1 ] = 0;
	segment_filename_copy_size                         = segment_filename_size;

	/* Determine the naming schema
	 */
	if( suffix == NULL )
	{
		/* Determine if the segment filename contains a suffix which contains a segment identifier
		 */
		segment_filename_copy_index = segment_filename_copy_size - 1;

		while( segment_filename_copy_index > 0 )
		{
			if( segment_filename_copy[ segment_filename_copy_index ] == (wchar_t) '.' )
			{
				identifier        = &( segment_filename_copy[ segment_filename_copy_index + 1 ] );
				identifier_length = ( segment_filename_copy_size - 1 ) - ( segment_filename_copy_index + 1 );

				break;
			}
			else if( ( segment_filename_copy[ segment_filename_copy_index ] == (wchar_t) '/' )
			      || ( segment_filename_copy[ segment_filename_copy_index ] == (wchar_t) '\\' ) )
			{
				break;
			}
			segment_filename_copy_index--;
		}
		if( identifier != NULL )
		{
			/* Check if the extension contains a known naming schema
			 */
			result = libsmraw_glob_wide_determine_naming_schema(
			          identifier,
			          identifier_length,
			          &naming_schema,
			          error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine naming schema.",
				 function );

				goto on_error;
			}
		}
		if( naming_schema == LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_UNKNOWN )
		{
			if( identifier == NULL )
			{
				segment_filename_copy_index = segment_filename_copy_size - 1;
			}
			/* Determine if the segment filename contains a segment identifier
			 */
			while( segment_filename_copy_index > 0 )
			{
				if( naming_schema == LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_NUMERIC )
				{
					if( segment_filename_copy[ segment_filename_copy_index ] != (wchar_t) '0' )
					{
						identifier = &( segment_filename_copy[ segment_filename_copy_index + 1 ] );

						break;
					}
					identifier_length++;
				}
				else if( naming_schema == LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_SPLIT )
				{
					if( segment_filename_copy[ segment_filename_copy_index ] != (wchar_t) 'a' )
					{
						identifier = &( segment_filename_copy[ segment_filename_copy_index + 1 ] );

						break;
					}
					identifier_length++;
				}
				else
				{
					if( ( segment_filename_copy[ segment_filename_copy_index ] == (wchar_t) '0' )
					 || ( segment_filename_copy[ segment_filename_copy_index ] == (wchar_t) '1' ) )
					{
						naming_schema     = LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_NUMERIC;
						identifier_length = 1;
					}
					else if( segment_filename_copy[ segment_filename_copy_index ] == (wchar_t) 'a' )
					{
						naming_schema     = LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_SPLIT;
						identifier_length = 1;
					}
					else if( ( segment_filename_copy[ segment_filename_copy_index ] == (wchar_t) '/' )
					      || ( segment_filename_copy[ segment_filename_copy_index ] == (wchar_t) '\\' ) )
					{
						break;
					}
				}
				segment_filename_copy_index--;
			}
		}
	}
	else
	{
		segment_filename_copy_index = ( segment_filename_copy_size - 1 ) - suffix_length;

		identifier        = &( segment_filename_copy[ segment_filename_copy_index ] );
		identifier_length = suffix_length;

		/* Ignore the extension dot
		 */
		if( identifier[ 0 ] == (wchar_t) '.' )
		{
			identifier        += 1;
			identifier_length -= 1;
		}
		result = libsmraw_glob_wide_determine_naming_schema(
		          identifier,
		          identifier_length,
		          &naming_schema,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine naming schema.",
			 function );

			goto on_error;
		}
	}
	if( naming_schema == LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_UNKNOWN )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid naming schema missing value.",
		 function );

		goto on_error;
	}
	if( naming_schema != LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_SINGLE )
	{
		if( naming_schema == LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_NUMERIC )
		{
			maximum_segment_file_identifier = 1;

			for( identifier_index = 0;
			     identifier_index < identifier_length;
			     identifier_index++ )
			{
				maximum_segment_file_identifier *= 10;
			}
			maximum_segment_file_identifier -= 1;
		}
		else if( naming_schema == LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_SPLIT )
		{
			maximum_segment_file_identifier = 1;

			for( identifier_index = 0;
			     identifier_index < identifier_length;
			     identifier_index++ )
			{
				maximum_segment_file_identifier *= 26;
			}
			maximum_segment_file_identifier -= 1;
		}
		else if( naming_schema == LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_X_OF_N )
		{
			for( identifier_index = identifier_length - 1;
			     identifier_index > 0;
			     identifier_index-- )
			{
				if( ( identifier[ identifier_index ] < (wchar_t) '0' )
				 || ( identifier[ identifier_index ] > (wchar_t) '9' ) )
				{
					break;
				}
			}
			identifier_index++;

			while( identifier_index < identifier_length )
			{
				maximum_segment_file_identifier *= 10;
				maximum_segment_file_identifier += (int) ( identifier[ identifier_index ] - (wchar_t) '0' );

				identifier_index++;
			}
			for( identifier_index = 0;
			     identifier_index < identifier_length;
			     identifier_index++ )
			{
				if( ( identifier[ identifier_index ] < (wchar_t) '0' )
				 || ( identifier[ identifier_index ] > (wchar_t) '9' ) )
				{
					break;
				}
			}
			identifier_length = identifier_index;
		}
		if( ( naming_schema == LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_NUMERIC )
		 || ( naming_schema == LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_X_OF_N ) )
		{
			segment_file_identifier = (int) ( identifier[ identifier_length - 1 ] - (wchar_t) '0' );

			if( identifier_length == 1 )
			{
				allow_identifier_expansion = 1;
			}
		}
		segment_filename_copy_length = segment_filename_copy_size - 1;

		while( *number_of_filenames < INT_MAX )
		{
			if( ( segment_file_identifier + 1 ) > maximum_segment_file_identifier )
			{
				if( ( allow_identifier_expansion == 0 )
				 || ( identifier_expansion_length == 0 ) )
				{
					break;
				}
				else if( naming_schema == LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_X_OF_N )
				{
					break;
				}
			}
			segment_file_identifier++;

			if( expand_identifier != 0 )
			{
				if( naming_schema == LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_NUMERIC )
				{
					maximum_segment_file_identifier *= 10;
				}
				segment_filename_copy_index = segment_filename_copy_length;

				while( segment_filename_copy_index > 0 )
				{
					if( &( segment_filename_copy[ segment_filename_copy_index ] ) == &( identifier[ identifier_length - 1 ] ) )
					{
						break;
					}
					segment_filename_copy[ segment_filename_copy_index + 1 ] = segment_filename_copy[ segment_filename_copy_index ];

					segment_filename_copy_index--;
				}
				identifier_expansion_length  -= 1;
				identifier_length            += 1;
				segment_filename_copy_length += 1;

				expand_identifier = 0;
			}
			identifier_divider = 1;

			if( ( naming_schema == LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_NUMERIC )
			 || ( naming_schema == LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_X_OF_N ) )
			{
				if( allow_identifier_expansion != 0 )
				{
					expand_identifier = 1;
				}
				for( identifier_index = 0;
				     identifier_index < identifier_length;
				     identifier_index++ )
				{
					identifier[ identifier_length - identifier_index - 1 ] = (wchar_t) '0' + (wchar_t) ( ( segment_file_identifier / identifier_divider ) % 10 );

					if( identifier[ identifier_length - identifier_index - 1 ] != (wchar_t) '9' )
					{
						expand_identifier = 0;
					}
					identifier_divider *= 10;
				}
			}
			else if( naming_schema == LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_SPLIT )
			{
				for( identifier_index = 0;
				     identifier_index < identifier_length;
				     identifier_index++ )
				{
					identifier[ identifier_length - identifier_index - 1 ] = (wchar_t) 'a' + (wchar_t) ( ( segment_file_identifier / identifier_divider ) % 26 );

					identifier_divider *= 26;
				}
			}
			segment_filename      = NULL;
			segment_filename_size = 0;

			result = libsmraw_glob_wide_exists_segment_file(
				  file_io_handle,
				  segment_filename_copy,
				  segment_filename_copy_length,
				  NULL,
				  0,
				  &segment_filename,
				  &segment_filename_size,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_GENERIC,
				 "%s: unable to determine if segment file: %ls exists.",
				 function,
				 segment_filename_copy );

				goto on_error;
			}
			else if( result == 0 )
			{
				memory_free(
				 segment_filename );

				segment_file_identifier--;

				break;
			}
			if( libsmraw_glob_wide_append_segment_file(
			     filenames,
			     number_of_filenames,
			     segment_filename,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append segment filename.",
				 function );

				memory_free(
				 segment_filename );

				goto on_error;
			}
		}
	}
	if( ( naming_schema == LIBSMRAW_SEGMENT_FILE_NAMING_SCHEMA_X_OF_N )
	 && ( segment_file_identifier != maximum_segment_file_identifier ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing segment filename(s).",
		 function );

		goto on_error;
	}
	memory_free(
	 segment_filename_copy );

	segment_filename_copy = NULL;

	if( libbfio_handle_free(
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to free file IO handle.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( segment_filename_copy != NULL )
	{
		memory_free(
		 segment_filename_copy );
	}
	if( *filenames != NULL )
	{
		libsmraw_glob_wide_free(
		 *filenames,
		 *number_of_filenames,
		 NULL );

		*filenames = NULL;
	}
	else if( segment_filename != NULL )
	{
		memory_free(
		 segment_filename );
	}
	*number_of_filenames = 0;

	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( -1 );
}

/* Frees the globbed wide filenames
 * Returns 1 if successful or -1 on error
 */
int libsmraw_glob_wide_free(
     wchar_t *filenames[],
     int number_of_filenames,
     libcerror_error_t **error )
{
	static char *function = "libsmraw_glob_wide_free";
	int filename_iterator = 0;

	if( filenames == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filenames.",
		 function );

		return( -1 );
	}
	if( number_of_filenames < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid number of filenames value less than zero.",
		 function );

		return( -1 );
	}
	for( filename_iterator = 0;
	     filename_iterator < number_of_filenames;
	     filename_iterator++ )
	{
		memory_free(
		 filenames[ filename_iterator ] );
	}
	memory_free(
	 filenames );

	return( 1 );
}

#endif

