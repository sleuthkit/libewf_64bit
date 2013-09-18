#! /usr/bin/env python
#
# Python script to open and close Expert Witness Compression format file(s) using pyewf
#
# Author:            Joachim Metz
# Creation date:     September 29, 2010
# Modification date: January 26, 2013
#

__author__    = "Joachim Metz"
__version__   = "20130126"
__date__      = "Jan 26, 2013"
__copyright__ = "Copyright (c) 2006-2012, Joachim Metz <joachim.metz@gmail.com>"
__license__   = "GNU LGPL version 3"

import sys
import pyewf

# ----------------------------------------------------------------------------
# Main
# ----------------------------------------------------------------------------

print "open_close.py " + __version__ + " (libewf " + pyewf.get_version() + ")\n"

argc = len( sys.argv )

if argc < 2:
	print "Usage: open_close.py filename(s)\n"

	sys.exit( 1 )

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
	handle.close()
except:
	print "Unable to close file(s)\n"
	print sys.exc_info()
 
	sys.exit( 1 )

sys.exit( 0 )

