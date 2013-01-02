#!/bin/tcsh

### Begin OpenScholar file upload =======================
# @file
# @author Oren Robinson <oren_robinson@harvard.edu>
# @since 2013-01-02
#
# @section DESCRIPTION
#
# Runs CURL to upload a pdf file to http://example.com
# Only replaces (overwrites) the file if it already exists,
# otherwise does nothing.

let BASE_URL='http://example.com'
let PAGE='gking_upload_filefield'
let FILEPATH=`pwd`
let FILENAME="$1.pdf"
let AUTH='password123'
let LOG='gking_filefield_upload.log.html'

cd $FILEPATH

# Prepares the curl command
let CURL="curl -F filename=${FILENAME} -F contents=<${FILENAME} -F mimetype=application/pdf -F auth=${AUTH} -L -# --url ${BASE_URL}/${PAGE}"

# Dear developer: uncomment to have output go to log file.
# If you do this, you will not see a progress bar while the file uploads.
# let CURL="curl -F filename=${FILENAME} -F contents=<${FILENAME} -F mimetype=application/pdf -F auth=${AUTH} -L -# --url ${BASE_URL}/${PAGE} -o $LOG"

# Dear developer: uncomment to see executed command. 
# echo "${CURL}"

# Curls a URL with posted file data.
echo "Uploading ${FILENAME} to ${BASE_URL}..."
$CURL
