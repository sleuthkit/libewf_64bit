#! /usr/bin/env python
#
# Python script to print the file entries in Expert Witness Compression format file(s) using pyewf
#
# Author:            Joachim Metz
# Creation date:     October 16, 2011
# Modification date: January 26, 2013
#

__author__    = "Joachim Metz"
__version__   = "20130126"
__date__      = "Jan 26, 2013"
__copyright__ = "Copyright (c) 2006-2013, Joachim Metz <joachim.metz@gmail.com>"
__license__   = "GNU LGPL version 3"

import datetime
import sys
import pyewf

# ----------------------------------------------------------------------------
# Functions
# ----------------------------------------------------------------------------

def print_file_entry( file_entry ):
	print "File entry:"

	try:
		name = file_entry.get_name()

	except:
		print "Unable to retrieve name\n"
		print sys.exc_info()[ 1 ]

		sys.exit( 1 )

	if name:
		print "Name\t\t\t: " + name.encode( "utf8" )


	try:
		creation_time = file_entry.get_creation_time()

	except:
		print "Unable to retrieve creation time\n"
		print sys.exc_info()[ 1 ]

		sys.exit( 1 )

	if creation_time:
		print "Creation time\t\t: " + creation_time.strftime("%Y-%m-%d %H:%M:%S")


	try:
		modification_time = file_entry.get_modification_time()

	except:
		print "Unable to retrieve modification time\n"
		print sys.exc_info()[ 1 ]

		sys.exit( 1 )

	if modification_time:
		print "Modification time\t: " + modification_time.strftime("%Y-%m-%d %H:%M:%S")


	try:
		access_time = file_entry.get_access_time()

	except:
		print "Unable to retrieve access time\n"
		print sys.exc_info()[ 1 ]

		sys.exit( 1 )

	if access_time:
		print "Access time\t\t: " + access_time.strftime("%Y-%m-%d %H:%M:%S")


	try:
		entry_modification_time = file_entry.get_entry_modification_time()

	except:
		print "Unable to retrieve entry modification time\n"
		print sys.exc_info()[ 1 ]

		sys.exit( 1 )

	if entry_modification_time:
		print "Entry modification time\t: " + entry_modification_time.strftime("%Y-%m-%d %H:%M:%S")


	try:
		md5_hash = file_entry.get_hash_value_md5()

	except:
		print "Unable to retrieve MD5 hash\n"
		print sys.exc_info()[ 1 ]

		sys.exit( 1 )

	if md5_hash:
		print "MD5 hash\t\t: " + md5_hash


	print ""


	try:
		number_of_sub_file_entries = file_entry.get_number_of_sub_file_entries()

	except:
		print "Unable to retrieve number of file entries\n"
		print sys.exc_info()[ 1 ]

		sys.exit( 1 )

	for sub_file_entry_index in range( 0, number_of_sub_file_entries ):
		try:
			sub_file_entry = file_entry.get_sub_file_entry( sub_file_entry_index )

		except:
			print "Unable to retrieve number of file entry: " + sub_file_entry_index + "\n"
			print sys.exc_info()[ 1 ]

			sys.exit( 1 )

		print_file_entry( sub_file_entry )


# ----------------------------------------------------------------------------
# Main
# ----------------------------------------------------------------------------

print "file_entries.py " + __version__ + " (libewf " + pyewf.get_version() + ")\n"

argc = len( sys.argv )

if argc < 2:
	print "Usage: file_entries.py filename(s)\n"

	sys.exit( 1 )

if argc == 2:
	try:
		filenames = pyewf.glob(
		             sys.argv[ 1 ] )

	except:
		print "Unable to glob filename(s)\n"
		print sys.exc_info()[ 1 ]

		sys.exit( 1 )
else:
	filenames = sys.argv[ 1: ]

handle = pyewf.handle();

if handle == None:
	print "Missing handle object\n"

	sys.exit( 1 )

try:
	# Open requires a list of filenames
	handle.open(
	 filenames )

except:
	print "Unable to open file(s)\n"
	print sys.exc_info()[ 1 ]

	sys.exit( 1 )

try:
	root_file_entry = handle.get_root_file_entry()

except:
	print "Unable to retrieve root file entry\n"
	print sys.exc_info()[ 1 ]

	sys.exit( 1 )

print "Single files:"
print_file_entry( root_file_entry )

sys.exit( 0 )

