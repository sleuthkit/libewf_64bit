/*
 * File entry functions
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
#include "libewf_extern.h"
#include "libewf_file_entry.h"
#include "libewf_handle.h"
#include "libewf_libcdata.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"
#include "libewf_libcstring.h"
#include "libewf_single_file_entry.h"
#include "libewf_single_file_tree.h"
#include "libewf_types.h"

/* Initializes the file entry and its values
 * Returns 1 if successful or -1 on error
 */
int libewf_file_entry_initialize(
     libewf_file_entry_t **file_entry,
     libewf_internal_handle_t *internal_handle,
     libcdata_tree_node_t *file_entry_tree_node,
     uint8_t flags,
     libcerror_error_t **error )
{
	libewf_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                             = "libewf_file_entry_initialize";

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( *file_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file entry value already set.",
		 function );

		return( -1 );
	}
	if( ( flags & ~( LIBEWF_ITEM_FLAG_MANAGED_FILE_ENTRY_TREE_NODE ) ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported flags: 0x%02" PRIx8 ".",
		 function,
		 flags );

		return( -1 );
	}
	internal_file_entry = memory_allocate_structure(
	                       libewf_internal_file_entry_t );

	if( internal_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file entry.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_file_entry,
	     0,
	     sizeof( libewf_internal_file_entry_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file entry.",
		 function );

		goto on_error;
	}
	internal_file_entry->internal_handle = internal_handle;
	internal_file_entry->flags           = flags;

	if( ( flags & LIBEWF_ITEM_FLAG_MANAGED_FILE_ENTRY_TREE_NODE ) == 0 )
	{
		internal_file_entry->file_entry_tree_node = file_entry_tree_node;
	}
	else
	{
		if( libcdata_tree_node_clone(
		     &( internal_file_entry->file_entry_tree_node ),
		     file_entry_tree_node,
		     (int (*)(intptr_t **, libcerror_error_t **)) &libewf_single_file_entry_free,
		     (int (*)(intptr_t **, intptr_t *, libcerror_error_t **)) &libewf_single_file_entry_clone,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy file entry tree node.",
			 function );

			goto on_error;
		}
	}
	*file_entry = (libewf_file_entry_t *) internal_file_entry;

	return( 1 );

on_error:
	if( internal_file_entry != NULL )
	{
		memory_free(
		 internal_file_entry );
	}
	return( -1 );
}

/* Frees a file entry
 * Returns 1 if successful or -1 on error
 */
int libewf_file_entry_free(
     libewf_file_entry_t **file_entry,
     libcerror_error_t **error )
{
	libewf_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                             = "libewf_file_entry_free";
	int result                                        = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( *file_entry != NULL )
	{
		internal_file_entry = (libewf_internal_file_entry_t *) *file_entry;
		*file_entry         = NULL;

		/* The internal_handle reference is freed elsewhere
		 */
		/* If not managed the file_entry_tree_node reference is freed elsewhere
		 */
		if( ( internal_file_entry->flags & LIBEWF_ITEM_FLAG_MANAGED_FILE_ENTRY_TREE_NODE ) != 0 )
		{
			if( internal_file_entry->file_entry_tree_node != NULL )
			{
				if( libcdata_tree_node_free(
				     &( internal_file_entry->file_entry_tree_node ),
				     (int (*)(intptr_t **, libcerror_error_t **)) &libewf_single_file_entry_free,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free file entry tree node.",
					 function );

					return( -1 );
				}
			}
		}
		memory_free(
		 internal_file_entry );
	}
	return( result );
}

/* Retrieves the type
 * Returns 1 if successful or -1 on error
 */
