/*
 * Information file functions
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
#include <file_stream.h>
#include <memory.h>
#include <types.h>

#include "libsmraw_information_file.h"
#include "libsmraw_libcerror.h"
#include "libsmraw_libcstring.h"
#include "libsmraw_libfvalue.h"

/* Creates the information file
 * Returns 1 if successful or -1 on error
 */
int libsmraw_information_file_initialize(
     libsmraw_information_file_t **information_file,
     libcerror_error_t **error )
{
	static char *function = "libsmraw_information_file_initialize";

	if( information_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid information file.",
		 function );

		return( -1 );
	}
	if( *information_file != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid information file value already set.",
		 function );

		return( -1 );
	}
	*information_file = memory_allocate_structure(
	                     libsmraw_information_file_t );

	if( *information_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create information file.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *information_file,
	     0,
	     sizeof( libsmraw_information_file_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear information file.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *information_file != NULL )
	{
		memory_free(
		 *information_file );

		*information_file = NULL;
	}
	return( -1 );
}

/* Frees the information file
 * Returns 1 if successful or -1 on error
 */
int libsmraw_information_file_free(
     libsmraw_information_file_t **information_file,
     libcerror_error_t **error )
{
	static char *function = "libsmraw_information_file_free";
	int result            = 1;

	if( information_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid information file.",
		 function );

		return( -1 );
	}
	if( *information_file != NULL )
	{
		if( ( *information_file )->name != NULL )
		{
			memory_free(
			 ( *information_file )->name );
		}
		memory_free(
		 *information_file );

		*information_file = NULL;
	}
	return( result );
}

/* Sets the filename
 * Returns 1 if successful or -1 on error
 */
int libsmraw_information_file_set_name(
     libsmraw_information_file_t *information_file,
     const libcstring_system_character_t *name,
     size_t name_length,
     libcerror_error_t **error )
{
	static char *function = "libsmraw_information_file_set_name";

	if( information_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid information file.",
		 function );

		return( -1 );
	}
	if( name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid name.",
		 function );

		return( -1 );
	}
	if( name_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid name length value exceeds maximum.",
		 function );

		return( -1 );
	}
	information_file->name = libcstring_system_string_allocate(
	                          name_length + 1 );

	if( information_file->name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to allocate name.",
		 function );

		return( -1 );
	}
	if( libcstring_system_string_copy(
	     information_file->name,
	     name,
	     name_length ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set name.",
		 function );

		return( -1 );
	}
	information_file->name[ name_length ] = 0;

	information_file->name_size = name_length + 1;

	return( 1 );
}

/* Opens a information file
 * Returns 1 if successful or -1 on error
 */
int libsmraw_information_file_open(
     libsmraw_information_file_t *information_file,
     const libcstring_system_character_t *mode,
     libcerror_error_t **error )
{
	static char *function = "libsmraw_information_file_open";

	if( information_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid information file.",
		 function );

		return( -1 );
	}
	if( information_file->name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid information file - missing name.",
		 function );

		return( -1 );
	}
	if( information_file->file_stream != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid information file - file stream already set.",
		 function );

		return( -1 );
	}
	if( mode == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mode.",
		 function );

		return( -1 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	information_file->file_stream = file_stream_open_wide(
	                                 information_file->name,
	                                 mode );
#else
	information_file->file_stream = file_stream_open(
	                                 information_file->name,
	                                 mode );
#endif

	if( information_file->file_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open: %" PRIs_LIBCSTRING_SYSTEM ".",
		 function,
		 information_file->name );

		return( -1 );
	}
	return( 1 );
}

/* Closes the information file
 * Returns the 0 if succesful or -1 on error
 */
int libsmraw_information_file_close(
     libsmraw_information_file_t *information_file,
     libcerror_error_t **error )
{
	static char *function = "libsmraw_information_file_close";

	if( information_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid information file.",
		 function );

		return( -1 );
	}
	if( information_file->name != NULL )
	{
		memory_free(
		 information_file->name );

		information_file->name = NULL;
	}
	if( information_file->file_stream != NULL )
	{
		if( file_stream_close(
		     information_file->file_stream ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close file stream.",
			 function );

			return( -1 );
		}
		information_file->file_stream = NULL;
	}
	return( 0 );
}

/* Reas a section with its values from the information file
 * Returns the 1 if succesful, 0 if no such section or -1 on error
 */
