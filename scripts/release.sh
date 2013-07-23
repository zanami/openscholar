#!/bin/bash

# @file
# release.sh
#
# Packages a release tar file for the latest version of openscholar.
#
# Usage:
#         ./scripts/release.sh
#
# The version will be detected from the openscholar.info file.

# Goes to repository root directory
ROOT="$(dirname $0)/.."
cd $ROOT
ROOT=$(pwd)

# Displays the working directory to the user
echo " "
echo "Repository found: $ROOT"

# Confirms to the user that the correct version is being used.
VERSION_INFO=`grep "version = 7.x" openscholar/openscholar.info`
VERSION=`echo $VERSION_INFO | sed -n 's/version = 7\.x-3\.\(.*\)/\1/p'`
echo " "
echo "In order to compress OpenScholar 7.x-3.${VERSION},"
echo "this script will:"
echo " - DESTROY your local docroot build"
echo " - RESET any changes you have made to your local git clone"
echo " "
echo "Note: To build a different version, either check out a different branch or edit openscholar.info and correct the version number."
echo " "
read -p "Continue? (y/n) "
if [[ ! $REPLY =~ ^[Yy]$ ]]
then
    exit 1
fi

# Finds the latest version tag
MATCH="SCHOLAR-3.$VERSION"
TAGS=$(git tag)
TAGNAME=$(echo "$TAGS" | (grep $MATCH -m1))
if [ -z $TAGNAME ]
then
  echo "No tag found mathching $MATCH."
  exit 0
fi
echo "Checking out tag $TAGNAME and running build script..."

# Stores the original branch to restore after process.
ORIG_BRANCH="$(git symbolic-ref HEAD 2>/dev/null)" ||
ORIG_BRANCH="(unnamed branch)"     # detached HEAD
ORIG_BRANCH=${ORIG_BRANCH##refs/heads/}

# Checks out the latest version tag for the detected minor version.
git checkout $TAGNAME >/dev/null 2>&1

# Removes any existing docroots or release builds.
rm -rf www #sudo?
rm -rf docroot #sudo?
DIRNAME="openscholar-7.x-3.$VERSION"
echo "Building tag $TAGNAME to directory $DIRNAME..."
if [ -d $DIRNAME ]
then
  rm -rf $DIRNAME # sudo?
fi

# Runs build and moves it to a new directory named after the version.
bash scripts/build >/dev/null 2>&1
if [ -d www ]
then
	mv www $DIRNAME
fi

# Removes unnecessary 1.5M of test files
TESTPATH_ORIG="openscholar/modules/os/modules/citation_distribute/plugins/service/swordapp-php-library-1.0/test-files"
TESTPATH_TEMP="release-temp"
mv $TESTPATH_ORIG $TESTPATH_TEMP

# Removes symbolic link, moves actual profile directory to build folder.
echo "Writing files..."
rm -rf $DIRNAME/profiles/openscholar
cp -R openscholar $DIRNAME/profiles/openscholar

# Removes other profiles, leaving only openscholar
echo "Removing unused files..."
rm -rf $DIRNAME/profiles/standard
rm -rf $DIRNAME/profiles/minimal
rm -rf $DIRNAME/profiles/testing

echo "Compressing..."
tar -cvzf $DIRNAME.tar.gz $DIRNAME/
# rm -rf $DIRNAME
FILESIZE=$(ls -lah $DIRNAME.tar.gz | awk '{ print $6}')

# Restores test files to original location.
mv $TESTPATH_TEMP $TESTPATH_ORIG

# Restores git to original branch.
git checkout $ORIG_BRANCH >/dev/null 2>&1
git reset --hard HEAD

echo " "
echo "Success: $DIRNAME.tar.gz ($FILESIZE)"
echo " "
exit
