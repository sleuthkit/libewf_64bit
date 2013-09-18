#!/bin/bash
#
# ewfverify testing script
#
# Copyright (c) 2006-2012, Joachim Metz <joachim.metz@gmail.com>
#
# Refer to AUTHORS for acknowledgements.
#
# This software is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this software.  If not, see <http://www.gnu.org/licenses/>.
#

EXIT_SUCCESS=0;
EXIT_FAILURE=1;
EXIT_IGNORE=77;

INPUT="input";
INPUT_ERROR="input_error";
INPUT_MORE="input_more";
TMP="tmp";

LS="ls";
TR="tr";
WC="wc";

test_verify()
{ 
	INPUT_FILE=$1;

	${EWFVERIFY} -q -d sha1 ${INPUT_FILE};

	RESULT=$?;

	echo "";

	echo -n "Testing ewfverify of input: ${INPUT_FILE} ";

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		echo " (FAIL)";
	else
		echo " (PASS)";
	fi
	return ${RESULT};
}

EWFVERIFY="../ewftools/ewfverify";

if ! test -x ${EWFVERIFY};
then
	EWFVERIFY="../ewftools/ewfverify.exe";
fi

if ! test -x ${EWFVERIFY};
then
	echo "Missing executable: ${EWFVERIFY}";

	exit ${EXIT_FAILURE};
fi

if ! test -d ${INPUT};
then
	echo "No ${INPUT} directory found, to test ewfverify create ${INPUT} directory and place EWF test files in directory.";

	exit ${EXIT_IGNORE};
fi

EXIT_RESULT=${EXIT_IGNORE};

if test -d ${INPUT};
then
	RESULT=`${LS} ${INPUT}/*.[esE]01 | ${TR} ' ' '\n' | ${WC} -l`;

	if test ${RESULT} -eq 0;
	then
		echo "No files found in ${INPUT} directory, to test ewfverify place EWF test files in directory.";
	else
		for FILENAME in `${LS} ${INPUT}/*.[esE]01 | ${TR} ' ' '\n'`;
		do
			if [ "${FILENAME}" = "${INPUT}/floppy-ewf1.35-best-compression.E01" ];
			then
				# experimental version only
				echo;

			elif [ "${FILENAME}" = "${INPUT}/floppy-ewf1.35-nocompression.E01" ];
			then
				# experimental version only
				echo;

			elif ! test_verify "${FILENAME}";
			then
				exit ${EXIT_FAILURE};
			fi
		done

		EXIT_RESULT=${EXIT_SUCCESS};
	fi
fi

if test -d ${INPUT_ERROR};
then
	RESULT=`${LS} ${INPUT_ERROR}/*.[esE]01 | ${TR} ' ' '\n' | ${WC} -l`;

	if test ${RESULT} -eq 0;
	then
		echo "No files found in ${INPUT_ERROR} directory, to test read place test files in directory.";
	else
		for FILENAME in `${LS} ${INPUT_ERROR}/*.[esE]01 | ${TR} ' ' '\n'`;
		do
			if test_verify "${FILENAME}";
			then
				exit ${EXIT_FAILURE};
			fi
		done

		EXIT_RESULT=${EXIT_SUCCESS};
	fi
fi

if test -d ${INPUT_MORE};
then
	RESULT=`${LS} ${INPUT_MORE}/*.[esE]01 | ${TR} ' ' '\n' | ${WC} -l`;

	if test ${RESULT} -eq 0;
	then
		echo "No files found in ${INPUT_MORE} directory, to test read place test files in directory.";
	else
		for FILENAME in `${LS} ${INPUT_MORE}/*.[esE]01 | ${TR} ' ' '\n'`;
		do
			if ! test_verify "${FILENAME}";
			then
				exit ${EXIT_FAILURE};
			fi
		done

		EXIT_RESULT=${EXIT_SUCCESS};
	fi
fi

exit ${EXIT_RESULT};

