timelapse_test
==============

A small program for capturing timelapse images.


Compilation
-----------
Requires:
 * POSIX compliant environment (e.g. Linux)
 * OpenCV2
 * g++

To build:
 1. `cd src/`
 2. `make`

Usage
-----
`$ ./capture_timelapse <timeout> <duration> <destination>`

 * `<timeout>` is in seconds, should be an integer greather than zero
 * `<duration>` is how long the timelapse capture should be run for.  It's of
   the format `DD:hh:mm:ss`.  Please include a leading zero.
    * `DD` is a value between `[00-99]` for days
	* `hh` is a value between `[00-23]` for hours
	* `mm` is a value between `[00-59]` for minutes
	* `ss` is a value between `[00-59]` for seconds
 * `<destination>` folder where to save all of the images that are captured

If any of the parameters are invalid or there is some sort of error, the
timelapse program will exit with a message.  At any time to stop the program
send an interrupt, terminate, or quit signal.  The program will also stop when
the capture is complete, or if the user runs out of disk space.

This program assumes that you have only one webcam isntalled, and will use the
"default," one that OpenCV will go to.  If anyone wants support for multiple
cameras, please add something in the GitHub issue tracker.

All images are saved to a `.jpg` extension and have a UNIX timestamp of when
they were captured.  e.g. `1405282940.jpg` would be the capture made on July
13th, 2014 at 20:22:20 UTC (alternatively the time when Mario Goetze kicked the
winning goal of the FIFA 2014 final match). The program will not run if there
is not enough space detected on the disk for all of the images.

A text file called `info.txt` will be left in `<destination>` with some
information about the timelapse capture.

If you want to compile the images into a film, a small bash script called
`compileToMove.sh` is provided in the `./src/` directory.  It requires `ffmpeg`
to be installed. 

Folder Description
------------------
 * `./toybox/` is just a little place where I tinker with code.  Some of it may
   be moved into the main source code.
 * `./src/` main source directory.