int libewf_file_entry_get_type(
     libewf_file_entry_t *file_entry,
     uint8_t *type,
     libcerror_error_t **error )
{
	libewf_internal_file_entry_t *internal_file_entry = NULL;
	libewf_single_file_entry_t *single_file_entry     = NULL;
	static char *function                             = "libewf_file_entry_get_type";

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libewf_internal_file_entry_t *) file_entry;

	if( libcdata_tree_node_get_value(
	     internal_file_entry->file_entry_tree_node,
	     (intptr_t **) &single_file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from file entry tree node.",
		 function );

		return( -1 );
	}
	if( libewf_single_file_entry_get_type(
	     single_file_entry,
	     type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve type.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the flags
 * Returns 1 if successful or -1 on error
 */
int libewf_file_entry_get_flags(
     libewf_file_entry_t *file_entry,
     uint32_t *flags,
     libcerror_error_t **error )
{
	libewf_internal_file_entry_t *internal_file_entry = NULL;
	libewf_single_file_entry_t *single_file_entry     = NULL;
	static char *function                             = "libewf_file_entry_get_flags";

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libewf_internal_file_entry_t *) file_entry;

	if( libcdata_tree_node_get_value(
	     internal_file_entry->file_entry_tree_node,
	     (intptr_t **) &single_file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from file entry tree node.",
		 function );

		return( -1 );
	}
	if( libewf_single_file_entry_get_flags(
	     single_file_entry,
	     flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve flags.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the media data offset
 * Returns 1 if successful or -1 on error
 */
int libewf_file_entry_get_media_data_offset(
     libewf_file_entry_t *file_entry,
     off64_t *media_data_offset,
     libcerror_error_t **error )
{
	libewf_internal_file_entry_t *internal_file_entry = NULL;
	libewf_single_file_entry_t *single_file_entry     = NULL;
	static char *function                             = "libewf_file_entry_get_media_data_offset";

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libewf_internal_file_entry_t *) file_entry;

	if( libcdata_tree_node_get_value(
	     internal_file_entry->file_entry_tree_node,
	     (intptr_t **) &single_file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from file entry tree node.",
		 function );

		return( -1 );
	}
	if( libewf_single_file_entry_get_data_offset(
	     single_file_entry,
	     media_data_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data offset.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the media data size
 * Returns 1 if successful or -1 on error
 */
int libewf_file_entry_get_media_data_size(
     libewf_file_entry_t *file_entry,
     size64_t *media_data_size,
     libcerror_error_t **error )
{
	libewf_internal_file_entry_t *internal_file_entry = NULL;
	libewf_single_file_entry_t *single_file_entry     = NULL;
	static char *function                             = "libewf_file_entry_get_media_data_size";

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libewf_internal_file_entry_t *) file_entry;

	if( libcdata_tree_node_get_value(
	     internal_file_entry->file_entry_tree_node,
	     (intptr_t **) &single_file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from file entry tree node.",
		 function );

		return( -1 );
	}
	if( libewf_single_file_entry_get_data_size(
	     single_file_entry,
	     media_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the duplicate media data offset
 * Returns 1 if successful or -1 on error
 */
int libewf_file_entry_get_duplicate_media_data_offset(
     libewf_file_entry_t *file_entry,
     off64_t *duplicate_media_data_offset,
     libcerror_error_t **error )
{
	libewf_internal_file_entry_t *internal_file_entry = NULL;
	libewf_single_file_entry_t *single_file_entry     = NULL;
	static char *function                             = "libewf_file_entry_get_media_data_offset";

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libewf_internal_file_entry_t *) file_entry;

	if( libcdata_tree_node_get_value(
	     internal_file_entry->file_entry_tree_node,
	     (intptr_t **) &single_file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from file entry tree node.",
		 function );

		return( -1 );
	}
	if( libewf_single_file_entry_get_duplicate_data_offset(
	     single_file_entry,
	     duplicate_media_data_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve duplicate data offset.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_entry_get_utf8_name_size(
     libewf_file_entry_t *file_entry,
     size_t *utf8_name_size,
     libcerror_error_t **error )
{
	libewf_internal_file_entry_t *internal_file_entry = NULL;
	libewf_single_file_entry_t *single_file_entry     = NULL;
	static char *function                             = "libewf_file_entry_get_utf8_name_size";

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libewf_internal_file_entry_t *) file_entry;

	if( libcdata_tree_node_get_value(
	     internal_file_entry->file_entry_tree_node,
	     (intptr_t **) &single_file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from file entry tree node.",
		 function );

		return( -1 );
	}
	if( libewf_single_file_entry_get_utf8_name_size(
	     single_file_entry,
	     utf8_name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 name size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded name value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_entry_get_utf8_name(
     libewf_file_entry_t *file_entry,
     uint8_t *utf8_name,
     size_t utf8_name_size,
     libcerror_error_t **error )
{
	libewf_internal_file_entry_t *internal_file_entry = NULL;
	libewf_single_file_entry_t *single_file_entry     = NULL;
	static char *function                             = "libewf_file_entry_get_utf8_name_size";

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libewf_internal_file_entry_t *) file_entry;

	if( libcdata_tree_node_get_value(
	     internal_file_entry->file_entry_tree_node,
	     (intptr_t **) &single_file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from file entry tree node.",
		 function );

		return( -1 );
	}
	if( libewf_single_file_entry_get_utf8_name(
	     single_file_entry,
	     utf8_name,
	     utf8_name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 name.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_entry_get_utf16_name_size(
     libewf_file_entry_t *file_entry,
     size_t *utf16_name_size,
     libcerror_error_t **error )
{
	libewf_internal_file_entry_t *internal_file_entry = NULL;
	libewf_single_file_entry_t *single_file_entry     = NULL;
	static char *function                             = "libewf_file_entry_get_utf16_name_size";

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libewf_internal_file_entry_t *) file_entry;

	if( libcdata_tree_node_get_value(
	     internal_file_entry->file_entry_tree_node,
	     (intptr_t **) &single_file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from file entry tree node.",
		 function );

		return( -1 );
	}
	if( libewf_single_file_entry_get_utf16_name_size(
	     single_file_entry,
	     utf16_name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 name size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded name value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_entry_get_utf16_name(
     libewf_file_entry_t *file_entry,
     uint16_t *utf16_name,
     size_t utf16_name_size,
     libcerror_error_t **error )
{
	libewf_internal_file_entry_t *internal_file_entry = NULL;
	libewf_single_file_entry_t *single_file_entry     = NULL;
	static char *function                             = "libewf_file_entry_get_utf16_name_size";

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libewf_internal_file_entry_t *) file_entry;

	if( libcdata_tree_node_get_value(
	     internal_file_entry->file_entry_tree_node,
	     (intptr_t **) &single_file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from file entry tree node.",
		 function );

		return( -1 );
	}
	if( libewf_single_file_entry_get_utf16_name(
	     single_file_entry,
	     utf16_name,
	     utf16_name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 name.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size
 * Returns 1 if successful or -1 on error
 */
int libewf_file_entry_get_size(
     libewf_file_entry_t *file_entry,
     size64_t *size,
     libcerror_error_t **error )
{
	libewf_internal_file_entry_t *internal_file_entry = NULL;
	libewf_single_file_entry_t *single_file_entry     = NULL;
	static char *function                             = "libewf_file_entry_get_size";

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libewf_internal_file_entry_t *) file_entry;

	if( libcdata_tree_node_get_value(
	     internal_file_entry->file_entry_tree_node,
	     (intptr_t **) &single_file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from file entry tree node.",
		 function );

		return( -1 );
	}
	if( libewf_single_file_entry_get_size(
	     single_file_entry,
	     size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the creation date and time
 * Returns 1 if successful or -1 on error
 */
int libewf_file_entry_get_creation_time(
     libewf_file_entry_t *file_entry,
     uint32_t *creation_time,
     libcerror_error_t **error )
{
	libewf_internal_file_entry_t *internal_file_entry = NULL;
	libewf_single_file_entry_t *single_file_entry     = NULL;
	static char *function                             = "libewf_file_entry_get_creation_time";

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libewf_internal_file_entry_t *) file_entry;

	if( libcdata_tree_node_get_value(
	     internal_file_entry->file_entry_tree_node,
	     (intptr_t **) &single_file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from file entry tree node.",
		 function );

		return( -1 );
	}
	if( libewf_single_file_entry_get_creation_time(
	     single_file_entry,
	     creation_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve creation time.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the (file) modification (last written) date and time
 * Returns 1 if successful or -1 on error
 */
int libewf_file_entry_get_modification_time(
     libewf_file_entry_t *file_entry,
     uint32_t *modification_time,
     libcerror_error_t **error )
{
	libewf_internal_file_entry_t *internal_file_entry = NULL;
	libewf_single_file_entry_t *single_file_entry     = NULL;
	static char *function                             = "libewf_file_entry_get_modification_time";

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libewf_internal_file_entry_t *) file_entry;

	if( libcdata_tree_node_get_value(
	     internal_file_entry->file_entry_tree_node,
	     (intptr_t **) &single_file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from file entry tree node.",
		 function );

		return( -1 );
	}
	if( libewf_single_file_entry_get_modification_time(
	     single_file_entry,
	     modification_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve modification time.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the access date and time
 * Returns 1 if successful or -1 on error
 */
int libewf_file_entry_get_access_time(
     libewf_file_entry_t *file_entry,
     uint32_t *access_time,
     libcerror_error_t **error )
{
	libewf_internal_file_entry_t *internal_file_entry = NULL;
	libewf_single_file_entry_t *single_file_entry     = NULL;
	static char *function                             = "libewf_file_entry_get_access_time";

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libewf_internal_file_entry_t *) file_entry;

	if( libcdata_tree_node_get_value(
	     internal_file_entry->file_entry_tree_node,
	     (intptr_t **) &single_file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from file entry tree node.",
		 function );

		return( -1 );
	}
	if( libewf_single_file_entry_get_access_time(
	     single_file_entry,
	     access_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve access time.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the (file system entry) modification date and time
 * Returns 1 if successful or -1 on error
 */
int libewf_file_entry_get_entry_modification_time(
     libewf_file_entry_t *file_entry,
     uint32_t *entry_modification_time,
     libcerror_error_t **error )
{
	libewf_internal_file_entry_t *internal_file_entry = NULL;
	libewf_single_file_entry_t *single_file_entry     = NULL;
	static char *function                             = "libewf_file_entry_get_entry_modification_time";

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libewf_internal_file_entry_t *) file_entry;

	if( libcdata_tree_node_get_value(
	     internal_file_entry->file_entry_tree_node,
	     (intptr_t **) &single_file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from file entry tree node.",
		 function );

		return( -1 );
	}
	if( libewf_single_file_entry_get_entry_modification_time(
	     single_file_entry,
	     entry_modification_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry modification time.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded MD5 hash value
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_file_entry_get_utf8_hash_value_md5(
     libewf_file_entry_t *file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libewf_internal_file_entry_t *internal_file_entry = NULL;
	libewf_single_file_entry_t *single_file_entry     = NULL;
	static char *function                             = "libewf_file_entry_get_utf8_hash_value_md5";
	int result                                        = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libewf_internal_file_entry_t *) file_entry;

	if( libcdata_tree_node_get_value(
	     internal_file_entry->file_entry_tree_node,
	     (intptr_t **) &single_file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from file entry tree node.",
		 function );

		return( -1 );
	}
	result = libewf_single_file_entry_get_utf8_hash_value_md5(
	          single_file_entry,
	          utf8_string,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve hash value: MD5.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the UTF-16 encoded MD5 hash value
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_file_entry_get_utf16_hash_value_md5(
     libewf_file_entry_t *file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libewf_internal_file_entry_t *internal_file_entry = NULL;
	libewf_single_file_entry_t *single_file_entry     = NULL;
	static char *function                             = "libewf_file_entry_get_utf16_hash_value_md5";
	int result                                        = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libewf_internal_file_entry_t *) file_entry;

	if( libcdata_tree_node_get_value(
	     internal_file_entry->file_entry_tree_node,
	     (intptr_t **) &single_file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from file entry tree node.",
		 function );

		return( -1 );
	}
	result = libewf_single_file_entry_get_utf16_hash_value_md5(
	          single_file_entry,
	          utf16_string,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve hash value: MD5.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the UTF-8 encoded SHA1 hash value
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_file_entry_get_utf8_hash_value_sha1(
     libewf_file_entry_t *file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libewf_internal_file_entry_t *internal_file_entry = NULL;
	libewf_single_file_entry_t *single_file_entry     = NULL;
	static char *function                             = "libewf_file_entry_get_utf8_hash_value_sha1";
	int result                                        = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libewf_internal_file_entry_t *) file_entry;

	if( libcdata_tree_node_get_value(
	     internal_file_entry->file_entry_tree_node,
	     (intptr_t **) &single_file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from file entry tree node.",
		 function );

		return( -1 );
	}
	result = libewf_single_file_entry_get_utf8_hash_value_sha1(
	          single_file_entry,
	          utf8_string,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve hash value: SHA1.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the UTF-16 encoded SHA1 hash value
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_file_entry_get_utf16_hash_value_sha1(
     libewf_file_entry_t *file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libewf_internal_file_entry_t *internal_file_entry = NULL;
	libewf_single_file_entry_t *single_file_entry     = NULL;
	static char *function                             = "libewf_file_entry_get_utf16_hash_value_sha1";
	int result                                        = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libewf_internal_file_entry_t *) file_entry;

	if( libcdata_tree_node_get_value(
	     internal_file_entry->file_entry_tree_node,
	     (intptr_t **) &single_file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from file entry tree node.",
		 function );

		return( -1 );
	}
	result = libewf_single_file_entry_get_utf16_hash_value_sha1(
	          single_file_entry,
	          utf16_string,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve hash value: SHA1.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Reads data at the current offset
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_file_entry_read_buffer(
         libewf_file_entry_t *file_entry,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error )
{
	libewf_internal_file_entry_t *internal_file_entry = NULL;
	libewf_single_file_entry_t *single_file_entry     = NULL;
	static char *function                             = "libewf_file_entry_read_buffer";
	off64_t data_offset                               = 0;
	off64_t duplicate_data_offset                     = 0;
	size64_t data_size                                = 0;
	size64_t size                                     = 0;
	size_t read_size                                  = 0;
	ssize_t read_count                                = 0;
	uint32_t flags                                    = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libewf_internal_file_entry_t *) file_entry;

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
	if( buffer_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid buffer size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( internal_file_entry->offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid file entry - offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( libcdata_tree_node_get_value(
	     internal_file_entry->file_entry_tree_node,
	     (intptr_t **) &single_file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from file entry tree node.",
		 function );

		return( -1 );
	}
	if( libewf_single_file_entry_get_size(
	     single_file_entry,
	     &size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size.",
		 function );

		return( -1 );
	}
	if( libewf_single_file_entry_get_data_offset(
	     single_file_entry,
	     &data_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data offset.",
		 function );

		return( -1 );
	}
	if( libewf_single_file_entry_get_data_size(
	     single_file_entry,
	     &data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data size.",
		 function );

		return( -1 );
	}
	if( libewf_single_file_entry_get_flags(
	     single_file_entry,
	     &flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve flags.",
		 function );

		return( -1 );
	}
	if( ( flags & LIBEWF_FILE_ENTRY_FLAG_SPARSE_DATA ) == 0 )
	{
		if( ( ( size == 0 )
		  &&  ( data_size != 1 ) )
		 || ( ( size != 0 )
		  &&  ( data_size != size ) ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported data size.",
			 function );

			return( -1 );
		}
	}
	else
	{
		if( ( data_size != 1 )
		 && ( data_size != size ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported data size.",
			 function );

			return( -1 );
		}
		if( libewf_single_file_entry_get_duplicate_data_offset(
		     single_file_entry,
		     &duplicate_data_offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve duplicate data offset.",
			 function );

			return( -1 );
		}
	}
	if( internal_file_entry->offset >= (off64_t) size )
	{
		return( 0 );
	}
	if( (off64_t) ( internal_file_entry->offset + buffer_size ) > (off64_t) size )
	{
		buffer_size = (size_t) ( size - internal_file_entry->offset );
	}
	if( ( flags & LIBEWF_FILE_ENTRY_FLAG_SPARSE_DATA ) == 0 )
	{
		data_offset += internal_file_entry->offset;
		read_size    = buffer_size;
	}
	else if( duplicate_data_offset >= 0 )
	{
		data_offset = duplicate_data_offset + internal_file_entry->offset;
		read_size   = buffer_size;
	}
	else
	{
		read_size = 1;
	}
	if( libewf_handle_seek_offset(
	     (libewf_handle_t *) internal_file_entry->internal_handle,
	     data_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset: %" PRIi64 ".",
		 function,
		 data_offset );

		return( -1 );
	}
	read_count = libewf_handle_read_buffer(
		      (libewf_handle_t *) internal_file_entry->internal_handle,
		      buffer,
		      read_size,
		      error );

	if( read_count <= -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read buffer.",
		 function );

		return( -1 );
	}
	internal_file_entry->offset += read_count;

	if( ( flags & LIBEWF_FILE_ENTRY_FLAG_SPARSE_DATA ) != 0 )
	{
		if( read_count == 1 )
		{
			if( memory_set(
			     &( ( (uint8_t *) buffer )[ 1 ] ),
			     ( (uint8_t *) buffer )[ 0 ],
			     buffer_size - 1 ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to set sparse data in buffer.",
				 function );

				return( -1 );
			}
			read_count = (ssize_t) buffer_size;
		}
	}
	return( read_count );
}

/* Reads data at a specific offset
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_file_entry_read_random(
         libewf_file_entry_t *file_entry,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error )
{
	static char *function = "libewf_file_entry_read_random";
	ssize_t read_count    = 0;

	if( libewf_file_entry_seek_offset(
	     file_entry,
	     offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset.",
		 function );

		return( -1 );
	}
	read_count = libewf_file_entry_read_buffer(
	              file_entry,
	              buffer,
	              buffer_size,
	              error );

	if( read_count <= -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read buffer.",
		 function );

		return( -1 );
	}
	return( read_count );
}

/* Seeks a certain offset of the data
 * Returns the offset if seek is successful or -1 on error
 */
off64_t libewf_file_entry_seek_offset(
         libewf_file_entry_t *file_entry,
         off64_t offset,
         int whence,
         libcerror_error_t **error )
{
	libewf_internal_file_entry_t *internal_file_entry = NULL;
	libewf_single_file_entry_t *single_file_entry     = NULL;
	static char *function                             = "libewf_file_entry_seek_offset";
	size64_t size                                     = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libewf_internal_file_entry_t *) file_entry;

	if( libcdata_tree_node_get_value(
	     internal_file_entry->file_entry_tree_node,
	     (intptr_t **) &single_file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from file entry tree node.",
		 function );

		return( -1 );
	}
	if( libewf_single_file_entry_get_size(
	     single_file_entry,
	     &size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size.",
		 function );

		return( -1 );
	}
	if( ( whence != SEEK_CUR )
	 && ( whence != SEEK_END )
	 && ( whence != SEEK_SET ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported whence.",
		 function );

		return( -1 );
	}
	if( whence == SEEK_CUR )
	{	
		offset += internal_file_entry->offset;
	}
	else if( whence == SEEK_END )
	{	
		offset += (off64_t) size;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: seeking offset: %" PRIi64 ".\n",
		 function,
		 offset );
	}
#endif
	if( offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid offset value out of bounds.",
		 function );

		return( -1 );
	}
	internal_file_entry->offset = offset;

	return( offset );
}

/* Retrieves the current offset of the data
 * Returns the offset if successful or -1 on error
 */
int libewf_file_entry_get_offset(
     libewf_file_entry_t *file_entry,
     off64_t *offset,
     libcerror_error_t **error )
{
	libewf_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                             = "libewf_file_entry_get_offset";

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libewf_internal_file_entry_t *) file_entry;

	if( internal_file_entry->file_entry_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file entry - missing file entry tree node.",
		 function );

		return( -1 );
	}
	if( offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset.",
		 function );

		return( -1 );
	}
	*offset = internal_file_entry->offset;

	return( 1 );
}

/* Retrieves the number of sub file entries
 * Returns 1 if successful or -1 on error
 */
int libewf_file_entry_get_number_of_sub_file_entries(
     libewf_file_entry_t *file_entry,
     int *number_of_sub_file_entries,
     libcerror_error_t **error )
{
	libewf_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                             = "libewf_file_entry_get_number_of_sub_file_entries";

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libewf_internal_file_entry_t *) file_entry;

	if( libcdata_tree_node_get_number_of_sub_nodes(
	     internal_file_entry->file_entry_tree_node,
	     number_of_sub_file_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sub file entries.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the sub file entry for the specific index
 * Returns 1 if successful or -1 on error
 */
int libewf_file_entry_get_sub_file_entry(
     libewf_file_entry_t *file_entry,
     int sub_file_entry_index,
     libewf_file_entry_t **sub_file_entry,
     libcerror_error_t **error )
{
	libewf_internal_file_entry_t *internal_file_entry = NULL;
	libcdata_tree_node_t *sub_node                    = NULL;
	static char *function                             = "libewf_file_entry_get_sub_file_entry";

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libewf_internal_file_entry_t *) file_entry;

	if( internal_file_entry->file_entry_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file entry - missing file entry tree node.",
		 function );

		return( -1 );
	}
	if( sub_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub file entry.",
		 function );

		return( -1 );
	}
	if( *sub_file_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: sub file entry already set.",
		 function );

		return( -1 );
	}
	if( libcdata_tree_node_get_sub_node_by_index(
	     internal_file_entry->file_entry_tree_node,
             sub_file_entry_index,
             &sub_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sub file entry tree node.",
		 function );

		return( -1 );
	}
	if( sub_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid sub node.",
		 function );

		return( -1 );
	}
	if( libewf_file_entry_initialize(
	     sub_file_entry,
	     internal_file_entry->internal_handle,
	     sub_node,
	     LIBEWF_ITEM_FLAGS_DEFAULT,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize sub file entry.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the sub file entry for the specific UTF-8 encoded name
 * Returns 1 if successful, 0 if no such sub file entry or -1 on error
 */
int libewf_file_entry_get_sub_file_entry_by_utf8_name(
     libewf_file_entry_t *file_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libewf_file_entry_t **sub_file_entry,
     libcerror_error_t **error )
{
	libewf_internal_file_entry_t *internal_file_entry = NULL;
	libewf_single_file_entry_t *sub_single_file_entry = NULL;
	libcdata_tree_node_t *sub_node                    = NULL;
	static char *function                             = "libewf_file_entry_get_sub_file_entry_by_utf8_name";
	int result                                        = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libewf_internal_file_entry_t *) file_entry;

	if( internal_file_entry->file_entry_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file entry - missing file entry tree node.",
		 function );

		return( -1 );
	}
	if( sub_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub file entry.",
		 function );

		return( -1 );
	}
	if( *sub_file_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: sub file entry already set.",
		 function );

		return( -1 );
	}
	result = libewf_single_file_tree_get_sub_node_by_utf8_name(
	          internal_file_entry->file_entry_tree_node,
	          utf8_string,
	          utf8_string_length,
	          &sub_node,
	          &sub_single_file_entry,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sub file entry by UTF-8 name.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libewf_file_entry_initialize(
		     sub_file_entry,
		     internal_file_entry->internal_handle,
		     sub_node,
		     LIBEWF_ITEM_FLAGS_DEFAULT,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize sub file entry.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the sub file entry for the specific UTF-8 encoded path
 * The path separator is the \ character
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
int libewf_file_entry_get_sub_file_entry_by_utf8_path(
     libewf_file_entry_t *file_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libewf_file_entry_t **sub_file_entry,
     libcerror_error_t **error )
{
	libewf_internal_file_entry_t *internal_file_entry = NULL;
	libewf_single_file_entry_t *single_file_entry     = NULL;
	libewf_single_file_entry_t *sub_single_file_entry = NULL;
	libcdata_tree_node_t *node                        = NULL;
	libcdata_tree_node_t *sub_node                    = NULL;
	uint8_t *utf8_string_segment                      = NULL;
	static char *function                             = "libewf_file_entry_get_sub_file_entry_by_utf8_path";
	size_t utf8_string_index                          = 0;
	size_t utf8_string_segment_length                 = 0;
	int result                                        = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libewf_internal_file_entry_t *) file_entry;

	if( utf8_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( utf8_string_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 string length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( sub_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub file entry.",
		 function );

		return( -1 );
	}
	if( *sub_file_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: sub file entry already set.",
		 function );

		return( -1 );
	}
	if( libcdata_tree_node_get_value(
	     internal_file_entry->file_entry_tree_node,
	     (intptr_t **) &single_file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from file entry tree node.",
		 function );

		return( -1 );
	}
	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing file entry values.",
		 function );

		return( -1 );
	}
	node = internal_file_entry->file_entry_tree_node;

	if( utf8_string_length > 0 )
	{
		/* Ignore a leading separator
		 */
		if( utf8_string[ utf8_string_index ] == (uint8_t) LIBEWF_SEPARATOR )
		{
			utf8_string_index++;
		}
	}
	if( ( utf8_string_length == 0 )
	 || ( utf8_string_length == 1 ) )
	{
		result = 1;
	}
	while( utf8_string_index < utf8_string_length )
	{
		utf8_string_segment        = (uint8_t *) &( utf8_string[ utf8_string_index ] );
		utf8_string_segment_length = 0;

		while( utf8_string_index < utf8_string_length )
		{
			if( ( utf8_string[ utf8_string_index ] == (uint8_t) LIBEWF_SEPARATOR )
			 || ( utf8_string[ utf8_string_index ] == (uint8_t) 0 ) )
			{
				utf8_string_index++;

				break;
			}
			utf8_string_index++;

			utf8_string_segment_length++;
		}
		if( utf8_string_segment_length == 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing sub file entry name.",
			 function );

			return( -1 );
		}
		result = libewf_single_file_tree_get_sub_node_by_utf8_name(
			  node,
			  utf8_string_segment,
			  utf8_string_segment_length,
			  &sub_node,
			  &sub_single_file_entry,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub node by name.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			break;
		}
		node = sub_node;
	}
	if( result != 0 )
	{
		if( libewf_file_entry_initialize(
		     sub_file_entry,
		     internal_file_entry->internal_handle,
		     sub_node,
		     LIBEWF_ITEM_FLAGS_DEFAULT,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize sub file entry.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the sub file entry for the specific UTF-16 encoded name
 * Returns 1 if successful, 0 if no such sub file entry or -1 on error
 */
int libewf_file_entry_get_sub_file_entry_by_utf16_name(
     libewf_file_entry_t *file_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libewf_file_entry_t **sub_file_entry,
     libcerror_error_t **error )
{
	libewf_internal_file_entry_t *internal_file_entry = NULL;
	libewf_single_file_entry_t *sub_single_file_entry = NULL;
	libcdata_tree_node_t *sub_node                    = NULL;
	static char *function                             = "libewf_file_entry_get_sub_file_entry_by_utf16_name";
	int result                                        = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libewf_internal_file_entry_t *) file_entry;

	if( internal_file_entry->file_entry_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file entry - missing file entry tree node.",
		 function );

		return( -1 );
	}
	if( sub_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub file entry.",
		 function );

		return( -1 );
	}
	if( *sub_file_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: sub file entry already set.",
		 function );

		return( -1 );
	}
	result = libewf_single_file_tree_get_sub_node_by_utf16_name(
	          internal_file_entry->file_entry_tree_node,
	          utf16_string,
	          utf16_string_length,
	          &sub_node,
	          &sub_single_file_entry,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sub file entry by UTF-16 name.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libewf_file_entry_initialize(
		     sub_file_entry,
		     internal_file_entry->internal_handle,
		     sub_node,
		     LIBEWF_ITEM_FLAGS_DEFAULT,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize sub file entry.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the sub file entry for the specific UTF-16 encoded path
 * The path separator is the \ character
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
int libewf_file_entry_get_sub_file_entry_by_utf16_path(
     libewf_file_entry_t *file_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libewf_file_entry_t **sub_file_entry,
     libcerror_error_t **error )
{
	libewf_internal_file_entry_t *internal_file_entry = NULL;
	libewf_single_file_entry_t *single_file_entry     = NULL;
	libewf_single_file_entry_t *sub_single_file_entry = NULL;
	libcdata_tree_node_t *node                        = NULL;
	libcdata_tree_node_t *sub_node                    = NULL;
	uint16_t *utf16_string_segment                    = NULL;
	static char *function                             = "libewf_file_entry_get_sub_file_entry_by_utf16_path";
	size_t utf16_string_index                         = 0;
	size_t utf16_string_segment_length                = 0;
	int result                                        = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libewf_internal_file_entry_t *) file_entry;

	if( utf16_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string.",
		 function );

		return( -1 );
	}
	if( utf16_string_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-16 string length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( sub_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub file entry.",
		 function );

		return( -1 );
	}
	if( *sub_file_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: sub file entry already set.",
		 function );

		return( -1 );
	}
	if( libcdata_tree_node_get_value(
	     internal_file_entry->file_entry_tree_node,
	     (intptr_t **) &single_file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from file entry tree node.",
		 function );

		return( -1 );
	}
	if( single_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing file entry values.",
		 function );

		return( -1 );
	}
	node = internal_file_entry->file_entry_tree_node;

	if( utf16_string_length > 0 )
	{
		/* Ignore a leading separator
		 */
		if( utf16_string[ utf16_string_index ] == (uint16_t) LIBEWF_SEPARATOR )
		{
			utf16_string_index++;
		}
	}
	if( ( utf16_string_length == 0 )
	 || ( utf16_string_length == 1 ) )
	{
		result = 1;
	}
	while( utf16_string_index < utf16_string_length )
	{
		utf16_string_segment        = (uint16_t *) &( utf16_string[ utf16_string_index ] );
		utf16_string_segment_length = 0;

		while( utf16_string_index < utf16_string_length )
		{
			if( ( utf16_string[ utf16_string_index ] == (uint16_t) LIBEWF_SEPARATOR )
			 || ( utf16_string[ utf16_string_index ] == (uint16_t) 0 ) )
			{
				utf16_string_index++;

				break;
			}
			utf16_string_index++;

			utf16_string_segment_length++;
		}
		if( utf16_string_segment_length == 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing sub file entry name.",
			 function );

			return( -1 );
		}
		result = libewf_single_file_tree_get_sub_node_by_utf16_name(
			  node,
			  utf16_string_segment,
			  utf16_string_segment_length,
			  &sub_node,
			  &sub_single_file_entry,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub node by name.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			break;
		}
		node = sub_node;
	}
	if( result != 0 )
	{
		if( libewf_file_entry_initialize(
		     sub_file_entry,
		     internal_file_entry->internal_handle,
		     sub_node,
		     LIBEWF_ITEM_FLAGS_DEFAULT,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize sub file entry.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

