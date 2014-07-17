#!/usr/bin/env bash
# A small little utility script to compile the images to a movie using ffmpeg

# Usage:
#  ./mkfilm <images_directoy> <film_filename>
#
#  <images_directory> -- Where all the images from your timelapse are stored.
#                        Will only look for .jpg images.
#  <file_filename> -- Where to save the film to and what filename.
#
# Examples:
#  ./mkfilm emerald/ emerald.mp4
#  ./mkfilm sapphire/ sapphire.webm
#
#

# Make sure we have enough arguemnts
if [ $# -lt 2 ]; then
	echo "Not enough arguemnts."
	echo "Usage:"
	echo "./mkfilm <images_directory> <film_filename>"
	exit 0
fi

# Argument values
imagesDir=$1
filmName=$2
jpgFiles=

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
			((n++))
		done

		for i in "${!images[@]}"; do
			echo "$i -> ${images[$i]}"
		done

	else
		# Was not able to find a single jpeg, just exit
		echo "Couldn't find .jpg files"
	fi
else
	# No directory, just exit
	echo "\"$imagesDir\" is not a valid directory."
	exit 1
fi