int libsmraw_information_file_read_section(
     libsmraw_information_file_t *information_file,
     const uint8_t *section_identifier,
     size_t section_identifier_length,
     libfvalue_table_t *values_table,
     libcerror_error_t **error )
{
	char input_string[ 128 ];

	libfvalue_value_t *value       = NULL;
	char *value_data               = NULL;
	char *value_identifier         = NULL;
	char *result_string            = NULL;
	static char *function          = "libsmraw_information_file_read_section";
	size_t input_string_index      = 0;
	size_t value_identifier_length = 0;
	size_t value_data_length       = 0;
	uint8_t in_section             = 0;
	int result                     = 0;

	if( information_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid information file.",
		 function );

		return( -1 );
	}
	if( information_file->file_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid information file - missing file stream.",
		 function );

		return( -1 );
	}
	if( section_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section identifier.",
		 function );

		return( -1 );
	}
	if( section_identifier_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid section identifier length value exceeds maximum.",
		 function );

		return( -1 );
	}
	/* Reset the offset to start of the file stream
	 */
	if( file_stream_seek_offset(
	     information_file->file_stream,
	     0,
	     SEEK_SET ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek stream offset: 0.",
		 function );

		return( -1 );
	}
	while( file_stream_at_end(
	        information_file->file_stream ) == 0 )
	{
		result_string = file_stream_get_string(
		                 information_file->file_stream,
		                 input_string,
		                 128 );

		if( result_string == NULL )
		{
			if( file_stream_at_end(
			     information_file->file_stream ) != 0 )
			{
				break;
			}
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: error reading string from file stream.",
			 function );

			return( -1 );
		}
		/* Skip leading white space
		 */
		for( input_string_index = 0;
		     input_string_index < 128;
		     input_string_index++ )
		{
			if( ( input_string[ input_string_index ] != '\t' )
			 && ( input_string[ input_string_index ] != '\n' )
			 && ( input_string[ input_string_index ] != '\f' )
			 && ( input_string[ input_string_index ] != '\v' )
			 && ( input_string[ input_string_index ] != '\r' )
			 && ( input_string[ input_string_index ] != ' ' ) )
			{
				break;
			}
		}
		/* Skip an empty line
		 */
		if( input_string[ input_string_index ] == 0 )
		{
			continue;
		}
		if( in_section != 0 )
		{
			/* Check for the end of the section
			 */
			if( ( ( input_string_index + section_identifier_length + 2 ) < 128 )
			 && ( input_string[ input_string_index ] == '<' )
			 && ( input_string[ input_string_index + 1 ] == '/' )
			 && ( libcstring_narrow_string_compare(
			       &( input_string[ input_string_index + 2 ] ),
			       section_identifier,
			       section_identifier_length ) == 0 )
			 && ( input_string[ input_string_index + section_identifier_length + 2 ] == '>' ) )
			{
				in_section = 0;
				result     = 1;

				break;
			}
			if( input_string[ input_string_index ] == '<' )
			{
				/* Determine the value identifier
				 */
				input_string_index++;

				value_identifier        = &( input_string[ input_string_index ] );
				value_identifier_length = 0;

				while( input_string_index < 128 )
				{
					if( ( ( input_string[ input_string_index ] >= 'A' )
					  &&  ( input_string[ input_string_index ] <= 'I' ) )
					 || ( ( input_string[ input_string_index ] >= 'J' )
					  &&  ( input_string[ input_string_index ] <= 'R' ) )
					 || ( ( input_string[ input_string_index ] >= 'S' )
					  &&  ( input_string[ input_string_index ] <= 'Z' ) )
					 || ( ( input_string[ input_string_index ] >= 'a' )
					  &&  ( input_string[ input_string_index ] <= 'i' ) )
					 || ( ( input_string[ input_string_index ] >= 'j' )
					  &&  ( input_string[ input_string_index ] <= 'r' ) )
					 || ( ( input_string[ input_string_index ] >= 's' )
					  &&  ( input_string[ input_string_index ] <= 'z' ) )
					 || ( ( input_string[ input_string_index ] >= '0' )
					  &&  ( input_string[ input_string_index ] <= '9' ) )
					 ||   ( input_string[ input_string_index ] == '_' ) )
					{
						value_identifier_length++;
					}
					else
					{
						break;
					}
					input_string_index++;
				}
				/* Check if there is a supported value identifier
				 */
				if( input_string[ input_string_index ] != '>' )
				{
					continue;
				}
				/* Make sure the value identifier is terminated by an end of string
				 */
				input_string[ input_string_index ] = 0;

				/* Determine the value
				 */
				input_string_index++;

				value_data        = &( input_string[ input_string_index ] );
				value_data_length = 0;

				while( input_string_index < 128 )
				{
					if( ( input_string[ input_string_index ] == 0 )
					 || ( input_string[ input_string_index ] == '<' ) )
					{
						break;
					}
					value_data_length++;

					input_string_index++;
				}
				/* Check if there is a supported value
				 */
				if( input_string[ input_string_index ] != '<' )
				{
					continue;
				}
				/* Make sure the value data is terminated by an end of string
				 */
				input_string[ input_string_index ] = 0;

				/* Check the value identifier
				 */
				input_string_index++;

				if( ( ( input_string_index + value_identifier_length + 2 ) >= 128 )
				 || ( input_string[ input_string_index ] != '/' )
				 || ( libcstring_narrow_string_compare(
				       &( input_string[ input_string_index + 1 ] ),
				       value_identifier,
				       value_identifier_length ) != 0 )
				 || ( input_string[ input_string_index + value_identifier_length + 1 ] != '>' ) )
				{
					continue;
				}
				if( libfvalue_value_type_initialize(
				     &value,
				     LIBFVALUE_VALUE_TYPE_STRING_UTF8,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create value: %s.",
					 function,
					 value_identifier );

					goto on_error;
				}
				if( libfvalue_value_set_identifier(
				     value,
				     (uint8_t *) value_identifier,
				     value_identifier_length,
				     LIBFVALUE_VALUE_FLAG_IDENTIFIER_MANAGED,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set identifier in value: %d.",
					 function,
					 value_identifier );

					goto on_error;
				}
				if( libfvalue_value_set_data(
				     value,
				     (uint8_t *) value_data,
				     value_data_length + 1,
				     LIBFVALUE_ENDIAN_NATIVE,
				     LIBFVALUE_VALUE_FLAG_DATA_MANAGED,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set value: %s data.",
					 function,
					 value_identifier );

					goto on_error;
				}
				if( libfvalue_table_set_value(
				     values_table,
				     value,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set value: %s in values table.",
					 function,
					 value_identifier );

					goto on_error;
				}
				value = NULL;
			}
		}
		else
		{
			/* Check for the start of the section
			 */
			if( ( ( input_string_index + section_identifier_length + 1 ) < 128 )
			 && ( input_string[ input_string_index ] == '<' )
			 && ( libcstring_narrow_string_compare(
			       &( input_string[ input_string_index + 1 ] ),
			       section_identifier,
			       section_identifier_length ) == 0 )
			 && ( input_string[ input_string_index + section_identifier_length + 1 ] == '>' ) )
			{
				in_section = 1;
			}
		}
	}
	return( result );

