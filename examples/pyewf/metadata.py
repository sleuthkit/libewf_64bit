#! /usr/bin/env python
#
# Python script to print the metadata in Expert Witness Compression format file(s) using pyewf
#
# Author:            Joachim Metz
# Creation date:     October 14, 2010
# Modification date: January 26, 2013
#

__author__    = "Joachim Metz"
__version__   = "20130126"
__date__      = "Jan 26, 2013"
__copyright__ = "Copyright (c) 2006-2013, Joachim Metz <joachim.metz@gmail.com>"
__license__   = "GNU LGPL version 3"

import sys
import pyewf

# ----------------------------------------------------------------------------
# Main
# ----------------------------------------------------------------------------

print "metadata.py " + __version__ + " (libewf " + pyewf.get_version() + ")\n"

argc = len( sys.argv )

if argc < 2:
	print "Usage: metadata.py filename(s)\n"

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
	media_size = handle.get_media_size()

except:
	print "Unable to retrieve media size\n"
	print sys.exc_info()[ 1 ]

	sys.exit( 1 )

try:
	header_values = handle.get_header_values()

except:
	print "Unable to retrieve header values\n"
	print sys.exc_info()[ 1 ]

	sys.exit( 1 )

try:
	hash_values = handle.get_hash_values()

except:
	print "Unable to retrieve hash values\n"
	print sys.exc_info()[ 1 ]

	sys.exit( 1 )

try:
	handle.close()
except:
	print "Unable to close file(s)\n"
	print sys.exc_info()
 
	sys.exit( 1 )

print "Media size: " + str( media_size ) + " bytes"
print ""

if len( header_values ) > 0:
	print "Header values:"

	for identifier, value in header_values.items():
		print identifier.encode( "utf8" ) + ": " + value.encode( "utf8" )

	print ""

if len( hash_values ) > 0:
	print "Hash values:"

	for identifier, value in hash_values.items():
		print identifier.encode( "utf8" ) + ": " + value.encode( "utf8" )

	print ""

sys.exit( 0 )

