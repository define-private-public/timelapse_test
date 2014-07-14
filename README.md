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
`$ ./capture_timelapse <timeout> <duration> <destination_folder>`

 * `<timeout>` is in seconds
 * `<duration>` is how long the timelapse capture should be run for.  It's of
   the format `DD:HH:MM:SS`.  Please include a leading zero.
    * `DD` is a value between `[00-99]` for days
	* `HH` is a value between `[00-23]` for hours
	* `MM` is a value between `[00-59]` for minutes
	* `SS` is a value between `[00-59]` for seconds
 * `<destination_folder>` where to save all of the images that are captured

If any of the parameters are invalid or there is some sort of error, the
timelapse program will exit with a message.  At any time to stop the program
send an interrupt, terminate, or quit signal.

All images are saved to a `.jpg` extension and have a UNIX timestamp of when
they were captured.  e.g. `1405282940.jpg` would be the capture made on July
13th, 2014 at 20:22:20 UTC (alternatively the time when Mario Goetze kicked the
winning goal of the FIFA 2014 final match).

A text file called `info.txt` will be left in `<destination_folder>` with some
information about the timelapse capture.

Folder Description
------------------
 * `./toybox/` is just a little place where I tinker with code.  Some of it may
   be moved into the main source code.
 * `./src/` main source directory.