on_error:
	if( value != NULL )
	{
		libfvalue_value_free(
		 &value,
		 NULL );
	}
	return( -1 );
}

/* Write a section with its values to the information file
 * Returns the 1 if succesful or -1 on error
 */
int libsmraw_information_file_write_section(
     libsmraw_information_file_t *information_file,
     const uint8_t *section_identifier,
     size_t section_identifier_length,
     libfvalue_table_t *values_table,
     libcerror_error_t **error )
{
	libfvalue_value_t *value = NULL;
	static char *function    = "libsmraw_information_file_write_section";
	ssize_t write_count      = 0;
	int number_of_values     = 0;
	int print_count          = 0;
	int result               = 1;
	int value_iterator       = 0;

	if( information_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid information file.",
		 function );

		return( -1 );
	}
	if( information_file->file_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid information file - missing file stream.",
		 function );

		return( -1 );
	}
	if( section_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section identifier.",
		 function );

		return( -1 );
	}
	if( section_identifier_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid section identifier length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( values_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid values table.",
		 function );

		return( -1 );
	}
	/* Write section start
	 */
	print_count = fprintf(
	               information_file->file_stream,
	               "<%s>\n",
	               section_identifier );

	if( ( print_count < 0 )
	 || ( (size_t) print_count > ( section_identifier_length + 3 ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write section start to file stream.",
		 function );

		return( -1 );
	}
	/* Write section values
	 */
	if( libfvalue_table_get_number_of_values(
	     values_table,
	     &number_of_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of values.",
		 function );

		return( -1 );
	}
	for( value_iterator = 0;
	     value_iterator < number_of_values;
	     value_iterator++ )
	{
		print_count = fprintf(
			       information_file->file_stream,
			       "\t" );

		if( ( print_count < 0 )
		 || ( (size_t) print_count > 1 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write to file stream.",
			 function );

			return( -1 );
		}
		if( libfvalue_table_get_value_by_index(
		     values_table,
		     value_iterator,
		     &value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value: %d from values table.",
			 function,
			 value_iterator );

			result = -1;

			continue;
		}
		write_count = libfvalue_value_write_to_file_stream(
		               value,
		               information_file->file_stream,
		               error );

		if( write_count <= 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write value: %d to file stream.",
			 function,
			 value_iterator );

			result = -1;
		}
		print_count = fprintf(
			       information_file->file_stream,
			       "\n" );

		if( ( print_count < 0 )
		 || ( (size_t) print_count > 1 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write to file stream.",
			 function );

			return( -1 );
		}
	}
	/* Write section end
	 */
	print_count = fprintf(
	               information_file->file_stream,
	               "</%s>\n\n",
	               section_identifier );

	if( ( print_count < 0 )
	 || ( (size_t) print_count > ( section_identifier_length + 5 ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write section end to file stream.",
		 function );

		return( -1 );
	}
	return( result );
}

