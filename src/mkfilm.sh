#!/usr/bin/env bash
# A small little utility script to compile the images to a movie using ffmpeg
# This program will rename all of the images in a directory, run them through
# ffmpeg, then restore them to their original names.  If you need to stop the
# script, just send the interrupt signal (^C) and it still should rename them.

# Usage:
#  ./mkfilm <images_directoy> <film_filename> <framerate>
#
#  <images_directory> -- Where all the images from your timelapse are stored.
#                        Will only look for .jpg images.
#  <file_filename> -- Where to save the film to and what filename.
#  <framerate> -- How many images to show per second
#
# Examples:
#  ./mkfilm emerald/ emerald.mp4 24
#  ./mkfilm sapphire/ sapphire.webm 30
#
# Description:

# Make sure we have enough arguemnts
if [ $# -lt 3 ]; then
	echo "Not enough arguemnts."
	echo "Usage:"
	echo "./mkfilm <images_directory> <film_filename> <framerate>"
	exit 0
fi

# Make sure that ffmpeg is installed
command -v ffmpeg
if [ $? -ne 0 ]; then
	echo "ffmpeg is not installed."
	exit 1
fi

# Argument values
imagesDir=$1
filmName=$2
framerate=$3
jpgFiles=


# Have to define this function here
# Restore the image files to their correct names
restore_filenames() {
	# Re-rename the images
	for i in ${!images[@]}; do
		mv ${images[$i]} $i
	done
}


# Check that the directory exists and then do all of the re-naming
if [ -d $imagesDir ]; then
	# Directory is there, make sure that it has images
	jpgFiles=$(ls $imagesDir/*.jpg)

	if [ $? -eq 0 ]; then
		# Figure out how many files we have to deal with
		numFiles=($(ls -l $imagesDir/*.jpg | wc -l) - 1)
		
		# Rename the images
		n=0
		declare -A images
		for f in $imagesDir/*.jpg; do
			newId=$(printf "%0${#numFiles}d" $n)
			images[$f]="$imagesDir/${newId}.jpg"
			mv $f ${images[$f]}
			((n++))
		done

		# Incase the user sends the interrupt signal, rename the files
		trap 'restore_filenames; exit 1' INT

		# Compile the film, rename, then exit
		ffmpeg -i $imagesDir/%${#numFiles}d.jpg -r $framerate $filmName
		restore_filenames
		exit 0
	else
		# Was not able to find a single jpeg, just exit
		echo "Couldn't find .jpg files"
	fi
else
	# No directory, just exit
	echo "\"$imagesDir\" is not a valid directory."
	exit 1
fi


