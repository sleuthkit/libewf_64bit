/*
 * C application to open and close Expert Witness Compression Format file(s) using libewf
 *
 * Copyright (c) 2006-2012, Joachim Metz <joachim.metz@gmail.com>
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

#include <stdlib.h>
#include <stdio.h>

#include <libewf.h>

int main( int argc, char * const argv[] )
{
	libewf_error_t *error   = NULL;
	libewf_handle_t *handle = NULL;

	if( argc <= 1 )
	{
		fprintf(
		 stderr,
		 "Usage: ./open_close filename(s)\n" );

		return( EXIT_FAILURE );
	}
	/* The function will return 1 if successful or -1 on error.
	 * On error the error 'object' is created by the library.
	 *
	 * handle must refer to NULL to create a new libewf handle 'object'.
	 *
	 * If error is NULL e.g. libewf_handle_initialize( &handle, NULL )
	 * no error 'object' is created
	 *
	 * The error 'object' can be freed by libewf_error_free()
	 */
	if( libewf_handle_initialize(
	     &handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize handle.\n" );

		goto on_error;
	}
	if( libewf_handle_open(
	     handle,
	     &( argv[ 1 ] ),
	     argc - 1,
	     LIBEWF_OPEN_READ,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to open file(s).\n" );

		goto on_error;
	}
	if( libewf_handle_close(
	     handle,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close handle.\n" );

		goto on_error;
	}
	if( libewf_handle_free(
	     &handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free handle.\n" );

		goto on_error;
	}
	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libewf_error_backtrace_fprint(
		 error,
		 stderr );
		libewf_error_free(
		 &error );
	}
	if( handle != NULL )
	{
		libewf_handle_free(
		 &handle,
		 NULL );
	}
	return( EXIT_FAILURE );
}

